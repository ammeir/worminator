/* Worminator Win32 Game Engine */
/* This program copyright 2001 by David Layne */
/* Last modified: 10:00 PM 7/6/01 */

/*
 * to do:
 * -add more lense effects
 * -contra triggers
 * -implement shotgun 'spread effect'
 * -fix tank graphics for all skins
 * -fix potential bug in level 14 where you can jump over the dynamite and get stuck without the tank
 * -make sure lifeforces are spawning correctly, and add red particles
 *
 * -add more bfgs to the game, and check on overall weapon distribution
 * -implement ingame readme function
 * -make a console :)
 * -DESTROY ALL BITMAPS!  MOST ARE LEFT STILL ALLOCATED!
 * -In fact, go over all dynamic memory.  Study the text message code as well, it may be problematic
 * -Editor is loaded with re-allocated dynamic memory as well :(
 */

/**************\
|** INCLUDES **|
\**************/
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "allegro.h"


#ifdef __CHAR_UNSIGNED__
#error "char must _not_ be unsigned for gcc add -fsigned-char to your CFLAGS"
#endif

// My files
#ifdef __unix__
#include "unix.h"
#endif

#ifdef PSVITA
#include "psvita.h"
extern void psv_set_app_dir(const char* dir);
extern void psv_install_render_timer();
extern void psv_remove_render_timer();
#endif

#include "prototypes.h"	// All function prototypes
#include "wormdata.h"	// Allegro data file header
#include "globals.h"	// Most global variables and typedefs needed to run worminator
#include "infodefs.h"	// Declarations describing info tiles
#include "grphdefs.h"	// Declarations describing graphical property tiles
#include "load_utils.h"	// Functions dealing with loading / saving
#include "text.h"	// Text routines and a global table
#include "sound.h"	// A couple special sound routines
#include "sprites.h"	// All sprite functions and declarations
#include "player.h"	// Functions dealing with the worminator
#include "animation.h"	// Functions to handle tile animation
#include "scripts.h"	// Introduction movies and special level events, as well as campaign game stuff
#include "demo.h"	// Functions for recording and playing demos
#include "editor.h"	// The integrated level editor
#include "menus.h"	// All the menu code for worminator
#include "beams.h"	// Functions to deal with beam objects


#define WORMINATOR_VERSION "3.0:  Please report bugs!"


void fade_in_pal(void *palette, int delay)
{
	set_palette(palette);
}

void fade_out_pal(int delay)
{
}

void blit_to_screen(BITMAP *bmp)
{
	acquire_screen();
	blit(bmp, screen, 0, 0, 0, (bmp->h == 200 ? 20 : 0), bmp->w, bmp->h);
	release_screen();
}

void stretch_blit_to_screen(BITMAP *bmp, int src_x, int src_y, int src_w, int src_h, int dst_x, int dst_y, int dst_w, int dst_h)
{
	acquire_screen();
	stretch_blit(bmp, screen, src_x, src_y, src_w, src_h, dst_x, dst_y, dst_w, dst_h);
	release_screen();
}



void savedisplay()
{
	if (screenbackup != NULL) destroy_bitmap(screenbackup);
	screenbackup = create_bitmap_ex(8, screen_width, screen_height);
	blit(swap_buffer, screenbackup, 0, 0, 0, 0, screen_width, screen_height);
}

void greenify()
{
	RGB newcol, oldcol;
	int i, avg = 0;;

	newcol.r = 0;
	newcol.b = 0;
	newcol.filler = 0;	// silence warnings.

	//game_is_running = FALSE;
	select_palette(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);

	for (i = 0; i < 256; i++) {
		avg = 0;
		get_color(i, &oldcol);
		avg += oldcol.r;
		avg += oldcol.g;
		avg += oldcol.b;
		avg /= 3;
		newcol.g = avg;
		destpal[i] = newcol;
		srcpal[i] = oldcol;
	}

	unselect_palette();

	pal_time = 64;

	//game_is_running = TRUE;
}

void little_tinge()
{
	RGB newcol, oldcol;
	int i, avg = 0;;

	if (pal_time < 0) {
		pal_time = -32; return;
	}
	if (pal_time > 0) return;

	game_is_running = FALSE;

	newcol.r = 63;
	newcol.g = 63;
	newcol.b = 0;
	newcol.filler = 0;	// silence warnings.

	for (i = 0; i < 256; i++) {
		avg = 0;
		get_color(i, &oldcol);
		/*avg += oldcol.r;
		 * avg += oldcol.g;
		 * avg += oldcol.b;
		 * avg /= 3;
		 * newcol.r = avg;
		 * newcol.g = avg;*/
		destpal[i] = oldcol;
		srcpal[i] = newcol;
	}

	pal_time = -16;

	game_is_running = TRUE;
}

void redify()
{
	RGB newcol, oldcol;
	int i, avg = 0;;

	newcol.g = 0;
	newcol.b = 0;

	for (i = 0; i < 256; i++) {
		avg = 0;
		get_color(i, &oldcol);
		avg += oldcol.r;
		avg += oldcol.g;
		avg += oldcol.b;
		avg /= 3;
		newcol.r = avg;
		set_color(i, &newcol);
	}
}

