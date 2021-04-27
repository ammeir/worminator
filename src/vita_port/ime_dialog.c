
#include "debug.h"
#include <string.h>
#include <stdbool.h>
#include <vita2d.h>
#include <psp2/types.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/message_dialog.h>
#include <psp2/ime_dialog.h>
#include <psp2/display.h>
#include <psp2/apputil.h>
#include <psp2/gxm.h>
#include <psp2/kernel/sysmem.h>


#define ALIGN(x, a)	(((x) + ((a) - 1)) & ~((a) - 1))
#define DISPLAY_WIDTH				960
#define DISPLAY_HEIGHT				544
#define DISPLAY_STRIDE_IN_PIXELS	1024
#define DISPLAY_BUFFER_COUNT		2
#define DISPLAY_MAX_PENDING_SWAPS	1

typedef struct{
	void*data;
	SceGxmSyncObject*sync;
	SceGxmColorSurface surf;
	SceUID uid;
}displayBuffer;


displayBuffer dbuf[DISPLAY_BUFFER_COUNT];
unsigned int backBufferIndex = 0;
unsigned int frontBufferIndex = 0;
static uint16_t ime_title_utf16[SCE_IME_DIALOG_MAX_TITLE_LENGTH];
static uint16_t ime_initial_text_utf16[SCE_IME_DIALOG_MAX_TEXT_LENGTH];

void *dram_alloc(unsigned int size, SceUID *uid){
	void *mem;
	*uid = sceKernelAllocMemBlock("gpu_mem", SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, ALIGN(size,256*1024), NULL);
	sceKernelGetMemBlockBase(*uid, &mem);
	sceGxmMapMemory(mem, ALIGN(size,256*1024), SCE_GXM_MEMORY_ATTRIB_READ | SCE_GXM_MEMORY_ATTRIB_WRITE);
	return mem;
}
void gxm_vsync_cb(const void *callback_data){
	sceDisplaySetFrameBuf(&(SceDisplayFrameBuf){sizeof(SceDisplayFrameBuf),
		*((void **)callback_data),DISPLAY_STRIDE_IN_PIXELS, 0,
		DISPLAY_WIDTH,DISPLAY_HEIGHT}, SCE_DISPLAY_SETBUF_NEXTFRAME);
}
void gxm_init(){
	sceGxmInitialize(&(SceGxmInitializeParams){0,DISPLAY_MAX_PENDING_SWAPS,gxm_vsync_cb,sizeof(void *),SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE});
	unsigned int i;
	for (i = 0; i < DISPLAY_BUFFER_COUNT; i++) {
		dbuf[i].data = dram_alloc(4*DISPLAY_STRIDE_IN_PIXELS*DISPLAY_HEIGHT, &dbuf[i].uid);
		sceGxmColorSurfaceInit(&dbuf[i].surf,SCE_GXM_COLOR_FORMAT_A8B8G8R8,SCE_GXM_COLOR_SURFACE_LINEAR,SCE_GXM_COLOR_SURFACE_SCALE_NONE,SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT,DISPLAY_WIDTH,DISPLAY_HEIGHT,DISPLAY_STRIDE_IN_PIXELS,dbuf[i].data);
		sceGxmSyncObjectCreate(&dbuf[i].sync);
	}
}
void gxm_swap(){
	sceGxmPadHeartbeat(&dbuf[backBufferIndex].surf, dbuf[backBufferIndex].sync);
	sceGxmDisplayQueueAddEntry(dbuf[frontBufferIndex].sync, dbuf[backBufferIndex].sync, &dbuf[backBufferIndex].data);
	frontBufferIndex = backBufferIndex;
	backBufferIndex = (backBufferIndex + 1) % DISPLAY_BUFFER_COUNT;
}
void gxm_term(){
	sceGxmTerminate();

	for (int i=0; i<DISPLAY_BUFFER_COUNT; ++i)
		sceKernelFreeMemBlock(dbuf[i].uid);
}

