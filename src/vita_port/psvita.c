
#include "psvita.h"
#include "ime_dialog.h"
#include "resources.h"
#include <dirent.h>
#include <psp2/kernel/threadmgr.h> 
#include <psp2/io/fcntl.h>  
#include <vita2d.h>
#include <psp2/ctrl.h>
#include <psp2/power.h> 

#define DISPLAY_WIDTH  960
#define DISPLAY_HEIGHT 544
#define SCE_ERROR_ERRNO_EEXIST 0x80010011

static int	fileExist(const char* path_to_file);
static int	dirExist(const char* path_to_dir);
static void waitForUserAck();
int			psvShowSplash();
int		    mkdir(const char* path, mode_t mode);

// Increase heap size.
int _newlib_heap_size_user = 64 * 1024 * 1024;

int psvInit()
{
	// Check that data files exist.
	int data_files = dirExist("ux0:data/worminator/data");
	
	if (!data_files){
		
		vita2d_init();
		vita2d_texture* error_msg = vita2d_load_PNG_buffer(img_error_data_files);
	
		if (!error_msg)
			return -1;

		vita2d_start_drawing();
		vita2d_clear_screen();
	    vita2d_draw_texture(error_msg, 330, 213); 
		vita2d_end_drawing();
		vita2d_swap_buffers();

		waitForUserAck();

		vita2d_free_texture(error_msg);
		return -1;
	}
 	
	// Create save folder
	if (!dirExist(PSV_SAVE_DIR))
		mkdir(PSV_SAVE_DIR, 0);

	return 0;
}

int psvInputDialog(const char* title, const char* initial_text, char* ret_buf)
{
	//PSV_DEBUG("psvInputDialog()");
	return showImeDialog(title, initial_text, ret_buf);
}

int psvChangeClockSpeed(int speed)
{
	if (speed == 444){
		// Overclock to 444MHz
		scePowerSetArmClockFrequency(444);
		scePowerSetBusClockFrequency(222);
		scePowerSetGpuClockFrequency(222);
		scePowerSetGpuXbarClockFrequency(166);
	}
	else{
		// Default to 333MHz
		scePowerSetArmClockFrequency(333);
		scePowerSetGpuClockFrequency(222);
		scePowerSetBusClockFrequency(222);
		scePowerSetGpuXbarClockFrequency(166);
	}

	return 0;
}

/* Missing functions in Vita SDK */

int chdir(const char *path)
{
	return 0;
}

int mkdir(const char* path, mode_t mode)
{
	int ret = sceIoMkdir(path, 0777); // Don't put mode parameter here. Give always full permissions.
	
	if (ret < 0){
		// Don't return error if dir already exist
		if (!(dirExist(path) && ret == (int)SCE_ERROR_ERRNO_EEXIST)){
			return -1;
		}
	}

	return 0;
}

int rmdir(const char *path)
{
	if (sceIoRemove(path) < 0)
		return -1;

	return 0;
}

//int usleep(long usec)
//{
//    sceKernelDelayThread(usec);
//    return 0;
//}

int usleep(useconds_t usec)
{
    sceKernelDelayThread(usec);
    return 0;
}

uid_t getuid(void)
{
    return 0;
}


/* Privates */

static int dirExist(const char* path_to_dir)
{
	SceUID fd = sceIoDopen(path_to_dir); 

	if(fd < 0){
		return 0;
	}

	sceIoDclose(fd);

	return 1;
}

static int fileExist(const char* path_to_file)
{
	SceUID fd = sceIoOpen(path_to_file, SCE_O_RDONLY, 0777);
	
	if(fd < 0)
		return 0;

	sceIoClose(fd);

	return 1;
}


static void waitForUserAck()
{
	SceCtrlData ctrl;

	while (1) {
		sceCtrlReadBufferPositive(0, &ctrl, 1);
		
		if (ctrl.buttons == SCE_CTRL_CROSS)
			return;
	}
}