void darken(char always)
{
	RGB newcol, oldcol;
	int i, avg = 0;;

	newcol.r = 0;
	newcol.b = 0;

	if (pal_time != 0 && !always) return;

	if (!always) {
		get_palette(temppal);
		select_palette(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		for (i = 0; i < 256; i++) {
			get_color(i, &oldcol);
			if (oldcol.r > temppal[i].r || oldcol.g > temppal[i].g || oldcol.b > temppal[i].b) {
				unselect_palette();
				return;
			}
		}
		unselect_palette();
	}

	select_palette(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);

	for (i = 0; i < 256; i++) {
		avg = 0;
		get_color(i, &oldcol);
		newcol.r = oldcol.r / 2;
		newcol.g = oldcol.g / 2;
		newcol.b = oldcol.b / 2;
		newcol.filler = 0;	// silence warnings.
		destpal[i] = newcol;
		srcpal[i] = oldcol;
	}

	unselect_palette();

	pal_time = 64;
}

void restore_palette(char always)
{
	int i, count = 0;

	if (pal_time != 0 && !always) return;

	get_pallete(srcpal);
	select_palette(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	for (i = 0; i < 256; i++) get_color(i, &destpal[i]);
	unselect_palette();
	pal_time = 64;

	if (always) return;

	for (i = 0; i < 256; i++) if (destpal[i].r == srcpal[i].r && destpal[i].g == srcpal[i].g && destpal[i].b == srcpal[i].b) count++;
	if (count == 256) pal_time = 0;
}

void new_wormcode(int i)
{
	wormcode[i][0] = rand() % (int)(screen_width * .8);	// X
	wormcode[i][1] = -200;					//rand() % screen_width;  // Y
	wormcode[i][2] = 1 + (rand() % 200) / 100.0;		// Speed
}

void generate_wormcode()
{
	int i;

	for (i = 0; i < 64; i++) new_wormcode(i);
}

void update_wormcode()
{
	int i;

	for (i = 0; i < 64; i++) {
		wormcode[i][1] += wormcode[i][2];
		if (wormcode[i][1] > (screen_height * .96)) new_wormcode(i);
	}
}

void draw_wormcode()
{
	int i;

	for (i = 0; i < 64; i++)
		draw_trans_sprite(double_buffer, worminator_data_file[WORMCODE].dat, wormcode[i][0], wormcode[i][1]);
}

/**************************\
|** GAME TIMING FUNCTION **|
\**************************/
void increment_speed_counter()
{
	if (game_is_running == TRUE) {
		speed_counter++;
		if (speed_counter > 256) speed_counter = 256;
	} else {
		speed_counter = 0;
		idle_counter++;
		if (idle_counter > 8192) idle_counter = 0;
		idle_speed_counter++;
		//	if (idle_speed_counter > 256) idle_speed_counter = 256;
	}
}
END_OF_FUNCTION(increment_speed_counter);


//int psvShowSplash()
//{
//	
//    char *filename;
//    BITMAP *bmp;
//    PALETTE pal;
//
//    filename = "ux0:data/worminator/data/kyuhen_splash.png";
//  
//    /* Make Allegro aware of PNG file format. */
//    register_png_file_type();
//
//    /* Load the PNG into a BITMAP structure. */
//    bmp = load_png(filename, pal);
//    if (!bmp) {
//		//set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
//		PSV_DEBUG("Error loading file `%s'", filename);
//		return -1;
//    }
//
//    /* If we have a palette, set it. */
//    if (bitmap_color_depth(bmp) == 8){
//		PSV_DEBUG("Setting palette...");
//		set_palette(pal);
//	}
//
//    /* Show it on the screen. */
//
//	clear(screen);
//	acquire_screen();
//	stretch_blit(bmp, screen, 0, 0, 960, 544, 0, 0, 512, 384);
//	release_screen(); 
//
//    destroy_bitmap(bmp);
//    return 0;
//}

/*******************\
|** MAIN FUNCTION **|
\*******************/
int main(int argc, char *argv[])
{
#ifdef ALLEGRO_WINDOWS
	int i;
	// Process all command line switches
	if (argc > 0) {
		for (i = 1; i < argc; i++) {
			if (strncasecmp(argv[i], "safe", 7) == 0) safe_mode = TRUE;
			else if (strncasecmp(argv[i], "-safe", 8) == 0) safe_mode = TRUE;
			else if (strncasecmp(argv[i], "/safe", 8) == 0) safe_mode = TRUE;
			/*else if (strncasecmp(argv[i], "skipintro", 9) == 0) skip_intro = TRUE;
			 * else if (strncasecmp(argv[i], "-skipintro", 10) == 0) skip_intro = TRUE;
			 * else if (strncasecmp(argv[i], "/skipintro", 10) == 0) skip_intro = TRUE;
			 * else if (strncasecmp(argv[i], "sissy", 5) == 0) sissy_mode = TRUE;
			 * else if (strncasecmp(argv[i], "-sissy", 6) == 0) sissy_mode = TRUE;
			 * else if (strncasecmp(argv[i], "/sissy", 6) == 0) sissy_mode = TRUE;
			 * else if (strncasecmp(argv[i], "hardcore", 8) == 0) hardcore_mode = TRUE;
			 * else if (strncasecmp(argv[i], "-hardcore", 9) == 0) hardcore_mode = TRUE;
			 * else if (strncasecmp(argv[i], "/hardcore", 9) == 0) hardcore_mode = TRUE;*/
		}
	}
#endif

	PSV_DEBUG("main()");

	// This dummy value indicates that no level is loaded
	current_level = -69;

	// Make sure that update_animations doesn't loop over uninitialised data
	tile_animations[0][0] = 255;	// Disable animations

	// Clear the console
	clear_console();

	// Run through all the preliminiary setup and stuff
	initialize();

	// Play the introduction, unless the skip flag is set
	if (!wormy_config.skip_intro) {
		//PSV_DEBUG("Playing introduction");
		add_console_line("Playing introduction.");
		play_introduction();
	} else {
		add_console_line("Skipping introduction.");
	}

	// First fade the introduction screen or any previous splash screens (like kyuhen).
	// Fades from the current palette to a solid black palette, at the 
    //specified speed (1 is the slowest, 64 is instantaneous). 
	fade_out(16); 
	// Draw the tittle screen. Note that it won't be yet visible until we fade in.
	stretch_blit(worminator_data_file[MAIN_TITLE_SCREEN].dat,
		        swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
	blit_to_screen(swap_buffer);
	// Wait on black screen a bit.
	rest(500); 
	// Now show the tittle screen.
	fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 16);

	// Main menu
	//add_console_line("Starting up menu.");
	wormy_menu();
	//add_console_line("Menu terminated.");
	//add_console_line(" ");

	int i = 0;

	//add_console_line("Starting core loop.");

	do {	// Start core game loop

		game_is_running = TRUE;

		while (speed_counter > 0 && time_to_quit == FALSE) {
			game_is_running = TRUE;
			handle_input();

			if (quake_timer > 0) quake_timer--;
			if (player.timer > 0) player.timer--;
			
			speed_counter--;
		}

		render_map();

	} while (time_to_quit == FALSE);// End core game loop

	add_console_line("Core loop complete, beginning shutdown.");
	add_console_line("Shutting down Worminator 3.");

	// Save the configuration
	add_console_line("Saving configuration.");
	wormy_config.screen_width = screen_width;
	wormy_config.screen_height = screen_height;
	save_config();
	add_console_line("Configuration saved.");

	// Shut down allegro library
	add_console_line("Shutting down allegro.");
	remove_keyboard();
	remove_mouse();
	remove_sound();
	remove_timer();

	// Clean out some memory
	add_console_line("Deallocating memory.");
	// Engine bitmaps
	if (screen_buffer != NULL) destroy_bitmap(screen_buffer);
	if (double_buffer != NULL) destroy_bitmap(double_buffer);
	if (stretch_buffer != NULL) destroy_bitmap(stretch_buffer);
	if (FSAA_buffer != NULL) destroy_bitmap(FSAA_buffer);
	if (worminator_tiles != NULL) destroy_bitmap(worminator_tiles);
	if (spawned_sprite != NULL) destroy_bitmap(spawned_sprite);
	if (screenbackup != NULL) destroy_bitmap(screenbackup);
	// Editor bitmaps
	if (worminator_info_tiles != NULL) destroy_bitmap(worminator_info_tiles);
	if (worminator_graphical_properties_tiles != NULL) destroy_bitmap(worminator_graphical_properties_tiles);
	if (ed_double_buffer != NULL) destroy_bitmap(ed_double_buffer);
	// Datafile
	if (worminator_data_file != NULL) unload_datafile(worminator_data_file);
	add_console_line("Memory freed.");

	add_console_line("Bye bye!");

	rest(4000);


	return 0;	// Return ok
}
END_OF_MAIN()

/******************\
|** HANDLE INPUT **|
\******************/
void handle_input()
{
	int i;

	// Console input code here!
	if (key[KEY_TILDE]) {
		if (console_key_state == 0) console_key_state = 1;
		else if (console_key_state == 1) console_key_state = 2;
	} else {
		console_key_state = 0;
	}

	if (console_key_state == 1) {
		inputString[0] = 0;	// Clean out input string
		while (keypressed()) readkey();
		if (console_state == 0) console_state = 1;
		else if (console_state == 1) console_state = 3;
		else if (console_state == 2) console_state = 3;
		else if (console_state == 3) console_state = 1;
	}

	if (console_state == 1 || console_state == 2) {
		if (console_position < 100) console_position++;
		else console_state = 2;
	} else if (console_state == 0 || console_state == 3) {
		if (console_position > -1) console_position--;
		else console_state = 0;
	}

	if (console_state == 1 || console_state == 2)
		getConsoleInput();

	// Do some idle timer junk
	idle_timer++;
	if (idle_timer > 16384) 
		idle_timer = 16384;

	for (i = 0; i < KEY_MAX; i++){
		if (key[i]) idle_timer = 0;

		// Lock down/read the keys
		lockkey[i] = key[i];
	}

	// Lock down/read the keys
	//for (i = 0; i < KEY_MAX; i++) lockkey[i] = key[i]; 

	// Update the input buffer
	if (playing_demo) {
		read_bytes((unsigned char *)input_buffer, sizeof(input_buffer), demofile);
	} else if (console_state == 1 || console_state == 2) {
		for (i = 0; i < 28; i++)
			input_buffer[i] = FALSE;
	} else {
		for (i = 0; i < 28; i++) {

#ifdef PSVITA
			// Vita doesn't have enough buttons for all the actions we need.
			// Use combo keys for stats and change ammo.
			// stats = triangle + left trigger.
			// change ammo = triangle + right trigger.
			if (i == INPUT_STATS){
				if (lockkey[KEY_BACKSPACE] && lockkey[KEY_LSHIFT]){
					input_buffer[INPUT_STATS] = 1;
					// Disable left trigger action.
					input_buffer[INPUT_BULLIT_TIME] = 0; 
				}
				else
					input_buffer[INPUT_STATS] = 0;
				
				continue;
			}
			if (i == INPUT_CHANGE_AMMO_TYPE){
				
				if (lockkey[KEY_BACKSPACE] && lockkey[KEY_RSHIFT]){
					input_buffer[INPUT_CHANGE_AMMO_TYPE] = 1;
					// Disable right trigger action.
					input_buffer[INPUT_WEAPON_NEXT] = 0;
				}
				else
					input_buffer[INPUT_CHANGE_AMMO_TYPE] = 0;
				
				continue;
			}
#endif
			
			if (wormy_config.controls[i] != -1 && lockkey[wormy_config.controls[i]]) 
				input_buffer[i] = TRUE;
			else if (wormy_config.secondary_controls[i] != -1 && lockkey[wormy_config.secondary_controls[i]]) 
				input_buffer[i] = TRUE;
			else 
				input_buffer[i] = FALSE;
		}
	}

	// Cheats
	if (wormy_config.godly_mode == TRUE) {
		hurt_player(10);
		update_player_stats();
	}
	if (wormy_config.wealth_mode == TRUE) {
		player.bullets_left = 400, player.shells_left = 50, player.rockets_left = 75, player.cells_left = 75, player.gasoline_left = 100, player.mortars_left = 25, player.plasma_left = 10, player.mines_left = 15, player.chainsaw_duration_left = 666, player.plasma_bullets_left = 250, player.explosive_shells_left = 25, player.micro_nukes_left = 1, player.walking_mines_left = 10, player.rails_left = 50, player.spikestars_left = 9, player.has_red_keycard = TRUE, player.has_green_keycard = TRUE, player.has_blue_keycard = TRUE;
		update_player_stats();
	}


	// Save keystates if recording a demo
	if (recording_demo) {
		i = 0; write_dword((unsigned int *)&i, demofile);
		write_bytes((unsigned char *)input_buffer, sizeof(input_buffer), demofile);
		rotate_sprite(swap_buffer, worminator_data_file[WORMINATOR_BFG].dat,
			      0, 0, itofix(global_timer * 16));
	}

	// Go to the main menu if nessicary and allow for hotkeys
	if (!playing_demo && !recording_demo) {
		if (key[KEY_ESC]) {
			wormy_menu();
			return;
		}
		if (key[KEY_F1]) {
			display_snapshot(0, FALSE);
			return;
		}
		if (key[KEY_F2]) {
			if (wormy_dummy_box(3, FALSE) == TRUE){
				save_game(save_game_filename());
			}
			return;
		}
		if (key[KEY_F3] && !access(save_game_filename(), R_OK)) {
			if (wormy_dummy_box(1, FALSE) == TRUE) load_saved_game(save_game_filename());
			return;
		}
		if (key[KEY_F4]) {
			if (wormy_dummy_box(2, FALSE) == TRUE)
				load_map(1, NULL, 1);
			return;
		}
		if (key[KEY_ENTER/*KEY_F10]*/]) {
			if (wormy_dummy_box(0, FALSE) == TRUE) time_to_quit = TRUE;
			return;
		}
		if (key[KEY_F11]) {
			wormy_options_menu();
			borders_dirty = TRUE;
			return;
		}
		if (key[KEY_F12]) {
			start_new_custom_game();
			return;
		}
		if (key[KEY_PRTSCR]) save_screenshot();
	} else if (recording_demo) {
		if (key[KEY_ESC]) {
			stop_demo_recording();
			wormy_menu();
			return;
		}
	}

	//if (key[KEY_H]) {redify(); while (key[KEY_H]) {} }

	// Update key states //////////////////////////////////////////////////////////////////////
	if (input_buffer[INPUT_JUMP]) {
		if (jump_key_state == 0) jump_key_state = 1;
		else if (jump_key_state == 1) jump_key_state = 2;
	} else {
		jump_key_state = 0;
	}

	if (input_buffer[INPUT_UP]) {
		if (up_key_state == 0) up_key_state = 1;
		else if (up_key_state == 1) up_key_state = 2;
	} else {
		up_key_state = 0;
	}

	if (input_buffer[INPUT_CHANGE_AMMO_TYPE]) {
		if (ammo_type_key_state == 0) ammo_type_key_state = 1;
		else if (ammo_type_key_state == 1) ammo_type_key_state = 2;
		override_autoswitch = TRUE;
	} else {
		ammo_type_key_state = 0;
	}

	if (input_buffer[INPUT_WEAPON_LAST]) {
		if (last_weapon_key_state == 0) last_weapon_key_state = 1;
		else if (last_weapon_key_state == 1) last_weapon_key_state = 2;
	} else {
		last_weapon_key_state = 0;
	}

	if (input_buffer[INPUT_WEAPON_NEXT]) {
		if (next_weapon_key_state == 0) next_weapon_key_state = 1;
		else if (next_weapon_key_state == 1) next_weapon_key_state = 2;
	} else {
		next_weapon_key_state = 0;
	}

	// Bullet time stuff
	if (wormy_config.cool_mode) bullet_time = 4096;
	if (wormy_config.may_tricks_mode == TRUE) {
		if (input_buffer[INPUT_BULLIT_TIME]) {
			if (bullet_time_key_state == 0) bullet_time_key_state = 1;
			else if (bullet_time_key_state == 1) bullet_time_key_state = 2;
		} else {
			bullet_time_key_state = 0;
		}

		if (bullet_time_key_state == 1) {
			if (bullet_time_engaged == FALSE && bullet_time > 0) {
				change_game_speed(GAME_SPEED_BULLET_TIME);
				bullet_time_engaged = TRUE;
				add_text_message_if_new("Bullit Time!");
				if (wormy_config.cool_mode == TRUE) greenify();
			} else if (bullet_time_engaged == TRUE) {
				change_game_speed(wormy_config.game_speed);
				bullet_time_engaged = FALSE;
				if (wormy_config.cool_mode == TRUE) restore_palette(TRUE);
			}
		}

		if (bullet_time_engaged == TRUE) {
			bullet_time -= 8;
			if (bullet_time <= 0) {
				change_game_speed(wormy_config.game_speed);
				bullet_time_engaged = FALSE;
				set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
			}
		} else {
			bullet_time++;
			if (bullet_time > 4096) bullet_time = 4096;
		}
	} else {
		if (bullet_time_engaged == TRUE) {
			change_game_speed(wormy_config.game_speed);
			bullet_time_engaged = FALSE;
			set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		}
		bullet_time = 0;
	}

	// Update togglers ////////////////////////////////////////////////////////////////////////
	if (input_buffer[INPUT_MAP]) {
		if (toggle_map == FALSE) toggle_map = TRUE;
		else if (toggle_map == TRUE) toggle_map = 69;
	} else {
		toggle_map = FALSE;
	}
	if (input_buffer[INPUT_STATS]) {
		if (toggle_stats == FALSE) toggle_stats = TRUE;
		else if (toggle_stats == TRUE) toggle_stats = 69;
	} else {
		toggle_stats = FALSE;
	}

	// Update toggles /////////////////////////////////////////////////////////////////////////
	if (toggle_map == TRUE) {
		if (display_map == TRUE) display_map = FALSE;
		else display_map = TRUE;
	}
	if (toggle_stats == TRUE) {
		if (display_stats == TRUE) display_stats = FALSE;
		else display_stats = TRUE;
	}

	// Update looking up/down info.
	if (input_buffer[INPUT_UP] && input_buffer[INPUT_LOOK] && player.in_water == FALSE && player.on_ground == TRUE && player.x_acceleration == 0) {
		if (look_timer > 0) look_timer = -31;
		if (look_shift < 0 && look_timer > -31) look_timer = -31;
		look_timer--;
		if (look_timer < -32) {
			look_shift--;
			if (look_shift < -48) look_shift = -48;
			look_timer = -31;
		}
	} else if (input_buffer[INPUT_DOWN] && input_buffer[INPUT_LOOK] && player.in_water == FALSE && player.on_ground == TRUE && player.x_acceleration == 0) {
		if (look_timer < 0) look_timer = 31;
		if (look_shift > 0 && look_timer < 31) look_timer = 31;
		look_timer++;
		if (look_timer > 32) {
			look_shift++;
			if (look_shift > 48) look_shift = 48;
			look_timer = 31;
		}
	} else if (((!input_buffer[INPUT_DOWN] && !input_buffer[INPUT_UP]) && !input_buffer[INPUT_LOOK]) || player.in_water == TRUE || player.on_ground == FALSE || player.x_acceleration != 0) {
		if (look_timer > 1) look_timer = 0;
		if (look_timer < -1) look_timer = 0;
		if (look_shift != 0) {
			look_timer++;
			if (look_timer > 1) {
				if (look_shift > 0) look_shift--;
				else look_shift++;
				look_timer = 0;
			}
		} else {
			look_timer = 0;
		}
	}

	if (input_buffer[INPUT_FIRE]) {
		fire_key_state++; override_autoswitch = FALSE;
	} else if (fire_key_state > 1) {
		fire_key_state++;
	} else {
		fire_key_state = 0;
	}

	// Update player water state and spawn splashes / bubbles
	if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 8) / 16)][(int)((player.y_position_in_pixels + 8) / 16)] == water_info_tile || worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 24) / 16)][(int)((player.y_position_in_pixels + 8) / 16)] == water_info_tile) {
		if (player.in_water == FALSE) spawn_sprite(WATER_SPLASH_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels - 20, 0, 0, 0);
		if (rand() % 64 == 32) spawn_sprite(WATER_BUBBLE_SPRITE, player.x_position_in_pixels + 16, player.y_position_in_pixels + 16, 0, 0, 0);
		player.in_water = TRUE, player.on_ground = TRUE;
	} else {
		if (player.in_water == TRUE && player.y_acceleration < 0) {
			player.turbo_launch = TRUE;  player.y_acceleration = -1.7;
		}
		player.in_water = FALSE;
	}

	// Update fire key state
	if (wormy_config.super_turkey_mode == TRUE) {
		if (fire_key_state > 64) fire_key_state = 0;
	} else if (player.skin == TANK_SKIN && fire_key_state > 64) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 1 && fire_key_state > 10) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 2 && fire_key_state > 128) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 3 && fire_key_state > 64) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 4 && fire_key_state > 128) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 5 && fire_key_state > 32) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 6 && fire_key_state > 46) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 7 && fire_key_state > 96) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 8 && fire_key_state > 384) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 9 && fire_key_state > 256) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 10 && fire_key_state > 9) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 11 && fire_key_state > 512) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 12 && fire_key_state > 1024) {
		fire_key_state = 0;
	} else if (player.skin != TANK_SKIN && player.current_weapon == 0) {
		fire_key_state = 0;
	}
	if (wormy_config.rapid_fire_mode == TRUE && fire_key_state > 5 && player.current_weapon < 8 && player.current_weapon != 6) fire_key_state = 0;

	// Tank recoil and tred damage
	if (player.skin == TANK_SKIN && fire_key_state == 2) {
		if (player.direction == 0) player.x_acceleration = -.75;
		else player.x_acceleration = .75;
	}
	if (player.skin == TANK_SKIN) {
		if (player.direction == 0 && player.x_acceleration < 0) do_damage(player.x_position_in_pixels, player.y_position_in_pixels + 48, 1, 16, 1234);
		if (player.direction == 1 && player.x_acceleration > 0) do_damage(player.x_position_in_pixels + 95, player.y_position_in_pixels + 48, 1, 16, 1234);
	}

	// Select a weapon if one of the number keys is pressed
	if (wormy_config.autoswitch_weapons == TRUE && check_weapon_autoswitch() != -666) desired_weapon = check_weapon_autoswitch();
	if (input_buffer[INPUT_WEAPON_0]) {
		desired_weapon = 0; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_1]) {
		desired_weapon = 1; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_2]) {
		desired_weapon = 2; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_3]) {
		desired_weapon = 3; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_4]) {
		desired_weapon = 4; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_5]) {
		desired_weapon = 5; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_6]) {
		desired_weapon = 6; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_7]) {
		desired_weapon = 7; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_8]) {
		desired_weapon = 8; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_9]) {
		desired_weapon = 9; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_10]) {
		desired_weapon = 10; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_11]) {
		desired_weapon = 11; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_12]) {
		desired_weapon = 12; override_autoswitch = TRUE;
	}
	if (input_buffer[INPUT_WEAPON_BEST]) {
		desired_weapon = best_weapon();
		override_autoswitch = TRUE;
	}
	if (last_weapon_key_state == 1) desired_weapon = last_weapon();
	if (next_weapon_key_state == 1) desired_weapon = next_weapon();

	// Switch weapons/ammo types if possible
	if (fire_key_state == 0 && desired_weapon >= 0 && player.current_weapon != desired_weapon) {
		player.current_weapon = desired_weapon;
		if (player.current_weapon == 10 && player.in_water == FALSE && player.chainsaw_duration_left > 0) play_sample(worminator_data_file[CHAINSAW_EQUIP_SOUND].dat, 255, 128, 1000, FALSE);
		player.ammo_type = 0;
		update_player_stats();
	}
	/*else if (fire_key_state == 0 && desired_weapon < 0 && !(player.current_weapon == -desired_weapon && player.ammo_type == 1)) {
	 *      player.current_weapon = -desired_weapon;
	 *      //if (player.current_weapon == 10 && player.in_water == FALSE && player.chainsaw_duration_left > 0) play_sample(worminator_data_file[CHAINSAW_EQUIP_SOUND].dat, 255, 128, 1000, FALSE);
	 *      player.ammo_type = 1;
	 *      update_player_stats();
	 *      }*/
	if (ammo_type_key_state == 1) {
		if (player.ammo_type == 0) player.ammo_type = 1;
		else player.ammo_type = 0;
		update_player_stats();
	}

	// Start fire state running if the player is pressing the fire key
	if (fire_key_state == 0 && input_buffer[INPUT_FIRE]) fire_key_state = 1;

	// Fire off weapons that fire at the start of the animation
	if (fire_key_state == 1) {
		if (player.skin == TANK_SKIN) {
			if (player.direction == 0) spawn_sprite(TANK_SHELL_SPRITE, player.x_position_in_pixels + 48, player.y_position_in_pixels + 14, .25, 0, 0);
			else spawn_sprite(TANK_SHELL_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels + 14, -.25, 0, 1);
			if (player.in_water) play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 750, FALSE);
			else play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
		} else if (wormy_config.super_turkey_mode == TRUE) {
			if (player.current_weapon == 1) {
				if (player.direction == 0) spawn_sprite(WORMINATOR_MORTAR_SPRITE, player.x_position_in_pixels + 10, player.y_position_in_pixels + 10, 1, -1.5, 0);
				else spawn_sprite(WORMINATOR_MORTAR_SPRITE, player.x_position_in_pixels + 10, player.y_position_in_pixels + 10, -1, -1.5, 1);
				if (sprites[number_of_sprites - 1].sprite_type == WORMINATOR_MORTAR_SPRITE) sprites[number_of_sprites - 1].frame = SUPER_TURKEY_FRIED_CHICKEN;
				if (player.in_water) play_sample(worminator_data_file[MORTAR_FIRE_SOUND].dat, 255, 128, 750, FALSE);
				else play_sample(worminator_data_file[MORTAR_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
			} else if (player.current_weapon == 2) {
				if (player.direction == 0) spawn_sprite(TANK_SHELL_SPRITE, player.x_position_in_pixels + 12, player.y_position_in_pixels + 6, .15, 0, 0);
				else spawn_sprite(TANK_SHELL_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels + 6, -.15, 0, 1);
				if (sprites[number_of_sprites - 1].sprite_type == TANK_SHELL_SPRITE) {
					sprites[number_of_sprites - 1].width = 16;
					//sprites[number_of_sprites - 1].x_position_in_pixels += 16;
					sprites[number_of_sprites - 1].frame = SUPER_TURKEY_MASHED_POTATOES;
				}
				if (player.in_water) play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 750, FALSE);
				else play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
			}
		} else if (player.current_weapon == 1 && !player.is_ducking) {
			if (player.ammo_type == 0 && player.bullets_left > 0) {
				if (player.direction == 0) spawn_sprite(WORMINATOR_BULLET_SPRITE, player.x_position_in_pixels + 27, player.y_position_in_pixels + 14 + (rand() % 5), 3, 0, 0);
				else spawn_sprite(WORMINATOR_BULLET_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels + 14 + (rand() % 5), -3, 0, 1);
				if (player.in_water) play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 750, FALSE);
				else play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.bullets_left--;
				update_player_stats();
			} else if (player.ammo_type == 1 && player.plasma_bullets_left > 0) {
				if (player.direction == 0) spawn_sprite(WORMINATOR_PLASMA_BULLET_SPRITE, player.x_position_in_pixels + 27, player.y_position_in_pixels + 12 + (rand() % 5), 2.5, 0, 0);
				else spawn_sprite(WORMINATOR_PLASMA_BULLET_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels + 12 + (rand() % 5), -2.5, 0, 1);
				if (player.in_water) play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 750, FALSE);
				else play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.plasma_bullets_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 2 && player.is_ducking == FALSE) {
			if (player.ammo_type == 0 && player.shells_left > 0) {
				fire_shotgun();
				if (player.in_water) play_sample(worminator_data_file[SHOTGUN_FIRE_SOUND].dat, 255, 128, 750, FALSE);
				else play_sample(worminator_data_file[SHOTGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.shells_left--;
				update_player_stats();
			} else if (player.ammo_type == 1 && player.explosive_shells_left > 0) {
				fire_shotgun();
				if (player.in_water) play_sample(worminator_data_file[SHOTGUN_FIRE_SOUND].dat, 255, 128, 750, FALSE);
				else play_sample(worminator_data_file[SHOTGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.explosive_shells_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 3 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.ammo_type == 0 && player.rockets_left > 0) {
				if (player.direction == 0) spawn_sprite(WORMINATOR_ROCKET_SPRITE, player.x_position_in_pixels + 24, player.y_position_in_pixels + 11, .25, 0, 0);
				else spawn_sprite(WORMINATOR_ROCKET_SPRITE, player.x_position_in_pixels - 9, player.y_position_in_pixels + 11, -.25, 0, 1);
				play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.rockets_left--;
				update_player_stats();
			} else if (player.ammo_type == 1 && player.micro_nukes_left > 0) {
				if (player.direction == 0) spawn_sprite(WORMINATOR_MICRO_NUKE_SPRITE, player.x_position_in_pixels + 24, player.y_position_in_pixels + 9, .25, 0, 0);
				else spawn_sprite(WORMINATOR_MICRO_NUKE_SPRITE, player.x_position_in_pixels - 8, player.y_position_in_pixels + 9, -.25, 0, 1);
				play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.micro_nukes_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 4 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.rockets_left > 1) {
				if (player.direction == 0) {
					spawn_sprite(WORMINATOR_ROCKET_SPRITE, player.x_position_in_pixels + 24, player.y_position_in_pixels + 11, .25, -.15, 0);
					spawn_sprite(WORMINATOR_ROCKET_SPRITE, player.x_position_in_pixels + 24, player.y_position_in_pixels + 11, .25, .15, 0);
				} else {
					spawn_sprite(WORMINATOR_ROCKET_SPRITE, player.x_position_in_pixels - 9, player.y_position_in_pixels + 11, -.25, -.15, 1);
					spawn_sprite(WORMINATOR_ROCKET_SPRITE, player.x_position_in_pixels - 9, player.y_position_in_pixels + 11, -.25, .15, 1);
				}
				play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.rockets_left -= 2;
				update_player_stats();
			}
		} else if (player.current_weapon == 5 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.cells_left > 0) {
				fire_laser_gun();
				play_sample(worminator_data_file[LASER_GUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.cells_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 6 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.gasoline_left > 0) {
				player.gasoline_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 7 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.mortars_left > 0) {
				if (player.direction == 0) spawn_sprite(WORMINATOR_MORTAR_SPRITE, player.x_position_in_pixels + 10, player.y_position_in_pixels + 10, 1, -1.5, 0);
				else spawn_sprite(WORMINATOR_MORTAR_SPRITE, player.x_position_in_pixels + 10, player.y_position_in_pixels + 10, -1, -1.5, 1);
				if (sprites[number_of_sprites - 1].sprite_type == WORMINATOR_MORTAR_SPRITE && wormy_config.cool_mode == TRUE) sprites[number_of_sprites - 1].frame = COOL_MODE_COCONUT_MORTAR;
				play_sample(worminator_data_file[MORTAR_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.mortars_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 8 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.plasma_left > 0) play_sample(worminator_data_file[BFG_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
		} else if (player.current_weapon == 10 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.chainsaw_duration_left > 0) {
				fire_chainsaw();
				player.chainsaw_duration_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 11 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.rails_left > 0) {
				fire_railgun();
				play_sample(worminator_data_file[LASER_GUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.rails_left--;
				update_player_stats();
			}
		} else if (player.current_weapon == 12 && player.in_water == FALSE && player.is_ducking == FALSE) {
			if (player.spikestars_left > 0) {
				if (player.direction == 0) {
					spawn_sprite(WORMINATOR_SPIKESTAR_SPRITE, player.x_position_in_pixels + 27, player.y_position_in_pixels + 12, 1.25, 0, 0);
					spawn_sprite(WORMINATOR_SPIKESTAR_SPRITE, player.x_position_in_pixels + 27, player.y_position_in_pixels + 12, 1.25, 0, 0);
					spawn_sprite(WORMINATOR_SPIKESTAR_SPRITE, player.x_position_in_pixels + 27, player.y_position_in_pixels + 12, 1.25, 0, 0);
					sprites[number_of_sprites - 3].frame = WORMINATOR_SPIKESTAR_RED_FLY;
					sprites[number_of_sprites - 3].rotate_factor = 64;
					sprites[number_of_sprites - 2].frame = WORMINATOR_SPIKESTAR_BLUE_FLY;
					sprites[number_of_sprites - 2].rotate_factor = 64;
					sprites[number_of_sprites - 1].frame = WORMINATOR_SPIKESTAR_YELLOW_FLY;
				} else {
					spawn_sprite(WORMINATOR_SPIKESTAR_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels + 12, -1.25, 0, 0);
					spawn_sprite(WORMINATOR_SPIKESTAR_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels + 12, -1.25, 0, 0);
					spawn_sprite(WORMINATOR_SPIKESTAR_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels + 12, -1.25, 0, 0);
					sprites[number_of_sprites - 3].frame = WORMINATOR_SPIKESTAR_RED_FLY;
					sprites[number_of_sprites - 3].rotate_factor = 64;
					sprites[number_of_sprites - 2].frame = WORMINATOR_SPIKESTAR_BLUE_FLY;
					sprites[number_of_sprites - 2].rotate_factor = 64;
					sprites[number_of_sprites - 1].frame = WORMINATOR_SPIKESTAR_YELLOW_FLY;
				}
				play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
				player.spikestars_left -= 3;
				update_player_stats();
			}
		}
	}

	// Do damage from the flamethrower if nessicary
	if (player.skin != TANK_SKIN && player.current_weapon == 6 && player.gasoline_left > 0 && fire_key_state != 0 && !player.is_ducking) {
		if (fire_key_state >= 6 && fire_key_state <= 38) {
			if (player.direction == 0) do_damage(player.x_position_in_pixels + 32, player.y_position_in_pixels + 8, 36, 14, .15);
			else do_damage(player.x_position_in_pixels - 37, player.y_position_in_pixels + 8, 36, 14, .05);
		} else {
			if (player.direction == 0) do_damage(player.x_position_in_pixels + 32, player.y_position_in_pixels + 7, 84, 16, .15);
			else do_damage(player.x_position_in_pixels - 64, player.y_position_in_pixels + 7, 84, 16, .05);
		}
	}

	// Fire off the bfg plasma ball if nessecary
	if (player.skin != TANK_SKIN && fire_key_state == 180 && player.current_weapon == 8 && player.plasma_left > 0 && player.in_water == FALSE && !player.is_ducking) {
		if (player.direction == 0) spawn_sprite(WORMINATOR_BFG_PLASMA_BALL_SPRITE, player.x_position_in_pixels + 31, player.y_position_in_pixels + 7, .25, 0, 0);
		else spawn_sprite(WORMINATOR_BFG_PLASMA_BALL_SPRITE, player.x_position_in_pixels - 31, player.y_position_in_pixels + 7, -.25, 0, 1);
		player.plasma_left--;
		update_player_stats();
		quake_timer += 48;
	}

	// Lay a mine if nessecary
	if (player.skin != TANK_SKIN && fire_key_state == 60 && player.current_weapon == 9 && player.ammo_type == 0 && player.mines_left > 0 && player.in_water == FALSE && !player.is_ducking) {
		if (player.direction == 0) spawn_sprite(WORMINATOR_MINE_SPRITE, player.x_position_in_pixels + 21, player.y_position_in_pixels + 27, 0, 0, 0);
		else spawn_sprite(WORMINATOR_MINE_SPRITE, player.x_position_in_pixels + 1, player.y_position_in_pixels + 27, 0, 0, 1);
		player.mines_left--;
		update_player_stats();
	} else if (player.skin != TANK_SKIN && fire_key_state == 60 && player.current_weapon == 9 && player.ammo_type == 1 && player.mines_left >= 0 && player.walking_mines_left > 0 && player.in_water == FALSE && !player.is_ducking) {
		if (player.direction == 0) spawn_sprite(WORMINATOR_WALKING_MINE_SPRITE, player.x_position_in_pixels + 21, player.y_position_in_pixels + 24, .25, 0, 0);
		else spawn_sprite(WORMINATOR_WALKING_MINE_SPRITE, player.x_position_in_pixels + 1, player.y_position_in_pixels + 24, -.25, 0, 1);
		player.walking_mines_left--;
		update_player_stats();
	}

	// Player movement override tiles
	if (player.no_control == TRUE) {
		control_player();
	}
	// Water keyscan movement code
	else if (player.in_water == TRUE) {
		if (input_buffer[INPUT_UP] || (input_buffer[INPUT_JUMP] && !input_buffer[INPUT_DOWN])) {
			player.y_acceleration -= .02;
		} else if (input_buffer[INPUT_DOWN]) {
			player.y_acceleration += .02;
		} else {
			if (player.y_acceleration > 0) {
				player.y_acceleration -= .02;
				if (player.y_acceleration < 0) player.y_acceleration = 0;
			} else {
				player.y_acceleration += .02;
				if (player.y_acceleration > 0) player.y_acceleration = 0;
			}
		}
		if (input_buffer[INPUT_LEFT]) {
			player.x_acceleration -= .02, player.direction = 1;
		} else if (input_buffer[INPUT_RIGHT]) {
			player.x_acceleration += .02, player.direction = 0;
		} else {
			if (player.x_acceleration > 0) {
				player.x_acceleration -= .02;
				if (player.x_acceleration < 0) player.x_acceleration = 0;
			} else {
				player.x_acceleration += .02;
				if (player.x_acceleration > 0) player.x_acceleration = 0;
			}
		}
	}
	// This processes the Worminator's ducking action
	else if (input_buffer[INPUT_DUCK] && player.on_ground == TRUE && player.is_ducking == FALSE && player.skin != TANK_SKIN) {
		player.x_acceleration = 0;
		player.is_ducking = TRUE;
	} else if (!input_buffer[INPUT_DUCK] && player.is_ducking == TRUE) {
		player.is_ducking = FALSE;
	}
	// Normal keyscan movement code
	else if (player.is_ducking == FALSE) {
		if (up_key_state == 1) {
			if (player.on_ground == TRUE)
				if (check_player_teleport() == TRUE) return;
		}

		if ((input_buffer[INPUT_LEFT]) && !(input_buffer[INPUT_RIGHT])) {
			player.direction = 1;
			if (player.on_ground == TRUE)
				player.x_acceleration -= .05;
			else
				player.x_acceleration -= .02;
		}

		if ((input_buffer[INPUT_RIGHT]) && !(input_buffer[INPUT_LEFT])) {
			player.direction = 0;
			if (player.on_ground == TRUE)
				player.x_acceleration += .05;
			else
				player.x_acceleration += .02;
		}

		if (input_buffer[INPUT_JUMP] && player.skin != TANK_SKIN) {
			if (input_buffer[INPUT_DOWN]) push_player(512, -512);
			else if (player.on_ground == TRUE) player.y_acceleration = -1.7;
		}
		if (input_buffer[INPUT_JUMP] && player.skin == TANK_SKIN && player.on_ground == TRUE) {
			if (wormy_config.remember_skin == TRUE) player.skin = wormy_config.skin;
			else player.skin = WORMINATOR_SKIN;
			player.y_acceleration = -1.7;
			spawn_sprite(TANK_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels, 0, 0, player.direction);
			player.y_position_in_pixels -= 16;
			player.x_position_in_pixels += 32;
		}
		if (wormy_config.fly_mode && input_buffer[INPUT_JUMP] && !input_buffer[INPUT_DOWN]) player.y_acceleration = -1.7;
		if (wormy_config.super_turkey_mode && jump_key_state == 1 && !input_buffer[INPUT_DOWN]) {
			if (player.y_acceleration > 0) player.y_acceleration = 0; player.y_acceleration -= 1.2; if (player.y_acceleration < -1.7) player.y_acceleration = -1.7;
		}
	}

	// GRAVITY /////////////////////////////////////////////////////////////////////////////////
	if (player.no_control == TRUE) {
	} else if (player.in_water == FALSE) {
		if (player.y_acceleration <= -.5 && !input_buffer[INPUT_JUMP] && player.turbo_launch == FALSE) player.y_acceleration = -.5;
		player.on_ground = FALSE;
		if (player.y_acceleration != 0 && player.on_ground == FALSE) {
			if (player.y_acceleration > 0) {
				player.turbo_launch = FALSE;
				if (player.y_acceleration <= 1) {
					if (player.no_control == FALSE) push_player(0, player.y_acceleration);
				} else {
					if (player.no_control == FALSE) push_player(0, 1);
					if (player.no_control == FALSE) push_player(0, player.y_acceleration - 1);
				}
			} else if (player.y_acceleration < 0) {
				if (player.y_acceleration >= -1) {
					if (player.no_control == FALSE) push_player(0, player.y_acceleration);
				} else {
					if (player.no_control == FALSE) push_player(0, -1);
					if (player.no_control == FALSE) push_player(0, player.y_acceleration + 1);
				}
			}
		}
		player.y_acceleration += 0.02;
		if (player.on_ground == TRUE && player.y_acceleration > 1) player.y_acceleration = 1;
		else if (player.y_acceleration > 2) player.y_acceleration = 2;
		if (player.y_acceleration < -1.7 && rand() % 32 > 16) spawn_stardust(player.x_position_in_pixels + 16, player.y_position_in_pixels + 32, 0, 0, 227, 235, 4, 1);
	}

	// LEFT AND RIGHT MOVEMENT CODE /////////////////////////////////////////////////////////////
	if (player.no_control == TRUE) {
	} else if (player.in_water == FALSE && !input_buffer[INPUT_LEFT] && !input_buffer[INPUT_RIGHT]) {
		if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels) / 16)][(int)((player.y_position_in_pixels + 32) / 16)] == half_slippery_info_tile || worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 16) / 16)][(int)((player.y_position_in_pixels + 32) / 16)] == half_slippery_info_tile || worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 32) / 16)][(int)((player.y_position_in_pixels + 32) / 16)] == half_slippery_info_tile) {
			if (player.x_acceleration > 0) {
				player.x_acceleration -= .015;
				if (player.x_acceleration < 0) player.x_acceleration = 0;
			} else {
				player.x_acceleration += .015;
				if (player.x_acceleration > 0) player.x_acceleration = 0;
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels) / 16)][(int)((player.y_position_in_pixels + 32) / 16)] == mostly_slippery_info_tile || worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 16) / 16)][(int)((player.y_position_in_pixels + 32) / 16)] == mostly_slippery_info_tile || worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 32) / 16)][(int)((player.y_position_in_pixels + 32) / 16)] == mostly_slippery_info_tile || player.skin == TANK_SKIN) {
			if (player.x_acceleration > 0) {
				player.x_acceleration -= .0025;
				if (player.x_acceleration < 0) player.x_acceleration = 0;
			} else {
				player.x_acceleration += .0025;
				if (player.x_acceleration > 0) player.x_acceleration = 0;
			}
		} else {
			player.x_acceleration = 0;
		}
	}

	if (player.x_acceleration < -1) player.x_acceleration = -1;
	if (player.x_acceleration > 1) player.x_acceleration = 1;

	if (player.no_control == FALSE) push_player(player.x_acceleration, 0);

	// UP AND DOWN MOVEMENT CODE (FOR WATER ONLY) ///////////////////////////////////////////////
	if (player.in_water == TRUE && player.no_control == FALSE) {
		if (player.y_acceleration < -1) player.y_acceleration = -1;
		if (player.y_acceleration > 1) player.y_acceleration = 1;

		if (player.no_control == FALSE) push_player(0, player.y_acceleration);
	}

	if (wormy_config.freeze_mode == FALSE) update_sprites();
	if (wormy_config.freeze_mode == FALSE) update_beams();

	pick_player_frame();

	run_scripts();

	// Kill the player if need be
	if (player.frame != 0 && player.health_bars_left < 1) {
		if (playing_demo) return;
		kill_player();
		if (recording_demo) stop_demo_recording();
	}

	update_text_messages();

	// Special difficulty stuff
	if (wormy_config.difficulty == DIFFICULTY_WIMP) {
		if (hp_regen_timer == 0) {
			hurt_player(1);
			hp_regen_timer = 1;
		}
		player.bullets_left = 400;
		if (player.shells_left >= 0) player.shells_left = 50;
		if (player.rockets_left >= 0) player.rockets_left = 75;
		if (player.cells_left >= 0) player.cells_left = 75;
		if (player.gasoline_left >= 0) player.gasoline_left = 100;
		if (player.mortars_left >= 0) player.mortars_left = 25;
		if (player.mines_left >= 0) player.mines_left = 15;
		if (player.chainsaw_duration_left >= 0) player.chainsaw_duration_left = 666;
		if (player.rails_left >= 0) player.rails_left = 50;
		update_player_stats();
	} else if (wormy_config.difficulty == DIFFICULTY_EASY && global_timer == 8) {
		player.bullets_left++;
		if (player.bullets_left > 400) player.bullets_left = 400;
		update_player_stats();
	}

	// Update weather if nessicary
	if (worminator_map.weather != 0) update_weather();

	// Move around the demo sign variables
	demospin++;
	if (demospin > 255) demospin = 0;
	demox += demoxdir;
	demoy += demoydir;
	if (abs(demox) > (screen_width * .8) - 128 || demox < 0) demoxdir *= -1;
	if (demoy > (screen_height * .96) - 32 || demoy < 0) demoydir *= -1;

	// Bored animation code
	if (!playing_demo && !recording_demo && idle_timer >= 4096) {
		spawn_sprite(BORED_SPRITE, 0, 0, 0, 0, 0);
		idle_timer = 0;
	}

	// Worminatrix code
	if (!playing_demo && !recording_demo) update_wormcode();

	// Set the correct palette
	if (pal_time != 0) {
		if (pal_time > 0) {
			pal_time--;
			fade_interpolate(srcpal, destpal, temppal, 64 - pal_time, 0, 255);
			if (pal_time != 0) set_palette_range(temppal, 0, 255, FALSE);
			else set_palette_range(destpal, 0, 255, FALSE);
		} else {
			pal_time += 2;
			if (pal_time > 0) pal_time = 0;
			fade_interpolate(srcpal, destpal, temppal, 64 + pal_time, 0, 255);
			if (pal_time != 0) set_palette_range(temppal, 0, 255, FALSE);
			else set_palette_range(destpal, 0, 255, FALSE);
		}
	}

	// Suicide key
	/*if (key[KEY_K]) {
	 *      if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, player.x_position_in_pixels + 16 + ((float)((rand() % 4000) - 2000.00) / 20.00), player.y_position_in_pixels - 8 + ((float)((rand() % 4000) - 2000.00) / 20.00), 0, 0, 0);
	 *      else spawn_sprite(SMALL_EXPLOSION_SPRITE, player.x_position_in_pixels + 16 + ((float)((rand() % 4000) - 2000.00) / 20.00), player.y_position_in_pixels + ((float)((rand() % 4000) - 2000.00) / 20.00), 0, 0, 0);
	 *      }*/
}