static void utf16_to_utf8(const uint16_t *src, uint8_t *dst) {
  int i;
  for (i = 0; src[i]; i++) {
    if ((src[i] & 0xFF80) == 0) {
      *(dst++) = src[i] & 0xFF;
    } else if((src[i] & 0xF800) == 0) {
      *(dst++) = ((src[i] >> 6) & 0xFF) | 0xC0;
      *(dst++) = (src[i] & 0x3F) | 0x80;
    } else if((src[i] & 0xFC00) == 0xD800 && (src[i + 1] & 0xFC00) == 0xDC00) {
      *(dst++) = (((src[i] + 64) >> 8) & 0x3) | 0xF0;
      *(dst++) = (((src[i] >> 2) + 16) & 0x3F) | 0x80;
      *(dst++) = ((src[i] >> 4) & 0x30) | 0x80 | ((src[i + 1] << 2) & 0xF);
      *(dst++) = (src[i + 1] & 0x3F) | 0x80;
      i += 1;
    } else {
      *(dst++) = ((src[i] >> 12) & 0xF) | 0xE0;
      *(dst++) = ((src[i] >> 6) & 0x3F) | 0x80;
      *(dst++) = (src[i] & 0x3F) | 0x80;
    }
  }

  *dst = '\0';
}

static void utf8_to_utf16(const uint8_t *src, uint16_t *dst) {
  int i;
  for (i = 0; src[i];) {
    if ((src[i] & 0xE0) == 0xE0) {
      *(dst++) = ((src[i] & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
      i += 3;
    } else if ((src[i] & 0xC0) == 0xC0) {
      *(dst++) = ((src[i] & 0x1F) << 6) | (src[i + 1] & 0x3F);
      i += 2;
    } else {
      *(dst++) = src[i];
      i += 1;
    }
  }

  *dst = '\0';
}

int showImeDialog(const char* title, const char* initial_text, char* ret_buf)
{
	// Show user input dialog. Render with vita2d wrapper.

	PSV_DEBUG("showImeDialog()");

	if (!ret_buf)
		return -1;

	vita2d_texture*	view_tex;
	char* view_tex_data;
	SceGxmSyncObject* sync;
	uint16_t input[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1] = {0};
	SceImeDialogParam param;
	bool finished = false;

	vita2d_init();

	view_tex = vita2d_create_empty_texture_format(DISPLAY_STRIDE_IN_PIXELS, 
		                                          DISPLAY_HEIGHT, 
											      (SceGxmTextureFormat)SCE_GXM_TEXTURE_FORMAT_U8U8U8U8_RGBA);
	if (!view_tex)
		return -1;
	
	view_tex_data = (char*)vita2d_texture_get_datap(view_tex);

	sceGxmSyncObjectCreate(&sync);
	sceAppUtilInit(&(SceAppUtilInitParam){}, &(SceAppUtilBootParam){});
	sceCommonDialogSetConfigParam(&(SceCommonDialogConfigParam){});
	sceImeDialogParamInit(&param);

	// Convert UTF8 to UTF16
	memset(ime_title_utf16, 0, sizeof(ime_title_utf16));
	memset(ime_initial_text_utf16, 0, sizeof(ime_initial_text_utf16));
	utf8_to_utf16((uint8_t *)title, ime_title_utf16);
	utf8_to_utf16((uint8_t *)initial_text, ime_initial_text_utf16);
 	
	param.supportedLanguages = SCE_IME_LANGUAGE_ENGLISH;
	param.languagesForced = SCE_TRUE;
	param.type = SCE_IME_DIALOG_TEXTBOX_MODE_DEFAULT;
	param.option = 0;
	param.textBoxMode = SCE_IME_DIALOG_TEXTBOX_MODE_DEFAULT;
	param.title = ime_title_utf16;
	param.maxTextLength = SCE_IME_DIALOG_MAX_TEXT_LENGTH;
	param.initialText = ime_initial_text_utf16;
	param.inputTextBuffer = input;

	// Initialize dialog.
	if (sceImeDialogInit(&param) < 0)
		return -1;
		
	while (!finished) {
		
		if (sceImeDialogGetStatus() == SCE_COMMON_DIALOG_STATUS_FINISHED) {
			// Dialog closed.
			SceImeDialogResult result = {0};
			sceImeDialogGetResult(&result);

			if (result.button == SCE_IME_DIALOG_BUTTON_ENTER){
				utf16_to_utf8(input, (uint8_t*)ret_buf);
			}
			
			finished = true;
		}
		else{
			// Get dialog draw data.
			sceCommonDialogUpdate(&(SceCommonDialogUpdateParam){{
						          NULL, view_tex_data, 0, 0,
						          DISPLAY_WIDTH,DISPLAY_HEIGHT,DISPLAY_STRIDE_IN_PIXELS, ""},
						          sync, ""});
		}

		// Draw the dialog.
		vita2d_start_drawing();
		vita2d_clear_screen();

		if (!finished)
			vita2d_draw_texture(view_tex, 0, 0);

		vita2d_end_drawing();
		vita2d_swap_buffers();

		// Slow down the loop.
		sceDisplayWaitVblankStart();
	}

	sceImeDialogTerm();
	vita2d_free_texture(view_tex);
	
	return 0;
}

int showImeDialog2(const char* title, const char* initial_text, char* ret_buf)
{
	// Show user input dialog. Render with gxm.

	PSV_DEBUG("showImeDialog2()");

	if (!ret_buf)
		return -1;

	uint16_t input[SCE_IME_DIALOG_MAX_TEXT_LENGTH + 1] = {0};
	SceImeDialogParam param;
	bool finished = false;

	sceAppUtilInit(&(SceAppUtilInitParam){}, &(SceAppUtilBootParam){});
	sceCommonDialogSetConfigParam(&(SceCommonDialogConfigParam){});

	gxm_init();

	sceImeDialogParamInit(&param);

	// Convert UTF8 to UTF16
	memset(ime_title_utf16, 0, sizeof(ime_title_utf16));
	memset(ime_initial_text_utf16, 0, sizeof(ime_initial_text_utf16));
	utf8_to_utf16((uint8_t *)title, ime_title_utf16);
	utf8_to_utf16((uint8_t *)initial_text, ime_initial_text_utf16);
 	
	param.supportedLanguages = SCE_IME_LANGUAGE_ENGLISH;
	param.languagesForced = SCE_TRUE;
	param.type = SCE_IME_DIALOG_TEXTBOX_MODE_DEFAULT;
	param.option = 0;
	param.textBoxMode = SCE_IME_DIALOG_TEXTBOX_MODE_DEFAULT;
	param.title = ime_title_utf16;
	param.maxTextLength = SCE_IME_DIALOG_MAX_TEXT_LENGTH;
	param.initialText = ime_initial_text_utf16;
	param.inputTextBuffer = input;

	// Initialize dialog.
	if (sceImeDialogInit(&param) < 0)
		return -1;
		
	while (!finished) {
		
		if (sceImeDialogGetStatus() == SCE_COMMON_DIALOG_STATUS_FINISHED) {
			// Dialog closed.
			SceImeDialogResult result = {0};
			sceImeDialogGetResult(&result);

			if (result.button == SCE_IME_DIALOG_BUTTON_ENTER){
				utf16_to_utf8(input, (uint8_t*)ret_buf);
			}
			
			// Clear current screen buffer
			memset(dbuf[0].data, 0xff000000, DISPLAY_HEIGHT*DISPLAY_STRIDE_IN_PIXELS*4);
			
			finished = true;
		}
		else{
			// Get dialog draw data.
			sceCommonDialogUpdate(&(SceCommonDialogUpdateParam){{
			                      NULL,dbuf[backBufferIndex].data,0,0,
			                      DISPLAY_WIDTH,DISPLAY_HEIGHT,DISPLAY_STRIDE_IN_PIXELS,""},
			                      dbuf[backBufferIndex].sync, ""});
		}

		// Draw the dialog.
		gxm_swap();

		// Slow down the loop.
		sceDisplayWaitVblankStart();
	}

	sceImeDialogTerm();
	gxm_term();
	
	return 0;
}

int isDialogRunning()
{
    SceCommonDialogStatus dialogStatus = sceImeDialogGetStatus();
    return (dialogStatus == SCE_COMMON_DIALOG_STATUS_RUNNING);
}