void warp_effect(BITMAP *bmp, int x, int y, int width, int height, int amplitude)
{
	BITMAP *buf = create_bitmap_ex(8, width, height);
	int i;						// Loop index
	float radianconverter = (3.1415927 / width);	// Float that we will use to convert from degrees to radians

	blit(bmp, buf, x, y, 0, 0, width, height);	// Load the buffer

	for (i = x; i < x + width; i++) stretch_blit(buf, bmp, i - x, 0, 1, height, i, y - amplitude * sin((i - x) * radianconverter), 1, height + 2 * (amplitude * sin((i - x) * radianconverter)));

	destroy_bitmap(buf);
}

void draw_fancy_pixel(BITMAP *bmp, int x, int y, int d)
{
	int darker_color = makecol(getr(d) / 2, getg(d) / 2, getb(d) / 2);

	putpixel(bmp, x + 1, y, darker_color);
	putpixel(bmp, x - 1, y, darker_color);
	putpixel(bmp, x, y + 1, darker_color);
	putpixel(bmp, x, y - 1, darker_color);
}

void fancy_line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color)
{
	do_line(bmp, x1, y1, x2, y2, color, draw_fancy_pixel);
	line(bmp, x1, y1, x2, y2, color);
}

/****************\
|** RENDER MAP **|
\****************/
void render_map()
{
	unsigned char x_tile_loop, y_tile_loop;
	char weather_loop;
	unsigned int start_x_tile = 0, start_y_tile = 0;
	int sx = screen_x_position, sy = screen_y_position;
	int plax_x, plax_y;
	int sprite_loop;
	int i, j;
	int topx = 0, topy = 0, bottomx = worminator_map.map_width, bottomy = worminator_map.map_height;

	// Show the automap if nessicary
	if (display_map == TRUE) {
		show_map();

		// PSVITA FIX: Inconsistent map sizes.
		// If we use 256/192 for source width/height, some maps look way too small,
		// even when stretched (e.g. level 2, level 18). If we use the actual sizes,
		// then they look big but overstretched in some cases. So some middle ground
		// has to be found here.
		int sw = worminator_map.map_width < 128? 128: worminator_map.map_width;
		int sh = worminator_map.map_height < 96? 96: worminator_map.map_height;

		stretch_blit(double_buffer, 
			         swap_buffer, 
					 0, 
					 0, 
					 sw /*256*/, 
					 sh /*192*/,
			         (int)(screen_width * 0.0125), 
					 (int)(screen_height * 0.02),
			         (int)(screen_width * .8), 
					 (int)(screen_height * .96));

		blit_to_screen(swap_buffer);
		return;
	}

	// Process the scroll stop tiles
	i = (player.x_position_in_pixels / 16) + 1;
	j = (player.y_position_in_pixels / 16) + 1;
	for (i = (player.x_position_in_pixels / 16) + 1; i >= 0; i--)
		if (worminator_map.graphical_properties_tile_grid[i][j] == scroll_stop_grph_tile || i == 0) {
			topx = i; break;
		}
	i = (player.x_position_in_pixels / 16) + 1;
	j = (player.y_position_in_pixels / 16) + 1;
	for (j = (player.y_position_in_pixels / 16) + 1; j >= 0; j--)
		if (worminator_map.graphical_properties_tile_grid[i][j] == scroll_stop_grph_tile || j == 0) {
			topy = j; break;
		}
	i = (player.x_position_in_pixels / 16) + 1;
	j = (player.y_position_in_pixels / 16) + 1;
	for (i = (player.x_position_in_pixels / 16) + 1; i < worminator_map.map_width; i++)
		if (worminator_map.graphical_properties_tile_grid[i][j] == scroll_stop_grph_tile || i == worminator_map.map_width - 1) {
			bottomx = i + 1; break;
		}
	i = (player.x_position_in_pixels / 16) + 1;
	j = (player.y_position_in_pixels / 16) + 1;
	for (j = (player.y_position_in_pixels / 16) + 1; j < worminator_map.map_height; j++)
		if (worminator_map.graphical_properties_tile_grid[i][j] == scroll_stop_grph_tile || j == worminator_map.map_height - 1) {
			bottomy = j + 1; break;
		}

	// Process FSAA if nessicary
	if (wormy_config.enable_FSAA == TRUE) {
		// Set up a dummy FSAA variable setting
		wormy_config.enable_FSAA = -69;

		// Do a quick swap of the screen width and height to fool the renderer
		plax_x = screen_width, plax_y = screen_height;
		screen_width = 320, screen_height = 200;

		// Set screen position
		screen_x_position = (int)(player.x_position_in_pixels -
				          ((int)(screen_width * .8) / 2) + 16);
		screen_y_position = (int)(player.y_position_in_pixels -
					  ((int)(screen_height * .96) / 2) + 16 + look_shift);

		if (screen_x_position > bottomx * 16 - (int)(screen_width * .8))
			screen_x_position = bottomx * 16 - (int)(screen_width * .8);
		if (screen_y_position > bottomy * 16 - (int)(screen_height * .96))
			screen_y_position = bottomy * 16 - (int)(screen_height * .96);

		if (screen_x_position < topx * 16)
			screen_x_position = topx * 16;
		if (screen_y_position < topy * 16)
			screen_y_position = topy * 16;

		sx = screen_x_position;
		sy = screen_y_position;

		// FSAA - Render a current sample as primary sample
		render_map();
		stretch_blit(double_buffer, FSAA_buffer, 0, 0, 256, 192, 0, 0,
			     (int)(plax_x * .8), (int)(plax_y * .96));

		// FSAA - Take a shifted sample
		screen_x_position++;
		screen_y_position++;
		render_map();
		stretch_blit(double_buffer, stretch_buffer, 0, 0, 256, 192, 0, 0,
			     (int)(plax_x * .8), (int)(plax_y * .96));
		draw_trans_sprite(FSAA_buffer, stretch_buffer, 1, 1);
		screen_y_position--;
		screen_x_position--;

		// Restore the FSAA variable setting and screen resolution
		wormy_config.enable_FSAA = TRUE;
		screen_width = plax_x, screen_height = plax_y;

		// Set screen position
		screen_x_position = (int)(player.x_position_in_pixels -
					  ((int)(screen_width * .8) / 2) + 16);
		screen_y_position = (int)(player.y_position_in_pixels -
					  ((int)(screen_height * .96) / 2) + 16 + look_shift);

		if (screen_x_position > bottomx * 16 - (int)(screen_width * .8))
			screen_x_position = bottomx * 16 - (int)(screen_width * .8);
		if (screen_y_position > bottomy * 16 - (int)(screen_height * .96))
			screen_y_position = bottomy * 16 - (int)(screen_height * .96);

		if (screen_x_position < topx * 16) screen_x_position = topx * 16;
		if (screen_y_position < topy * 16) screen_y_position = topy * 16;

		sx = screen_x_position;
		sy = screen_y_position;

		// Update the controls
		if (borders_dirty) {
			stretch_blit(screen_buffer, swap_buffer, 0, 0, 320, 4, 0, 0,
				     screen_width, (int)(screen_height * 0.02));
			stretch_blit(screen_buffer, swap_buffer, 0, 4, 4, 192, 0,
				     (int)(screen_height * 0.02), (int)(screen_width * 0.0125),
				     screen_height - 2 * (int)(screen_height * 0.02));
			stretch_blit(screen_buffer, swap_buffer, 0, 196, 320, 4, 0,
				     screen_height - (int)(screen_height * 0.02),
				     screen_width, (int)(screen_height * 0.02));
			borders_dirty = FALSE;
		}
		if (stats_dirty) {
			stretch_blit(screen_buffer, swap_buffer, 260, 4, 60, 192,
				     (int)(screen_width * 0.8125), (int)(screen_height * 0.02),
				     screen_width - (int)(screen_width * 0.8125),
				     screen_height - 2 * (int)(screen_height * 0.02));
			stats_dirty = FALSE;
		}

		// FSAA - Draw final aliased image
		if (quake_timer > 0 && !recording_demo && !playing_demo) {
			set_clip(swap_buffer, (int)(screen_width * 0.0125), (int)(screen_height * 0.02),
				 (int)(screen_width * .8) + 8, (int)(screen_height * .96) + 8);
			if (quake_timer > 512)
				quake_timer = 512;
			if (quake_timer > 256)
				blit(FSAA_buffer, swap_buffer, 0, 0,
				     (int)(screen_width * 0.0125) + rand() % 16 - 8,
				     (int)(screen_height * 0.02) + rand() % 16 - 8,
				     (int)(screen_width * .8), (int)(screen_height * .96));
			else if (quake_timer > 128)
				blit(FSAA_buffer, swap_buffer, 0, 0,
				     (int)(screen_width * 0.0125) + rand() % 8 - 4,
				     (int)(screen_height * 0.02) + rand() % 8 - 4,
				     (int)(screen_width * .8), (int)(screen_height * .96));
			else if (quake_timer > 64)
				blit(FSAA_buffer, swap_buffer, 0, 0,
				     (int)(screen_width * 0.0125) + rand() % 4 - 2,
				     (int)(screen_height * 0.02) + rand() % 4 - 2,
				     (int)(screen_width * .8), (int)(screen_height * .96));
			else
				blit(FSAA_buffer, swap_buffer, 0, 0,
				     (int)(screen_width * 0.0125) + rand() % 2 - 1,
				     (int)(screen_height * 0.02) + rand() % 2 - 1,
				     (int)(screen_width * .8), (int)(screen_height * .96));
			set_clip(swap_buffer, 0, 0, screen_width, screen_height);
		} else {
			blit(FSAA_buffer, swap_buffer, 0, 0,
			     (int)(screen_width * 0.0125), (int)(screen_height * 0.02),
			     (int)(screen_width * .8), (int)(screen_height * .96));
		}

		// The scene has been rendered, so we can return
		return;
	}

	// Set screen position
	if (player.skin == TANK_SKIN) {
		screen_x_position = (int)(player.x_position_in_pixels - ((int)(screen_width * .8) / 2) + 48);
		screen_y_position = (int)(player.y_position_in_pixels - ((int)(screen_height * .96) / 2) + 32 + look_shift);
	} else {
		screen_x_position = (int)(player.x_position_in_pixels - ((int)(screen_width * .8) / 2) + 16);
		screen_y_position = (int)(player.y_position_in_pixels - ((int)(screen_height * .96) / 2) + 16 + look_shift);
	}

	if (screen_x_position > bottomx * 16 - (int)(screen_width * .8)) screen_x_position = bottomx * 16 - (int)(screen_width * .8);
	if (screen_y_position > bottomy * 16 - (int)(screen_height * .96)) screen_y_position = bottomy * 16 - (int)(screen_height * .96);

	if (screen_x_position < topx * 16) screen_x_position = topx * 16;
	if (screen_y_position < topy * 16) screen_y_position = topy * 16;

	sx = screen_x_position;
	sy = screen_y_position;

	// Set the parallax background offsets
	if (parallax_background->w == (int)(screen_width * .8) || worminator_map.map_width == 16) plax_x = 0;
	else plax_x = (float)screen_x_position * (float)((float)(parallax_background->w - (int)(screen_width * .8)) / (worminator_map.map_width * 16 - (int)(screen_width * .8)));

	if (parallax_background->h == (int)(screen_height * .96) || worminator_map.map_height == 12) plax_y = 0;
	else plax_y = (float)screen_y_position * (float)((float)(parallax_background->h - (int)(screen_height * .96)) / (worminator_map.map_height * 16 - (int)(screen_height * .96)));

	// Draw the parallaxing background
	if (parallax_background->w < (int)(screen_width * .8) || parallax_background->h < (int)(screen_height * .96)) stretch_blit(parallax_background, double_buffer, 0, 0, parallax_background->w, parallax_background->h, 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));
	else blit(parallax_background, double_buffer, plax_x, plax_y, 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));

	// Snow
	if (worminator_map.weather == 1) {
		for (weather_loop = 0; weather_loop < 64; weather_loop++) 
			putpixel(double_buffer, weather[weather_loop][0], weather[weather_loop][1], 255);
	}
	// Rain
	else if (worminator_map.weather == 2) {
		for (weather_loop = 0; weather_loop < 64; weather_loop++) {
			putpixel(double_buffer, weather[weather_loop][0], weather[weather_loop][1], 20);
			putpixel(double_buffer, weather[weather_loop][0], weather[weather_loop][1] - 1, 22);
			putpixel(double_buffer, weather[weather_loop][0] + 1, weather[weather_loop][1] - 2, 24);
			putpixel(double_buffer, weather[weather_loop][0] + 1, weather[weather_loop][1] - 3, 26);
		}
	}

	// Find starting tile offsets
	while (sx >= 16) {
		start_x_tile++;
		sx -= 16;
	}
	;

	while (sy >= 16) {
		start_y_tile++;
		sy -= 16;
	}
	;

	for (x_tile_loop = 0; x_tile_loop < ((int)(screen_width * .8) / 16) + 2; x_tile_loop++) {
		for (y_tile_loop = 0; y_tile_loop < ((int)(screen_height * .96) / 16) + 2; y_tile_loop++) {
			// If the background tile is masked, draw it allowing for transparency
			if (worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == masked_background_grph_tile || worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == masked_everything_grph_tile) 
				masked_blit(worminator_tiles, double_buffer, worminator_map.background_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * 16, 0, x_tile_loop * 16 - sx, y_tile_loop * 16 - sy, 16, 16);

			// If the tile is a normal background tile,  draw it.
			else if (worminator_map.background_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != 0) 
				blit(worminator_tiles, double_buffer, worminator_map.background_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * 16, 0, x_tile_loop * 16 - sx, y_tile_loop * 16 - sy, 16, 16);

			// Draw the foreground tile if it is a masked tile and has a tile
			if (worminator_map.foreground_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != 0)
				if (worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == masked_foreground_grph_tile || worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == masked_everything_grph_tile) 
					masked_blit(worminator_tiles, double_buffer, worminator_map.foreground_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * 16, 0, x_tile_loop * 16 - sx, y_tile_loop * 16 - sy, 16, 16);
		}
	}

	// Render all sprites
	for (sprite_loop = 0; sprite_loop < number_of_sprites; sprite_loop++) {
		if (sprites[sprite_loop].sprite_type == STARDUST_PARTICLE_SPRITE) {
			putpixel(double_buffer, (int)(sprites[sprite_loop].x_position_in_pixels) - screen_x_position, (int)(sprites[sprite_loop].y_position_in_pixels) - screen_y_position, sprites[sprite_loop].frame);
		} else if (sprites[sprite_loop].sprite_type == CIGARETTE_BUTT_ENEMY_SPRITE) {
			rotate_scaled_sprite(double_buffer, worminator_data_file[sprites[sprite_loop].frame].dat, (int)(sprites[sprite_loop].x_position_in_pixels) - screen_x_position, (int)(sprites[sprite_loop].y_position_in_pixels) - screen_y_position, itofix(sprites[sprite_loop].rotate_factor), ftofix(sprites[sprite_loop].timer / 128.0));
		} else if (sprites[sprite_loop].sprite_type == PERVERT_MESSAGE_SPRITE || sprites[sprite_loop].sprite_type == BORED_SPRITE) {
		} else if (sprites[sprite_loop].frame != 0) {
			if (sprites[sprite_loop].sprite_type == WORMINATOR_BFG_PLASMA_BALL_SPRITE || sprites[sprite_loop].sprite_type == BFG_PLASMA_BALL_EXPLOSION_SPRITE) {
				warp_effect(double_buffer, (int)(sprites[sprite_loop].x_position_in_pixels) - screen_x_position, (int)(sprites[sprite_loop].y_position_in_pixels) - screen_y_position - 8, 48, 32, 16);
				// This loop scans for enemies that can be hurt and draws 'death rays' to them
				for (i = 0; i < number_of_sprites; i++) if (sprites[i].hit_points_left > 0) if (sqrt((sprites[sprite_loop].x_position_in_pixels - sprites[i].x_position_in_pixels) * (sprites[sprite_loop].x_position_in_pixels - sprites[i].x_position_in_pixels) + (sprites[sprite_loop].y_position_in_pixels - sprites[i].y_position_in_pixels) * (sprites[sprite_loop].y_position_in_pixels - sprites[i].y_position_in_pixels)) < 212) fancy_line(double_buffer, sprites[sprite_loop].x_position_in_pixels + 16 - screen_x_position, sprites[sprite_loop].y_position_in_pixels + 8 - screen_y_position, sprites[i].x_position_in_pixels + sprites[i].width / 2 - screen_x_position, sprites[i].y_position_in_pixels + sprites[i].height / 2 - screen_y_position, makecol(0, 255, 0));
			}
			if (sprites[sprite_loop].sprite_type == MEDIUM_EXPLOSION_SPRITE || sprites[sprite_loop].sprite_type == SMALL_EXPLOSION_SPRITE) {
				draw_trans_sprite(double_buffer, worminator_data_file[sprites[sprite_loop].frame].dat, (int)(sprites[sprite_loop].x_position_in_pixels - screen_x_position), (int)(sprites[sprite_loop].y_position_in_pixels - screen_y_position));
			} else if (sprites[sprite_loop].rotate_factor == 0) {
				if (sprites[sprite_loop].direction == 0) draw_sprite(double_buffer, worminator_data_file[sprites[sprite_loop].frame].dat, (int)(sprites[sprite_loop].x_position_in_pixels) - screen_x_position, (int)(sprites[sprite_loop].y_position_in_pixels) - screen_y_position);
				else draw_sprite_h_flip(double_buffer, worminator_data_file[sprites[sprite_loop].frame].dat, (int)(sprites[sprite_loop].x_position_in_pixels) - screen_x_position, (int)(sprites[sprite_loop].y_position_in_pixels) - screen_y_position);
			} else {
				rotate_sprite(double_buffer, worminator_data_file[sprites[sprite_loop].frame].dat, (int)(sprites[sprite_loop].x_position_in_pixels) - screen_x_position, (int)(sprites[sprite_loop].y_position_in_pixels) - screen_y_position, itofix(sprites[sprite_loop].rotate_factor));
			}
		}
	}

	// Render all beams and draw the player
	render_beams();
	draw_player();

	
	// Render all foreground tiles and a couple graphical properties tiles
	for (x_tile_loop = 0; x_tile_loop < ((int)(screen_width * .8) / 16) + 2; x_tile_loop++) {
		for (y_tile_loop = 0; y_tile_loop < ((int)(screen_height * .96) / 16) + 2; y_tile_loop++) {
			if (worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == parallaxing_foreground_grph_tile) {
				blit(parallax_background, double_buffer, x_tile_loop * 16 - sx + plax_x, y_tile_loop * 16 - sy + plax_y, x_tile_loop * 16 - sx, y_tile_loop * 16 - sy, 16, 16);
			} else if (worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == water_mask_grph_tile) {
				blit(worminator_tiles, spawned_sprite, 129 * 16, 0, 0, 0, 16, 16);
				draw_trans_sprite(double_buffer, spawned_sprite, x_tile_loop * 16 - sx, y_tile_loop * 16 - sy);
			}
			if (worminator_map.foreground_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != 0) {
				if (worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == translucent_foreground_grph_tile) {
					blit(worminator_tiles, spawned_sprite, worminator_map.foreground_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * 16, 0, 0, 0, 16, 16);
					draw_trans_sprite(double_buffer, spawned_sprite, x_tile_loop * 16 - sx, y_tile_loop * 16 - sy);
				} else if (worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != masked_foreground_grph_tile && worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != masked_everything_grph_tile && worminator_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != hide_grph_tile) {
					masked_blit(worminator_tiles, double_buffer, worminator_map.foreground_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * 16, 0, x_tile_loop * 16 - sx, y_tile_loop * 16 - sy, 16, 16);
				}
			}
		}
	}

	// tested from here
	
	// Draw a laser sight if nessicary
	if (wormy_config.show_laser_sight == TRUE && player.current_weapon == 2 && player.shells_left >= 0 && player.skin != TANK_SKIN) 
		draw_sprite(double_buffer, worminator_data_file[SHOTGUN_LASER_SIGHT].dat, find_laser_sight() - screen_x_position - 2, player.y_position_in_pixels + 15 - screen_y_position - 1);

	// Draw spinning arrow if appropriate
	if ((worminator_map.info_tile_grid[(int)((player.x_position_in_pixels - 4) / 16)][(int)(player.y_position_in_pixels / 16)] >= teleporter_1_info_tile && worminator_map.info_tile_grid[(int)((player.x_position_in_pixels - 4) / 16)][(int)(player.y_position_in_pixels / 16)] <= teleporter_16_info_tile) || (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 7) / 16)][(int)(player.y_position_in_pixels / 16)] >= teleporter_1_info_tile && worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 7) / 16)][(int)(player.y_position_in_pixels / 16)] <= teleporter_16_info_tile) || (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 18) / 16)][(int)(player.y_position_in_pixels / 16)] >= teleporter_1_info_tile && worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 18) / 16)][(int)(player.y_position_in_pixels / 16)] <= teleporter_16_info_tile)) {
		if (global_timer_3 < 32) 
			draw_sprite(double_buffer, worminator_data_file[PINK_ARROW_SPIN_01 + (int)(global_timer_3 / 8)].dat, player.x_position_in_pixels - screen_x_position, player.y_position_in_pixels - screen_y_position - 40 + ((global_timer_3 / 2) - 8) / 2);
		else 
			draw_sprite(double_buffer, worminator_data_file[PINK_ARROW_SPIN_01 + (int)(global_timer_3 / 8)].dat, player.x_position_in_pixels - screen_x_position, player.y_position_in_pixels - screen_y_position - 40 + (16 - ((global_timer_3 / 2) - 16) - 8) / 2);
	}

	// Stretch or Re-position the rendered view if nessicary
	if (wormy_config.stretch_levels == 0) {
		if ((bottomx - topx) * 16 < (int)(screen_width * .8) && (bottomy - topy) * 16 >= (int)(screen_height * .96)) {
			blit(double_buffer, stretch_buffer, 0, 0, 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));
			stretch_blit(stretch_buffer, double_buffer, 0, 0, (bottomx - topx) * 16, (int)(screen_height * .96), 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));
		} else if ((bottomx - topx) * 16 >= (int)(screen_width * .8) && (bottomy - topy) * 16 < (int)(screen_height * .96)) {
			blit(double_buffer, stretch_buffer, 0, 0, 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));
			stretch_blit(stretch_buffer, double_buffer, 0, 0, (int)(screen_width * .8), (bottomy - topy) * 16, 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));
		} else if ((bottomx - topx) * 16 < (int)(screen_width * .8) && (bottomy - topy) * 16 < (int)(screen_height * .96)) {
			blit(double_buffer, stretch_buffer, 0, 0, 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));
			stretch_blit(stretch_buffer, double_buffer, 0, 0, (bottomx - topx) * 16, (bottomy - topy) * 16, 0, 0, (int)(screen_width * .8), (int)(screen_height * .96));
		}
	} else {
		if ((bottomx - topx) * 16 < (int)(screen_width * .8)) {
			blit(double_buffer, double_buffer, 0, 0, ((int)(screen_width * .8) - (bottomx - topx) * 16) / 2, 0, (bottomx - topx) * 16, (int)(screen_height * .96));
			rectfill(double_buffer, 0, 0, ((int)(screen_width * .8) - (bottomx - topx) * 16) / 2, (int)(screen_height * .96), 244);
			rectfill(double_buffer, (int)(screen_width * .8) - (((int)(screen_width * .8) - (bottomx - topx) * 16) / 2), 0, (int)(screen_width * .8), (int)(screen_height * .96), 244);
		}
		if ((bottomy - topy) * 16 < (int)(screen_height * .96)) {
			blit(double_buffer, double_buffer, 0, 0, 0, ((int)(screen_height * .96) - (bottomy - topy) * 16) / 2, (int)(screen_width * .8), (bottomy - topy) * 16);
			rectfill(double_buffer, 0, 0, (int)(screen_width * .8), ((int)(screen_height * .96) - (bottomy - topy) * 16) / 2, 244);
			rectfill(double_buffer, 0, (int)(screen_height * .96) - (((int)(screen_height * .96) - (bottomy - topy) * 16) / 2), (int)(screen_width * .8), (int)(screen_height * .96), 244);
		}
	}

	// Draw special sprites
	for (sprite_loop = 0; sprite_loop < number_of_sprites; sprite_loop++)
		if (sprites[sprite_loop].sprite_type == PERVERT_MESSAGE_SPRITE || sprites[sprite_loop].sprite_type == BORED_SPRITE) 
			draw_sprite(double_buffer, worminator_data_file[sprites[sprite_loop].frame].dat, (int)(sprites[sprite_loop].x_position_in_pixels) - screen_x_position, (int)(sprites[sprite_loop].y_position_in_pixels) - screen_y_position);

	// Tested from here

	// Draw text messages
	if (screen_width <= 512) {
		text_mode(255);
		if (text_messages[0].timer != -1) textout(double_buffer, worminator_data_file[DEFAULT_WORMINATOR_FONT].dat, text_messages[0].message, 0, 0, -1);
		if (text_messages[1].timer != -1) textout(double_buffer, worminator_data_file[DEFAULT_WORMINATOR_FONT].dat, text_messages[1].message, 0, 7, -1);
		if (text_messages[2].timer != -1) textout(double_buffer, worminator_data_file[DEFAULT_WORMINATOR_FONT].dat, text_messages[2].message, 0, 14, -1);
		if (text_messages[3].timer != -1) textout(double_buffer, worminator_data_file[DEFAULT_WORMINATOR_FONT].dat, text_messages[3].message, 0, 21, -1);
		text_mode(-1);
	} else {
		text_mode(255);
		if (text_messages[0].timer != -1) textout(double_buffer, font, text_messages[0].message, 0, 0, 5);
		if (text_messages[1].timer != -1) textout(double_buffer, font, text_messages[1].message, 0, 10, 5);
		if (text_messages[2].timer != -1) textout(double_buffer, font, text_messages[2].message, 0, 20, 5);
		if (text_messages[3].timer != -1) textout(double_buffer, font, text_messages[3].message, 0, 30, 5);
		text_mode(-1);
	}

	// Show a fps counter if nessicary
	if (show_fps == TRUE){
		sprintf(message, "FPS: %i", fps);
		textout(double_buffer, font, message, (int)(screen_width * .8) - 128, 0, 5);
	}

	// Show a bullet time counter, if may tricks mode is on
	if (wormy_config.may_tricks_mode == TRUE) {
		rectfill(spawned_sprite, 0, 0, 16, 64, 250);
		for (x_tile_loop = 0; x_tile_loop < 4; x_tile_loop++)
			draw_trans_sprite(double_buffer, spawned_sprite, 8, (int)(screen_height * .96) - 72 + x_tile_loop * 16);
		rectfill(stretch_buffer, 0, 0, 16, 64, 0);
		if (bullet_time_engaged == TRUE) rectfill(stretch_buffer, 0, 64 - (bullet_time / 64), 16, 64, 232);
		else rectfill(stretch_buffer, 0, 64 - (bullet_time / 64), 16, 64, 255);
		for (x_tile_loop = 0; x_tile_loop < 4; x_tile_loop++) {
			blit(stretch_buffer, spawned_sprite, 0, x_tile_loop * 16, 0, 0, 16, 16);
			draw_sprite(double_buffer, spawned_sprite, 8, (int)(screen_height * .96) - 72 + x_tile_loop * 16);
		}
	}

	// Show level stats if appropriate
	if (display_stats == TRUE) show_stats();

	// Tested from here

	// Put up the demo sign if needed
	//if (playing_demo/* && mouse_frame >= 4*/) rotate_sprite(double_buffer, worminator_data_file[DEMO_SIGN].dat, demox, demoy, itofix(demospin));
	if (playing_demo && mouse_frame >= 4) 
		draw_sprite(double_buffer, worminator_data_file[DEMO_SIGN].dat, ((int)(screen_width * .8) / 2) - 64, ((int)(screen_height * .96) / 2) - 64);

	// Draw wormcode if nessicary
	if (bullet_time_engaged && wormy_config.cool_mode == TRUE) 
		draw_wormcode();

	// Draw the console if nessicary
	if (console_state != 0) 
		draw_console(double_buffer, (float)((float)(console_position) / 100.0) * (screen_height / 2));

	// Handle screen shakes and do actual rendering
	if (wormy_config.enable_FSAA == FALSE) {
		// Update the controls
		if (borders_dirty) {
			stretch_blit(screen_buffer, swap_buffer, 0, 0, 320, 4, 0, 0, screen_width, (int)(screen_height * 0.02));
			stretch_blit(screen_buffer, swap_buffer, 0, 4, 4, 192, 0, (int)(screen_height * 0.02), (int)(screen_width * 0.0125), screen_height - 2 * (int)(screen_height * 0.02));
			stretch_blit(screen_buffer, swap_buffer, 0, 196, 320, 4, 0, screen_height - (int)(screen_height * 0.02), screen_width, (int)(screen_height * 0.02));
			borders_dirty = FALSE;
		}
		if (stats_dirty) {
			stretch_blit(screen_buffer, swap_buffer, 260, 4, 60, 192, (int)(screen_width * 0.8125), (int)(screen_height * 0.02), screen_width - (int)(screen_width * 0.8125), screen_height - 2 * (int)(screen_height * 0.02));
			stats_dirty = FALSE;
		}

		if (quake_timer > 0 && !recording_demo && !playing_demo) {
			set_clip(swap_buffer, (int)(screen_width * 0.0125), (int)(screen_height * 0.02), (int)(screen_width * .8) + 8, (int)(screen_height * .96) + 8);
			if (quake_timer > 512) quake_timer = 512;
			if (quake_timer > 256) blit(double_buffer, swap_buffer, 0, 0, (int)(screen_width * 0.0125) + rand() % 16 - 8, (int)(screen_height * 0.02) + rand() % 16 - 8, (int)(screen_width * .8), (int)(screen_height * .96));
			else if (quake_timer > 128) blit(double_buffer, swap_buffer, 0, 0, (int)(screen_width * 0.0125) + rand() % 8 - 4, (int)(screen_height * 0.02) + rand() % 8 - 4, (int)(screen_width * .8), (int)(screen_height * .96));
			else if (quake_timer > 64) blit(double_buffer, swap_buffer, 0, 0, (int)(screen_width * 0.0125) + rand() % 4 - 2, (int)(screen_height * 0.02) + rand() % 4 - 2, (int)(screen_width * .8), (int)(screen_height * .96));
			else blit(double_buffer, swap_buffer, 0, 0, (int)(screen_width * 0.0125) + rand() % 2 - 1, (int)(screen_height * 0.02) + rand() % 2 - 1, (int)(screen_width * .8), (int)(screen_height * .96));
			set_clip(swap_buffer, 0, 0, screen_width, screen_height);
		} else {
			blit(double_buffer, swap_buffer, 0, 0, (int)(screen_width * 0.0125), (int)(screen_height * 0.02), (int)(screen_width * .8), (int)(screen_height * .96));
		}
	}

	frame_counter++;
	if (frame_counter > 32768) frame_counter = 32768;

	
	blit_to_screen(swap_buffer);


	// game_is_running can be changed from under us by update_animations, this
	// should be fixed but just to be sure put the check for it in the while.

	while (game_is_running && speed_counter == 0){ 
		rest(1);
	}
}

/****************\
|** SHOW STATS **|
\****************/
void show_stats()
{
	int scanner;
	int enemies_left = 0, items_left = 0;
	int xshift = 0;
	char mapname[256];

	strcpy(mapname, "Custom level.");
	//mapname = "Custom level";
	if (current_level == 1) strcpy(mapname, "Worminator factory");
	if (current_level == 2) strcpy(mapname, "Sewers");
	if (current_level == 3) strcpy(mapname, "Cyberslime building");
	if (current_level == 4) strcpy(mapname, "Airship");
	if (current_level == 5) strcpy(mapname, "Nuclear plant");
	if (current_level == 6) strcpy(mapname, "Scientist's lair");
	if (current_level == 7) strcpy(mapname, "Stomach");
	if (current_level == 8) strcpy(mapname, "Carnival");
	if (current_level == 9) strcpy(mapname, "Lab");
	if (current_level == 10) strcpy(mapname, "Battlegrounds");
	if (current_level == 11) strcpy(mapname, "Submarine base");
	if (current_level == 12) strcpy(mapname, "Hospital");
	if (current_level == 13) strcpy(mapname, "City");
	if (current_level == 14) strcpy(mapname, "Tank joyride");
	if (current_level == 15) strcpy(mapname, "Factory");
	if (current_level == 16) strcpy(mapname, "Defend");
	if (current_level == 17) strcpy(mapname, "Boss");
	if (current_level == 18) strcpy(mapname, "Wormgypt");
	if (current_level == 19) strcpy(mapname, "Ice cave");

	if (wormy_config.may_tricks_mode == TRUE) xshift = 32;

	// First, we have to actually count items and enemies
	for (scanner = 0; scanner < number_of_sprites; scanner++) {
		if (sprites[scanner].sprite_active != 0) {
			if (sprites[scanner].sprite_type >= BLUE_SLIME_ENEMY_SPRITE && sprites[scanner].sprite_type <= BOSS_ENEMY_SPRITE && sprites[scanner].hit_points_left != -666 && sprites[scanner].sprite_type != MECHANICAL_CRUSHER_ENEMY_SPRITE && sprites[scanner].sprite_type != BIG_GREEN_SLIME_BLOB_ENEMY_SPRITE) enemies_left++;
			else if (sprites[scanner].sprite_type >= STOMPING_FOOT_ENEMY_SPRITE && sprites[scanner].sprite_type <= CIGARETTE_BUTT_ENEMY_SPRITE) enemies_left++;
			else if (sprites[scanner].sprite_type >= HEALTH_1_SPRITE && sprites[scanner].sprite_type <= MUD_BEER_SPRITE) items_left++;
			else if (sprites[scanner].sprite_type >= PLASMA_BULLETS_SMALL_SPRITE && sprites[scanner].sprite_type <= BIG_TASTY_BALL_O_DIRT_SPRITE) items_left++;
		}
	}

	// Now, display the stats
	if (enemies_left == 0 && items_left == 0) textprintf(double_buffer, font, xshift, (int)(screen_height * .96) - 27, makecol(255, 255, 0), mapname, current_level);
	else textprintf(double_buffer, font, xshift, (int)(screen_height * .96) - 27, makecol(0, 255, 255), mapname, current_level);

	textprintf(double_buffer, font, xshift, (int)(screen_height * .96) - 18, makecol(255, 0, 0), "Enemies remaining: %-5d", enemies_left);
	textprintf(double_buffer, font, xshift, (int)(screen_height * .96) - 9, makecol(0, 255, 0), "Items remaining: %-5d", items_left);
}

/**************\
|** SHOW MAP **|
\**************/
void show_map()
{
	int scanner;
	int x, y;

	// PSVITA FIX: Exiting the map requires a long press of a button.
	// Optimizing this cpu hungry function solves the issue.
	
	// Fill background with black so we don't have to draw every pixel.
	rectfill(double_buffer, 0, 0, 256, 192, 0); 
	// Precalculate colors.
	int red_color = makecol(255, 0, 0);
	int green_color = makecol(0, 255, 0);
	int blue_color = makecol(0, 0, 255);
	int white_color =  makecol(225, 225, 225);
	int grey_color =  makecol(128, 128, 128);
	int cyan_color = makecol(0, 255, 255);
	int yellow_color = makecol(255, 255, 0);
	int map_width = worminator_map.map_width;
	int map_height = worminator_map.map_height;

	// Draw in all info tile data
	for (x = 0; x < 256; x++) {
		for (y = 0; y < 192; y++) {
			// Off map
			if (x > map_width || y > map_height) putpixel(double_buffer, x, y, grey_color);

			// Solid tiles
			else if (worminator_map.info_tile_grid[x][y] == solid_info_tile) putpixel(double_buffer, x, y, white_color);
			else if (worminator_map.info_tile_grid[x][y] == conveyor_belt_right_info_tile || worminator_map.info_tile_grid[x][y] == conveyor_belt_left_info_tile) putpixel(double_buffer, x, y, white_color);
			else if (worminator_map.info_tile_grid[x][y] == half_slippery_info_tile || worminator_map.info_tile_grid[x][y] == mostly_slippery_info_tile) putpixel(double_buffer, x, y, white_color);

			// Painful tiles and water
			else if (worminator_map.info_tile_grid[x][y] == water_info_tile) putpixel(double_buffer, x, y, makecol(0, 0, 192));
			else if (worminator_map.info_tile_grid[x][y] >= lethal_info_tile && worminator_map.info_tile_grid[x][y] <= painful_from_bottom_info_tile) putpixel(double_buffer, x, y, makecol(255, 128, 0));

			// Sort of solid tiles, such as one sided or breakable blocks and slopes
			else if (worminator_map.info_tile_grid[x][y] >= solid_from_top_no_drop_info_tile && worminator_map.info_tile_grid[x][y] <= solid_from_left_no_ammo_info_tile) putpixel(double_buffer, x, y, grey_color);
			else if (worminator_map.info_tile_grid[x][y] == slope_up_left_to_right_45_info_tile || worminator_map.info_tile_grid[x][y] == slope_up_right_to_left_45_info_tile) putpixel(double_buffer, x, y, grey_color);
			else if (worminator_map.info_tile_grid[x][y] == slope_up_left_to_right_225_right_info_tile || worminator_map.info_tile_grid[x][y] == slope_up_right_to_left_225_left_info_tile) putpixel(double_buffer, x, y, grey_color);
			else if (worminator_map.info_tile_grid[x][y] == breakable_wall_info_tile) putpixel(double_buffer, x, y, grey_color);

			// Teleporters and exits
			else if (worminator_map.info_tile_grid[x][y] >= teleporter_1_info_tile && worminator_map.info_tile_grid[x][y] <= quick_teleporter_16_info_tile) putpixel(double_buffer, x, y, cyan_color);
			else if (worminator_map.info_tile_grid[x][y] >= quick_teleporter_exit_1_info_tile && worminator_map.info_tile_grid[x][y] <= quick_teleporter_exit_16_info_tile) putpixel(double_buffer, x, y, cyan_color);
			else if (worminator_map.info_tile_grid[x][y] >= exit_to_level_1_info_tile && worminator_map.info_tile_grid[x][y] <= quick_exit_to_level_16_info_tile) putpixel(double_buffer, x, y, cyan_color);

			// It must be empty/insignificant if we got here
			//else putpixel(double_buffer, x, y, makecol(0, 0, 0));
		}
	}

	// Draw all items and enemies
	for (scanner = 0; scanner < number_of_sprites; scanner++) {
		if (sprites[scanner].sprite_active != 0) {
			if (sprites[scanner].sprite_type >= BLUE_SLIME_ENEMY_SPRITE && sprites[scanner].sprite_type <= BOSS_ENEMY_SPRITE && sprites[scanner].hit_points_left != -666) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), red_color);
			} else if (sprites[scanner].sprite_type >= STOMPING_FOOT_ENEMY_SPRITE && sprites[scanner].sprite_type <= CIGARETTE_BUTT_ENEMY_SPRITE) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), red_color);
			} else if (sprites[scanner].sprite_type >= HEALTH_1_SPRITE && sprites[scanner].sprite_type <= MUD_BEER_SPRITE) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), green_color);
			} else if (sprites[scanner].sprite_type >= PLASMA_BULLETS_SMALL_SPRITE && sprites[scanner].sprite_type <= BIG_TASTY_BALL_O_DIRT_SPRITE) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), green_color);
			} else if (sprites[scanner].sprite_type == FLOATING_PLATFORM_SPRITE) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), white_color);  putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16) + 1, (sprites[scanner].y_position_in_pixels / 16), white_color);
			} else if (sprites[scanner].sprite_type == RED_DOOR_SPRITE && sprites[scanner].action_state == 0) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), red_color);  putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16) + 1, red_color);
			} else if (sprites[scanner].sprite_type == GREEN_DOOR_SPRITE && sprites[scanner].action_state == 0) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), green_color);  putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16) + 1, green_color);
			} else if (sprites[scanner].sprite_type == BLUE_DOOR_SPRITE && sprites[scanner].action_state == 0) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), blue_color);  putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16) + 1, blue_color);
			} else if (sprites[scanner].sprite_type == SMALL_EXPLOSIVE_BARREL_SPRITE || sprites[scanner].sprite_type == SMALL_EXPLOSIVE_NUCLEAR_CRATE_SPRITE) {
				putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16), yellow_color);  putpixel(double_buffer, (sprites[scanner].x_position_in_pixels / 16), (sprites[scanner].y_position_in_pixels / 16) + 1, yellow_color);
			}
		}
	}

	// Draw the player
	putpixel(double_buffer, ((player.x_position_in_pixels + 4) / 16), ((player.y_position_in_pixels + 2) / 16), makecol(255, 128, 255));
	putpixel(double_buffer, ((player.x_position_in_pixels + 4) / 16), ((player.y_position_in_pixels + 2) / 16) + 1, makecol(255, 128, 255));
	putpixel(double_buffer, ((player.x_position_in_pixels + 4) / 16) + 1, ((player.y_position_in_pixels + 2) / 16), makecol(255, 128, 255));
	putpixel(double_buffer, ((player.x_position_in_pixels + 4) / 16) + 1, ((player.y_position_in_pixels + 2) / 16) + 1, makecol(255, 128, 255));
}

/********************\
|** UPDATE WEATHER **|
\********************/
void update_weather()
{
	char weather_loop;

	// Snow
	if (worminator_map.weather == 1) {
		for (weather_loop = 0; weather_loop < 64; weather_loop++) {
			weather[weather_loop][0] += (rand() % 20 - 10) / 10 * 2;
			weather[weather_loop][0] += player.x_acceleration * -1.5;

			weather[weather_loop][1]++;

			if (weather[weather_loop][1] > screen_height) weather[weather_loop][1] -= screen_height;

			if (weather[weather_loop][0] < 0) weather[weather_loop][0] += screen_width;
			else if (weather[weather_loop][0] >= screen_width) weather[weather_loop][0] -= screen_width;
		}
	}
	// Rain
	else if (worminator_map.weather == 2) {
		for (weather_loop = 0; weather_loop < 64; weather_loop++) {
			weather[weather_loop][0] += -1.5;
			weather[weather_loop][0] += player.x_acceleration * -.75;

			weather[weather_loop][1] += 2;

			if (weather[weather_loop][1] > screen_height) weather[weather_loop][1] -= screen_height;

			if (weather[weather_loop][0] < 0) weather[weather_loop][0] += screen_width;
			else if (weather[weather_loop][0] >= screen_width) weather[weather_loop][0] -= screen_width;
		}
	}
}

/******************\
|** FIRE SHOTGUN **|
\******************/
void fire_shotgun()
{
	char done_looping = FALSE;
	char sw, sh;
	unsigned char spawn_loop;
	unsigned char test_tile;
	int x1, x2, y;
	int sx, sy;
	int hit_x, hit_sprite = -1;
	int x_tile, y_tile;
	int collision_detect_loop;

	y = player.y_position_in_pixels + 15 + (rand() % 2);

	if (player.direction == 0) {
		x1 = x2 = player.x_position_in_pixels + 27;
		hit_x = 4096;
		do {
			if (x2 > worminator_map.map_width * 16) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)((x2 + 16) / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_left_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 += 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x1 && sx <= x2) || (sx + sw >= x1 && sx + sw <= x2)))
					if (sx < hit_x) hit_sprite = collision_detect_loop, hit_x = sx;
			}
		}
		if (hit_sprite != -1) sprites[hit_sprite].hit_points_left -= (1 + (float)(rand() % 10) / 10.00), x2 = hit_x;
		else spawn_stardust(((int)(x2 / 16) * 16) + 16, y, 0, 0, 255, 240, 4, 16);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, (int)(x2 * 16) / 16, y, -1, (float)((rand() % 20) - 10.00) / 10.00, 0);
		if (player.ammo_type == 1) spawn_sprite(SMALL_EXPLOSION_SPRITE, find_laser_sight() - 4, y - 8, 0, 0, 0);
	} else {
		x1 = x2 = player.x_position_in_pixels + 4;
		hit_x = 0;
		do {
			if (x2 < 0) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)((x2 - 16) / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_right_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 -= 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x2 && sx <= x1) || (sx + sw >= x2 && sx + sw <= x1)))
					if (sx > hit_x) hit_sprite = collision_detect_loop, hit_x = sx;
			}
		}
		if (hit_sprite != -1) sprites[hit_sprite].hit_points_left -= (1 + (float)(rand() % 10) / 10.00), x2 = hit_x + sprites[hit_sprite].width;
		else spawn_stardust((int)(x2 / 16) * 16, y, 0, 0, 255, 240, 4, 16);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, (int)(x2 * 16) / 16, y, 1, (float)((rand() % 20) - 10.00) / 10.00, 0);
		if (player.ammo_type == 1) spawn_sprite(SMALL_EXPLOSION_SPRITE, find_laser_sight() - 12, y - 8, 0, 0, 0);
	}

	if (test_tile == breakable_wall_info_tile && hit_sprite == -1) {
		if (player.direction == 0) x_tile = (int)((x2 + 16) / 16);
		else x_tile = (int)((x2 - 16) / 16);
		y_tile = (int)(y / 16);
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, x_tile * 16 + 8, y_tile * 16 + 8, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		worminator_map.foreground_tile_grid[x_tile][y_tile] = 0;
		worminator_map.info_tile_grid[x_tile][y_tile] = blank_info_tile;
	}
}

/**********************\
|** FIND LASER SIGHT **|
\**********************/
int find_laser_sight()
{
	char done_looping = FALSE;
	char sw, sh;
	unsigned char test_tile;
	int x1, x2, y;
	int sx, sy;
	int hit_x, hit_sprite = -1;
	int collision_detect_loop;

	y = player.y_position_in_pixels + 15 + (rand() % 2);

	if (player.direction == 0) {
		x1 = x2 = player.x_position_in_pixels + 27;
		hit_x = 4096;
		do {
			if (x2 > worminator_map.map_width * 16) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)(x2 / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_left_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 += 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x1 && sx <= x2) || (sx + sw >= x1 && sx + sw <= x2)))
					if (sx < hit_x) hit_sprite = collision_detect_loop, hit_x = sx;
			}
		}
		if (hit_sprite != -1) x2 = hit_x;
		else x2 = x2 / 16 * 16;
		return x2;
	} else {
		x1 = x2 = player.x_position_in_pixels + 4;
		hit_x = 0;
		do {
			if (x2 < 0) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)((x2 - 16) / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_right_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 -= 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x2 && sx <= x1) || (sx + sw >= x2 && sx + sw <= x1)))
					if (sx > hit_x) hit_sprite = collision_detect_loop, hit_x = sx;
			}
		}
		if (hit_sprite != -1) x2 = hit_x + sprites[hit_sprite].width;
		else x2 = x2 / 16 * 16;
		return x2;
	}
}

/********************\
|** FIRE LASER GUN **|
\********************/
void fire_laser_gun()
{
	char done_looping = FALSE;
	char sw, sh;
	unsigned char spawn_loop;
	unsigned char test_tile;
	int x1, x2, y;
	int sx, sy;
	int collision_detect_loop;
	int beamc = 217 - ((rand() % 10) * 16);
	float beamdmg = .4 + ((float)(rand() % 20) / 40.00);

	y = player.y_position_in_pixels + 13;

	if (player.direction == 0) {
		x1 = x2 = player.x_position_in_pixels + 36;
		do {
			if (x2 > worminator_map.map_width * 16) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)(x2 / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_left_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 += 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x1 && sx <= x2) || (sx + sw >= x1 && sx + sw <= x2))) {
					sprites[collision_detect_loop].hit_points_left -= beamdmg;
					beamdmg -= .1;
					if (beamdmg < .3) beamdmg = .3;
					for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sx + (sw / 2), sy + (sh / 2), (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
				}
			}
		}
		x2 = (int)(x2 / 16) * 16;
		// color 271
		spawn_beam(player.x_position_in_pixels + 37, player.y_position_in_pixels + 12, x2, 0, beamc, SOLID_COLOR_BEAM);
		spawn_beam(player.x_position_in_pixels + 37, player.y_position_in_pixels + 13, x2, 0, beamc, SOLID_COLOR_BEAM);
		spawn_beam(player.x_position_in_pixels + 37, player.y_position_in_pixels + 14, x2, 0, beamc, SOLID_COLOR_BEAM);
	} else {
		x1 = x2 = player.x_position_in_pixels - 4;
		do {
			if (x2 < 0) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)(x2 / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_right_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 -= 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x2 && sx <= x1) || (sx + sw >= x2 && sx + sw <= x1))) {
					sprites[collision_detect_loop].hit_points_left -= beamdmg;
					beamdmg -= .1;
					if (beamdmg < .3) beamdmg = .3;
					for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sx + (sw / 2), sy + (sh / 2), (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
				}
			}
		}
		x2 = ((int)(x2 / 16) * 16) + 16;
		spawn_beam(x2, player.y_position_in_pixels + 12, player.x_position_in_pixels - 6, 0, beamc, SOLID_COLOR_BEAM);
		spawn_beam(x2, player.y_position_in_pixels + 13, player.x_position_in_pixels - 6, 0, beamc, SOLID_COLOR_BEAM);
		spawn_beam(x2, player.y_position_in_pixels + 14, player.x_position_in_pixels - 6, 0, beamc, SOLID_COLOR_BEAM);
	}
}

/*******************\
|** FIRE CHAINSAW **|
\*******************/
void fire_chainsaw()
{
	char do_damage;
	char spawn_loop;
	int sprite_loop;
	int sx, sy, sw, sh;
	int x, y, w = 24, h = 12;

	// Assign a few variables
	if (player.direction == 0) {
		x = player.x_position_in_pixels + 22;
		y = player.y_position_in_pixels + 12;
	} else {
		x = player.x_position_in_pixels - 14;
		y = player.y_position_in_pixels + 12;
	}

	// Check all sprites
	for (sprite_loop = 0; sprite_loop < number_of_sprites; sprite_loop++) {
		if (sprites[sprite_loop].hit_points_left > 0) {
			do_damage = TRUE;

			sx = sprites[sprite_loop].x_position_in_pixels;
			sy = sprites[sprite_loop].y_position_in_pixels;
			sw = sprites[sprite_loop].width;
			sh = sprites[sprite_loop].height;

			// This is the actual collision detection
			if (x > sx && x - sx >= sw) do_damage = FALSE;
			else if (x <= sx && sx - x >= w) do_damage = FALSE;
			if (y > sy && y - sy >= sh) do_damage = FALSE;
			else if (y <= sy && sy - y >= h) do_damage = FALSE;

			if (do_damage == TRUE) {
				spawn_shrapgibs(x + (w / 2), y + (h / 2), 4, 4);
				for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, x + (w / 2), y + (h / 2), (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
				quake_timer += 16;
				sprites[sprite_loop].hit_points_left -= .5;
			}
		}
	}
}

/******************\
|** FIRE RAILGUN **|
\******************/
void fire_railgun()
{
	char done_looping = FALSE;
	char sw, sh;
	unsigned char spawn_loop;
	unsigned char test_tile;
	int x1, x2, y;
	int sx, sy;
	int collision_detect_loop;
	int scrw = screen_width, scrh = screen_height;

	// Fudge the screen width/height for a demo
	if (recording_demo || playing_demo) {
		screen_width = 640;  screen_height = 480;
	}

	y = player.y_position_in_pixels + 13;

	if (player.direction == 0) {
		x1 = x2 = player.x_position_in_pixels + 36;
		do {
			if (x2 > worminator_map.map_width * 16) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)(x2 / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_left_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 += 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x1 && sx <= x2) || (sx + sw >= x1 && sx + sw <= x2))) {
					sprites[collision_detect_loop].hit_points_left -= 2.5;
					for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sx + (sw / 2), sy + (sh / 2), (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
				}
			}
		}
		x2 = (int)(x2 / 16) * 16;

		// Draw a railtrail
		sx = ((int)(screen_width * .8) / 2) + 32;
		if (x2 - (player.x_position_in_pixels + 37) < ((int)(screen_width * .8) / 2) + 32) sx = x2 - (player.x_position_in_pixels + 37);
		for (y = 0; y < sx; y++) {
			x1 = fixtof(fsin(itofix(y * 12))) * 4;
			if (rand() % 64 < 32) spawn_stardust(player.x_position_in_pixels + 37 + y, player.y_position_in_pixels + 13 + x1, 0, 0, 217 + (float)((rand() % 20) - 10.00) / 10.00, 212 + (float)((rand() % 20) - 10.00) / 10.00, 1, 1);
			else spawn_stardust(player.x_position_in_pixels + 37 + y, player.y_position_in_pixels + 13 + x1, (float)((rand() % 20) - 10.00) / 80.00, (float)((rand() % 20) - 10.00) / 80.00, 217 - 64 + (float)((rand() % 20) - 10.00) / 10.00, 212 - 64 + (float)((rand() % 20) - 10.00) / 10.00, 1, 1);
		}
		if (y >= sx) x1 = 2;
		spawn_stardust(player.x_position_in_pixels + 37 + sx, player.y_position_in_pixels + 13 + x1, 0, 0, 6, 2, 4, 16);
	} else {
		x1 = x2 = player.x_position_in_pixels - 4;
		do {
			if (x2 < 0) done_looping = TRUE;
			test_tile = worminator_map.info_tile_grid[(int)(x2 / 16)][(int)(y / 16)];

			if (test_tile == solid_info_tile) done_looping = TRUE;
			else if (test_tile == solid_from_right_no_ammo_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_top_info_tile) done_looping = TRUE;
			else if (test_tile == painful_from_bottom_info_tile) done_looping = TRUE;
			else if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile && test_tile != slope_up_left_to_right_225_left_info_tile && test_tile != slope_up_right_to_left_225_right_info_tile) done_looping = TRUE;
			else if (test_tile == breakable_wall_info_tile) done_looping = TRUE;

			if (done_looping == FALSE) x2 -= 16;
		} while (done_looping == FALSE);
		for (collision_detect_loop = 0; collision_detect_loop < number_of_sprites; collision_detect_loop++) {
			if (sprites[collision_detect_loop].hit_points_left > 0) {
				sx = sprites[collision_detect_loop].x_position_in_pixels;
				sy = sprites[collision_detect_loop].y_position_in_pixels;
				sw = sprites[collision_detect_loop].width, sh = sprites[collision_detect_loop].height;
				if (y >= sy && y <= sy + sh && ((sx >= x2 && sx <= x1) || (sx + sw >= x2 && sx + sw <= x1))) {
					sprites[collision_detect_loop].hit_points_left -= 2.5;
					for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sx + (sw / 2), sy + (sh / 2), (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
				}
			}
		}
		x2 = ((int)(x2 / 16) * 16) + 16;

		// Draw a railtrail
		sx = ((int)(screen_width * .8) / 2) + 32;
		if (player.x_position_in_pixels - 6 - x2 < ((int)(screen_width * .8) / 2) + 32) sx = player.x_position_in_pixels - 6 - x2;
		for (y = 0; y < sx; y++) {
			x1 = fixtof(fsin(itofix(y * 12))) * 4;
			if (rand() % 64 < 32) spawn_stardust(player.x_position_in_pixels - 6 - y, player.y_position_in_pixels + 13 + x1, (float)((rand() % 20) - 10.00) / 80.00, (float)((rand() % 20) - 10.00) / 80.00, 217 + (float)((rand() % 20) - 10.00) / 10.00, 212 + (float)((rand() % 20) - 10.00) / 10.00, 1, 1);
			else spawn_stardust(player.x_position_in_pixels - 6 - y, player.y_position_in_pixels + 13 + x1, 0, 0, 217 - 64 + (float)((rand() % 20) - 10.00) / 10.00, 212 - 64 + (float)((rand() % 20) - 10.00) / 10.00, 1, 1);
		}
		if (y >= sx) x1 = 2;
		spawn_stardust(player.x_position_in_pixels - 6 - sx, player.y_position_in_pixels + 13 + x1, 0, 0, 6, 2, 4, 16);
	}

	// Unfudge the screen width/height
	screen_width = scrw;  screen_height = scrh;
}

/**************\
|** LOAD MAP **|
\**************/
char load_map(char new_level, char *_file_name, char _clear_player)
{
	PSV_DEBUG("load_map(), level = %d, file name = %s", new_level, _file_name);
	FILE *map_file;
	unsigned char sprite_spawn_loop, sprite_type;
	char file_name[512];

	// Disable the game speed timer
	game_is_running = FALSE;

	/* make sure we get either passed a level or a filename */
	if ((new_level == -1) && (_file_name == NULL)) {
		alert("FATAL error: load_map called", "without a filename or level number.", "Aborting!", "OK", 0, 0, 0);
		PSV_DEBUG("FATAL error: load_map called without a filename or level number. Aborting!");
		exit(1);
	}

	// Open the map file
	if (_file_name)
		snprintf(file_name, sizeof(file_name), "%s", _file_name);
	else
		snprintf(file_name, sizeof(file_name), DATADIR "map%02d.map", new_level);

	//PSV_DEBUG("Opening map file: %s", file_name);

	if ((map_file = fopen(file_name, "rb")) == NULL) {
		add_console_line("WARNING: Invalid map file name passed to load_map():");
		add_console_line(file_name);
		add_console_line("Ignoring load map request.");
		alert("Warning: could not open map file:", file_name, "Ignoring load map request!", "OK", 0, 0, 0);
		return FALSE;
	}

	// Set current level & clear player if requested
	current_level = new_level;
	strncpy(current_level_mapfile_path, file_name, sizeof(current_level_mapfile_path));
	if (_clear_player) {
		clear_player();
		player.score = 0;
	}

	// Remove earth quakes
	quake_timer = 0;

	// Fade out the display to black and set the pallete to black to cover up drawings
	fade_out(32);
	set_pallete(black_pallete);

	// BlackOut the screen
	rectfill(swap_buffer, 0, 0, screen_width, screen_height, 255);
	blit_to_screen(swap_buffer);

	// Reset all the sprite states
	clear_sprites();

	// Clear the letter order player variable and make the player face right
	player.letter_order = 0;
	player.direction = 0;


	// Display a level briefing if appropriate
	if (!playing_demo && !recording_demo && player_just_died == FALSE && current_level > 0 && current_level < 7) {
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		display_snapshot(current_level + 5, FALSE);
		show_mouse(NULL);
		set_pallete(black_pallete);
	}
	// Display a level briefing or cutscene if appropriate
	if (!playing_demo && !recording_demo && player_just_died == FALSE && current_level >= 7 && current_level <= 19) {
		if (current_level == 7) {
			set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
			display_recap(FALSE);
		}
		if (current_level == 8) {
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_CUTSCENE_01].dat,
				     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(500);
			fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
			rest(2500);
			fade_out(8);
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_CUTSCENE_02].dat,
				     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(500);
			fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
			rest(2500);
			fade_out(8);
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_CUTSCENE_03].dat,
				     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			play_sample(worminator_data_file[ROCKET_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
			rest(500);
			fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
			play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
			rest(100);
			play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
			rest(100);
			play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
			rest(100);
			play_sample(worminator_data_file[MINIGUN_FIRE_SOUND].dat, 255, 128, 1000, FALSE);
			rest(7000);
			fade_out(8);
		}
		if (current_level == 9) {
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_CUTSCENE_04].dat, 
				     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(500);
			fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
			rest(7500);
			fade_out(8);
		}
		if (current_level == 10) {
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_CUTSCENE_05].dat,
				     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(500);
			fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
			rest(7500);
			fade_out(8);
		}
		if (current_level == 17) {
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_CUTSCENE_06].dat,
				     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(500);
			fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
			rest(7500);
			fade_out(8);
		}
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		display_snapshot(current_level + 8, FALSE);
		show_mouse(NULL);
		set_pallete(black_pallete);
	}

	// Read the entire map and close the file.
	read_worminator_map(&worminator_map, map_file);
	fclose(map_file);

	// This re-calculates the animated tiles data array
	build_animation_table();

	// Position the player in the map
	player.x_position_in_pixels = worminator_map.starting_sprites[0].x_position_in_pixels;
	player.y_position_in_pixels = worminator_map.starting_sprites[0].y_position_in_pixels;

	// Spawn every sprite in the level
	for (sprite_spawn_loop = 1; sprite_spawn_loop < 255; sprite_spawn_loop++) {

		sprite_type = worminator_map.starting_sprites[sprite_spawn_loop].sprite_type;

		//////// NCPs /////////////////////////////////////////////////////////////////////////////////
		if (sprite_type == 1) spawn_sprite(WORM_GIRL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 2) spawn_sprite(WORM_BOY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 3) spawn_sprite(WORM_COP_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 4) spawn_sprite(WORM_GENTLEMEN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 5) spawn_sprite(WORM_STRIPPER_ONE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 6) spawn_sprite(WORM_BLONDE_NUDE_ONE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Getable Items ////////////////////////////////////////////////////////////////////////
		if (sprite_type == 16) spawn_sprite(HEALTH_1_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 17) spawn_sprite(HEALTH_2_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 18) spawn_sprite(HEALTH_5_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 19) spawn_sprite(HEALTH_10_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 20) spawn_sprite(MINIGUN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 21) spawn_sprite(BULLETS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 22) spawn_sprite(BULLETS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 23) spawn_sprite(SHOTGUN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 24) spawn_sprite(SHELLS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 25) spawn_sprite(SHELLS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 26) spawn_sprite(ROCKET_LAUNCHER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 27) spawn_sprite(ROCKETS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 28) spawn_sprite(ROCKETS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 29) spawn_sprite(LASER_GUN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 30) spawn_sprite(CELLS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 31) spawn_sprite(CELLS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 32) spawn_sprite(FLAMETHROWER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 33) spawn_sprite(GASOLINE_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 34) spawn_sprite(GASOLINE_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 35) spawn_sprite(MORTAR_CANNON_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 36) spawn_sprite(MORTARS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 37) spawn_sprite(MORTARS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 38) spawn_sprite(BFG_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 39) spawn_sprite(PLASMA_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 40) spawn_sprite(PLASMA_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 41) spawn_sprite(MINELAYER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 42) spawn_sprite(MINES_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 43) spawn_sprite(MINES_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Worminator 2 Getable Items ///////////////////////////////////////////////////////////
		if (sprite_type == 7) spawn_sprite(PLASMA_BULLETS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 8) spawn_sprite(PLASMA_BULLETS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 9) spawn_sprite(EXPLOSIVE_SHELLS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 10) spawn_sprite(EXPLOSIVE_SHELLS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 11) spawn_sprite(MICRO_NUKE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 12) spawn_sprite(RAILS_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 13) spawn_sprite(RAILS_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 14) spawn_sprite(WALKING_MINES_SMALL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 15) spawn_sprite(WALKING_MINES_LARGE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 44) spawn_sprite(CHAINSAW_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 45) spawn_sprite(RAILGUN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 46) spawn_sprite(STARDUSTER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 47) spawn_sprite(SPIKESTARS_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Enemies //////////////////////////////////////////////////////////////////////////////
		if (sprite_type == 48) spawn_sprite(SMALL_GUN_TURRET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 49) spawn_sprite(SMALL_GUN_TURRET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 50) spawn_sprite(MORTAR_GUN_TURRET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 51) spawn_sprite(MORTAR_GUN_TURRET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 52) spawn_sprite(BIG_GUN_TURRET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 53) spawn_sprite(BIG_GUN_TURRET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 54) spawn_sprite(FLYING_PROBE_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 55) spawn_sprite(FLYING_PROBE_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 56) spawn_sprite(ROBOTIC_DOG_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 57) spawn_sprite(ROBOTIC_DOG_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 58) spawn_sprite(DECIMATER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 59) spawn_sprite(DECIMATER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 60) spawn_sprite(GIANT_ROBOT_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 61) spawn_sprite(GIANT_ROBOT_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 62) spawn_sprite(SMALL_RUNNER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 63) spawn_sprite(SMALL_RUNNER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 64) spawn_sprite(MECHANICAL_CRUSHER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 65) spawn_sprite(MECHANICAL_CRUSHER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 66) spawn_sprite(NUCLEAR_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 67) spawn_sprite(NUCLEAR_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 68) spawn_sprite(LASER_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 69) spawn_sprite(LASER_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 70) spawn_sprite(ROCKET_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 71) spawn_sprite(ROCKET_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 72) spawn_sprite(RIFLE_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 73) spawn_sprite(RIFLE_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 74) spawn_sprite(BIG_GREEN_SLIME_BLOB_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 75) spawn_sprite(BIG_GREEN_SLIME_BLOB_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 76) spawn_sprite(GUMDROP_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 77) spawn_sprite(GUMDROP_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 78) spawn_sprite(BIG_NUCLEAR_BOMB_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 79) spawn_sprite(SMALL_NUCLEAR_BOMB_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 80) spawn_sprite(FIREWORM_SEGMENT_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 81) spawn_sprite(FIREWORM_SEGMENT_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 82) spawn_sprite(FLAME_JET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 83) spawn_sprite(FLAME_JET_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 84) spawn_sprite(WORM_CANNON_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 85) spawn_sprite(WORM_CANNON_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 86) spawn_sprite(WERMAID_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 87) spawn_sprite(WERMAID_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 88) spawn_sprite(BLUE_SLIME_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 89) spawn_sprite(BLUE_SLIME_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 90) spawn_sprite(THUG_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 91) spawn_sprite(THUG_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 92) spawn_sprite(PUNK_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 93) spawn_sprite(PUNK_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 104) spawn_sprite(SPEAR_TAPEWORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 105) spawn_sprite(SPEAR_TAPEWORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 106) spawn_sprite(EGG_TAPEWORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 107) spawn_sprite(EGG_TAPEWORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 108) spawn_sprite(HEAVY_WEAPON_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 109) spawn_sprite(HEAVY_WEAPON_REBEL_WORM_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 110) spawn_sprite(BOSS_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 111) spawn_sprite(BOSS_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 157) spawn_sprite(CIGARETTE_BUTT_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 158) spawn_sprite(STOMPING_FOOT_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 1);
		if (sprite_type == 159) spawn_sprite(STOMPING_FOOT_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Platforms ////////////////////////////////////////////////////////////////////////////
		if (sprite_type >= 96 && sprite_type < 104) {
			spawn_sprite(FLOATING_PLATFORM_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
			sprites[number_of_sprites - 1].frame = FLOATING_PLATFORM_01 + sprite_type - 96;
		}

		//////// Explosives and Misc //////////////////////////////////////////////////////////////////
		if (sprite_type == 112) spawn_sprite(SMALL_EXPLOSIVE_BARREL_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 113) spawn_sprite(SMALL_EXPLOSIVE_NUCLEAR_CRATE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 114) spawn_sprite(SMALL_SPINNING_FAN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 115) spawn_sprite(MEDIUM_SPINNING_FAN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 116) spawn_sprite(LARGE_SPINNING_FAN_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 117) spawn_sprite(WORMINATOR_MINE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels + 3, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels + 11, 0, 0, 0);
		if (sprite_type == 118) spawn_sprite(WORMINATOR_WALKING_MINE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels + 3, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels + 8, .25, 0, 0);
		if (sprite_type == 119) spawn_sprite(WORMINATOR_WALKING_MINE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels + 3, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels + 8, -.25, 0, 0);
		if (sprite_type == 120) spawn_sprite(PUSHABLE_CRATE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 121) spawn_sprite(GIANT_ROCKET_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 122) spawn_sprite(BOULDER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels + 8, 0, 0, 0);
		if (sprite_type == 123) spawn_sprite(BOULDER_ENEMY_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels + 8, 0, 0, 1);
		if (sprite_type == 124) spawn_sprite(EXPLODING_CAR_01_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 125) spawn_sprite(EXPLODING_CAR_02_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Doors ////////////////////////////////////////////////////////////////////////////////
		if (sprite_type == 128) spawn_sprite(GREY_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 129) {
			spawn_sprite(GREY_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
			sprites[number_of_sprites - 1].frame = GREY_DOOR_02;
		}
		if (sprite_type == 130) {
			spawn_sprite(GREY_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
			sprites[number_of_sprites - 1].frame = GREY_DOOR_03;
		}
		if (sprite_type == 131) spawn_sprite(RED_KEYCARD_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 132) spawn_sprite(RED_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 133) {
			spawn_sprite(RED_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
			sprites[number_of_sprites - 1].frame = RED_DOOR_02;
		}
		if (sprite_type == 134) spawn_sprite(RED_DOOR_TOP_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 135) spawn_sprite(RED_DOOR_BOTTOM_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 136) spawn_sprite(GREEN_KEYCARD_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 137) spawn_sprite(GREEN_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 138) {
			spawn_sprite(GREEN_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
			sprites[number_of_sprites - 1].frame = GREEN_DOOR_02;
		}
		if (sprite_type == 139) spawn_sprite(GREEN_DOOR_TOP_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 140) spawn_sprite(GREEN_DOOR_BOTTOM_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 141) spawn_sprite(BLUE_KEYCARD_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 142) spawn_sprite(BLUE_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 143) {
			spawn_sprite(BLUE_DOOR_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
			sprites[number_of_sprites - 1].frame = BLUE_DOOR_02;
		}

		//////// Items Only Worth Points //////////////////////////////////////////////////////////////
		if (sprite_type == 144) spawn_sprite(LETTER_W_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 145) spawn_sprite(LETTER_O_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 146) spawn_sprite(LETTER_R_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 147) spawn_sprite(LETTER_M_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 148) spawn_sprite(SODA_SIXPACK_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 149) spawn_sprite(DOUGHNUTS_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 150) spawn_sprite(BOX_OF_DOUGHNUTS_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 151) spawn_sprite(MUD_BEER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 152) spawn_sprite(DIRT_CHIPS_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 153) spawn_sprite(MUD_ICE_CREAM_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 154) spawn_sprite(MINI_MOUNT_N_DO_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 155) spawn_sprite(MUD_PIE_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 156) spawn_sprite(TANK_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 95) spawn_sprite(BIG_TASTY_BALL_O_DIRT_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Parallaxing Backgrounds //////////////////////////////////////////////////////////////
		if (sprite_type >= 160 && sprite_type < 170)
			parallax_background = worminator_data_file[PARALLAXING_BACKGROUND_01 + sprite_type - 160].dat;

		//////// Spawners /////////////////////////////////////////////////////////////////////////////
		if (sprite_type == 170) spawn_sprite(BLUE_SPARK_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 171) spawn_sprite(RED_SPARK_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 172) spawn_sprite(WHITE_SPARK_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 173) spawn_sprite(GOLD_SPARK_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 174) spawn_sprite(WATER_DROP_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 175) spawn_sprite(NUCLEAR_BOMB_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 186) spawn_sprite(FIREWORK_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 187) spawn_sprite(BOTTLE_ROCKET_SPAWNER_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Palette Effectors ////////////////////////////////////////////////////////////////////
		if (sprite_type == 188) spawn_sprite(DARK_PALETTE_EFFECT_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);
		if (sprite_type == 189) spawn_sprite(LIGHT_PALETTE_EFFECT_SPRITE, worminator_map.starting_sprites[sprite_spawn_loop].x_position_in_pixels, worminator_map.starting_sprites[sprite_spawn_loop].y_position_in_pixels, 0, 0, 0);

		//////// Music ////////////////////////////////////////////////////////////////////////////////
		if (player_just_died == FALSE) {
			if (sprite_spawn_loop == 254 && wormy_config.cool_mode == TRUE) {
				play_midi(worminator_data_file[COOL_MODE_MUSIC].dat, TRUE);
			} else {
				if (sprite_type == 176) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_01].dat, TRUE);
				if (sprite_type == 177) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_02].dat, TRUE);
				if (sprite_type == 178){ 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_03].dat, TRUE);
				}
				if (sprite_type == 179) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_04].dat, TRUE);
				if (sprite_type == 180) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_05].dat, TRUE);
				if (sprite_type == 181) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_06].dat, TRUE);
				if (sprite_type == 182) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_07].dat, TRUE);
				if (sprite_type == 183) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_08].dat, TRUE);
				if (sprite_type == 184) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_09].dat, TRUE);
				if (sprite_type == 185) 
					play_midi(worminator_data_file[WORMINATOR_MUSIC_10].dat, TRUE);
			}
		}
	}

	// Set screen position
	screen_x_position = (int)(player.x_position_in_pixels - ((int)(screen_width * .8) / 2) + 16);
	screen_y_position = (int)(player.y_position_in_pixels - ((int)(screen_height * .96) / 2) + 16 + look_shift);

	if (screen_x_position > worminator_map.map_width * 16 - (int)(screen_width * .8)) screen_x_position = worminator_map.map_width * 16 - (int)(screen_width * .8);
	if (screen_y_position > worminator_map.map_height * 16 - (int)(screen_height * .96)) screen_y_position = worminator_map.map_height * 16 - (int)(screen_height * .96);

	if (screen_x_position < 0) screen_x_position = 0;
	if (screen_y_position < 0) screen_y_position = 0;

	// Reset the display
	set_pallete(black_pallete);
	blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 0, 0, 0, 0, 320, 200);
	update_player_stats();
	if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE) hurt_player(10);
	else hurt_player(0);
	borders_dirty = TRUE;
	render_map();
	fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 32);

	// Reset the just_died variable
	player_just_died = FALSE;

	// Reset the display map and stats variables
	display_map = FALSE;
	display_stats = FALSE;

	// This prevents choppiness from building up
	speed_counter = 0;

	// This clears the idle timer
	idle_timer = 0;

	// Disable bullet time
	//bullet_time_engaged = FALSE;
	return TRUE;
}

/*********************\
|** LOAD SAVED GAME **|
\*********************/
char load_saved_game(char *saved_game_file_name)
{
	// This is the saved game file that needs to be loaded
	FILE *saved_game_file;
	unsigned char sprite_spawn_loop, sprite_type;

	// Disable the game speed timer
	game_is_running = FALSE;

	// Open the files
	saved_game_file = fopen(saved_game_file_name, "rb");
	if (!saved_game_file) {
		alert("Warning: could not open savegame- or demo-file:", saved_game_file_name, "Ignoring load game or demo request!", "OK", 0, 0, 0);
		return FALSE;
	}

	// Use the save game file as mapfile for the current_level
	snprintf(current_level_mapfile_path, sizeof(current_level_mapfile_path), "%s",
		 saved_game_file_name);

	// Remove earth quakes
	quake_timer = 0;

	// Fade out the display
	fade_out(32);

	// Set the pallete to black so the player can't see if we draw anything
	set_pallete(black_pallete);

	// Load the data from the saved game file
	read_worminator_map(&worminator_map, saved_game_file);
	read_dword((unsigned int *)&number_of_sprites, saved_game_file);
	read_sprites(sprites, sizeof(sprites) / sizeof(SPRITE), saved_game_file);
	read_player(&player, saved_game_file);
	read_byte((unsigned char *)&current_level, saved_game_file);

	// We dont need to read anything else, so we can close the files
	fclose(saved_game_file);

	build_animation_table();

	// Restart the music and select the correct parallaxing background
	for (sprite_spawn_loop = 1; sprite_spawn_loop < 255; sprite_spawn_loop++) {
		sprite_type = worminator_map.starting_sprites[sprite_spawn_loop].sprite_type;

		//////// Parallaxing Backgrounds //////////////////////////////////////////////////////////////
		if (sprite_type >= 160 && sprite_type < 170)
			parallax_background = worminator_data_file[PARALLAXING_BACKGROUND_01 + sprite_type - 160].dat;

		//////// Music ////////////////////////////////////////////////////////////////////////////////
		if (sprite_spawn_loop == 254 && wormy_config.cool_mode == TRUE) {
			play_midi(worminator_data_file[COOL_MODE_MUSIC].dat, TRUE);
		} else {
			if (sprite_type == 176) play_midi(worminator_data_file[WORMINATOR_MUSIC_01].dat, TRUE);
			if (sprite_type == 177) play_midi(worminator_data_file[WORMINATOR_MUSIC_02].dat, TRUE);
			if (sprite_type == 178) play_midi(worminator_data_file[WORMINATOR_MUSIC_03].dat, TRUE);
			if (sprite_type == 179) play_midi(worminator_data_file[WORMINATOR_MUSIC_04].dat, TRUE);
			if (sprite_type == 180) play_midi(worminator_data_file[WORMINATOR_MUSIC_05].dat, TRUE);
			if (sprite_type == 181) play_midi(worminator_data_file[WORMINATOR_MUSIC_06].dat, TRUE);
			if (sprite_type == 182) play_midi(worminator_data_file[WORMINATOR_MUSIC_07].dat, TRUE);
			if (sprite_type == 183) play_midi(worminator_data_file[WORMINATOR_MUSIC_08].dat, TRUE);
			if (sprite_type == 184) play_midi(worminator_data_file[WORMINATOR_MUSIC_09].dat, TRUE);
			if (sprite_type == 185) play_midi(worminator_data_file[WORMINATOR_MUSIC_10].dat, TRUE);
		}
	}

	// Set screen position
	screen_x_position = (int)(player.x_position_in_pixels - ((int)(screen_width * .8) / 2) + 16);
	screen_y_position = (int)(player.y_position_in_pixels - ((int)(screen_height * .96) / 2) + 16 + look_shift);

	if (screen_x_position > worminator_map.map_width * 16 - (int)(screen_width * .8)) screen_x_position = worminator_map.map_width * 16 - (int)(screen_width * .8);
	if (screen_y_position > worminator_map.map_height * 16 - (int)(screen_height * .96)) screen_y_position = worminator_map.map_height * 16 - (int)(screen_height * .96);

	if (screen_x_position < 0) screen_x_position = 0;
	if (screen_y_position < 0) screen_y_position = 0;

	// Leave whatever weapon the player had when saved alone
	desired_weapon = player.current_weapon;

	// Reset the display
	blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 0, 0, 0, 0, 320, 200);
	update_player_stats();
	hurt_player(0);
	borders_dirty = TRUE;
	render_map();
	fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 32);

	// Reset the display map and stats variables
	display_map = FALSE;
	display_stats = FALSE;

	// This resets the idle timer
	idle_timer = 0;

	return TRUE;	// success
}

/***************\
|** SAVE GAME **|
\***************/
void save_game(char *saved_game_file_name)
{
	//PSV_DEBUG("save_game()");

	FILE *saved_game_file;

	// Disable the game speed timer
	game_is_running = FALSE;

	saved_game_file = fopen(saved_game_file_name, "wb");
	if (!saved_game_file) {
		alert("Warning: could not open savegame-file:", saved_game_file_name, "Ignoring save game request!", "OK", 0, 0, 0);
		return;
	}

	write_worminator_map(&worminator_map, saved_game_file);
	write_dword((unsigned int *)&number_of_sprites, saved_game_file);
	write_sprites(sprites, sizeof(sprites) / sizeof(SPRITE), saved_game_file);
	write_player(&player, saved_game_file);
	write_byte((unsigned char *)&current_level, saved_game_file);

	fclose(saved_game_file);

	speed_counter = 0;
}

/*****************\
|** LOAD CONFIG **|
\*****************/
void load_config()
{
	// Open the file if it exists, and continue
#ifdef __unix__
	char config_file_name[64];
	char *home_dir = get_home_dir();

	snprintf(config_file_name, sizeof(config_file_name) / sizeof(char),
		 "%s/.worm3.cfg", home_dir ? home_dir : ".");

	override_config_file(config_file_name);
#elif defined (PSVITA)
	char config_file_name[64];
	
	snprintf(config_file_name, sizeof(config_file_name) / sizeof(char),
		 "%sworm3.cfg", PSV_APP_DIR);

	override_config_file(config_file_name);
#else
	override_config_file("worm3.cfg");
#endif

	// Disable the game speed timer and remove earth quakes
	game_is_running = FALSE;

	// Read in all the config settings, defaults will be used if config is missing
#ifdef DINGUX
	wormy_config.screen_width = get_config_int("Options", "screen_width", 320);
	wormy_config.screen_height = get_config_int("Options", "screen_height", 200);
#elif defined (PSVITA) 
	// Make the sprites and screen text bigger by lowering the res. Looks better on Vita.
	wormy_config.screen_width = get_config_int("Options", "screen_width", 512);
	wormy_config.screen_height = get_config_int("Options", "screen_height", 384);
#else
	wormy_config.screen_width = get_config_int("Options", "screen_width", 640);
	wormy_config.screen_height = get_config_int("Options", "screen_height", 480);
#endif

	wormy_config.controls[INPUT_UP] = get_config_int("Keys", "up", KEY_UP);
	wormy_config.controls[INPUT_DOWN] = get_config_int("Keys", "down", KEY_DOWN);
	wormy_config.controls[INPUT_LEFT] = get_config_int("Keys", "left", KEY_LEFT);
	wormy_config.controls[INPUT_RIGHT] = get_config_int("Keys", "right", KEY_RIGHT);
	wormy_config.controls[INPUT_JUMP] = get_config_int("Keys", "jump", KEY_SPACE);
	wormy_config.controls[INPUT_FIRE] = get_config_int("Keys", "fire", KEY_ALT);

#ifdef DINGUX
	wormy_config.controls[INPUT_DUCK] = get_config_int("Keys", "duck", KEY_LSHIFT);
	wormy_config.controls[INPUT_MAP] = get_config_int("Keys", "map", KEY_ENTER);
	wormy_config.controls[INPUT_STATS] = get_config_int("Keys", "stats", KEY_SPACE);

	wormy_config.controls[INPUT_WEAPON_LAST] =
					get_config_int("Keys", "weapon_last", KEY_TAB);
	wormy_config.controls[INPUT_WEAPON_NEXT] =
					get_config_int("Keys", "weapon_next", KEY_BACKSPACE);

#elif defined (PSVITA)
	// Vita is lacking buttons to handle all the actions we need.
	// The solution is to use combo keys.
	// Stats will be Triangle + L.
	// Change ammo will be Triangle + R.
	wormy_config.controls[INPUT_DUCK] = get_config_int("Keys", "duck", KEY_LCONTROL);
	wormy_config.controls[INPUT_MAP] = get_config_int("Keys", "map", KEY_TAB);
	wormy_config.controls[INPUT_STATS] = get_config_int("Keys", "stats", KEY_BACKSPACE); 
	wormy_config.controls[INPUT_CHANGE_AMMO_TYPE] = get_config_int("Keys","change_ammo", KEY_BACKSPACE);
	wormy_config.controls[INPUT_WEAPON_NEXT] = get_config_int("Keys", "weapon_next", KEY_RSHIFT);
	wormy_config.controls[INPUT_BULLIT_TIME] = get_config_int("Keys", "bullettime", KEY_LSHIFT);
#else
	wormy_config.controls[INPUT_DUCK] = get_config_int("Keys", "duck", KEY_Z);
	wormy_config.controls[INPUT_MAP] = get_config_int("Keys", "map", KEY_TAB);
	wormy_config.controls[INPUT_STATS] = get_config_int("Keys", "stats",
							    KEY_CAPSLOCK);
	wormy_config.controls[INPUT_BULLIT_TIME] = get_config_int("Keys", "bullettime",
								  KEY_LSHIFT);
	wormy_config.controls[INPUT_LOOK] = get_config_int("Keys", "look",
							   KEY_V);
	wormy_config.controls[INPUT_WEAPON_0] = get_config_int("Keys", "weapon0",
							       KEY_1);
	wormy_config.controls[INPUT_WEAPON_1] = get_config_int("Keys", "weapon1",
							       KEY_2);
	wormy_config.controls[INPUT_WEAPON_2] = get_config_int("Keys", "weapon2",
							       KEY_3);
	wormy_config.controls[INPUT_WEAPON_3] = get_config_int("Keys", "weapon3",
							       KEY_4);
	wormy_config.controls[INPUT_WEAPON_4] = get_config_int("Keys", "weapon4",
							       KEY_5);
	wormy_config.controls[INPUT_WEAPON_5] = get_config_int("Keys", "weapon5",
							       KEY_6);
	wormy_config.controls[INPUT_WEAPON_6] = get_config_int("Keys", "weapon6",
							       KEY_7);
	wormy_config.controls[INPUT_WEAPON_7] = get_config_int("Keys", "weapon7",
							       KEY_8);
	wormy_config.controls[INPUT_WEAPON_8] = get_config_int("Keys", "weapon8",
							       KEY_9);
	wormy_config.controls[INPUT_WEAPON_9] = get_config_int("Keys", "weapon9",
							       KEY_0);
	wormy_config.controls[INPUT_WEAPON_10] = get_config_int("Keys", "weapon10",
								KEY_MINUS);
	wormy_config.controls[INPUT_WEAPON_11] = get_config_int("Keys", "weapon11",
								KEY_EQUALS);
	wormy_config.controls[INPUT_WEAPON_12] = get_config_int("Keys", "weapon12",
								KEY_BACKSLASH);
	wormy_config.controls[INPUT_WEAPON_BEST] = get_config_int("Keys",
								  "weapon_best", KEY_B);
	wormy_config.controls[INPUT_WEAPON_LAST] = get_config_int("Keys",
								  "weapon_last", KEY_OPENBRACE);
	wormy_config.controls[INPUT_WEAPON_NEXT] = get_config_int("Keys",
								  "weapon_next", KEY_CLOSEBRACE);
	wormy_config.controls[INPUT_CHANGE_AMMO_TYPE] = get_config_int("Keys",
								       "change_ammo", KEY_TILDE);

	wormy_config.secondary_controls[INPUT_UP] = get_config_int("SecondaryKeys",
								   "up", -1);
	wormy_config.secondary_controls[INPUT_DOWN] = get_config_int("SecondaryKeys",
								     "down", -1);
	wormy_config.secondary_controls[INPUT_LEFT] = get_config_int("SecondaryKeys",
								     "left", -1);
	wormy_config.secondary_controls[INPUT_RIGHT] = get_config_int("SecondaryKeys",
								      "right", -1);
	wormy_config.secondary_controls[INPUT_JUMP] = get_config_int("SecondaryKeys",
								     "jump", KEY_RCONTROL);
	wormy_config.secondary_controls[INPUT_FIRE] = get_config_int("SecondaryKeys",
								     "fire", KEY_SPACE);
	wormy_config.secondary_controls[INPUT_DUCK] = get_config_int("SecondaryKeys",
								     "duck", -1);
	wormy_config.secondary_controls[INPUT_MAP] = get_config_int("SecondaryKeys",
								    "map", -1);
	wormy_config.secondary_controls[INPUT_STATS] = get_config_int("SecondaryKeys",
								      "stats", -1);
	wormy_config.secondary_controls[INPUT_BULLIT_TIME] =
		get_config_int("SecondaryKeys", "bullettime", KEY_RSHIFT);
	wormy_config.secondary_controls[INPUT_LOOK] =
		get_config_int("SecondaryKeys", "look", -1);
	wormy_config.secondary_controls[INPUT_WEAPON_0] =
		get_config_int("SecondaryKeys", "weapon0", KEY_1_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_1] =
		get_config_int("SecondaryKeys", "weapon1", KEY_2_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_2] =
		get_config_int("SecondaryKeys", "weapon2", KEY_3_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_3] =
		get_config_int("SecondaryKeys", "weapon3", KEY_4_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_4] =
		get_config_int("SecondaryKeys", "weapon4", KEY_5_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_5] =
		get_config_int("SecondaryKeys", "weapon5", KEY_6_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_6] =
		get_config_int("SecondaryKeys", "weapon6", KEY_7_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_7] =
		get_config_int("SecondaryKeys", "weapon7", KEY_8_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_8] =
		get_config_int("SecondaryKeys", "weapon8", KEY_9_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_9] =
		get_config_int("SecondaryKeys", "weapon9", KEY_0_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_10] =
		get_config_int("SecondaryKeys", "weapon10", KEY_SLASH_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_11] =
		get_config_int("SecondaryKeys", "weapon11", KEY_ASTERISK);
	wormy_config.secondary_controls[INPUT_WEAPON_12] =
		get_config_int("SecondaryKeys", "weapon12", KEY_MINUS_PAD);
	wormy_config.secondary_controls[INPUT_WEAPON_BEST] =
		get_config_int("SecondaryKeys", "weapon_best", -1);
	wormy_config.secondary_controls[INPUT_WEAPON_LAST] =
		get_config_int("SecondaryKeys", "weapon_last", KEY_COMMA);
	wormy_config.secondary_controls[INPUT_WEAPON_NEXT] =
		get_config_int("SecondaryKeys", "weapon_next", KEY_STOP);
	wormy_config.secondary_controls[INPUT_CHANGE_AMMO_TYPE] =
		get_config_int("SecondaryKeys", "change_ammo", KEY_F);
#endif
	wormy_config.high_scores[0] = get_config_int("HighScore", "highscore0", 10000);
	wormy_config.high_scores[1] = get_config_int("HighScore", "highscore1", 10000);
	wormy_config.high_scores[2] = get_config_int("HighScore", "highscore2", 9000);
	wormy_config.high_scores[3] = get_config_int("HighScore", "highscore3", 8000);
	wormy_config.high_scores[4] = get_config_int("HighScore", "highscore4", 7000);
	wormy_config.high_scores[5] = get_config_int("HighScore", "highscore5", 6000);
	wormy_config.high_scores[6] = get_config_int("HighScore", "highscore6", 5000);
	wormy_config.high_scores[7] = get_config_int("HighScore", "highscore7", 1000);
	wormy_config.high_scores[8] = get_config_int("HighScore", "highscore8", 1000);
	wormy_config.high_scores[9] = get_config_int("HighScore", "highscore9", 1000);
	snprintf(wormy_config.high_score_names[0], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename0", "The Worminator"));
	snprintf(wormy_config.high_score_names[1], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename1", "--------------"));
	snprintf(wormy_config.high_score_names[2], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename2", "David Layne"));
	snprintf(wormy_config.high_score_names[3], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename3", "Keith Palmer"));
	snprintf(wormy_config.high_score_names[4], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename4", "Kurtis Palmer"));
	snprintf(wormy_config.high_score_names[5], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename5", "Jeff Nielson"));
	snprintf(wormy_config.high_score_names[6], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename6", "Dave Cha"));
	snprintf(wormy_config.high_score_names[7], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename7", "--------------"));
	snprintf(wormy_config.high_score_names[8], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename8", "Read the readme"));
	snprintf(wormy_config.high_score_names[9], 16, "%s", get_config_string(
			 "HighScoreNames", "highscorename9", "file for help!"));

	wormy_config.game_speed = get_config_int("Options", "game_speed",
						 GAME_SPEED_NORMAL);
	wormy_config.difficulty = get_config_int("Options", "difficulty",
						 DIFFICULTY_EASY);
	wormy_config.skin = get_config_int("Options", "skin", 0);

	wormy_config.particle_detail = get_config_int("Options", "show_particles", 0);
#ifdef ALLEGRO_WINDOWS
	wormy_config.safe_mode = get_config_int("Options", "safe_mode", 1);
#endif
	wormy_config.stretch_levels = get_config_int("Options", "stretch_levels", 1);
	wormy_config.remember_skin = get_config_int("Options", "remember_skin", TRUE);
	wormy_config.may_tricks_mode = get_config_int("Options", "may_tricks_mode",
						      TRUE);
	wormy_config.autoswitch_weapons = get_config_int("Options",
							 "autoswitch_weapons", TRUE);
	wormy_config.switch_weapons_on_pickup = get_config_int("Options",
							       "switch_weapons_on_pickup", FALSE);

	wormy_config.godly_mode = get_config_int("Options", "godly_mode", FALSE);
	wormy_config.freeze_mode = get_config_int("Options", "freeze_mode", FALSE);
	wormy_config.rapid_fire_mode = get_config_int("Options", "rapid_fire_mode",
						      FALSE);
	wormy_config.fly_mode = get_config_int("Options", "fly_mode", FALSE);
	wormy_config.wealth_mode = get_config_int("Options", "wealth_mode", FALSE);
	wormy_config.super_turkey_mode = get_config_int("Options", "super_turkey_mode",
							FALSE);
	wormy_config.ekg_mode = get_config_int("Options", "ekg_mode", FALSE);
	wormy_config.cool_mode = get_config_int("Options", "cool_mode", FALSE);

	wormy_config.sound_volume = get_config_int("Options", "sound_volume", 160);
	wormy_config.music_volume = get_config_int("Options", "music_volume", 160);
	wormy_config.disable_sound = get_config_int("Options", "disable_sound", FALSE);
	wormy_config.disable_music = get_config_int("Options", "disable_music", FALSE);

	wormy_config.skip_intro = get_config_int("Options", "skip_intro", FALSE);
	wormy_config.display_particles = get_config_int("Options", "display_particles",
							TRUE);
	wormy_config.show_laser_sight = get_config_int("Options", "show_laser_sight",
						       TRUE);
	wormy_config.run_windowed = get_config_int("Options", "run_windowed", FALSE);
	wormy_config.enable_FSAA = get_config_int("Options", "enable_FSAA", FALSE);

	speed_counter = 0;

#ifdef ALLEGRO_WINDOWS
	// Special overrides
	if (safe_mode) {
		wormy_config.screen_width = 320;  wormy_config.screen_height = 200;
		wormy_config.disable_sound = TRUE;
		wormy_config.disable_music = TRUE;
		wormy_config.safe_mode = 0;
		wormy_config.run_windowed = TRUE;
	}
#endif
}

/*****************\
|** SAVE CONFIG **|
\*****************/
void save_config()
{
	// Disable the game speed timer
	game_is_running = FALSE;

#ifdef PSVITA
	// Make sure allegro writes to worm3.cfg
	char config_file_name[64];
	snprintf(config_file_name, sizeof(config_file_name) / sizeof(char), "%sworm3.cfg", PSV_APP_DIR);
	override_config_file(config_file_name);
#endif

	// Save all the config settings
	set_config_int("Options", "screen_width", wormy_config.screen_width);
	set_config_int("Options", "screen_height", wormy_config.screen_height);

	set_config_int("Keys", "up", wormy_config.controls[INPUT_UP]);
	set_config_int("Keys", "down", wormy_config.controls[INPUT_DOWN]);
	set_config_int("Keys", "left", wormy_config.controls[INPUT_LEFT]);
	set_config_int("Keys", "right", wormy_config.controls[INPUT_RIGHT]);
	set_config_int("Keys", "jump", wormy_config.controls[INPUT_JUMP]);
	set_config_int("Keys", "fire", wormy_config.controls[INPUT_FIRE]);
	set_config_int("Keys", "duck", wormy_config.controls[INPUT_DUCK]);
	set_config_int("Keys", "map", wormy_config.controls[INPUT_MAP]);
	set_config_int("Keys", "stats", wormy_config.controls[INPUT_STATS]);
	set_config_int("Keys", "bullettime", wormy_config.controls[INPUT_BULLIT_TIME]);
	set_config_int("Keys", "look", wormy_config.controls[INPUT_LOOK]);
	set_config_int("Keys", "weapon0", wormy_config.controls[INPUT_WEAPON_0]);
	set_config_int("Keys", "weapon1", wormy_config.controls[INPUT_WEAPON_1]);
	set_config_int("Keys", "weapon2", wormy_config.controls[INPUT_WEAPON_2]);
	set_config_int("Keys", "weapon3", wormy_config.controls[INPUT_WEAPON_3]);
	set_config_int("Keys", "weapon4", wormy_config.controls[INPUT_WEAPON_4]);
	set_config_int("Keys", "weapon5", wormy_config.controls[INPUT_WEAPON_5]);
	set_config_int("Keys", "weapon6", wormy_config.controls[INPUT_WEAPON_6]);
	set_config_int("Keys", "weapon7", wormy_config.controls[INPUT_WEAPON_7]);
	set_config_int("Keys", "weapon8", wormy_config.controls[INPUT_WEAPON_8]);
	set_config_int("Keys", "weapon9", wormy_config.controls[INPUT_WEAPON_9]);
	set_config_int("Keys", "weapon10", wormy_config.controls[INPUT_WEAPON_10]);
	set_config_int("Keys", "weapon11", wormy_config.controls[INPUT_WEAPON_11]);
	set_config_int("Keys", "weapon12", wormy_config.controls[INPUT_WEAPON_12]);
	set_config_int("Keys", "weapon_best",
		       wormy_config.controls[INPUT_WEAPON_BEST]);
	set_config_int("Keys", "weapon_last",
		       wormy_config.controls[INPUT_WEAPON_LAST]);
	set_config_int("Keys", "weapon_next",
		       wormy_config.controls[INPUT_WEAPON_NEXT]);
	set_config_int("Keys", "change_ammo",
		       wormy_config.controls[INPUT_CHANGE_AMMO_TYPE]);

	set_config_int("SecondaryKeys", "up",
		       wormy_config.secondary_controls[INPUT_UP]);
	set_config_int("SecondaryKeys", "down",
		       wormy_config.secondary_controls[INPUT_DOWN]);
	set_config_int("SecondaryKeys", "left",
		       wormy_config.secondary_controls[INPUT_LEFT]);
	set_config_int("SecondaryKeys", "right",
		       wormy_config.secondary_controls[INPUT_RIGHT]);
	set_config_int("SecondaryKeys", "jump",
		       wormy_config.secondary_controls[INPUT_JUMP]);
	set_config_int("SecondaryKeys", "fire",
		       wormy_config.secondary_controls[INPUT_FIRE]);
	set_config_int("SecondaryKeys", "duck",
		       wormy_config.secondary_controls[INPUT_DUCK]);
	set_config_int("SecondaryKeys", "map",
		       wormy_config.secondary_controls[INPUT_MAP]);
	set_config_int("SecondaryKeys", "stats",
		       wormy_config.secondary_controls[INPUT_STATS]);
	set_config_int("SecondaryKeys", "bullettime",
		       wormy_config.secondary_controls[INPUT_BULLIT_TIME]);
	set_config_int("SecondaryKeys", "look",
		       wormy_config.secondary_controls[INPUT_LOOK]);
	set_config_int("SecondaryKeys", "weapon0",
		       wormy_config.secondary_controls[INPUT_WEAPON_0]);
	set_config_int("SecondaryKeys", "weapon1",
		       wormy_config.secondary_controls[INPUT_WEAPON_1]);
	set_config_int("SecondaryKeys", "weapon2",
		       wormy_config.secondary_controls[INPUT_WEAPON_2]);
	set_config_int("SecondaryKeys", "weapon3",
		       wormy_config.secondary_controls[INPUT_WEAPON_3]);
	set_config_int("SecondaryKeys", "weapon4",
		       wormy_config.secondary_controls[INPUT_WEAPON_4]);
	set_config_int("SecondaryKeys", "weapon5",
		       wormy_config.secondary_controls[INPUT_WEAPON_5]);
	set_config_int("SecondaryKeys", "weapon6",
		       wormy_config.secondary_controls[INPUT_WEAPON_6]);
	set_config_int("SecondaryKeys", "weapon7",
		       wormy_config.secondary_controls[INPUT_WEAPON_7]);
	set_config_int("SecondaryKeys", "weapon8",
		       wormy_config.secondary_controls[INPUT_WEAPON_8]);
	set_config_int("SecondaryKeys", "weapon9",
		       wormy_config.secondary_controls[INPUT_WEAPON_9]);
	set_config_int("SecondaryKeys", "weapon10",
		       wormy_config.secondary_controls[INPUT_WEAPON_10]);
	set_config_int("SecondaryKeys", "weapon11",
		       wormy_config.secondary_controls[INPUT_WEAPON_11]);
	set_config_int("SecondaryKeys", "weapon12",
		       wormy_config.secondary_controls[INPUT_WEAPON_12]);
	set_config_int("SecondaryKeys", "weapon_best",
		       wormy_config.secondary_controls[INPUT_WEAPON_BEST]);
	set_config_int("SecondaryKeys", "weapon_last",
		       wormy_config.secondary_controls[INPUT_WEAPON_LAST]);
	set_config_int("SecondaryKeys", "weapon_next",
		       wormy_config.secondary_controls[INPUT_WEAPON_NEXT]);
	set_config_int("SecondaryKeys", "change_ammo",
		       wormy_config.secondary_controls[INPUT_CHANGE_AMMO_TYPE]);

	set_config_int("HighScore", "highscore0", wormy_config.high_scores[0]);
	set_config_int("HighScore", "highscore1", wormy_config.high_scores[1]);
	set_config_int("HighScore", "highscore2", wormy_config.high_scores[2]);
	set_config_int("HighScore", "highscore3", wormy_config.high_scores[3]);
	set_config_int("HighScore", "highscore4", wormy_config.high_scores[4]);
	set_config_int("HighScore", "highscore5", wormy_config.high_scores[5]);
	set_config_int("HighScore", "highscore6", wormy_config.high_scores[6]);
	set_config_int("HighScore", "highscore7", wormy_config.high_scores[7]);
	set_config_int("HighScore", "highscore8", wormy_config.high_scores[8]);
	set_config_int("HighScore", "highscore9", wormy_config.high_scores[9]);
	set_config_string("HighScoreNames", "highscorename0",
			  wormy_config.high_score_names[0]);
	set_config_string("HighScoreNames", "highscorename1",
			  wormy_config.high_score_names[1]);
	set_config_string("HighScoreNames", "highscorename2",
			  wormy_config.high_score_names[2]);
	set_config_string("HighScoreNames", "highscorename3",
			  wormy_config.high_score_names[3]);
	set_config_string("HighScoreNames", "highscorename4",
			  wormy_config.high_score_names[4]);
	set_config_string("HighScoreNames", "highscorename5",
			  wormy_config.high_score_names[5]);
	set_config_string("HighScoreNames", "highscorename6",
			  wormy_config.high_score_names[6]);
	set_config_string("HighScoreNames", "highscorename7",
			  wormy_config.high_score_names[7]);
	set_config_string("HighScoreNames", "highscorename8",
			  wormy_config.high_score_names[8]);
	set_config_string("HighScoreNames", "highscorename9",
			  wormy_config.high_score_names[9]);

	set_config_int("Options", "game_speed", wormy_config.game_speed);
	set_config_int("Options", "difficulty", wormy_config.difficulty);
	set_config_int("Options", "skin", wormy_config.skin);

	set_config_int("Options", "show_particles", wormy_config.particle_detail);
#ifdef ALLEGRO_WINDOWS
	set_config_int("Options", "safe_mode", wormy_config.safe_mode);
#endif
	set_config_int("Options", "stretch_levels", wormy_config.stretch_levels);
	set_config_int("Options", "remember_skin", wormy_config.remember_skin);
	set_config_int("Options", "may_tricks_mode", wormy_config.may_tricks_mode);
	set_config_int("Options", "autoswitch_weapons",
		       wormy_config.autoswitch_weapons);
	set_config_int("Options", "switch_weapons_on_pickup",
		       wormy_config.switch_weapons_on_pickup);

	set_config_int("Options", "godly_mode", wormy_config.godly_mode);
	set_config_int("Options", "freeze_mode", wormy_config.freeze_mode);
	set_config_int("Options", "rapid_fire_mode", wormy_config.rapid_fire_mode);
	set_config_int("Options", "fly_mode", wormy_config.fly_mode);
	set_config_int("Options", "wealth_mode", wormy_config.wealth_mode);
	set_config_int("Options", "super_turkey_mode", wormy_config.super_turkey_mode);
	set_config_int("Options", "ekg_mode", wormy_config.ekg_mode);
	set_config_int("Options", "cool_mode", wormy_config.cool_mode);

	set_config_int("Options", "sound_volume", wormy_config.sound_volume);
	set_config_int("Options", "music_volume", wormy_config.music_volume);
	set_config_int("Options", "disable_sound", wormy_config.disable_sound);
	set_config_int("Options", "disable_music", wormy_config.disable_music);

	set_config_int("Options", "skip_intro", wormy_config.skip_intro);
	set_config_int("Options", "display_particles", wormy_config.display_particles);
	set_config_int("Options", "show_laser_sight", wormy_config.show_laser_sight);
	set_config_int("Options", "run_windowed", wormy_config.run_windowed);
	set_config_int("Options", "enable_FSAA", wormy_config.enable_FSAA);

	// Flush any changes to disk
	override_config_file(NULL);

	speed_counter = 0;
}

/****************\
|** LOAD TILES **|
\****************/
void load_tiles()
{
	//PSV_DEBUG("load_tiles()");

	BITMAP *tiles;
	int build_tileset_loop;

	// Disable the game speed timer
	game_is_running = FALSE;

	tiles = worminator_data_file[TILES].dat;

	worminator_tiles = create_bitmap_ex(8, (tiles->h / 16) * 256, 16);
	worminator_info_tiles = create_bitmap_ex(8, 3072, 16);
	worminator_graphical_properties_tiles = create_bitmap_ex(8, 3072, 16);

	// Copy the background and foreground tiles row by row
	for (build_tileset_loop = 0; build_tileset_loop < (tiles->h / 16); build_tileset_loop++)
		blit(tiles, worminator_tiles, 0, build_tileset_loop * 16, build_tileset_loop * 256, 0, 256, 16);

	// Copy the info and graphical properties tiles row by row (for the editor)
	for (build_tileset_loop = 0; build_tileset_loop < 12; build_tileset_loop++) {
		blit(worminator_data_file[ED_INFO_TILES].dat, worminator_info_tiles, 0, build_tileset_loop * 16, build_tileset_loop * 256, 0, 256, 16);
		blit(worminator_data_file[ED_GRAPHICAL_PROPERTIES_TILES].dat, worminator_graphical_properties_tiles, 0, build_tileset_loop * 16, build_tileset_loop * 256, 0, 256, 16);
	}
}

/***********************\
|** CHANGE GAME SPEED **|
\***********************/
void change_game_speed(int speed)
{
	//PSV_DEBUG("change_game_speed(), speed=%d", speed);
	// Remove the game speed timer
	remove_int(increment_speed_counter);

	// Reset the game speed timer
	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(speed));

	// Clear the game speed timer
	speed_counter = 0;
}

/***********************\
|** CHANGE RESOLUTION **|
\***********************/
void change_resolution(int x, int y)
{
	char weather_loop;
	int result=0;

	// Change screen width and height variables
	screen_width = x;
	screen_height = y;

	if (screen_height == 200)
		screen_height = 240;

	// Resize the display
#ifdef ALLEGRO_WINDOWS
	if (wormy_config.safe_mode == 1 && wormy_config.run_windowed == FALSE)
		result = set_gfx_mode(GFX_DIRECTX, screen_width, screen_height, 0, 0);
	else if (wormy_config.safe_mode == 0 && wormy_config.run_windowed == FALSE)
		result = set_gfx_mode(GFX_SAFE, screen_width, screen_height, 0, 0);
	else if (wormy_config.safe_mode == 1 && wormy_config.run_windowed == TRUE)
		result = set_gfx_mode(GFX_DIRECTX_WIN, screen_width, screen_height, 0, 0);
	else if (wormy_config.safe_mode == 0 && wormy_config.run_windowed == TRUE)
		result = set_gfx_mode(GFX_GDI, screen_width, screen_height, 0, 0);
#else
	if (wormy_config.run_windowed)
		result = set_gfx_mode(GFX_AUTODETECT_WINDOWED, screen_width, screen_height, 0, 0);
	else
		result = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, screen_width, screen_height, 0, 0);
#endif

	if (result) {
		log2file("ERROR: %s", allegro_error);
		exit(-1);
	}

	screen_height = y;

	// Reset the pallete
	set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);

	// Reset the double buffer and other blitting bitmaps
	destroy_bitmap(double_buffer);
	destroy_bitmap(stretch_buffer);
	destroy_bitmap(FSAA_buffer);
	destroy_bitmap(ed_double_buffer);
	double_buffer = create_bitmap_ex(8, (int)(screen_width * .8), (int)(screen_height * .96));
	stretch_buffer = create_bitmap_ex(8, (int)(screen_width * .8), (int)(screen_height * .96));
	FSAA_buffer = create_bitmap_ex(8, (int)(screen_width * .8), (int)(screen_height * .96));
	ed_double_buffer = create_bitmap_ex(8, screen_width, screen_height);

	// Reset the weather system with some random values
	for (weather_loop = 0; weather_loop < 64; weather_loop++) {
		weather[weather_loop][0] = rand() % screen_width;
		weather[weather_loop][1] = rand() % screen_height;
	}

	// We use allegro's dialog functions, so this is required because the default colors suck with our pallete.
	gui_fg_color = 122;
	gui_bg_color = 133;

	// Reset the demo position
	demox = demoy = 0;
}


/*****************\
|** RESET SOUND **|
\*****************/
void reset_sound()
{
	//PSV_DEBUG("reset_sound()");

	// Reset the sound system
	remove_sound();

	int restart_music = 0;

	if (wormy_config.disable_sound == FALSE && wormy_config.disable_music == FALSE){
		install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0);
	    restart_music = 1;
	}
	else if (wormy_config.disable_sound == FALSE && wormy_config.disable_music == TRUE){
		install_sound(DIGI_AUTODETECT, 0, 0);
	}
	else if (wormy_config.disable_sound == TRUE && wormy_config.disable_music == FALSE){ 
		install_sound(0, MIDI_AUTODETECT, 0);
		restart_music = 1;
	}
	
	if (!restart_music)
		return;

	set_volume(wormy_config.sound_volume, wormy_config.music_volume);

	// Restart the music
	if (current_level != -69) {
		unsigned char sprite_spawn_loop, sprite_type;
		WORMINATOR_MAP_FORMAT buffer_map;

		//PSV_DEBUG("Opening map file: %s", current_level_mapfile_path);
		FILE *map_file = fopen(current_level_mapfile_path, "rb");
		if (!map_file) {
			alert("FATAL error: could not reload map file:", current_level_mapfile_path, "in reset_sound().  Aborting!", "OK", 0, 0, 0);
			PSV_DEBUG("FATAL error: could not reload map file: %s, current_level_mapfile_path");
			exit(1);
		}

		// Read the entire map and close the file.
		read_worminator_map(&buffer_map, map_file);
		fclose(map_file);

		// Scan all sprites and look for a music one
		for (sprite_spawn_loop = 1; sprite_spawn_loop < 255; sprite_spawn_loop++) {
			sprite_type = buffer_map.starting_sprites[sprite_spawn_loop].sprite_type;

			//////// Music ////////////////////////////////////////////////////////////////////////////////
			if (sprite_spawn_loop == 254 && wormy_config.cool_mode == TRUE) {
				play_midi(worminator_data_file[COOL_MODE_MUSIC].dat, TRUE);
			} else {
				if (sprite_type == 176) play_midi(worminator_data_file[WORMINATOR_MUSIC_01].dat, TRUE);
				if (sprite_type == 177) play_midi(worminator_data_file[WORMINATOR_MUSIC_02].dat, TRUE);
				if (sprite_type == 178) play_midi(worminator_data_file[WORMINATOR_MUSIC_03].dat, TRUE);
				if (sprite_type == 179) play_midi(worminator_data_file[WORMINATOR_MUSIC_04].dat, TRUE);
				if (sprite_type == 180) play_midi(worminator_data_file[WORMINATOR_MUSIC_05].dat, TRUE);
				if (sprite_type == 181) play_midi(worminator_data_file[WORMINATOR_MUSIC_06].dat, TRUE);
				if (sprite_type == 182) play_midi(worminator_data_file[WORMINATOR_MUSIC_07].dat, TRUE);
				if (sprite_type == 183) play_midi(worminator_data_file[WORMINATOR_MUSIC_08].dat, TRUE);
				if (sprite_type == 184) play_midi(worminator_data_file[WORMINATOR_MUSIC_09].dat, TRUE);
				if (sprite_type == 185) play_midi(worminator_data_file[WORMINATOR_MUSIC_10].dat, TRUE);
			}
		}
	}
}


/*************************\
|** INITIALIZE FUNCTION **|
\*************************/
void initialize()
{
	int i;
	unsigned char weather_loop;

#ifdef PSVITA
	// Initialize Vita specific stuff.
	if (psvInit() < 0){
		add_console_line("ERROR: PSVITA init failed! Aborting.");
		exit(1);
	}

	// Set the application directory.
	psv_set_app_dir(PSV_APP_DIR);
	// Tell allegro where to look for the data files.
	set_allegro_resource_path(0, PSV_DATA_DIR);
	// Overclock to fasten the booting process.
	psvChangeClockSpeed(444);
#endif

	// Disable the game speed timer
	game_is_running = FALSE;

	// Initilization messages
	add_console_line("Welcome to Worminator 3 version 3.1!");
	add_console_line(" ");
	add_console_line("Starting up...");

	// Initalize the allegro library.
	add_console_line("Initilizing allegro.");
	if (allegro_init()) {
		fprintf(stderr, "Fatal error could not initialize Allegro\n");
		exit(1);
	}
	add_console_line("Initilizing keyboard.");
	install_keyboard();
	
#ifndef PSVITA // Disable mouse for Vita. Joystick is awkward to use as a mouse.
	add_console_line("Initilizing mouse.");
	install_mouse();
	show_mouse(screen);
#endif

	add_console_line("Initilizing timer.");
	install_timer();
	reserve_voices(8, 8);
	//midi_voices = 8;

	// Load from the configuration file
	add_console_line("Loading configuration.");
	load_config();

	// Set color depth before loading any gfx data
	//set_color_depth(16); // PSVITA: Colors are wrong on vita screen (e.g. red is blue).
	set_color_depth(8);
	set_color_conversion(COLORCONV_NONE);

	// Load data files
	add_console_line("Loading datafile.");
	packfile_password("security lockout");

	//PSV_DEBUG("Loading datafile, DATADIR=%s", DATADIR);
	if (exists(DATADIR "wormdata.dat"))
		worminator_data_file = load_datafile(DATADIR "wormdata.dat");
	if (!worminator_data_file) {
		add_console_line("ERROR:  Data file not found!  Aborting.");
		allegro_message("Error . . .\nUnable to locate an appropriate Data File.\nPlease re-install Worminator 3.");
		exit(1);
	}
	// clear packfile_password again so that the .cfg file gets read and written
	// without a password
	packfile_password(NULL);

	// Create cache bitmaps and other graphical stuff
	add_console_line("Creating bitmaps.");
	
	swap_buffer = create_bitmap_ex(8, wormy_config.screen_width, wormy_config.screen_height);
	screen_buffer = create_bitmap_ex(8, 320, 200);
	double_buffer = create_bitmap_ex(8, (int)(screen_width * .8), (int)(screen_height * .96));
	stretch_buffer = create_bitmap_ex(8, (int)(screen_width * .8), (int)(screen_height * .96));
	FSAA_buffer = create_bitmap_ex(8, (int)(screen_width * .8), (int)(screen_height * .96));
	spawned_sprite = create_bitmap_ex(8, 16, 16);
	
	create_trans_table(&trans_table, worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 128, 128, 128, 0);
	color_map = &trans_table;

	// Load the tiles into a cache bitmap
	add_console_line("Loading tiles.");
	load_tiles();

	// Cache the parallaxing background for this map
	parallax_background = worminator_data_file[PARALLAXING_BACKGROUND_01].dat;

	// Set up the animation timer to update 100 times a second
	add_console_line("Installing timers.");
	LOCK_FUNCTION(update_animations);
	install_int(update_animations, 17/*10*/); // Why so frequently?

	// Set up the game speed timer
	LOCK_VARIABLE(speed_counter);
	LOCK_VARIABLE(idle_counter);
	LOCK_FUNCTION(increment_speed_counter);
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(GAME_SPEED_NORMAL));
	change_game_speed(wormy_config.game_speed);

	// Set up the soundz
	add_console_line("Initilizing sound");
	
	reset_sound();

	// Initialize the display
	add_console_line("Initilizing video");
	change_resolution(wormy_config.screen_width, wormy_config.screen_height);
	set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	set_display_switch_mode(SWITCH_AMNESIA);

	// Make text drawn in transparent mode
	add_console_line("Completing initilization.");
	text_mode(-1);

	// Set up the mouse cursor
#if !defined (DINGUX) && !defined (PSVITA)
	set_mouse_sprite(worminator_data_file[MOUSE_CURSOR_01].dat);
#endif

	// Start the weather system with some random values
	for (weather_loop = 0; weather_loop < 64; weather_loop++) {
		weather[weather_loop][0] = rand() % screen_width;
		weather[weather_loop][1] = rand() % screen_height;
	}

	// We use allegro's dialog functions, so this is required because the default colors suck with our pallete.
	gui_fg_color = 122;
	gui_bg_color = 133;

	// Clear out the message attributes
	text_messages[0].timer = -1;
	text_messages[1].timer = -1;
	text_messages[2].timer = -1;
	text_messages[3].timer = -1;

	key_led_flag = 1;	// Stop the capslock key from toggling capslock

	/* key location information */
	for (i = 0; i < KEY_MAX; i++) keylocations[i] = -1;
	keylocations[KEY_ESC] = 0;
	keylocations[KEY_F1] = 1;
	keylocations[KEY_F2] = 2;
	keylocations[KEY_F3] = 3;
	keylocations[KEY_F4] = 4;
	keylocations[KEY_F5] = 5;
	keylocations[KEY_F6] = 6;
	keylocations[KEY_F7] = 7;
	keylocations[KEY_F8] = 8;
	keylocations[KEY_F9] = 9;
	keylocations[KEY_F10] = 10;
	keylocations[KEY_F11] = 11;
	keylocations[KEY_F12] = 12;
	keylocations[KEY_PRTSCR] = 13;
	keylocations[KEY_SCRLOCK] = 14;
	keylocations[KEY_PAUSE] = 15;
	keylocations[KEY_TILDE] = 16;
	keylocations[KEY_1] = 17;
	keylocations[KEY_2] = 18;
	keylocations[KEY_3] = 19;
	keylocations[KEY_4] = 20;
	keylocations[KEY_5] = 21;
	keylocations[KEY_6] = 22;
	keylocations[KEY_7] = 23;
	keylocations[KEY_8] = 24;
	keylocations[KEY_9] = 25;
	keylocations[KEY_0] = 26;
	keylocations[KEY_MINUS] = 27;
	keylocations[KEY_EQUALS] = 28;
	keylocations[KEY_BACKSLASH] = 29;
	keylocations[KEY_BACKSPACE] = 30;
	keylocations[KEY_INSERT] = 31;
	keylocations[KEY_HOME] = 32;
	keylocations[KEY_PGUP] = 33;
	keylocations[KEY_NUMLOCK] = 34;
	keylocations[KEY_SLASH_PAD] = 35;
	keylocations[KEY_ASTERISK] = 36;
	keylocations[KEY_MINUS_PAD] = 37;
	keylocations[KEY_TAB] = 38;
	keylocations[KEY_Q] = 39;
	keylocations[KEY_W] = 40;
	keylocations[KEY_E] = 41;
	keylocations[KEY_R] = 42;
	keylocations[KEY_T] = 43;
	keylocations[KEY_Y] = 44;
	keylocations[KEY_U] = 45;
	keylocations[KEY_I] = 46;
	keylocations[KEY_O] = 47;
	keylocations[KEY_P] = 48;
	keylocations[KEY_OPENBRACE] = 49;
	keylocations[KEY_CLOSEBRACE] = 50;
	keylocations[KEY_DEL] = 51;
	keylocations[KEY_END] = 52;
	keylocations[KEY_PGDN] = 53;
	keylocations[KEY_7_PAD] = 54;
	keylocations[KEY_8_PAD] = 55;
	keylocations[KEY_9_PAD] = 56;
	keylocations[KEY_PLUS_PAD] = 57;
	keylocations[KEY_CAPSLOCK] = 58;
	keylocations[KEY_A] = 59;
	keylocations[KEY_S] = 60;
	keylocations[KEY_D] = 61;
	keylocations[KEY_F] = 62;
	keylocations[KEY_G] = 63;
	keylocations[KEY_H] = 64;
	keylocations[KEY_J] = 65;
	keylocations[KEY_K] = 66;
	keylocations[KEY_L] = 67;
	keylocations[KEY_COLON] = 68;
	keylocations[KEY_QUOTE] = 69;
	keylocations[KEY_ENTER] = 70;
	keylocations[KEY_4_PAD] = 71;
	keylocations[KEY_5_PAD] = 72;
	keylocations[KEY_6_PAD] = 73;
	keylocations[KEY_LSHIFT] = 74;
	keylocations[KEY_Z] = 75;
	keylocations[KEY_X] = 76;
	keylocations[KEY_C] = 77;
	keylocations[KEY_V] = 78;
	keylocations[KEY_B] = 79;
	keylocations[KEY_N] = 80;
	keylocations[KEY_M] = 81;
	keylocations[KEY_COMMA] = 82;
	keylocations[KEY_STOP] = 83;
	keylocations[KEY_SLASH] = 84;
	keylocations[KEY_RSHIFT] = 85;
	keylocations[KEY_UP] = 86;
	keylocations[KEY_1_PAD] = 87;
	keylocations[KEY_2_PAD] = 88;
	keylocations[KEY_3_PAD] = 89;
	keylocations[KEY_ENTER_PAD] = 90;
	keylocations[KEY_LCONTROL] = 91;
	keylocations[KEY_ALT] = 92;
	keylocations[KEY_SPACE] = 93;
	keylocations[KEY_ALTGR] = 94;
	keylocations[KEY_RCONTROL] = 95;
	keylocations[KEY_LEFT] = 96;
	keylocations[KEY_DOWN] = 97;
	keylocations[KEY_RIGHT] = 98;
	keylocations[KEY_0_PAD] = 99;

	generate_wormcode();

	add_console_line("Startup complete!");
	add_console_line("Initilization complete, no errors!");
	add_console_line(" ");

#ifdef PSVITA
	// Back to default speed.
	psvChangeClockSpeed(333);
#endif

}


