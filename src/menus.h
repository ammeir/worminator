/* -- Menus.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * For function descriptions and explanations, see proto.h
 * For a detailed explanation of how the engine works, see
 * the documents that came with this source code package. */

#include "psvita.h"

/*************************\
|** MISC MENU FUNCTIONS **|
\*************************/
int tranx(int value)
{
	return screen_width * (value / 320.0);
}

int trany(int value)
{
	return screen_height * (value / 200.0);
}

int untranx(int value)
{
	return 320 * (value / (float)(screen_width));
}

int untrany(int value)
{
	return 200 * (value / (float)(screen_height));
}

char mouse_clicked_region(int x1, int y1, int x2, int y2)
{
	if (mouse_b & 1 && mouse_x >= tranx(x1) && mouse_y >= trany(y1) && mouse_x <= tranx(x2) && mouse_y <= trany(y2)) return TRUE;
	else return FALSE;
}

char mouse_in_region(int x1, int y1, int x2, int y2)
{
	if (mouse_x >= tranx(x1) && mouse_y >= trany(y1) && mouse_x <= tranx(x2) && mouse_y <= trany(y2)) return TRUE;
	else return FALSE;
}

char region_clicked(int x1, int y1, int x2, int y2)
{
	if (mouse_clicked_region(x1, y1, x2, y2) == TRUE) {
		while (mouse_b & 1);
		if (mouse_in_region(x1, y1, x2, y2) == TRUE) return TRUE;
		else return FALSE;
	} else {
		return FALSE;
	}
}

char mouse_clicked_region_notran(int x1, int y1, int x2, int y2)
{
	if (mouse_b & 1 && mouse_x >= x1 && mouse_y >= y1 && mouse_x <= x2 && mouse_y <= y2) return TRUE;
	else return FALSE;
}

char mouse_in_region_notran(int x1, int y1, int x2, int y2)
{
	if (mouse_x >= x1 && mouse_y >= y1 && mouse_x <= x2 && mouse_y <= y2) return TRUE;
	else return FALSE;
}

char region_clicked_notran(int x1, int y1, int x2, int y2)
{
	if (mouse_clicked_region_notran(x1, y1, x2, y2) == TRUE) {
		while (mouse_b & 1);
		if (mouse_in_region_notran(x1, y1, x2, y2) == TRUE) return TRUE;
		else return FALSE;
	} else {
		return FALSE;
	}
}

// Draws a radial wipe of img on bmp with given angle
void radial_wipe(BITMAP *bmp, BITMAP *img, int x, int y, unsigned char theta)
{
	int w = img->w, h = img->h;
	int i, j;

	// Quadrant one
	if (theta >= 64) {
		masked_blit(img, bmp, w / 2, 0, x + w / 2, y, w / 2, h / 2);
	} else {
		for (i = 0; i < w / 2; i++) {
			j = i * tan(theta * 360 / 256 * RADIAN_CONVERTER);
			if (j > h / 2) j = h / 2; if (j < 0) j = 0;
			masked_blit(img, bmp, w / 2 + i, h / 2 - j, x + w / 2 + i, y + h / 2 - j, 1, j);
		}
		return;
	}

	// Quadrant two
	if (theta >= 128) {
		masked_blit(img, bmp, 0, 0, x, y, w / 2, h / 2);
	} else {
		for (i = w / 2; i >= 0; i--) {
			j = (h / 2) - abs((i - w / 2) * tan(theta * 360 / 256 * RADIAN_CONVERTER));
			if (j > h / 2) j = h / 2; if (j < 0) j = 0;
			masked_blit(img, bmp, i, 0, x + i, y, 1, j);
		}
		return;
	}

	// Quadrant three
	if (theta >= 192) {
		masked_blit(img, bmp, 0, h / 2, x, y + h / 2, w / 2, h / 2);
	} else {
		for (i = w / 2; i >= 0; i--) {
			j = abs((i - w / 2) * tan(theta * 360 / 256 * RADIAN_CONVERTER));
			if (j > h / 2) j = h / 2; if (j < 0) j = 0;
			masked_blit(img, bmp, i, h / 2, x + i, y + h / 2, 1, j);
		}
		return;
	}

	// Quadrant four
	if (theta == 255) {
		masked_blit(img, bmp, w / 2, h / 2, x + w / 2, y + h / 2, w / 2, h / 2);
	} else {
		for (i = 0; i < w / 2; i++) {
			j = (h / 2) - abs(i * tan(theta * 360 / 256 * RADIAN_CONVERTER));
			if (j > h / 2) j = h / 2; if (j < 0) j = 0;
			masked_blit(img, bmp, w / 2 + i, h - j, x + w / 2 + i, y + h - j, 1, j);
		}
		return;
	}
}

void do_radial_wipe(BITMAP *bmp, BITMAP *img, int x, int y, int delay, int step, char direction)
{
	int i;

	if (direction == 0) {	// Clockwise
		for (i = 0; i < 256; i += step) {
			radial_wipe(bmp, img, x, y, (unsigned char)(i));
			blit_to_screen(bmp);
			rest(delay);
		}
	} else if (direction == 1) {	// Counterclockwise
		for (i = 256; i > 0; i -= step) {
			radial_wipe(bmp, img, x, y, (unsigned char)(i));
			blit_to_screen(bmp);
			rest(delay);
		}
	}
	radial_wipe(bmp, img, x, y, 255);
	blit_to_screen(bmp);
}

char *save_game_filename(void)
{
#ifdef __unix__
	static char file_name[64];
	char *home_dir = get_home_dir();

	snprintf(file_name, sizeof(file_name) / sizeof(char),
		 "%s/.worm3.sav", home_dir ? home_dir : ".");
	return file_name;
#elif defined (PSVITA)
	static char file_name[64];
	
	snprintf(file_name, sizeof(file_name) / sizeof(char),
		 "%sworm3.sav", PSV_SAVE_DIR);
	return file_name;
#else
	return "saved.dat";
#endif
}

#define SIMPLE_START_EP1	0
#define SIMPLE_START_EP2	1
#define SIMPLE_START_TUTORIAL	2
#define SIMPLE_START_CUSTOM	3

char *start_names[] = {
	"EPISODE 1",
    "EPISODE 2",
	"TUTORIAL",
	"CUSTOM LEVEL",
    NULL};

#define SIMPLE_MENU_START	    0
#define SIMPLE_MENU_LOAD_GAME	1
#define SIMPLE_MENU_SAVE_GAME	2
#define SIMPLE_MENU_OPTIONS	    3
#define SIMPLE_MENU_SKINS       4
#define SIMPLE_MENU_CHEATS      5
#define SIMPLE_MENU_HIGH_SCORES	6
#define SIMPLE_MENU_HELP	    7
#define SIMPLE_MENU_QUIT	    8
#define SIMPLE_MENU_TOTAL	(SIMPLE_MENU_QUIT + 1)

char *simple_names[] = {
	"START GAME",
	"LOAD GAME",
	"SAVE GAME",
	"OPTIONS",
	"SKINS",
	"CHEATS",
	"HIGH SCORES",
	"HELP",
	"QUIT", 
	NULL};

#define SIMPLE_OP_GAMESPEED	    0
#define SIMPLE_OP_DIFFICULTY	1
#define SIMPLE_OP_PARTDETAIL	2
#define SIMPLE_OP_MAYTRICKS	    3
#define SIMPLE_OP_AUTOWEAPONS	4
#define SIMPLE_OP_WEAPONPICK	5
#define SIMPLE_OP_SKIPINTRO	    6
#define SIMPLE_OP_DISPPARTICLE	7
#define SIMPLE_OP_SHOWLASER	    8
#define SIMPLE_OP_DISABLEMUSIC	9
#define SIMPLE_OP_DISABLESOUND	10

char *option_names[] = {
	"GAME SPEED",		// very slow, slow, normal, fast, very fast
	"DIFICULTY",		//
	"PARTICLE DETAIL",	// full, half
	"MAY TRICKS MODE",	// on, off
	"AUTOSWITCH WEAPONS",	// on, off
	"SWITCH WEAPONS ON PICKUP", // on. off
	"SKIP INTRO",		// on, off
	"DISPLAY PARTICLES",	// on, off
	"SHOW LASER SIGHT",	// on, off
	"ENABLE MUSIC",		// on, off
	"ENABLE SOUND",		// on, off
	NULL
};

char *skin_names[] = {
	"ORIGINAL SKIN",
	"DAMAGED WORM 1 SKIN",
	"DAMAGED WORM 2 SKIN",
	"DAMAGED WORM 3 SKIN",
	"THE BRAIN SKIN",		
	"THE YIPNIK MIKIMBO SKIN",	
	"ORIGINAL WORMINATOR SKIN",	
	"HOTDOG HERO SKIN",	
	"VOODOO BADIE SKIN", 
	"YUANTI SKIN",		
	"MELON O DEATH SKIN",	
	"BAKED TURKEY SKIN",
	"WATERMELON WONDER SKIN",	
	"THE INVERTEBRATOR SKIN",
	"BROWNIE SKIN",
	"EVIL CUBE O DEATH SKIN",
	"ENDO SKELETON SKIN",
	"MUMMY WORM SKIN",
	"THE ANGEL SKIN",
	"THE DEVIL SKIN",
	"TANK_SKIN",
	NULL
};

char *cheat_names[] = {
	"GODLY MODE",
	"FREEZE MODE",
	"RAPID FIRE MODE",
	"FLY MODE",
	"WEALTH MODE",		
	"SUPER TURKEY MODE",	
	"EKG MODE",	
	"COOL MODE",	
	NULL
};

char *help_names[] = {
	"GENERAL",
	"ENEMIES",
	"WEAPONS",
	"STORY",	
	"CREDITS",
	NULL
};

void blit_simple_menu(char *names[], int chosen_option, BITMAP* backup_bitmap)
{
	int i = 0;
	int y = 20;
	int items = 0;

	// Count menu items.
	for (i=0; names[i]; i++)
		items++;
	
	if (current_level == -69){
		// No maps loaded. Draw the menu straight on the swap buffer.
		y = 100;

		// Erase the old menu entries.
		blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
			 swap_buffer->w, swap_buffer->h);

		for (i=0; names[i]; i++, y += 12) {
			textout_centre_ex(swap_buffer, font, names[i],
					  screen_width / 2,  y,
					  (i == chosen_option ? 255 : 7), 16);
		}

	}else{
		// Draw the menu on a background box to make it more visible.

		// Create background bitmap. 
		BITMAP* backgroud_box = create_bitmap(254, 174);

		rectfill(backgroud_box, 0, 0, backgroud_box->w, backgroud_box->h, 200); // Frame
		rectfill(backgroud_box, 1, 1, backgroud_box->w-2, backgroud_box->h-2, 16);
		
		// Calculate y pos of first menu item.
		y = (backgroud_box->h - (items * 12)) / 2;

		for (i=0; names[i]; i++, y += 12) {
			textout_centre_ex(backgroud_box, font, names[i],
							  backgroud_box->w/2,  y,
							  (i == chosen_option ? 255 : 7), 16);
		}

		// Draw to background box to the swap buffer.
		blit(backgroud_box, swap_buffer, 0, 0, 
			(swap_buffer->w - backgroud_box->w) / 2, 
			(swap_buffer->h - backgroud_box->h) / 2, 
			backgroud_box->w, backgroud_box->h); 

		destroy_bitmap(backgroud_box);
	}

	blit_to_screen(swap_buffer);
}

void blit_simple_skins_menu(char *names[], int highlighted, int selected, BITMAP* backup_bitmap)
{
	int i = 0;
	int y = 20;
	int items = 0;
	char selected_name[64] = {0};

	// Count menu items.
	for (i=0; names[i]; i++)
		items++;
	
	if (current_level == -69){
		// No maps loaded. Draw the menu straight on the swap buffer.
		y = 80;

		// Erase the old menu entries.
		blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
			 swap_buffer->w, swap_buffer->h);

		for (i=0; names[i]; i++, y += 12) {

			if (i == selected){
				strcpy(selected_name, "--> ");
				strcat(selected_name, names[i]);
				strcat(selected_name, " <--");
			}else{
				strcpy(selected_name, names[i]);
			}

			textout_centre_ex(swap_buffer, font, selected_name,
					  screen_width / 2,  y,
					  (i == highlighted ? 255 : 7), 16);
		}

	}else{
		// Draw the menu on a background box to make it more visible.

		// Create background bitmap. 
		BITMAP* backgroud_box = create_bitmap(270, 300);

		rectfill(backgroud_box, 0, 0, backgroud_box->w, backgroud_box->h, 200); // Frame
		rectfill(backgroud_box, 1, 1, backgroud_box->w-2, backgroud_box->h-2, 16);
		
		// Calculate y pos of first menu item.
		y = (backgroud_box->h - (items * 12)) / 2;

		for (i=0; names[i]; i++, y += 12) {

			if (i == selected){
				strcpy(selected_name, "--> ");
				strcat(selected_name, names[i]);
				strcat(selected_name, " <--");
			}else{
				strcpy(selected_name, names[i]);
			}
			
			textout_centre_ex(backgroud_box, font, selected_name,
							  backgroud_box->w/2,  y,
							  (i == highlighted ? 255 : 7), 16);
		}

		// Draw to background box to the swap buffer.
		blit(backgroud_box, swap_buffer, 0, 0, 
			(swap_buffer->w - backgroud_box->w) / 2, 
			(swap_buffer->h - backgroud_box->h) / 2, 
			backgroud_box->w, backgroud_box->h); 

		destroy_bitmap(backgroud_box);
	}

	blit_to_screen(swap_buffer);
}

//void blit_simple_menu(char *names[], int chosen_option)
//{
//	int i = 0;
//	int y = 20;
//	//int box_width = 0;
//	//int box_height = y;
//	int items = 0;
//
//	// Count menu items.
//	for (i=0; names[i]; i++)
//		items++;
//
//	// Create background bitmaps. 
//	BITMAP* inner_box = create_bitmap(250, 170); 
//	BITMAP* outer_box = create_bitmap(254, 174); // For the frame.
//
//	//BITMAP* tmp = create_bitmap(box_width, box_height); // background bitmap
//	clear_to_color(inner_box, 16); 
//	clear_to_color(outer_box, 200); 
//
//	// Calculate y pos of first menu item.
//	y = (inner_box->h - (items * 12)) / 2;
//
//	for (i=0; names[i]; i++, y += 12) {
//		textout_centre_ex(inner_box, font, names[i],
//						  inner_box->w/2,  y,
//						  (i == chosen_option ? 255 : 7), 16);
//	}
//
//
//	blit(inner_box, outer_box, 0, 0, 2, 2, inner_box->w, inner_box->h); 
//
//	// Draw to screen bitmap.
//	/*blit(outer_box, screen, 0, 0, 
//		(screen->w - outer_box->w) / 2, 
//		(screen->h - outer_box->h) / 2, 
//		outer_box->w, outer_box->h); */
//
//	// Draw to screen bitmap.
//	blit(outer_box, swap_buffer, 0, 0, 
//		(swap_buffer->w - outer_box->w) / 2, 
//		(swap_buffer->h - outer_box->h) / 2, 
//		outer_box->w, outer_box->h); 
//
//	blit_to_screen(swap_buffer);
//	
//	destroy_bitmap(inner_box);
//	destroy_bitmap(outer_box);
//}

void update_options()
{
	//PSV_DEBUG("update_options()");
	static char game_speed[256];
	static char difficulty[256];

	// GAME SPEED
	switch (wormy_config.game_speed) {
	case GAME_SPEED_VERY_SLOW:
		strcpy(game_speed, "GAME SPEED: VERY SLOW");
		break;
	case GAME_SPEED_SLOW:
		strcpy(game_speed, "GAME SPEED: SLOW");
		break;
	case GAME_SPEED_NORMAL:
		strcpy(game_speed, "GAME SPEED: NORMAL");
		break;
	case GAME_SPEED_FAST:
		strcpy(game_speed, "GAME SPEED: FAST");
		break;
	case GAME_SPEED_VERY_FAST:
		strcpy(game_speed, "GAME SPEED: VERY FAST");
		break;
	}

	option_names[SIMPLE_OP_GAMESPEED] = game_speed;

	// DIFFICULTY
	switch (wormy_config.difficulty) {
	case DIFFICULTY_WIMP:
		strcpy(difficulty, "DIFFICULTY: WIMP");
		break;
	case DIFFICULTY_EASY:
		strcpy(difficulty, "DIFFICULTY: EASY");
		break;
	case DIFFICULTY_NORMAL:
		strcpy(difficulty, "DIFFICULTY: NORMAL");
		break;
	case DIFFICULTY_HARD:
		strcpy(difficulty, "DIFFICULTY: HARD");
		break;
	case DIFFICULTY_NIGHTMARE:
		strcpy(difficulty, "DIFFICULTY: NIGHTMARE");
		break;
	}

	option_names[SIMPLE_OP_DIFFICULTY] = difficulty;

	// PARTICLE DETAIL
	if (wormy_config.particle_detail == 0)	// 0 - full
		option_names[SIMPLE_OP_PARTDETAIL] = "PARTICLE DETAIL: FULL";
	else					// 1 - half
		option_names[SIMPLE_OP_PARTDETAIL] = "PARTICLE DETAIL: HALF";

	// MAY TRICKS MODE
	if (wormy_config.may_tricks_mode == TRUE)
		option_names[SIMPLE_OP_MAYTRICKS] = "MAY TRICKS MODE: ON";
	else
		option_names[SIMPLE_OP_MAYTRICKS] = "MAY TRICKS MODE: OFF";

	// AUTO SWITCH WEAPONS
	if (wormy_config.autoswitch_weapons == TRUE)
		option_names[SIMPLE_OP_AUTOWEAPONS] = "AUTOSWITCH WEAPONS: ON";
	else
		option_names[SIMPLE_OP_AUTOWEAPONS] = "AUTOSWITCH WEAPONS: OFF";

	// SWITCH WEAPONS ON PICKUP
	if (wormy_config.switch_weapons_on_pickup == TRUE)
		option_names[SIMPLE_OP_WEAPONPICK] = "SWITCH WEAPONS ON PICKUP: ON";
	else
		option_names[SIMPLE_OP_WEAPONPICK] = "SWITCH WEAPONS ON PICKUP: OFF";

	// SKIP INTRO
	if (wormy_config.skip_intro == TRUE)
		option_names[SIMPLE_OP_SKIPINTRO] = "SKIP INTRO: ON";
	else
		option_names[SIMPLE_OP_SKIPINTRO] = "SKIP INTRO: OFF";

	// DISPLAY PARTICLES
	if (wormy_config.display_particles == TRUE)
		option_names[SIMPLE_OP_DISPPARTICLE] = "DISPLAY PARTICLES: ON";
	else
		option_names[SIMPLE_OP_DISPPARTICLE] = "DISPLAY PARTICLES: OFF";

	// SHOW LASER SIGHT
	if (wormy_config.show_laser_sight == TRUE)
		option_names[SIMPLE_OP_SHOWLASER] = "SHOW LASER SIGHT: ON";
	else
		option_names[SIMPLE_OP_SHOWLASER] = "SHOW LASER SIGHT: OFF";

	// MUSIC
	if (wormy_config.disable_music == TRUE)
		option_names[SIMPLE_OP_DISABLEMUSIC] = "DISABLE MUSIC: ON";
	else
		option_names[SIMPLE_OP_DISABLEMUSIC] = "DISABLE MUSIC: OFF";

	// SOUND
	if (wormy_config.disable_sound == TRUE)
		option_names[SIMPLE_OP_DISABLESOUND] = "DISABLE SOUND: ON";
	else
		option_names[SIMPLE_OP_DISABLESOUND] = "DISABLE SOUND: OFF";	
}

void update_cheats_menu()
{
	//PSV_DEBUG("update_cheats()");
	cheat_names[GODLY_MODE] = wormy_config.godly_mode? "GODLY MODE: ON": "GODLY MODE: OFF";
	cheat_names[FREEZE_MODE] = wormy_config.freeze_mode? "FREEZE MODE: ON": "FREEZE MODE: OFF";
	cheat_names[RAPID_FIRE_MODE] = wormy_config.rapid_fire_mode? "RAPID FIRE MODE: ON": "RAPID FIRE MODE: OFF";
	cheat_names[FLY_MODE] = wormy_config.fly_mode? "FLY MODE: ON": "FLY MODE: OFF";
	cheat_names[WEALTH_MODE] = wormy_config.wealth_mode? "WEALTH MODE: ON": "WEALTH MODE: OFF";
	cheat_names[SUPER_TURKEY_MODE] = wormy_config.super_turkey_mode? "SUPER TURKEY MODE: ON": "SUPER TURKEY MODE: OFF";
	cheat_names[EKG_MODE] = wormy_config.ekg_mode? "EKG MODE: ON": "EKG MODE: OFF";
	cheat_names[COOL_MODE] = wormy_config.cool_mode? "COOL MODE: ON": "COOL MODE: OFF";
}

/* should be called from wormy_menu() only */
void wormy_options_submenu(BITMAP* backup_bitmap)
{
	static int chosen_suboption = 0;
	int toggle;
	int loop_count = 0;
	int save_needed = 0;
	int must_change_sound = FALSE;
	update_options();

	do {
		idle_speed_counter = 0;
		toggle = 0;

		if (key[KEY_UP]) {
			while (key[KEY_UP]) rest(1);
			if (chosen_suboption > SIMPLE_OP_GAMESPEED)
				chosen_suboption--;
		}

		if (key[KEY_DOWN]) {
			while (key[KEY_DOWN]) rest(1);
			if (chosen_suboption < SIMPLE_OP_DISABLESOUND)
				chosen_suboption++;
		}

		if (key[KEY_RIGHT]) {
			while (key[KEY_RIGHT]) rest(1);

			switch (chosen_suboption) {
			case SIMPLE_OP_GAMESPEED:
				if (wormy_config.game_speed == GAME_SPEED_VERY_SLOW)
					wormy_config.game_speed = GAME_SPEED_SLOW;
				else if (wormy_config.game_speed == GAME_SPEED_SLOW)
					wormy_config.game_speed = GAME_SPEED_NORMAL;
				else if (wormy_config.game_speed == GAME_SPEED_NORMAL)
					wormy_config.game_speed = GAME_SPEED_FAST;
				else if (wormy_config.game_speed == GAME_SPEED_FAST)
					wormy_config.game_speed = GAME_SPEED_VERY_FAST;
				break;
			case SIMPLE_OP_DIFFICULTY:
				if (wormy_config.difficulty < DIFFICULTY_NIGHTMARE)
					wormy_config.difficulty++;
				break;
			}

			toggle = 1;
		}

		if (key[KEY_LEFT]) {
			while (key[KEY_LEFT]) rest(1);

			switch (chosen_suboption) {
			case SIMPLE_OP_GAMESPEED:
				if (wormy_config.game_speed == GAME_SPEED_VERY_FAST)
					wormy_config.game_speed = GAME_SPEED_FAST;
				else if (wormy_config.game_speed == GAME_SPEED_FAST)
					wormy_config.game_speed = GAME_SPEED_NORMAL;
				else if (wormy_config.game_speed == GAME_SPEED_NORMAL)
					wormy_config.game_speed = GAME_SPEED_SLOW;
					else if (wormy_config.game_speed == GAME_SPEED_SLOW)
					wormy_config.game_speed = GAME_SPEED_VERY_SLOW;
				break;
			case SIMPLE_OP_DIFFICULTY:
				if (wormy_config.difficulty > DIFFICULTY_WIMP)
					wormy_config.difficulty--;
				break;
			}

			toggle = 1;
		}

		if (toggle) {
			switch (chosen_suboption) {
			case SIMPLE_OP_PARTDETAIL:
				if (wormy_config.particle_detail == 0)
					wormy_config.particle_detail = 1;
				else
					wormy_config.particle_detail = 0;
				break;
			case SIMPLE_OP_MAYTRICKS:
				if (wormy_config.may_tricks_mode == TRUE)
					wormy_config.may_tricks_mode = FALSE;
				else
					wormy_config.may_tricks_mode = TRUE;
				break;
			case SIMPLE_OP_AUTOWEAPONS:
				if (wormy_config.autoswitch_weapons == TRUE)
					wormy_config.autoswitch_weapons = FALSE;
				else
					wormy_config.autoswitch_weapons = TRUE;
			case SIMPLE_OP_WEAPONPICK:
				if (wormy_config.switch_weapons_on_pickup == TRUE)
					wormy_config.switch_weapons_on_pickup = FALSE;
				else
					wormy_config.switch_weapons_on_pickup = TRUE;
				break;
			case SIMPLE_OP_SKIPINTRO:
				if (wormy_config.skip_intro == TRUE)
					wormy_config.skip_intro = FALSE;
				else
					wormy_config.skip_intro = TRUE;
				break;
			case SIMPLE_OP_DISPPARTICLE:
				if (wormy_config.display_particles == TRUE)
					wormy_config.display_particles = FALSE;
				else
					wormy_config.display_particles = TRUE;
				break;
			case SIMPLE_OP_SHOWLASER:
				if (wormy_config.show_laser_sight == TRUE)
					wormy_config.show_laser_sight = FALSE;
				else
					wormy_config.show_laser_sight = TRUE;
				break;
			case SIMPLE_OP_DISABLEMUSIC:
				if (wormy_config.disable_music == TRUE)
					wormy_config.disable_music = FALSE;
				else
					wormy_config.disable_music = TRUE;
				must_change_sound = TRUE;
				break;
			case SIMPLE_OP_DISABLESOUND:
				if (wormy_config.disable_sound == TRUE)
					wormy_config.disable_sound = FALSE;
				else
					wormy_config.disable_sound = TRUE;
				must_change_sound = TRUE;
			}

			update_options();
			save_needed = 1;
		}

		if (key[KEY_ESC] || key[KEY_LCONTROL]) {
			while (key[KEY_ESC] || key[KEY_LCONTROL]) rest(1);
			goto _exit;
		}

		if (idle_speed_counter == 0){
			rest(1);
		}

		blit_simple_menu(option_names, chosen_suboption, backup_bitmap);
	} while (TRUE);

_exit:
	// Change the game speed if nessecary
	change_game_speed(wormy_config.game_speed);
	// Change the sound if nessecary
	if (must_change_sound == TRUE) reset_sound();

#ifdef PSVITA
	if (save_needed){
		// Normally the settings are saved when quitting the game from the menu.
		// We have to take into account that on Vita you can also quit with PS button.
		save_config(); 
	}
#endif
}

/* should be called from wormy_menu() only */
void wormy_skins_submenu(BITMAP* backup_bitmap)
{
	static int highlighted = 0;
	int selected = wormy_config.skin;
	int loop_count = 0;
	int save_needed = 0;
	
	do {
		idle_speed_counter = 0;

		if (key[KEY_UP]) {
			while (key[KEY_UP]) rest(1);
			if (highlighted > WORMINATOR_SKIN)
				highlighted--;
		}

		if (key[KEY_DOWN]) {
			while (key[KEY_DOWN]) rest(1);
			if (highlighted < TANK_SKIN)
				highlighted++;
		}

		if (key[KEY_SPACE]) {
			while (key[KEY_DOWN]) rest(1);
			player.skin = selected = highlighted;
			save_needed = 1;
		}

		if (key[KEY_ESC] || key[KEY_LCONTROL]) {
			while (key[KEY_ESC] || key[KEY_LCONTROL]) rest(1);
			goto _exit;
		}

		if (idle_speed_counter == 0){
			rest(1);
		}

		blit_simple_skins_menu(skin_names, highlighted, selected, backup_bitmap);
	} while (TRUE);

_exit:

#ifdef PSVITA
	if (save_needed){
		// Store the skin in the config file.
		//if (wormy_config.remember_skin == TRUE)
		wormy_config.skin = player.skin;
		// Normally the settings are saved when quitting the game from the menu.
		// We have to take into account that on Vita you can also quit with PS button.
		save_config(); 
	}
#endif
}

/* should be called from wormy_menu() only */
void wormy_cheats_submenu(BITMAP* backup_bitmap)
{
	static int highlighted = 0;
	int toggle;
	int save_needed = 0;
	
	update_cheats_menu();

	do {
		idle_speed_counter = 0;
		toggle = 0;

		if (key[KEY_UP]) {
			while (key[KEY_UP]) rest(1);
			if (highlighted > GODLY_MODE)
				highlighted--;
		}

		if (key[KEY_DOWN]) {
			while (key[KEY_DOWN]) rest(1);
			if (highlighted < COOL_MODE)
				highlighted++;
		}

		if (key[KEY_RIGHT]) {
			while (key[KEY_RIGHT]) rest(1);
			toggle = 1;
		}

		if (key[KEY_LEFT]) {
			while (key[KEY_LEFT]) rest(1);
			toggle = 1;
		}

		if (toggle) {
			switch (highlighted) {
			case GODLY_MODE:
				wormy_config.godly_mode = wormy_config.godly_mode? FALSE: TRUE;
				break;
			case FREEZE_MODE:
				wormy_config.freeze_mode = wormy_config.freeze_mode? FALSE: TRUE;
				break;
			case RAPID_FIRE_MODE:
				wormy_config.rapid_fire_mode = wormy_config.rapid_fire_mode? FALSE: TRUE;
				break;
			case FLY_MODE:
				wormy_config.fly_mode = wormy_config.fly_mode? FALSE: TRUE;
				break;
			case WEALTH_MODE:
				wormy_config.wealth_mode = wormy_config.wealth_mode? FALSE: TRUE;
				break;
			case SUPER_TURKEY_MODE:
				wormy_config.super_turkey_mode = wormy_config.super_turkey_mode? FALSE: TRUE;
				break;
			case EKG_MODE:
				wormy_config.ekg_mode = wormy_config.ekg_mode? FALSE: TRUE;
				break;
			case COOL_MODE:
				wormy_config.cool_mode = wormy_config.cool_mode? FALSE: TRUE;
				break;
			}

			update_cheats_menu();
			save_needed = 1;
		}

		if (key[KEY_ESC] || key[KEY_LCONTROL]) {
			while (key[KEY_ESC] || key[KEY_LCONTROL]) rest(1);
			goto _exit;
		}

		if (idle_speed_counter == 0){
			rest(1);
		}

		blit_simple_menu(cheat_names, highlighted, backup_bitmap);
	} while (TRUE);

_exit:

#ifdef PSVITA
	if (save_needed){
		// Normally the settings are saved when quitting the game from the menu.
		// We have to take into account that on Vita you can also quit with PS button.
		save_config(); 
	}
#endif
}

/* should be called from wormy_menu() only */
void wormy_help_submenu(BITMAP* backup_bitmap)
{
	static int highlighted = 0;

	do {
		idle_speed_counter = 0;

		if (key[KEY_UP]) {
			while (key[KEY_UP]) rest(1);
			if (highlighted > HELP_GENERAL)
				highlighted--;
		}

		if (key[KEY_DOWN]) {
			while (key[KEY_DOWN]) rest(1);
			if (highlighted < HELP_CREDITS)
				highlighted++;
		}

		if (key[KEY_SPACE]) {
			while (key[KEY_SPACE]) rest(1);
			switch (highlighted){
			case HELP_GENERAL:
				display_snapshot(0, TRUE);
				break;
			case HELP_ENEMIES:
				display_snapshot(1, TRUE);
				break;
			case HELP_WEAPONS:
				display_snapshot(2, TRUE);
				break;
			case HELP_STORY:
				display_story(TRUE);
				break;
			case HELP_CREDITS:
				display_snapshot(3, TRUE);
				break;
			}
		}

		if (key[KEY_ESC] || key[KEY_LCONTROL]) {
			while (key[KEY_ESC] || key[KEY_LCONTROL]) rest(1);
			break;
		}

		if (idle_speed_counter == 0){
			rest(1);
		}

		blit_simple_menu(help_names, highlighted, backup_bitmap);
	} while (TRUE);
}

/* should be called from wormy_menu() only */
char wormy_start_submenu(BITMAP* backup_bitmap)
{
	//PSV_DEBUG("wormy_start_submenu");
	static int chosen_suboption = 0;

	do {
		idle_speed_counter = 0;

		if (key[KEY_UP]) {
			while (key[KEY_UP]) rest(1);
			if (chosen_suboption > SIMPLE_START_EP1)
				chosen_suboption--;
		}

		if (key[KEY_DOWN]) {
			while (key[KEY_DOWN]) rest(1);
			
			if (chosen_suboption < SIMPLE_START_CUSTOM)
				chosen_suboption++;
		}

		if (key[KEY_SPACE]) {
			while (key[KEY_SPACE]) rest(1);

			switch (chosen_suboption) {
			case SIMPLE_START_EP1:
				if (load_map(1, NULL, 1)) {
					return TRUE;
				}
				break;
			case SIMPLE_START_EP2:
				if (load_map(7, NULL, 1)) {
					return TRUE;
				}
				break;
			case SIMPLE_START_TUTORIAL:
				if (load_map(-1, DATADIR "tutorial.map", 1)) {
					return TRUE;
				}
				break;
			case SIMPLE_START_CUSTOM:
				if (start_new_custom_game())
					return TRUE;
				// User propably pressed cancel. 
				// Wait for key release so that we won't enter to the file explorer again.
				while (key[KEY_SPACE]) rest(1);
				break;
			default:
				break;
			}
		}

		if (key[KEY_ESC] || key[KEY_LCONTROL]) {
			while (key[KEY_ESC] || key[KEY_LCONTROL]) rest(1);
			return FALSE;
			idle_speed_counter = 0;
		}

		if (idle_speed_counter == 0) rest(1);

		blit_simple_menu(start_names, chosen_suboption, backup_bitmap);
	} while (TRUE);

	return FALSE;
}

void wormy_menu()
{
	//PSV_DEBUG("wormy_menu()");
	BITMAP *backup_bitmap = create_bitmap_ex(8, swap_buffer->w, swap_buffer->h); 
	char close_menu = FALSE;
	char esc_status = 0;
	int chosen_option = 0;
	static int demo_number = 1;

	// Pause the music
	midi_pause();

	// Set the esc status
	if (key[KEY_ESC] || key[KEY_LCONTROL]) esc_status = 1;

	// Pause the game while the menu is open
	game_is_running = FALSE;
	idle_counter = 0;
	//int loop_counter = 0;

	blit(swap_buffer, backup_bitmap, 0, 0, 0, 0, swap_buffer->w, swap_buffer->h);

	// This is the core menu loop
	do {
		idle_speed_counter = 0;

		if (key[KEY_UP]) {
			while (key[KEY_UP]) rest(1);
			if (chosen_option > SIMPLE_MENU_START)
				chosen_option--;
			idle_counter = 0;
		}

		if (key[KEY_DOWN]) {
			while (key[KEY_DOWN]) rest(1);
			if (chosen_option < SIMPLE_MENU_QUIT)
				chosen_option++;
			idle_counter = 0;
		}

		if (key[KEY_SPACE]) {
			while (key[KEY_SPACE]) rest(1);

			switch (chosen_option) {
			case SIMPLE_MENU_START:
				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);

				if (wormy_start_submenu(backup_bitmap))
					close_menu = TRUE;

				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);
				break;
			case SIMPLE_MENU_LOAD_GAME:
				if (!access(save_game_filename(), R_OK)) {
					if (load_saved_game(save_game_filename()))
						close_menu = TRUE;
				} else {
					alert("You do not appear to have a", "previous saved game to load.  Start or load", "a game instead!", "OK", 0, 0, 0);
					while (key[KEY_SPACE]) rest(1); // Prevents displaying alert message in a loop.
				}
				break;
			case SIMPLE_MENU_SAVE_GAME:
				if (current_level != -69) {
					char* names[2] = {"Saving game...", 0};
					blit_simple_menu(names, 0, backup_bitmap);
					save_game(save_game_filename());
					close_menu = TRUE;
				} else {
					alert("You must be playing a game before", "you can save one.  Start or load", "a game and try again!", "OK", 0, 0, 0);
					while (key[KEY_SPACE]) rest(1); // Prevents displaying alert message in a loop.
				}
				break;
			case SIMPLE_MENU_OPTIONS:
				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);

				wormy_options_submenu(backup_bitmap);

				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);
				break;
			case SIMPLE_MENU_SKINS:
				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);

				wormy_skins_submenu(backup_bitmap);

				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);
				break;
			case SIMPLE_MENU_CHEATS:
				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);

				wormy_cheats_submenu(backup_bitmap);

				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);
				break;
			case SIMPLE_MENU_HELP:
				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);

				wormy_help_submenu(backup_bitmap);

				blit(backup_bitmap, swap_buffer, 0, 0, 0, 0,
					swap_buffer->w, swap_buffer->h);
				break;
			case SIMPLE_MENU_HIGH_SCORES:
				display_snapshot(14, TRUE);
				break;

			case SIMPLE_MENU_QUIT:
				if (current_level > 0)
					check_high_score();
				time_to_quit = TRUE;
				close_menu = TRUE;
				break;

			default:
				break;
			}

			idle_counter = 0;
		}

		// Go back to game if esc pressed
		if (key[KEY_ESC] || key[KEY_LCONTROL]) {
			if (current_level != -69 && esc_status == 0) {
				while (key[KEY_ESC] || key[KEY_LCONTROL]) rest(1);
				close_menu = TRUE;
				idle_counter = 0;
			}
		} else {
			esc_status = 0;
		}

		// Play a demo after idle timeout.
		if (idle_counter >= 4096 && current_level == -69) {

			char demo_file_name[64];
	
			if (demo_number == 1){
				sprintf(demo_file_name,"%sdemo1.dem", PSV_DATA_DIR);
				play_demo(demo_file_name);
			}
			else if (demo_number == 2){ 
				sprintf(demo_file_name,"%sdemo2.dem", PSV_DATA_DIR);
				play_demo(demo_file_name);
			}
			else{ 
				sprintf(demo_file_name,"%sdemo3.dem", PSV_DATA_DIR);
				play_demo(demo_file_name);
			}

			while (key[KEY_SPACE]) rest(1);

			idle_counter = 0;
			current_level = -69;
			if (++demo_number > 3) 
				demo_number = 1;
			player.score = 0;
			bullet_time_engaged = FALSE;
			midi_pause();
		}

		if (idle_speed_counter == 0)
			rest(1);

		blit_simple_menu(simple_names, chosen_option, backup_bitmap);
	} while (close_menu == FALSE);

	show_mouse(NULL);

	if (!time_to_quit) {
		blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 0, 0, 0, 0, 320, 200);
		update_player_stats();
		hurt_player(0);
		borders_dirty = TRUE;
		render_map();
	}

	midi_resume();
	destroy_bitmap(backup_bitmap);
}

/****************\
|** WORMY MENU **|
\****************/
void wormy_menu_old()
{
	BITMAP *backup_bitmap = create_bitmap_ex(8, 320, 200);
	char close_menu = FALSE;
	char close_submenu = FALSE;
	char esc_status = 0;
	char cool_mode_status = wormy_config.cool_mode;
	static int demo_number = 1;

	// Pause the music
	midi_pause();

	// Set the esc status
	if (key[KEY_ESC] || key[KEY_LCONTROL]) esc_status = 1;

	// Pause the game while the menu is open
	game_is_running = FALSE;
	idle_counter = 0;

	// Reset the pallete, just in case it has become corrupted
	//set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);

	// Draw in the wormy menu bar and show the mouse
	draw_sprite(swap_buffer, worminator_data_file[WORMY_MENU_BAR].dat, 0, 0);
	savedisplay();
	show_mouse(screen);
	blit_to_screen(swap_buffer);

	// This is the core menu loop
	do {
		idle_speed_counter = 0;
		// Game submenu button
		if (mouse_b & 1 && mouse_x >= 8 && mouse_x <= 50 && mouse_y >= 4 && mouse_y <= 17) {
			while (mouse_b & 1);
			if (mouse_x >= 8 && mouse_x <= 50 && mouse_y >= 4 && mouse_y <= 17) {
				// Backup what is currently where the new menu will be and then draw the menu
				show_mouse(NULL);
				blit(swap_buffer, backup_bitmap, 12, 20, 0, 0, 80, 176);
				draw_sprite(swap_buffer, worminator_data_file[WORMY_GAME_MENU].dat, 12, 20);
				savedisplay();
				blit_to_screen(swap_buffer);
				show_mouse(screen);

				// Begin game submenu loop
				do {
					idle_speed_counter = 0;
					// New game submenu button
					if (region_clicked_notran(25, 23, 86, 32) == TRUE) {
						char close_subsubmenu = FALSE;
						// Backup what is currently where the new menu will be and then draw the submenu
						show_mouse(NULL);
						blit(swap_buffer, backup_bitmap, 92, 20, 92, 20, 80, 64);
						draw_sprite(swap_buffer, worminator_data_file[WORMY_NEW_GAME_SUBMENU].dat, 92, 20);
						savedisplay();
						blit_to_screen(swap_buffer);
						show_mouse(screen);
						/* Close the submenu we are a subsubmenu of
						 * once we (the subsubmenu) are done */
						close_submenu = TRUE;
						do {
							// Episode 1 button
							if (region_clicked_notran(114, 24, 157, 34) == TRUE) {
								close_subsubmenu = TRUE;
								if ((current_level == -69) || (wormy_dummy_box(2, TRUE) == TRUE))
									if (load_map(1, NULL, 1))
										close_menu = TRUE;
							}
							// Episode 2 button
							else if (region_clicked_notran(114, 40, 158, 50) == TRUE) {
								close_subsubmenu = TRUE;
								if ((current_level == -69) || (wormy_dummy_box(2, TRUE) == TRUE))
									if (load_map(7, NULL, 1))
										close_menu = TRUE;
							}
							// New custom game submenu button
							else if (region_clicked_notran(106, 56, 169, 65) == TRUE) {
								close_subsubmenu = TRUE;
								if (start_new_custom_game())
									close_menu = TRUE;
							}
							// Bonus level button
							else if (region_clicked_notran(110, 69, 161, 79) == TRUE) {
								close_subsubmenu = TRUE;
								if ((current_level == -69) || (wormy_dummy_box(2, TRUE) == TRUE))
									if (load_map(-1, DATADIR "tutorial.map", 1))
										close_menu = TRUE;
							}
							// Clicked off subsubmenu - issue return command
							else if (mouse_b & 1 && (mouse_x < 92 || mouse_x > 171 || mouse_y < 20 || mouse_y > 83)) {
								close_subsubmenu = TRUE;
								// cancel the closing of the submenu we're part of another option on this
								// menu might have been selected
								close_submenu = FALSE;
							}
						} while (close_subsubmenu == FALSE);
						// Clean up this sub menu
						show_mouse(NULL);
						blit(backup_bitmap, swap_buffer, 92, 20, 92, 20, 80, 64);
						savedisplay();
						blit_to_screen(swap_buffer);
						show_mouse(screen);
					}
					// Load game submenu button
					else if (region_clicked_notran(29, 36, 84, 46) == TRUE) {
						close_submenu = TRUE;
						if (!access(save_game_filename(), R_OK)) {
							if ((current_level == -69) || (wormy_dummy_box(1, TRUE) == TRUE))
								if (load_saved_game(save_game_filename()))
									close_menu = TRUE;
						} else {
							alert("You do not appear to have a", "previous saved game to load.  Start or load", "a game instead!", "OK", 0, 0, 0);
						}
					}
					// Save game submenu button
					else if (region_clicked_notran(29, 52, 83, 66) == TRUE) {
						close_submenu = TRUE;
						if (current_level != -69) {
							if (wormy_dummy_box(3, TRUE) == TRUE) {
								save_game(save_game_filename());
								close_menu = TRUE;
							}
						} else {
							alert("You must be playing a game before", "you can save one.  Start or load", "a game and try again!", "OK", 0, 0, 0);
						}
					}
					// Editor submenu button
					else if (region_clicked_notran(41, 86, 69, 96) == TRUE) {
						editor();
						savedisplay();
						close_submenu = TRUE;
					}
					// Options menu submenu button
					else if (region_clicked_notran(37, 102, 74, 112) == TRUE) {
						wormy_options_menu();
						if (current_level == -69) {
							stretch_blit(worminator_data_file[MAIN_TITLE_SCREEN].dat, swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
						} else {
							// Set screen position
							screen_x_position = (int)(player.x_position_in_pixels - ((int)(screen_width * .8) / 2) + 16);
							screen_y_position = (int)(player.y_position_in_pixels - ((int)(screen_height * .96) / 2) + 16 + look_shift);

							if (screen_x_position > worminator_map.map_width * 16 - (int)(screen_width * .8)) screen_x_position = worminator_map.map_width * 16 - (int)(screen_width * .8);
							if (screen_y_position > worminator_map.map_height * 16 - (int)(screen_height * .96)) screen_y_position = worminator_map.map_height * 16 - (int)(screen_height * .96);

							if (screen_x_position < 0) screen_x_position = 0;
							if (screen_y_position < 0) screen_y_position = 0;
							borders_dirty = TRUE;
							stats_dirty = TRUE;
							render_map();
						}
						draw_sprite(swap_buffer, worminator_data_file[WORMY_MENU_BAR].dat, 0, 0);
						blit(swap_buffer, backup_bitmap, 12, 20, 0, 0, 80, 96);
						savedisplay();
						blit_to_screen(swap_buffer);
						show_mouse(screen);
						close_submenu = TRUE;
					}
					// Record demo button
					else if (region_clicked_notran(25, 118, 89, 128) == TRUE) {
						close_submenu = TRUE;
						if (current_level != -69) {
							record_a_demo();
							close_menu = TRUE;
							player.score = 0;
							bullet_time_engaged = FALSE;
							midi_pause();
						} else {
							alert("You must be playing a game before", "you can start recording a demo.  Start or", "load a game and try again!", "OK", 0, 0, 0);
						}
						savedisplay();
					}
					// Play demo button
					else if (region_clicked_notran(27, 133, 86, 144) == TRUE) {
						close_submenu = TRUE;
						if ((current_level == -69) || (alert("WARNING", "Playing a demo will stop any current game", "in progress.  Make sure you have saved!", "OK", "Cancel", 13, 0) == 1)) {
							/* play_a_demo returns non-0 if the gamestate was changed / messed up,
							 * in this case restore the initial menu screen / setup. */
							if (play_a_demo()) {
								stretch_blit(worminator_data_file[MAIN_TITLE_SCREEN].dat,
									     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
								draw_sprite(swap_buffer, worminator_data_file[WORMY_MENU_BAR].dat, 0, 0);
								blit(swap_buffer, backup_bitmap, 12, 20, 0, 0, 80, 176);
								savedisplay();
								blit_to_screen(swap_buffer);
								show_mouse(screen);
								current_level = -69;
								midi_pause();
								player.score = 0;
								bullet_time_engaged = FALSE;
							}
						}
					}
					// High scores button
					else if (region_clicked_notran(30, 150, 85, 160) == TRUE) {
						display_snapshot(14, TRUE);
						close_submenu = TRUE;
					}
					// Exit game submenu button
					else if (region_clicked_notran(27, 180, 84, 190) == TRUE) {
						close_submenu = TRUE;
						if ((current_level == -69) || (wormy_dummy_box(0, TRUE) == TRUE)) {
							show_mouse(NULL);
							if (current_level > 0) check_high_score();
							time_to_quit = TRUE;
							close_menu = TRUE;
						}
					}
					// Clicked off submenu - issue return command
					else if (mouse_b & 1 && (mouse_x < 12 || mouse_x > 91 || mouse_y < 20 || mouse_y > 195)) {
						/* while (mouse_b & 1);
						 * if (mouse_x < 12 || mouse_x > 91 || mouse_y < 20 || mouse_y > 195) */close_submenu = TRUE;
					}

					if (idle_speed_counter == 0) rest(1);
					blit_to_screen(swap_buffer);
				} while (close_submenu == FALSE);

				// Clean up this sub menu
				show_mouse(NULL);
				blit(backup_bitmap, swap_buffer, 0, 0, 12, 20, 80, 176);
				savedisplay();
				show_mouse(screen);
				blit_to_screen(swap_buffer);
				close_submenu = FALSE;
				idle_counter = 0;
			}
		}
		// Skins submenu button
		else if (player.skin != TANK_SKIN && mouse_b & 1 && mouse_x >= 68 && mouse_x <= 95 && mouse_y >= 6 && mouse_y <= 18) {
			while (mouse_b & 1);
			if (mouse_x >= 68 && mouse_x <= 95 && mouse_y >= 6 && mouse_y <= 18) {
				// Backup what is currently where the new menu will be and then draw the menu
				show_mouse(NULL);
				blit(swap_buffer, backup_bitmap, 64, 21, 0, 0, 256, 160);
				draw_sprite(swap_buffer, worminator_data_file[WORMY_SKINS_MENU].dat, 64, 21);
				savedisplay();
				show_mouse(screen);
				blit_to_screen(swap_buffer);

				// Begin game submenu loop
				do {
					idle_speed_counter = 0;
					// Worminator skin submenu button
					if (mouse_b & 1 && mouse_x >= 97 && mouse_x <= 165 && mouse_y >= 25 && mouse_y <= 36) {
						while (mouse_b & 1);
						if (mouse_x >= 97 && mouse_x <= 165 && mouse_y >= 25 && mouse_y <= 36) {
							player.skin = WORMINATOR_SKIN;
							close_submenu = TRUE;
						}
					}
					// Damaged Worm 1 skin submenu button
					else if (mouse_b & 1 && mouse_x >= 84 && mouse_x <= 181 && mouse_y >= 39 && mouse_y <= 50) {
						while (mouse_b & 1);
						if (mouse_x >= 84 && mouse_x <= 181 && mouse_y >= 39 && mouse_y <= 50) {
							player.skin = DAMAGED_WORM_1_SKIN;
							close_submenu = TRUE;
						}
					}
					// Damaged Worm 2 skin submenu button
					else if (mouse_b & 1 && mouse_x >= 84 && mouse_x <= 180 && mouse_y >= 54 && mouse_y <= 65) {
						while (mouse_b & 1);
						if (mouse_x >= 84 && mouse_x <= 180 && mouse_y >= 54 && mouse_y <= 65) {
							player.skin = DAMAGED_WORM_2_SKIN;
							close_submenu = TRUE;
						}
					}
					// Damaged Worm 3 skin submenu button
					else if (mouse_b & 1 && mouse_x >= 84 && mouse_x <= 180 && mouse_y >= 69 && mouse_y <= 80) {
						while (mouse_b & 1);
						if (mouse_x >= 84 && mouse_x <= 180 && mouse_y >= 69 && mouse_y <= 80) {
							player.skin = DAMAGED_WORM_3_SKIN;
							close_submenu = TRUE;
						}
					}
					// The Brain skin submenu button
					else if (mouse_b & 1 && mouse_x >= 108 && mouse_x <= 154 && mouse_y >= 85 && mouse_y <= 95) {
						while (mouse_b & 1);
						if (mouse_x >= 108 && mouse_x <= 154 && mouse_y >= 85 && mouse_y <= 95) {
							player.skin = THE_BRAIN_SKIN;
							close_submenu = TRUE;
						}
					}
					// The Yipnik Mikimbo skin submenu button
					else if (mouse_b & 1 && mouse_x >= 76 && mouse_x <= 187 && mouse_y >= 100 && mouse_y <= 111) {
						while (mouse_b & 1);
						if (mouse_x >= 76 && mouse_x <= 187 && mouse_y >= 100 && mouse_y <= 111) {
							player.skin = THE_YIPNIK_MIKIMBO_SKIN;
							close_submenu = TRUE;
						}
					}
					// Original Worminator skin submenu button
					else if (mouse_b & 1 && mouse_x >= 76 && mouse_x <= 187 && mouse_y >= 117 && mouse_y <= 128) {
						while (mouse_b & 1);
						if (mouse_x >= 76 && mouse_x <= 187 && mouse_y >= 117 && mouse_y <= 128) {
							player.skin = ORIGINAL_WORMINATOR_SKIN;
							close_submenu = TRUE;
						}
					}
					// Hotdog Hero skin submenu button
					else if (mouse_b & 1 && mouse_x >= 102 && mouse_x <= 158 && mouse_y >= 133 && mouse_y <= 143) {
						while (mouse_b & 1);
						if (mouse_x >= 102 && mouse_x <= 158 && mouse_y >= 133 && mouse_y <= 143) {
							player.skin = HOTDOG_HERO_SKIN;
							close_submenu = TRUE;
						}
					}
					// VooDoo Badie skin submenu button
					else if (mouse_b & 1 && mouse_x >= 100 && mouse_x <= 159 && mouse_y >= 148 && mouse_y <= 158) {
						while (mouse_b & 1);
						if (mouse_x >= 100 && mouse_x <= 159 && mouse_y >= 148 && mouse_y <= 158) {
							player.skin = VOODOO_BADIE_SKIN;
							close_submenu = TRUE;
						}
					}
					// Yuanti skin submenu button
					else if (mouse_b & 1 && mouse_x >= 111 && mouse_x <= 145 && mouse_y >= 164 && mouse_y <= 174) {
						while (mouse_b & 1);
						if (mouse_x >= 111 && mouse_x <= 145 && mouse_y >= 164 && mouse_y <= 174) {
							player.skin = YUANTI_SKIN;
							close_submenu = TRUE;
						}
					}
					// Melon O' Death skin submenu button
					else if (mouse_b & 1 && mouse_x >= 215 && mouse_x <= 298 && mouse_y >= 24 && mouse_y <= 35) {
						while (mouse_b & 1);
						if (mouse_x >= 215 && mouse_x <= 298 && mouse_y >= 24 && mouse_y <= 35) {
							player.skin = MELON_O_DEATH_SKIN;
							close_submenu = TRUE;
						}
					}
					// Baked Turkey skin submenu button
					else if (mouse_b & 1 && mouse_x >= 224 && mouse_x <= 293 && mouse_y >= 40 && mouse_y <= 50) {
						while (mouse_b & 1);
						if (mouse_x >= 224 && mouse_x <= 293 && mouse_y >= 40 && mouse_y <= 50) {
							player.skin = BAKED_TURKEY_SKIN;
							close_submenu = TRUE;
						}
					}
					// Watermelon Wonder skin submenu button
					else if (mouse_b & 1 && mouse_x >= 197 && mouse_x <= 316 && mouse_y >= 55 && mouse_y <= 65) {
						while (mouse_b & 1);
						if (mouse_x >= 197 && mouse_x <= 316 && mouse_y >= 55 && mouse_y <= 65) {
							player.skin = WATERMELON_WONDER_SKIN;
							close_submenu = TRUE;
						}
					}
					// The Invertebrator skin submenu button
					else if (mouse_b & 1 && mouse_x >= 214 && mouse_x <= 301 && mouse_y >= 70 && mouse_y <= 81) {
						while (mouse_b & 1);
						if (mouse_x >= 214 && mouse_x <= 301 && mouse_y >= 70 && mouse_y <= 81) {
							player.skin = THE_INVERTEBRATOR_SKIN;
							close_submenu = TRUE;
						}
					}
					// Brownie skin submenu button
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 316 && mouse_y >= 85 && mouse_y <= 95) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 316 && mouse_y >= 85 && mouse_y <= 95) {
							player.skin = BROWNIE_SKIN;
							close_submenu = TRUE;
						}
					}
					// Evil Cube 'O Death skin submenu button
					else if (mouse_b & 1 && mouse_x >= 211 && mouse_x <= 305 && mouse_y >= 100 && mouse_y <= 112) {
						while (mouse_b & 1);
						if (mouse_x >= 211 && mouse_x <= 305 && mouse_y >= 100 && mouse_y <= 112) {
							player.skin = EVIL_CUBE_O_DEATH_SKIN;
							close_submenu = TRUE;
						}
					}
					// Endo-skeleton skin submenu button
					else if (mouse_b & 1 && mouse_x >= 221 && mouse_x <= 295 && mouse_y >= 117 && mouse_y <= 127) {
						while (mouse_b & 1);
						if (mouse_x >= 221 && mouse_x <= 295 && mouse_y >= 117 && mouse_y <= 127) {
							player.skin = ENDO_SKELETON_SKIN;
							close_submenu = TRUE;
						}
					}
					// Mummy Worm skin submenu button
					else if (mouse_b & 1 && mouse_x >= 207 && mouse_x <= 308 && mouse_y >= 133 && mouse_y <= 144) {
						while (mouse_b & 1);
						if (mouse_x >= 207 && mouse_x <= 308 && mouse_y >= 133 && mouse_y <= 144) {
							player.skin = MUMMY_WORM_SKIN;
							close_submenu = TRUE;
						}
					}
					// The Angel skin submenu button
					else if (mouse_b & 1 && mouse_x >= 233 && mouse_x <= 280 && mouse_y >= 148 && mouse_y <= 158) {
						while (mouse_b & 1);
						if (mouse_x >= 233 && mouse_x <= 280 && mouse_y >= 148 && mouse_y <= 158) {
							player.skin = THE_ANGEL_SKIN;
							close_submenu = TRUE;
						}
					}
					// The Devil skin submenu button
					else if (mouse_b & 1 && mouse_x >= 236 && mouse_x <= 278 && mouse_y >= 163 && mouse_y <= 173) {
						while (mouse_b & 1);
						if (mouse_x >= 236 && mouse_x <= 278 && mouse_y >= 163 && mouse_y <= 173) {
							player.skin = THE_DEVIL_SKIN;
							close_submenu = TRUE;
						}
					}
					// Clicked off submenu - issue return command
					else if (mouse_b & 1 && (mouse_x < 64 || mouse_x > 320 || mouse_y < 21 || mouse_y > 180)) {
						/* while (mouse_b & 1);
						 * if (mouse_x < 64 || mouse_x > 320 || mouse_y < 21 || mouse_y > 180) */close_submenu = TRUE;
					}
					if (idle_speed_counter == 0) rest(1);
					blit_to_screen(swap_buffer);
				} while (close_submenu == FALSE);

				// Clean up this sub menu
				show_mouse(NULL);
				blit(backup_bitmap, swap_buffer, 0, 0, 64, 21, 256, 160);
				savedisplay();
				show_mouse(screen);
				blit_to_screen(swap_buffer);
				close_submenu = FALSE;
				idle_counter = 0;

				// Store the skin in the config file if appropriate
				if (wormy_config.remember_skin == TRUE) wormy_config.skin = player.skin;
			}
		}
		// Help submenu button
		else if (mouse_b & 1 && mouse_x >= 116 && mouse_x <= 137 && mouse_y >= 4 && mouse_y <= 16) {
			while (mouse_b & 1);
			if (mouse_x >= 116 && mouse_x <= 137 && mouse_y >= 4 && mouse_y <= 16) {
				// Backup what is currently where the new menu will be and then draw the menu
				show_mouse(NULL);
				blit(swap_buffer, backup_bitmap, 119, 18, 0, 0, 64, 128);
				draw_sprite(swap_buffer, worminator_data_file[WORMY_HELP_MENU].dat, 119, 18);
				savedisplay();
				show_mouse(screen);
				blit_to_screen(swap_buffer);

				// Begin help submenu loop
				do {
					idle_speed_counter = 0;
					// General submenu button
					if (mouse_b & 1 && mouse_x >= 137 && mouse_x <= 175 && mouse_y >= 23 && mouse_y <= 33) {
						while (mouse_b & 1);
						if (mouse_x >= 137 && mouse_x <= 175 && mouse_y >= 23 && mouse_y <= 33) {
							display_snapshot(0, TRUE);
							close_submenu = TRUE;
						}
					}
					// Enemies submenu button
					else if (mouse_b & 1 && mouse_x >= 134 && mouse_x <= 179 && mouse_y >= 38 && mouse_y <= 48) {
						while (mouse_b & 1);
						if (mouse_x >= 134 && mouse_x <= 179 && mouse_y >= 38 && mouse_y <= 48) {
							display_snapshot(1, TRUE);
							close_submenu = TRUE;
						}
					}
					// Weapons submenu button
					else if (mouse_b & 1 && mouse_x >= 134 && mouse_x <= 180 && mouse_y >= 50 && mouse_y <= 63) {
						while (mouse_b & 1);
						if (mouse_x >= 134 && mouse_x <= 180 && mouse_y >= 50 && mouse_y <= 63) {
							display_snapshot(2, TRUE);
							close_submenu = TRUE;
						}
					}
					// Credits submenu button
					else if (mouse_b & 1 && mouse_x >= 139 && mouse_x <= 172 && mouse_y >= 68 && mouse_y <= 79) {
						while (mouse_b & 1);
						if (mouse_x >= 139 && mouse_x <= 172 && mouse_y >= 68 && mouse_y <= 79) {
							display_snapshot(3, TRUE);
							close_submenu = TRUE;
						}
					}
					// Story submenu button
					else if (mouse_b & 1 && mouse_x >= 142 && mouse_x <= 171 && mouse_y >= 84 && mouse_y <= 95) {
						while (mouse_b & 1);
						if (mouse_x >= 142 && mouse_x <= 171 && mouse_y >= 84 && mouse_y <= 95) {
							display_story(TRUE);
							close_submenu = TRUE;
						}
					}
					// Controls submenu button
					else if (mouse_b & 1 && mouse_x >= 134 && mouse_x <= 178 && mouse_y >= 99 && mouse_y <= 110) {
						while (mouse_b & 1);
						if (mouse_x >= 134 && mouse_x <= 178 && mouse_y >= 99 && mouse_y <= 110) {
							display_snapshot(4, TRUE);
							close_submenu = TRUE;
						}
					}
					// About submenu button
					else if (mouse_b & 1 && mouse_x >= 136 && mouse_x <= 173 && mouse_y >= 129 && mouse_y <= 139) {
						while (mouse_b & 1);
						if (mouse_x >= 136 && mouse_x <= 173 && mouse_y >= 129 && mouse_y <= 139) {
							display_snapshot(5, TRUE);
							close_submenu = TRUE;
						}
					}
					// Clicked off submenu - issue return command
					else if (mouse_b & 1 && (mouse_x < 119 || mouse_x > 182 || mouse_y < 18 || mouse_y > 145)) {
						/* while (mouse_b & 1);
						 * if (mouse_x < 119 || mouse_x > 182 || mouse_y < 18 || mouse_y > 145) */close_submenu = TRUE;
					}
					if (idle_speed_counter == 0) rest(1);
					blit_to_screen(swap_buffer);
				} while (close_submenu == FALSE);

				// Clean up this sub menu
				show_mouse(NULL);
				blit(backup_bitmap, swap_buffer, 0, 0, 119, 18, 64, 128);
				savedisplay();
				show_mouse(screen);
				blit_to_screen(swap_buffer);
				close_submenu = FALSE;
				idle_counter = 0;
			}
		}
		// Cheats submenu button
		else if (mouse_b & 1 && mouse_x >= 176 && mouse_x <= 210 && mouse_y >= 3 && mouse_y <= 14) {
			while (mouse_b & 1);
			if (mouse_x >= 176 && mouse_x <= 210 && mouse_y >= 3 && mouse_y <= 14) {
				// Backup what is currently where the new menu will be and then draw the menu
				show_mouse(NULL);
				draw_sprite(swap_buffer, worminator_data_file[WORMY_MENU_BAR_SHOW_CHEATS].dat, 0, 0);
				blit(swap_buffer, backup_bitmap, 177, 18, 0, 0, 128, 160);
				draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
				if (wormy_config.godly_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 25, 8, 8);
				if (wormy_config.freeze_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 40, 8, 8);
				if (wormy_config.rapid_fire_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 56, 8, 8);
				if (wormy_config.fly_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 71, 8, 8);
				if (wormy_config.wealth_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 86, 8, 8);
				if (wormy_config.super_turkey_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 101, 8, 8);
				if (wormy_config.ekg_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 116, 8, 8);
				if (wormy_config.cool_mode == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 131, 8, 8);
				if (show_fps == TRUE)
					blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat,
						swap_buffer, 8, 0, 291, 163, 8, 8);
				savedisplay();
				show_mouse(screen);
				blit_to_screen(swap_buffer);

				// Begin help submenu loop
				do {
					idle_speed_counter = 0;
					// Godly mode checkbox
					if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 23 && mouse_y <= 34) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 23 && mouse_y <= 34) {
							if (wormy_config.godly_mode == TRUE) wormy_config.godly_mode = FALSE;
							else wormy_config.godly_mode = TRUE;
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// Freeze mode checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 38 && mouse_y <= 49) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 38 && mouse_y <= 49) {
							if (wormy_config.freeze_mode == TRUE) wormy_config.freeze_mode = FALSE;
							else wormy_config.freeze_mode = TRUE;
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// Rapid fire mode checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 54 && mouse_y <= 65) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 54 && mouse_y <= 65) {
							if (wormy_config.rapid_fire_mode == TRUE) wormy_config.rapid_fire_mode = FALSE;
							else wormy_config.rapid_fire_mode = TRUE;
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// Fly mode checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 69 && mouse_y <= 80) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 69 && mouse_y <= 80) {
							if (wormy_config.fly_mode == TRUE) wormy_config.fly_mode = FALSE;
							else wormy_config.fly_mode = TRUE;
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// Wealth mode checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 84 && mouse_y <= 95) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 84 && mouse_y <= 95) {
							if (wormy_config.wealth_mode == TRUE) wormy_config.wealth_mode = FALSE;
							else wormy_config.wealth_mode = TRUE;
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// Super turkey mode checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 99 && mouse_y <= 110) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 99 && mouse_y <= 110) {
							if (wormy_config.super_turkey_mode == TRUE) {
								wormy_config.super_turkey_mode = FALSE;
							} else {
								wormy_config.super_turkey_mode = TRUE;
								if (player.skin == TANK_SKIN) {
									if (wormy_config.remember_skin == TRUE) player.skin = wormy_config.skin;
									else player.skin = WORMINATOR_SKIN;
									player.y_acceleration = -1.7;
									spawn_sprite(TANK_SPRITE, player.x_position_in_pixels, player.y_position_in_pixels, 0, 0, player.direction);
									player.y_position_in_pixels -= 16;
									player.x_position_in_pixels += 32;
								}
							}
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// EKG mode checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 114 && mouse_y <= 125) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 114 && mouse_y <= 125) {
							if (wormy_config.ekg_mode == TRUE) wormy_config.ekg_mode = FALSE;
							else wormy_config.ekg_mode = TRUE;
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// Cool mode checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 129 && mouse_y <= 140) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 129 && mouse_y <= 140) {
							if (wormy_config.cool_mode == TRUE)
								wormy_config.cool_mode = FALSE;
								/*midi_resume();
								 * stop_midi();
								 * reset_sound();
								 * midi_pause();*/
							else
								wormy_config.cool_mode = TRUE;
								/*midi_resume();
								 * stop_midi();
								 * reset_sound();
								 * midi_pause();*/
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// FPS display checkbox
					else if (mouse_b & 1 && mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 159 && mouse_y <= 172) {
						while (mouse_b & 1);
						if (mouse_x >= 194 && mouse_x <= 300 && mouse_y >= 159 && mouse_y <= 172) {
							if (show_fps == TRUE) show_fps = FALSE;
							else show_fps = TRUE;
							show_mouse(NULL);
							draw_sprite(swap_buffer, worminator_data_file[WORMY_CHEATS_MENU].dat, 177, 18);
							if (wormy_config.godly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 25, 8, 8);
							if (wormy_config.freeze_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 40, 8, 8);
							if (wormy_config.rapid_fire_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 56, 8, 8);
							if (wormy_config.fly_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 71, 8, 8);
							if (wormy_config.wealth_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 86, 8, 8);
							if (wormy_config.super_turkey_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 101, 8, 8);
							if (wormy_config.ekg_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 116, 8, 8);
							if (wormy_config.cool_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 131, 8, 8);
							if (show_fps == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, swap_buffer, 8, 0, 291, 163, 8, 8);
							savedisplay();
							show_mouse(screen);
						}
					}
					// Clicked off submenu - issue return command
					else if (mouse_b & 1 && (mouse_x < 177 || mouse_x > 304 || mouse_y < 18 || mouse_y > 145)) {
						/* while (mouse_b & 1);
						 * if (mouse_x < 177 || mouse_x > 304 || mouse_y < 18 || mouse_y > 145) */close_submenu = TRUE;
					}
					if (idle_speed_counter == 0) rest(1);
					blit_to_screen(swap_buffer);
				} while (close_submenu == FALSE);

				// Clean up this sub menu
				show_mouse(NULL);
				blit(backup_bitmap, swap_buffer, 0, 0, 177, 18, 128, 160);
				savedisplay();
				show_mouse(screen);
				blit_to_screen(swap_buffer);
				close_submenu = FALSE;
				idle_counter = 0;
			}
		}
		// Resume game button
		else if (mouse_b & 1 && mouse_x >= 236 && mouse_x <= 311 && mouse_y >= 6 && mouse_y <= 17) {
			while (mouse_b & 1);
			if (mouse_x >= 236 && mouse_x <= 311 && mouse_y >= 6 && mouse_y <= 17 && current_level != -69) close_menu = TRUE;
			idle_counter = 0;
		}

		// Go back to game if esc pressed
		if (key[KEY_ESC] || key[KEY_LCONTROL]) {
			if (current_level != -69 && esc_status == 0) {
				while (key[KEY_ESC] || key[KEY_LCONTROL]);
				close_menu = TRUE;
				idle_counter = 0;
			}
		} else {
			esc_status = 0;
		}

		// Play a demo if appropriate
		if (idle_counter >= 4096 && current_level == -69) {
			show_mouse(NULL);
			if (demo_number == 1) play_demo("demo1.dem");
			else if (demo_number == 2) play_demo("demo2.dem");
			else play_demo("demo3.dem");
			stretch_blit(worminator_data_file[MAIN_TITLE_SCREEN].dat, swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			draw_sprite(swap_buffer, worminator_data_file[WORMY_MENU_BAR].dat, 0, 0);
			blit(swap_buffer, backup_bitmap, 12, 20, 0, 0, 80, 96);
			savedisplay();
			show_mouse(screen);
			blit_to_screen(swap_buffer);
			idle_counter = 0;
			current_level = -69;
			demo_number++;
			if (demo_number > 3) demo_number = 1;
			player.score = 0;
			bullet_time_engaged = FALSE;
			midi_pause();
		}

		if (idle_speed_counter == 0) rest(1);
		blit_to_screen(swap_buffer);
	} while (close_menu == FALSE);

	// Hide the mouse pointer and refresh the display
	show_mouse(NULL);
	blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 0, 0, 0, 0, 320, 200);
	update_player_stats();
	hurt_player(0);
	borders_dirty = TRUE;
	render_map();
	destroy_bitmap(backup_bitmap);
	midi_resume();
	if (cool_mode_status != wormy_config.cool_mode) reset_sound();
	if (wormy_config.cool_mode && bullet_time_engaged) greenify();
}

/************************\
|** WORMY OPTIONS MENU **|
\************************/
void wormy_options_menu()
{
	BITMAP *temp_buffer = create_bitmap_ex(8, 320, 200);
	int new_sw = screen_width, new_sh = screen_height;
	char close_options_menu = FALSE;
	char redraw_options_menu = TRUE;
	char must_change_resolution = FALSE;
	char must_change_sound = FALSE;

	// Pause the game while looking at the options menu
	game_is_running = FALSE;

	// This is the core snapshot loop
	do {
		idle_speed_counter = 0;

		// Re-draw the options menu if nessecary
		if (redraw_options_menu == TRUE) {
			show_mouse(NULL);
			blit(worminator_data_file[WORMY_OPTIONS_MENU].dat, temp_buffer, 0, 0, 0, 0, 320, 200);

			// Display the resolution
			if (new_sh == 200) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 12, 3, 106, 38, 69, 7);
			else if (new_sh == 240) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 12, 12, 106, 38, 69, 7);
			else if (new_sh == 300) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 12, 21, 106, 38, 69, 7);
			else if (new_sh == 384) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 12, 30, 106, 38, 69, 7);
			else if (new_sh == 480) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 12, 39, 106, 38, 69, 7);
			else if (new_sh == 600) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 12, 48, 106, 38, 69, 7);
			else if (new_sh == 768) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 9, 57, 103, 38, 74, 7);
			else if (new_sh == 864) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 9, 66, 103, 38, 74, 7);
			else if (new_sh == 960) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 9, 75, 103, 38, 74, 7);
			else if (new_sh == 1024) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 9, 84, 103, 38, 74, 7);
			else if (new_sh == 1200) blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 9, 93, 103, 38, 74, 7);

			// Display the game speed
			if (wormy_config.game_speed == GAME_SPEED_VERY_SLOW) blit(worminator_data_file[WORMY_OPTIONS_MENU_GAME_SPEED_DROPDOWN].dat, temp_buffer, 27, 3, 119, 52, 42, 9);
			else if (wormy_config.game_speed == GAME_SPEED_SLOW) blit(worminator_data_file[WORMY_OPTIONS_MENU_GAME_SPEED_DROPDOWN].dat, temp_buffer, 40, 14, 132, 53, 17, 7);
			else if (wormy_config.game_speed == GAME_SPEED_NORMAL) blit(worminator_data_file[WORMY_OPTIONS_MENU_GAME_SPEED_DROPDOWN].dat, temp_buffer, 34, 24, 126, 53, 29, 7);
			else if (wormy_config.game_speed == GAME_SPEED_FAST) blit(worminator_data_file[WORMY_OPTIONS_MENU_GAME_SPEED_DROPDOWN].dat, temp_buffer, 40, 34, 132, 53, 18, 7);
			else if (wormy_config.game_speed == GAME_SPEED_VERY_FAST) blit(worminator_data_file[WORMY_OPTIONS_MENU_GAME_SPEED_DROPDOWN].dat, temp_buffer, 28, 44, 119, 52, 43, 9);

			// Display the difficulty
			if (wormy_config.difficulty == DIFFICULTY_WIMP) blit(worminator_data_file[WORMY_OPTIONS_MENU_DIFFICULTY_DROPDOWN].dat, temp_buffer, 35, 2, 128, 68, 24, 9);
			else if (wormy_config.difficulty == DIFFICULTY_EASY) blit(worminator_data_file[WORMY_OPTIONS_MENU_DIFFICULTY_DROPDOWN].dat, temp_buffer, 37, 12, 130, 68, 21, 9);
			else if (wormy_config.difficulty == DIFFICULTY_NORMAL) blit(worminator_data_file[WORMY_OPTIONS_MENU_DIFFICULTY_DROPDOWN].dat, temp_buffer, 32, 24, 126, 69, 29, 7);
			else if (wormy_config.difficulty == DIFFICULTY_HARD) blit(worminator_data_file[WORMY_OPTIONS_MENU_DIFFICULTY_DROPDOWN].dat, temp_buffer, 37, 35, 131, 69, 19, 7);
			else if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE) blit(worminator_data_file[WORMY_OPTIONS_MENU_DIFFICULTY_DROPDOWN].dat, temp_buffer, 26, 45, 120, 68, 41, 9);

			// Blit the appropriate patches
			if (wormy_config.particle_detail == 0) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 0, 0, 162, 89, 8, 8);
			else if (wormy_config.particle_detail == 1) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 0, 0, 180, 89, 8, 8);
#ifdef ALLEGRO_WINDOWS
			if (wormy_config.safe_mode == 0) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 0, 0, 162, 105, 8, 8);
			else if (wormy_config.safe_mode == 1) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 0, 0, 180, 105, 8, 8);
#endif
			if (wormy_config.stretch_levels == 0) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 0, 0, 162, 120, 8, 8);
			else if (wormy_config.stretch_levels == 1) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 0, 0, 180, 120, 8, 8);
			if (wormy_config.remember_skin == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 144, 141, 8, 8);
			if (wormy_config.may_tricks_mode == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 144, 155, 8, 8);
			if (wormy_config.autoswitch_weapons == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 144, 169, 8, 8);
			if (wormy_config.switch_weapons_on_pickup == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 144, 183, 8, 8);
			if (wormy_config.disable_sound == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 299, 67, 8, 8);
			if (wormy_config.disable_music == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 299, 81, 8, 8);
			if (wormy_config.skip_intro == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 299, 104, 8, 8);
			if (wormy_config.display_particles == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 299, 117, 8, 8);
			if (wormy_config.show_laser_sight == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 299, 131, 8, 8);
			if (wormy_config.run_windowed == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 299, 145, 8, 8);
			if (wormy_config.enable_FSAA == TRUE) blit(worminator_data_file[WORMY_OPTIONS_MENU_GLOW_PATCH].dat, temp_buffer, 8, 0, 299, 159, 8, 8);

			// Display the sound and music volume
			sprintf(message, "%-3d", wormy_config.music_volume);
			textout(temp_buffer, worminator_data_file[SMALL_NUMBERS_FONT].dat, message, 280, 30, -1);
			sprintf(message, "%-3d", wormy_config.sound_volume);
			textout(temp_buffer, worminator_data_file[SMALL_NUMBERS_FONT].dat, message, 280, 46, -1);

			// Draw on the final swap_buffer, and display mouse
			stretch_blit(temp_buffer, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			savedisplay();
			show_mouse(screen);
			blit_to_screen(swap_buffer);

			// Make sure that we don't endlessly redraw.
			redraw_options_menu = FALSE;
		}

		// Resolution dropdown
		if (region_clicked(183, 37, 189, 43) == TRUE) {
			// Drop down the menu
			show_mouse(NULL);
			blit(worminator_data_file[WORMY_OPTIONS_MENU_RESOLUTION_DROPDOWN].dat, temp_buffer, 0, 0, 99, 44, 92, 103);
			stretch_blit(temp_buffer, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			savedisplay();
			show_mouse(screen);
			blit_to_screen(swap_buffer);

			// Begin the resolution dropdown loop
			do {
				idle_speed_counter = 0;
				if (region_clicked(111, 47, 179, 53) == TRUE) {
					new_sw = 320, new_sh = 200, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(111, 56, 179, 62) == TRUE) {
					new_sw = 320, new_sh = 240, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(111, 65, 179, 71) == TRUE) {
					new_sw = 400, new_sh = 300, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(111, 74, 179, 80) == TRUE) {
					new_sw = 512, new_sh = 384, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(111, 83, 179, 89) == TRUE) {
					new_sw = 640, new_sh = 480, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(111, 92, 179, 98) == TRUE) {
					new_sw = 800, new_sh = 600, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(108, 101, 181, 107) == TRUE) {
					new_sw = 1024, new_sh = 768, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(108, 110, 181, 116) == TRUE) {
					new_sw = 1152, new_sh = 864, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(108, 119, 181, 125) == TRUE) {
					new_sw = 1280, new_sh = 960, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(108, 128, 181, 134) == TRUE) {
					new_sw = 1280, new_sh = 1024, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (region_clicked(108, 137, 181, 143) == TRUE) {
					new_sw = 1600, new_sh = 1200, must_change_resolution = TRUE, close_options_menu = TRUE;
				} else if (mouse_b & 1 && mouse_in_region(99, 44, 190, 146) == FALSE) {
					while (mouse_b & 1);
					close_options_menu = TRUE;
				}
				if (idle_speed_counter == 0) rest(1);
			} while (close_options_menu == FALSE);

			// Reset this fudged variable, and make sure display is redrawn
			close_options_menu = FALSE;
			redraw_options_menu = TRUE;
		}

		// Game speed dropdown
		if (region_clicked(183, 53, 189, 59) == TRUE) {
			// Drop down the menu
			show_mouse(NULL);
			blit(worminator_data_file[WORMY_OPTIONS_MENU_GAME_SPEED_DROPDOWN].dat, temp_buffer, 0, 0, 99, 60, 92, 56);
			stretch_blit(temp_buffer, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			savedisplay();
			show_mouse(screen);
			blit_to_screen(swap_buffer);

			// Begin the resolution dropdown loop
			do {
				idle_speed_counter = 0;
				if (region_clicked(126, 63, 167, 71) == TRUE) {
					wormy_config.game_speed = GAME_SPEED_VERY_SLOW, close_options_menu = TRUE;
				} else if (region_clicked(139, 74, 155, 80) == TRUE) {
					wormy_config.game_speed = GAME_SPEED_SLOW, close_options_menu = TRUE;
				} else if (region_clicked(133, 84, 161, 90) == TRUE) {
					wormy_config.game_speed = GAME_SPEED_NORMAL, close_options_menu = TRUE;
				} else if (region_clicked(139, 94, 156, 100) == TRUE) {
					wormy_config.game_speed = GAME_SPEED_FAST, close_options_menu = TRUE;
				} else if (region_clicked(127, 104, 169, 112) == TRUE) {
					wormy_config.game_speed = GAME_SPEED_VERY_FAST, close_options_menu = TRUE;
				} else if (mouse_b & 1 && mouse_in_region(99, 60, 190, 115) == FALSE) {
					while (mouse_b & 1);
					close_options_menu = TRUE;
				}
				if (idle_speed_counter == 0) rest(1);
			} while (close_options_menu == FALSE);

			// Reset this fudged variable, and make sure display is redrawn
			close_options_menu = FALSE;
			redraw_options_menu = TRUE;
		}

		// Difficulty dropdown
		if (region_clicked(183, 69, 189, 75) == TRUE) {
			// Drop down the menu
			show_mouse(NULL);
			blit(worminator_data_file[WORMY_OPTIONS_MENU_DIFFICULTY_DROPDOWN].dat, temp_buffer, 0, 0, 99, 76, 92, 56);
			stretch_blit(temp_buffer, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			savedisplay();
			show_mouse(screen);
			blit_to_screen(swap_buffer);

			// Begin the resolution dropdown loop
			do {
				idle_speed_counter = 0;
				if (region_clicked(134, 78, 157, 86) == TRUE) {
					wormy_config.difficulty = DIFFICULTY_WIMP, close_options_menu = TRUE;
				} else if (region_clicked(136, 88, 156, 96) == TRUE) {
					wormy_config.difficulty = DIFFICULTY_EASY, close_options_menu = TRUE;
				} else if (region_clicked(131, 100, 159, 106) == TRUE) {
					wormy_config.difficulty = DIFFICULTY_NORMAL, close_options_menu = TRUE;
				} else if (region_clicked(136, 111, 154, 117) == TRUE) {
					wormy_config.difficulty = DIFFICULTY_HARD, close_options_menu = TRUE;
				} else if (region_clicked(125, 121, 165, 129) == TRUE) {
					wormy_config.difficulty = DIFFICULTY_NIGHTMARE, close_options_menu = TRUE;
				} else if (mouse_b & 1 && mouse_in_region(99, 76, 190, 131) == FALSE) {
					while (mouse_b & 1);
					close_options_menu = TRUE;
				}
				if (idle_speed_counter == 0) rest(1);
			} while (close_options_menu == FALSE);

			// Reset this fudged variable, and make sure display is redrawn
			close_options_menu = FALSE;
			redraw_options_menu = TRUE;
		}

		// Particle detail checkholes
		if (region_clicked(162, 89, 169, 96) == TRUE) {
			wormy_config.particle_detail = 0;
			redraw_options_menu = TRUE;
		} else if (region_clicked(180, 89, 187, 96) == TRUE) {
			wormy_config.particle_detail = 1;
			redraw_options_menu = TRUE;
		}

#ifdef ALLEGRO_WINDOWS
		// Safe mode checkholes
		if (region_clicked(162, 105, 169, 112) == TRUE) {
			wormy_config.safe_mode = 0;
			must_change_resolution = TRUE;
			redraw_options_menu = TRUE;
		} else if (region_clicked(180, 105, 187, 112) == TRUE) {
			wormy_config.safe_mode = 1;
			must_change_resolution = TRUE;
			redraw_options_menu = TRUE;
		}
#endif

		// Stretch levels checkholes
		if (region_clicked(162, 120, 169, 127) == TRUE) {
			wormy_config.stretch_levels = 0;
			redraw_options_menu = TRUE;
		} else if (region_clicked(180, 120, 187, 127) == TRUE) {
			wormy_config.stretch_levels = 1;
			redraw_options_menu = TRUE;
		}

		// Remember skin checkbox
		if (region_clicked(144, 141, 151, 148) == TRUE) {
			if (wormy_config.remember_skin == TRUE) wormy_config.remember_skin = FALSE;
			else wormy_config.remember_skin = TRUE;
			redraw_options_menu = TRUE;
		}

		// May tricks mode checkbox
		if (region_clicked(144, 155, 151, 162) == TRUE) {
			if (wormy_config.may_tricks_mode == TRUE) wormy_config.may_tricks_mode = FALSE;
			else wormy_config.may_tricks_mode = TRUE;
			redraw_options_menu = TRUE;
		}

		// Autoswitch weapons checkbox
		if (region_clicked(144, 169, 151, 176) == TRUE) {
			if (wormy_config.autoswitch_weapons == TRUE) wormy_config.autoswitch_weapons = FALSE;
			else wormy_config.autoswitch_weapons = TRUE;
			redraw_options_menu = TRUE;
		}

		// Switch weapons on pickup checkbox
		if (region_clicked(144, 183, 151, 190) == TRUE) {
			if (wormy_config.switch_weapons_on_pickup == TRUE) wormy_config.switch_weapons_on_pickup = FALSE;
			else wormy_config.switch_weapons_on_pickup = TRUE;
			redraw_options_menu = TRUE;
		}

		// Volume controls
		if (region_clicked(305, 27, 311, 33) == TRUE) {
			if (wormy_config.music_volume < 250) wormy_config.music_volume += 5;
			else wormy_config.music_volume = 255;
			must_change_sound = TRUE;
			redraw_options_menu = TRUE;
		}
		if (region_clicked(305, 35, 311, 41) == TRUE) {
			if (wormy_config.music_volume >= 5) wormy_config.music_volume -= 5;
			else wormy_config.music_volume = 0;
			must_change_sound = TRUE;
			redraw_options_menu = TRUE;
		}
		if (region_clicked(305, 43, 311, 49) == TRUE) {
			if (wormy_config.sound_volume <= 250) wormy_config.sound_volume += 5;
			else wormy_config.sound_volume = 255;
			must_change_sound = TRUE;
			redraw_options_menu = TRUE;
		}
		if (region_clicked(305, 51, 311, 57) == TRUE) {
			if (wormy_config.sound_volume >= 5) wormy_config.sound_volume -= 5;
			else wormy_config.sound_volume = 0;
			must_change_sound = TRUE;
			redraw_options_menu = TRUE;
		}

		// Disable sound checkbox
		if (region_clicked(299, 67, 306, 75) == TRUE) {
			if (wormy_config.disable_sound == TRUE) wormy_config.disable_sound = FALSE;
			else wormy_config.disable_sound = TRUE;
			must_change_sound = TRUE;
			redraw_options_menu = TRUE;
		}

		// Disable music checkbox
		if (region_clicked(299, 81, 306, 88) == TRUE) {
			if (wormy_config.disable_music == TRUE) wormy_config.disable_music = FALSE;
			else wormy_config.disable_music = TRUE;
			must_change_sound = TRUE;
			redraw_options_menu = TRUE;
		}

		// Skip introduction checkbox
		if (region_clicked(299, 104, 306, 111) == TRUE) {
			if (wormy_config.skip_intro == TRUE) wormy_config.skip_intro = FALSE;
			else wormy_config.skip_intro = TRUE;
			redraw_options_menu = TRUE;
		}

		// Display particles checkbox
		if (region_clicked(299, 117, 306, 124) == TRUE) {
			if (wormy_config.display_particles == TRUE) wormy_config.display_particles = FALSE;
			else wormy_config.display_particles = TRUE;
			redraw_options_menu = TRUE;
		}

		// Show laser sight checkbox
		if (region_clicked(299, 131, 306, 138) == TRUE) {
			if (wormy_config.show_laser_sight == TRUE) wormy_config.show_laser_sight = FALSE;
			else wormy_config.show_laser_sight = TRUE;
			redraw_options_menu = TRUE;
		}

		// Run windowed checkbox
		if (region_clicked(299, 145, 306, 152) == TRUE) {
			if (wormy_config.run_windowed == TRUE) wormy_config.run_windowed = FALSE;
			else wormy_config.run_windowed = TRUE;
			must_change_resolution = TRUE;
			redraw_options_menu = TRUE;
		}

		// Enable FSAA checkbox
		if (region_clicked(299, 159, 306, 166) == TRUE) {
			if (wormy_config.enable_FSAA == TRUE) wormy_config.enable_FSAA = FALSE;
			else wormy_config.enable_FSAA = TRUE;
			redraw_options_menu = TRUE;
		}

		// Controls menu button
		if (mouse_clicked_region(190, 174, 259, 190) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(190, 174, 259, 190) == TRUE) wormy_controls_menu();
		}

		// Ok button
		if (mouse_clicked_region(271, 174, 304, 190) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(271, 174, 304, 190) == TRUE) close_options_menu = TRUE;
		}

		// Optional enter keypress
		if (key[KEY_ENTER]) {
			while (key[KEY_ENTER]);
			close_options_menu = TRUE;
		}

		if (idle_speed_counter == 0) rest(1);
	} while (close_options_menu == FALSE);

	// Restore the previous mouse pointer display
	show_mouse(NULL);

	// Change the resolution if nessecary, and update screen width and height
	screen_width = new_sw, screen_height = new_sh;
	if (must_change_resolution == TRUE) {
		// Don't do this with the callback set this causes crashes on unix
		change_resolution(screen_width, screen_height);
	}

	// Change the sound if nessecary
	if (must_change_sound == TRUE) reset_sound();

	// Change the game speed if nessecary
	change_game_speed(wormy_config.game_speed);

	//show_mouse(screen);
	destroy_bitmap(temp_buffer);
}

/***************************\
|** START NEW CUSTOM GAME **|
\***************************/
char start_new_custom_game()
{
	//PSV_DEBUG("start_new_custom_game()");

	char retval = FALSE;
	char file_name[1024] = "";
	BITMAP *backup_bitmap = create_bitmap_ex(8, swap_buffer->w, swap_buffer->h);

	// Back up existing screen
	show_mouse(NULL);
	blit(swap_buffer, backup_bitmap, 0, 0, 0, 0, swap_buffer->w, swap_buffer->h);

	if (file_select_ex("Select a map data file (.map files)", file_name, "map", sizeof(file_name), 300, 300) != 0)
		retval = load_map(-1, file_name, 1);

	// Restore the previous screen and mouse pointer display
	show_mouse(NULL);
	draw_sprite(swap_buffer, backup_bitmap, 0, 0);
	destroy_bitmap(backup_bitmap);
	return retval;
}

/*****************\
|** PLAY A DEMO **|
\*****************/
/* retval:
 * 0 done nothing, game state unchanged
 * 1 partially loaded, but couldn't play game state has been changed
 * 2 successfully played, game state has been changed
 */
char play_a_demo()
{
	char retval = 0;
	char file_name[256] = "";
	BITMAP *backup_bitmap = create_bitmap_ex(8, 320, 200);

	// Back up existing screen
	show_mouse(NULL);
	blit(swap_buffer, backup_bitmap, 0, 0, 0, 0, 320, 200);

	if (file_select_ex("Select a demo to play (.dem files)", file_name, "dem", sizeof(file_name), 0, 0) != 0)
		retval = play_demo(file_name);

	// Restore the previous screen and mouse pointer display
	show_mouse(NULL);
	draw_sprite(swap_buffer, backup_bitmap, 0, 0);
	destroy_bitmap(backup_bitmap);
	return retval;
}

/*******************\
|** RECORD A DEMO **|
\*******************/
void record_a_demo()
{
	char file_name[256] = "";
	BITMAP *backup_bitmap = create_bitmap_ex(8, 320, 200);

	// Back up existing screen
	show_mouse(NULL);
	blit(swap_buffer, backup_bitmap, 0, 0, 0, 0, 320, 200);

	if (file_select_ex("Save demo as (.dem files)", file_name, "dem", sizeof(file_name), 0, 0) != 0)
		start_demo_recording(file_name);

	// Restore the previous screen and mouse pointer display
	show_mouse(NULL);
	draw_sprite(swap_buffer, backup_bitmap, 0, 0);
	destroy_bitmap(backup_bitmap);
}

/*********************\
|** WORMY DUMMY BOX **|
\*********************/
char wormy_dummy_box(char dummy_question_patch_number, char show_mouse_pointer)
{
	BITMAP *backup_bitmap = create_bitmap_ex(8, 128, 64);
	char close_dummy_box = FALSE;

	// Pause the game while the dummy box is open
	game_is_running = FALSE;

	// Back up existing area under the dummy box
	blit(swap_buffer, backup_bitmap, (screen_width / 2) - 64, (screen_height / 2) - 32, 0, 0, 128, 64);

	// Draw in the wormy dummy box along with its patch, and then show the mouse pointer
	show_mouse(NULL);
	draw_sprite(swap_buffer, worminator_data_file[WORMY_DUMMY_BOX].dat, (screen_width / 2) - 64, (screen_height / 2) - 32);
	if (dummy_question_patch_number == 0) draw_sprite(swap_buffer, worminator_data_file[WORMY_DUMMY_BOX_EXIT_GAME_PATCH].dat, (screen_width / 2) - 48, (screen_height / 2) - 16);
	else if (dummy_question_patch_number == 1) draw_sprite(swap_buffer, worminator_data_file[WORMY_DUMMY_BOX_LOAD_GAME_PATCH].dat, (screen_width / 2) - 48, (screen_height / 2) - 16);
	else if (dummy_question_patch_number == 2) draw_sprite(swap_buffer, worminator_data_file[WORMY_DUMMY_BOX_NEW_GAME_PATCH].dat, (screen_width / 2) - 48, (screen_height / 2) - 16);
	else if (dummy_question_patch_number == 3) draw_sprite(swap_buffer, worminator_data_file[WORMY_DUMMY_BOX_SAVE_GAME_PATCH].dat, (screen_width / 2) - 48, (screen_height / 2) - 16);
	savedisplay();
	show_mouse(screen);
	blit_to_screen(swap_buffer);

	// This is the core dummy box loop
	do {
		idle_speed_counter = 0;
		// Yes button
		if (mouse_b & 1 && mouse_x >= (screen_width / 2) - 47 && mouse_x <= (screen_width / 2) - 14 && mouse_y >= (screen_height / 2) + 12 && mouse_y <= (screen_height / 2) + 28) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width / 2) - 47 && mouse_x <= (screen_width / 2) - 14 && mouse_y >= (screen_height / 2) + 12 && mouse_y <= (screen_height / 2) + 28) {
				show_mouse(NULL);
				draw_sprite(swap_buffer, backup_bitmap, (screen_width / 2) - 64, (screen_height / 2) - 32);
				if (show_mouse_pointer == TRUE) show_mouse(screen);
				destroy_bitmap(backup_bitmap);
				return TRUE;
			}
		}

		// Optional Y keypress
		if (key[KEY_Y]) {
			while (key[KEY_Y]);
			show_mouse(NULL);
			draw_sprite(swap_buffer, backup_bitmap, (screen_width / 2) - 64, (screen_height / 2) - 32);
			if (show_mouse_pointer == TRUE) show_mouse(screen);
			destroy_bitmap(backup_bitmap);
			return TRUE;
		}
		// No button
		else if (mouse_b & 1 && mouse_x >= (screen_width / 2) + 12 && mouse_x <= (screen_width / 2) + 45 && mouse_y >= (screen_height / 2) + 11 && mouse_y <= (screen_height / 2) + 27) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width / 2) + 12 && mouse_x <= (screen_width / 2) + 45 && mouse_y >= (screen_height / 2) + 11 && mouse_y <= (screen_height / 2) + 27) {
				show_mouse(NULL);
				draw_sprite(swap_buffer, backup_bitmap, (screen_width / 2) - 64, (screen_height / 2) - 32);
				if (show_mouse_pointer == TRUE) show_mouse(screen);
				destroy_bitmap(backup_bitmap);
				return FALSE;
			}
		}

		// Optional N keypress
		if (key[KEY_N]) {
			while (key[KEY_N]);
			show_mouse(NULL);
			draw_sprite(swap_buffer, backup_bitmap, (screen_width / 2) - 64, (screen_height / 2) - 32);
			if (show_mouse_pointer == TRUE) show_mouse(screen);
			destroy_bitmap(backup_bitmap);
			return FALSE;
		}
		blit_to_screen(swap_buffer);
		if (idle_speed_counter == 0) rest(1);
	} while (close_dummy_box == FALSE);

	// This should never be reached, but return FALSE just as a precaution
	return FALSE;
}

/**********************\
|** DISPLAY SNAPSHOT **|
\**********************/
void display_snapshot(char snapshot_number, char show_mouse_pointer)
{
	BITMAP *backup_bitmap = create_bitmap_ex(8, screen_width, screen_height);
	BITMAP *buffer_bitmap = create_bitmap_ex(8, 320, 200);
	int i;
	char close_snapshot = FALSE;
	char buf[256];

	// Pause the game while looking at a snapshot
	game_is_running = FALSE;

	// Back up existing screen
	show_mouse(NULL);
	blit(swap_buffer, backup_bitmap, 0, 0, 0, 0, screen_width, screen_height);

	// Draw the snapshot
	if (snapshot_number == 0) {
		stretch_blit(worminator_data_file[WORMY_GENERAL_HELP].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 1) {
		stretch_blit(worminator_data_file[WORMY_ENEMIES_HELP].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 2) {
		stretch_blit(worminator_data_file[WORMY_WEAPONS_HELP].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 3) {
		stretch_blit(worminator_data_file[WORMY_CREDITS_HELP].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 4) {
		stretch_blit(worminator_data_file[WORMY_CONTROLS_HELP].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 5) {
		stretch_blit(worminator_data_file[WORMY_ABOUT_HELP].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 6) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_01_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 7) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_02_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 8) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_03_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 9) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_04_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 10) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_05_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 11) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_06_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 12) {
		stretch_blit(worminator_data_file[WORMY_ENDING_MESSAGE].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 13) {
		stretch_blit(worminator_data_file[WORMY_FAMOUS_LAST_WORDS].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 14) {
		blit(worminator_data_file[WORMY_HIGH_SCORES].dat, buffer_bitmap, 0, 0, 0, 0, 320, 200);
		for (i = 0; i < 10; i++) {
			sprintf(buf, "%s", &wormy_config.high_score_names[i][0]);
			textout(buffer_bitmap, font, buf, 45, 40 + 12 * i, makecol(0, 0, 0));
			sprintf(buf, "- %i", wormy_config.high_scores[i]);
			textout(buffer_bitmap, font, buf, 45 + 134, 40 + 12 * i, makecol(0, 0, 0));
		}
		stretch_blit(buffer_bitmap, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 15) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_07_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 16) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_08_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 17) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_09_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 18) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_10_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 19) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_11_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 20) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_12_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 21) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_13_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 22) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_14_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 23) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_15_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 24) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_16_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 25) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_17_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 26) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_18_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 27) {
		stretch_blit(worminator_data_file[WORMY_LEVEL_19_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 28) {
		stretch_blit(worminator_data_file[WORMY_CRAPATANA_BRIEFING].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	} else if (snapshot_number == 29) {
		stretch_blit(worminator_data_file[WORMY_GOODBYE_MESSAGE].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	}
	savedisplay();
	blit_to_screen(swap_buffer);

#if !defined (DINGUX) && !defined (PSVITA)
	show_mouse(screen);
#endif

	// This is the core snapshot loop
	do {
		idle_speed_counter = 0;
		// Ok button
		if (mouse_b & 1 && mouse_x >= (screen_width * .846875) && mouse_x <= (screen_width * .95) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width * .846875) && mouse_x <= (screen_width * .95) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) close_snapshot = TRUE;
		}

		// Optional enter keypress
		if (key[KEY_ENTER]) {
			while (key[KEY_ENTER]);
			close_snapshot = TRUE;
		}

		// Optional lctrl keypress
		if (key[KEY_SPACE]) {
			while (key[KEY_SPACE]);
			close_snapshot = TRUE;
		}

		if (idle_speed_counter == 0) rest(1);
	} while (close_snapshot == FALSE);

	// Restore the previous screen and mouse pointer display
	show_mouse(NULL);
	blit(backup_bitmap, swap_buffer, 0, 0, 0, 0, screen_width, screen_height);
#if !defined (DINGUX) && !defined (PSVITA)
	if (show_mouse_pointer == TRUE) show_mouse(screen);
#endif
	destroy_bitmap(backup_bitmap);
	destroy_bitmap(buffer_bitmap);
}

/*******************\
|** DISPLAY STORY **|
\*******************/
void display_story(char show_mouse_pointer)
{
	BITMAP *backup_bitmap = create_bitmap_ex(8, screen_width, screen_height);
	BITMAP *temp_buffer = create_bitmap_ex(8, screen_width, screen_height);
	char close_story = FALSE;
	char story_page_number = 0;

	// Pause the game while looking at the story
	game_is_running = FALSE;

	// Back up existing screen
	show_mouse(NULL);
	blit(swap_buffer, backup_bitmap, 0, 0, 0, 0, screen_width, screen_height);

	// Draw the current story screen
	stretch_blit(worminator_data_file[WORMY_STORY_PAGE_01].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	savedisplay();
#if !defined (DINGUX) && !defined (PSVITA)
	show_mouse(screen);
#endif
	blit_to_screen(swap_buffer);

	// This is the core story loop
	do {
		idle_speed_counter = 0;
		// Left Arrow
		if (story_page_number > 0 && mouse_b & 1 && mouse_x >= (screen_width * .54375) && mouse_x <= (screen_width * .646875) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width * .54375) && mouse_x <= (screen_width * .646875) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
				play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
				story_page_number--;
				if (story_page_number < 0) story_page_number = 0;
				show_mouse(NULL);
				stretch_blit(worminator_data_file[WORMY_STORY_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
				do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
				savedisplay();
				#if !defined (DINGUX) && !defined (PSVITA)
				show_mouse(screen);
				#endif
			}
		}

		// Optional left arrow
		if (story_page_number > 0 && key[KEY_LEFT]) {
			while (key[KEY_LEFT]);
			play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
			story_page_number--;
			if (story_page_number < 0) story_page_number = 0;
			show_mouse(NULL);
			stretch_blit(worminator_data_file[WORMY_STORY_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
			savedisplay();
			#if !defined (DINGUX) && !defined (PSVITA)
			show_mouse(screen);
			#endif
		}

		// Right Arrow
		if (story_page_number < 3 && mouse_b & 1 && mouse_x >= (screen_width * .671875) && mouse_x <= (screen_width * .775) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width * .671875) && mouse_x <= (screen_width * .775) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
				play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
				story_page_number++;
				if (story_page_number > 3) story_page_number = 3;
				show_mouse(NULL);
				stretch_blit(worminator_data_file[WORMY_STORY_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
				do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
				savedisplay();
				#if !defined (DINGUX) && !defined (PSVITA)
				show_mouse(screen);
				#endif
			}
		}

		// Optional right arrow
		if (story_page_number < 3 && key[KEY_RIGHT]) {
			while (key[KEY_RIGHT]);
			play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
			story_page_number++;
			if (story_page_number > 3) story_page_number = 3;
			show_mouse(NULL);
			stretch_blit(worminator_data_file[WORMY_STORY_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
			savedisplay();
			#if !defined (DINGUX) && !defined (PSVITA)
			show_mouse(screen);
			#endif
			play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
		}

		// Ok button
		if (mouse_b & 1 && mouse_x >= (screen_width * .846875) && mouse_x <= (screen_width * .95) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width * .846875) && mouse_x <= (screen_width * .95) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) close_story = TRUE;
		}

		// Optional enter keypress
		if (key[KEY_ENTER]) {
			while (key[KEY_ENTER]);
			close_story = TRUE;
		}

		// Optional lctrl keypress
		if (key[KEY_SPACE]) {
			while (key[KEY_SPACE]);
			close_story = TRUE;
		}

		if (idle_speed_counter == 0) rest(1);
		blit_to_screen(swap_buffer);
	} while (close_story == FALSE);

	// Restore the previous screen and moust pointer display
	show_mouse(NULL);
	blit(backup_bitmap, swap_buffer, 0, 0, 0, 0, screen_width, screen_height);
	savedisplay();
#if !defined (DINGUX) && !defined (PSVITA)
	if (show_mouse_pointer == TRUE) show_mouse(screen);
#endif
	destroy_bitmap(backup_bitmap);
	destroy_bitmap(temp_buffer);
}

/*******************\
|** DISPLAY RECAP **|
\*******************/
void display_recap(char show_mouse_pointer)
{
	BITMAP *backup_bitmap = create_bitmap_ex(8, screen_width, screen_height);
	BITMAP *temp_buffer = create_bitmap_ex(8, screen_width, screen_height);
	char close_story = FALSE;
	char story_page_number = 0;

	// Pause the game while looking at the story
	game_is_running = FALSE;

	// Back up existing screen
	show_mouse(NULL);
	blit(swap_buffer, backup_bitmap, 0, 0, 0, 0, screen_width, screen_height);

	// Draw the current story screen
	stretch_blit(worminator_data_file[WORMY_STORY_RECAP_PAGE_01].dat, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
	savedisplay();
#if !defined (DINGUX) && !defined (PSVITA)
	show_mouse(screen);
#endif
	blit_to_screen(swap_buffer);

	// This is the core story loop
	do {
		idle_speed_counter = 0;
		// Left Arrow
		if (story_page_number > 0 && mouse_b & 1 && mouse_x >= (screen_width * .54375) && mouse_x <= (screen_width * .646875) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width * .54375) && mouse_x <= (screen_width * .646875) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
				play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
				story_page_number--;
				if (story_page_number < 0) story_page_number = 0;
				show_mouse(NULL);
				stretch_blit(worminator_data_file[WORMY_STORY_RECAP_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
				do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
				savedisplay();
				#if !defined (DINGUX) && !defined (PSVITA)
				show_mouse(screen);
				#endif
			}
		}

		// Optional left arrow
		if (story_page_number > 0 && key[KEY_LEFT]) {
			while (key[KEY_LEFT]);
			play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
			story_page_number--;
			if (story_page_number < 0) story_page_number = 0;
			show_mouse(NULL);
			stretch_blit(worminator_data_file[WORMY_STORY_RECAP_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
			savedisplay();
			#if !defined (DINGUX) && !defined (PSVITA)
			show_mouse(screen);
			#endif
		}

		// Right Arrow
		if (story_page_number < 1 && mouse_b & 1 && mouse_x >= (screen_width * .671875) && mouse_x <= (screen_width * .775) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width * .671875) && mouse_x <= (screen_width * .775) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
				play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
				story_page_number++;
				if (story_page_number > 1) story_page_number = 1;
				show_mouse(NULL);
				stretch_blit(worminator_data_file[WORMY_STORY_RECAP_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
				do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
				savedisplay();
				#if !defined (DINGUX) && !defined (PSVITA)
				show_mouse(screen);
				#endif
			}
		}

		// Optional right arrow
		if (story_page_number < 1 && key[KEY_RIGHT]) {
			while (key[KEY_RIGHT]);
			play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
			story_page_number++;
			if (story_page_number > 1) story_page_number = 1;
			show_mouse(NULL);
			stretch_blit(worminator_data_file[WORMY_STORY_RECAP_PAGE_01 + story_page_number].dat, temp_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			do_radial_wipe(swap_buffer, temp_buffer, 0, 0, 1, 5, 0);
			savedisplay();
			#if !defined (DINGUX) && !defined (PSVITA)
			show_mouse(screen);
			#endif
			play_sample(worminator_data_file[PAGE_TURN_SOUND].dat, 255, 128, 1000, FALSE);
		}

		// Ok button
		if (mouse_b & 1 && mouse_x >= (screen_width * .846875) && mouse_x <= (screen_width * .95) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) {
			while (mouse_b & 1);
			if (mouse_x >= (screen_width * .846875) && mouse_x <= (screen_width * .95) && mouse_y >= (screen_height * .87) && mouse_y <= (screen_height * .95)) close_story = TRUE;
		}

		// Optional enter keypress
		if (key[KEY_ENTER]) {
			while (key[KEY_ENTER]);
			close_story = TRUE;
		}

		// Optional lctrl keypress
		if (key[KEY_SPACE]) {
			while (key[KEY_SPACE]);
			close_story = TRUE;
		}

		if (idle_speed_counter == 0) rest(1);
		blit_to_screen(swap_buffer);
	} while (close_story == FALSE);

	// Restore the previous screen and moust pointer display
	show_mouse(NULL);
	blit(backup_bitmap, swap_buffer, 0, 0, 0, 0, screen_width, screen_height);
	savedisplay();
#if !defined (DINGUX) && !defined (PSVITA)
	if (show_mouse_pointer == TRUE) show_mouse(screen);
#endif
	destroy_bitmap(backup_bitmap);
	destroy_bitmap(temp_buffer);
}

/**********************\
|** CHECK HIGH SCORE **|
\**********************/
void check_high_score()
{
	int script_loop, script_loop_2, buf;

	// Test the input dialog after selecting quit from main menu.
	//player.score = 1010;

	if (!playing_demo && !recording_demo) {
		for (script_loop = 0; script_loop < 10; script_loop++) {
			if (player.score >= wormy_config.high_scores[script_loop]) {
				buf = script_loop;
				if (buf != 9) for (script_loop = 9; script_loop > buf; script_loop--) {
						wormy_config.high_scores[script_loop] = wormy_config.high_scores[script_loop - 1];
						for (script_loop_2 = 0; script_loop_2 < 16; script_loop_2++) wormy_config.high_score_names[script_loop][script_loop_2] = wormy_config.high_score_names[script_loop - 1][script_loop_2];
					}
				get_highscore_name(buf);
				display_snapshot(14, TRUE);
				break;
			}
		}
	}
}

/************************\
|** GET HIGHSCORE NAME **|
\************************/
void get_highscore_name(int position)
{
	BITMAP *backup = create_bitmap_ex(8, 128, 64);
	BITMAP *buffer = create_bitmap_ex(8, 128, 64);
	int pos = 0, keypress;
	char input[256];
	char done = FALSE;

#if !defined (DINGUX) && !defined (PSVITA)
	show_mouse(NULL);
#endif

	// Backup the and initialize the buffer
	blit(swap_buffer, backup, (screen_width / 2) - 64, (screen_height / 2) - 32, 0, 0, 128, 64);
	blit(worminator_data_file[WORMY_HIGH_SCORE_INPUT_BOX].dat, buffer, 0, 0, 0, 0, 128, 64);
	savedisplay();

	// Clear out the input array
	for (pos = 0; pos < 16; pos++) input[pos] = 0;
	pos = 0;

	// Draw the input box on screen
	draw_sprite(swap_buffer, buffer, (screen_width / 2) - 64, (screen_height / 2) - 32);
	blit_to_screen(swap_buffer);

	// Accept input
#ifdef PSVITA
	while (!done){
		// Show input dialog box after user presses x-button.
		if (key[KEY_SPACE]){
			
			// To prevent flickering we have to stop the main rendering timer.
			psv_remove_render_timer();

			psvInputDialog("Enter your name", "", input);
			
			// Restore main rendering timer.
			psv_install_render_timer();

			blit(worminator_data_file[WORMY_HIGH_SCORE_INPUT_BOX].dat, buffer, 0, 0, 0, 0, 128, 64);
			textout(buffer, font, input, 6, 49, 5);
			draw_sprite(swap_buffer, buffer, (screen_width / 2) - 64, (screen_height / 2) - 32);
			blit_to_screen(swap_buffer);
			savedisplay();
			done = TRUE;
		}

		rest(10);
	}
#else
	while (!done) {
		clear_keybuf();
		keypress = readkey();
		if (keypress >> 8 == KEY_ENTER || keypress >> 8 == KEY_ESC) {
			done = TRUE;
		} else if (keypress >> 8 == KEY_BACKSPACE) {
			pos--; if (pos == -1) pos = 0; input[pos] = 0;
		} else if (pos < 14) {
			if (keypress >> 8 >= KEY_A && keypress >> 8 <= KEY_Z) {
				if (key[KEY_LSHIFT] || key[KEY_RSHIFT]) {
					input[pos] = (keypress >> 8) - KEY_A + 65; pos++;
				} else {
					input[pos] = (keypress >> 8) - KEY_A + 97; pos++;
				}
			} else if (keypress >> 8 >= KEY_0 && keypress >> 8 <= KEY_9) {
				input[pos] = (keypress >> 8) - KEY_0 + 48; pos++;
			} else if (keypress >> 8 == KEY_SPACE) {
				input[pos] = 32; pos++;
			}
		}

		blit(worminator_data_file[WORMY_HIGH_SCORE_INPUT_BOX].dat, buffer, 0, 0, 0, 0, 128, 64);
		textout(buffer, font, input, 6, 49, 5);
		draw_sprite(swap_buffer, buffer, (screen_width / 2) - 64, (screen_height / 2) - 32);
		blit_to_screen(swap_buffer);
		savedisplay();
	}

#endif

	// Wait for enter to be released
	while (key[KEY_ENTER]);

	// Write the name and score into the high score table
	strcpy(&wormy_config.high_score_names[position][0], input);
	wormy_config.high_scores[position] = player.score;

	// Restore the display
	blit(backup, swap_buffer, 0, 0, (screen_width / 2) - 64, (screen_height / 2) - 32, 128, 64);

	// Clear out the bitmaps
	destroy_bitmap(backup);
	destroy_bitmap(buffer);
}

/********************\
|** DOFINALESCROLL **|
\********************/
void dofinalescroll()
{
	int position = 0;
	BITMAP *buf = create_bitmap_ex(8, 320, 200);
	BITMAP *screenbuf = create_bitmap_ex(8, screen_width, screen_height);

	change_game_speed(12);
	clear_keybuf();

	do {	// Start core game loop
		game_is_running = TRUE;
		while (speed_counter > 0 && time_to_quit == FALSE) {
			position++;
			speed_counter--;
		}
		stretch_blit(worminator_data_file[WORMY_CUTSCENE_06].dat, screenbuf, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
		clear_bitmap(buf);
		if (position < 200) blit(worminator_data_file[WORMY_SCROLLING_FINALE].dat, buf, 0, 0, 0, 200 - position, 320, 200);
		else blit(worminator_data_file[WORMY_SCROLLING_FINALE].dat, buf, 0, position - 200, 0, 0, 320, 200);
		masked_stretch_blit(buf, screenbuf, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
		blit(screenbuf, swap_buffer, 0, 0, 0, 0, screen_width, screen_height);
		blit_to_screen(swap_buffer);
		if (speed_counter == 0) rest(1);
	} while (position <= 1000 && !keypressed());	// End core game loop*/

	change_game_speed(wormy_config.game_speed);

	destroy_bitmap(buf);
	destroy_bitmap(screenbuf);
}

/**************\
|** DRAW KEY **|
\**************/
void draw_key(BITMAP *bmp, int x, int y, int key)
{
	if (key != -1 && keylocations[key] != -1) blit(worminator_data_file[KEYS].dat, bmp, 0, keylocations[key] * 12, x, y, 64, 12);
	else if (key == -1) blit(worminator_data_file[KEYS].dat, bmp, 0, 101 * 12, x, y, 64, 12);
}

/*****************\
|** GET NEW KEY **|
\*****************/
int get_new_key()
{
	BITMAP *backup = create_bitmap_ex(8, 128, 64);
	int keyprs = 0, i;
	char oldkeys[KEY_MAX];
	char done = FALSE;

	// Backup the screen
	show_mouse(NULL);
	blit(swap_buffer, backup, (screen_width / 2) - 64, (screen_height / 2) - 32, 0, 0, 128, 64);

	// Draw the press any key box on the screen
	draw_sprite(swap_buffer, worminator_data_file[WORMY_PRESS_ANY_KEY_BOX].dat, (screen_width / 2) - 64, (screen_height / 2) - 32);
	savedisplay();
#if !defined (DINGUX) && !defined (PSVITA)
	show_mouse(screen);
#endif
	blit_to_screen(swap_buffer);

	for (i = 0; i < KEY_MAX; i++) {
		if (key[i]) oldkeys[i] = TRUE;
		else oldkeys[i] = FALSE;
	}

	while (!done) {
		for (i = 0; i < KEY_MAX; i++) if (key[i] != oldkeys[i]) {
				while (key[i]);
				keyprs = i;
				done = TRUE;
			}
		rest(1);
	}

	if (keyprs == KEY_ESC) keyprs = -1;
	if (keyprs >= KEY_F1 && keyprs <= KEY_F12) keyprs = -1;

	// Restore the display
	show_mouse(NULL);
	blit(backup, swap_buffer, 0, 0, (screen_width / 2) - 64, (screen_height / 2) - 32, 128, 64);
	savedisplay();
#if !defined (DINGUX) && !defined (PSVITA)
	show_mouse(screen);
#endif
	blit_to_screen(swap_buffer);

	return keyprs;
}

/***************************\
|** WORMY CONTROLS MENU 1 **|
\***************************/
int wormy_controls_menu_1()
{
	BITMAP *temp_buffer = create_bitmap_ex(8, 320, 200);
	char close_controls_menu = FALSE;
	char redraw_controls_menu = TRUE;
	int return_value = 0;

	// Pause the game while looking at the options menu
	game_is_running = FALSE;

	// This is the core snapshot loop
	do {
		idle_speed_counter = 0;
		// Re-draw the options menu if nessecary
		if (redraw_controls_menu == TRUE) {
			show_mouse(NULL);
			blit(worminator_data_file[WORMY_CONTROLS_MENU_1].dat, temp_buffer, 0, 0, 0, 0, 320, 200);

			draw_key(temp_buffer, 145, 41, wormy_config.controls[INPUT_UP]);
			draw_key(temp_buffer, 145, 54, wormy_config.controls[INPUT_DOWN]);
			draw_key(temp_buffer, 145, 67, wormy_config.controls[INPUT_LEFT]);
			draw_key(temp_buffer, 145, 80, wormy_config.controls[INPUT_RIGHT]);

			draw_key(temp_buffer, 145, 100, wormy_config.controls[INPUT_JUMP]);
			draw_key(temp_buffer, 145, 113, wormy_config.controls[INPUT_FIRE]);
			draw_key(temp_buffer, 145, 126, wormy_config.controls[INPUT_DUCK]);
			draw_key(temp_buffer, 145, 139, wormy_config.controls[INPUT_LOOK]);
			draw_key(temp_buffer, 145, 152, wormy_config.controls[INPUT_BULLIT_TIME]);

			draw_key(temp_buffer, 238, 41, wormy_config.secondary_controls[INPUT_UP]);
			draw_key(temp_buffer, 238, 54, wormy_config.secondary_controls[INPUT_DOWN]);
			draw_key(temp_buffer, 238, 67, wormy_config.secondary_controls[INPUT_LEFT]);
			draw_key(temp_buffer, 238, 80, wormy_config.secondary_controls[INPUT_RIGHT]);

			draw_key(temp_buffer, 238, 100, wormy_config.secondary_controls[INPUT_JUMP]);
			draw_key(temp_buffer, 238, 113, wormy_config.secondary_controls[INPUT_FIRE]);
			draw_key(temp_buffer, 238, 126, wormy_config.secondary_controls[INPUT_DUCK]);
			draw_key(temp_buffer, 238, 139, wormy_config.secondary_controls[INPUT_LOOK]);
			draw_key(temp_buffer, 238, 152, wormy_config.secondary_controls[INPUT_BULLIT_TIME]);

			// Draw on the final swap_buffer, and display mouse
			stretch_blit(temp_buffer, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			savedisplay();
			show_mouse(screen);

			// Make sure that we don't endlessly redraw.
			redraw_controls_menu = FALSE;
		}

		// Primiary up key
		if (region_clicked(145, 41, 208, 52) == TRUE) {
			wormy_config.controls[INPUT_UP] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary down key
		if (region_clicked(145, 54, 208, 65) == TRUE) {
			wormy_config.controls[INPUT_DOWN] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary left key
		if (region_clicked(145, 67, 208, 78) == TRUE) {
			wormy_config.controls[INPUT_LEFT] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary right key
		if (region_clicked(145, 80, 208, 91) == TRUE) {
			wormy_config.controls[INPUT_RIGHT] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary jump key
		if (region_clicked(145, 100, 208, 111) == TRUE) {
			wormy_config.controls[INPUT_JUMP] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary fire key
		if (region_clicked(145, 113, 208, 124) == TRUE) {
			wormy_config.controls[INPUT_FIRE] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary duck key
		if (region_clicked(145, 126, 208, 137) == TRUE) {
			wormy_config.controls[INPUT_DUCK] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary look key
		if (region_clicked(145, 139, 208, 150) == TRUE) {
			wormy_config.controls[INPUT_LOOK] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary bullit time key
		if (region_clicked(145, 152, 208, 163) == TRUE) {
			wormy_config.controls[INPUT_BULLIT_TIME] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary up key
		if (region_clicked(238, 41, 301, 52) == TRUE) {
			wormy_config.secondary_controls[INPUT_UP] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary down key
		if (region_clicked(238, 54, 301, 65) == TRUE) {
			wormy_config.secondary_controls[INPUT_DOWN] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary left key
		if (region_clicked(238, 67, 301, 78) == TRUE) {
			wormy_config.secondary_controls[INPUT_LEFT] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary right key
		if (region_clicked(238, 80, 301, 91) == TRUE) {
			wormy_config.secondary_controls[INPUT_RIGHT] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary jump key
		if (region_clicked(238, 100, 301, 111) == TRUE) {
			wormy_config.secondary_controls[INPUT_JUMP] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary fire key
		if (region_clicked(238, 113, 301, 124) == TRUE) {
			wormy_config.secondary_controls[INPUT_FIRE] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary duck key
		if (region_clicked(238, 126, 301, 137) == TRUE) {
			wormy_config.secondary_controls[INPUT_DUCK] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary look key
		if (region_clicked(238, 139, 301, 150) == TRUE) {
			wormy_config.secondary_controls[INPUT_LOOK] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary bullit time key
		if (region_clicked(238, 152, 301, 163) == TRUE) {
			wormy_config.secondary_controls[INPUT_BULLIT_TIME] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Right arrow
		if (mouse_clicked_region(218, 178, 251, 194) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(218, 178, 251, 194) == TRUE) {
				close_controls_menu = TRUE; return_value = 1;
			}
		}

		// Ok button
		if (mouse_clicked_region(275, 178, 308, 194) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(275, 178, 308, 194) == TRUE) {
				close_controls_menu = TRUE; return_value = 0;
			}
		}

		// Optional enter keypress
		if (key[KEY_ENTER]) {
			while (key[KEY_ENTER]);
			close_controls_menu = TRUE;
		}

		if (idle_speed_counter == 0) rest(1);
		blit_to_screen(swap_buffer);
	} while (close_controls_menu == FALSE);

	destroy_bitmap(temp_buffer);
	return return_value;
}

/***************************\
|** WORMY CONTROLS MENU 2 **|
\***************************/
int wormy_controls_menu_2()
{
	BITMAP *temp_buffer = create_bitmap_ex(8, 320, 200);
	char close_controls_menu = FALSE;
	char redraw_controls_menu = TRUE;
	int return_value = 0;

	// Pause the game while looking at the options menu
	game_is_running = FALSE;

	// This is the core snapshot loop
	do {
		idle_speed_counter = 0;
		// Re-draw the options menu if nessecary
		if (redraw_controls_menu == TRUE) {
			show_mouse(NULL);
			blit(worminator_data_file[WORMY_CONTROLS_MENU_2].dat, temp_buffer, 0, 0, 0, 0, 320, 200);

			draw_key(temp_buffer, 145, 41, wormy_config.controls[INPUT_MAP]);
			draw_key(temp_buffer, 145, 54, wormy_config.controls[INPUT_STATS]);

			draw_key(temp_buffer, 145, 74, wormy_config.controls[INPUT_WEAPON_NEXT]);
			draw_key(temp_buffer, 145, 87, wormy_config.controls[INPUT_WEAPON_LAST]);
			draw_key(temp_buffer, 145, 100, wormy_config.controls[INPUT_WEAPON_BEST]);
			draw_key(temp_buffer, 145, 113, wormy_config.controls[INPUT_CHANGE_AMMO_TYPE]);
			draw_key(temp_buffer, 145, 126, wormy_config.controls[INPUT_WEAPON_0]);
			draw_key(temp_buffer, 145, 139, wormy_config.controls[INPUT_WEAPON_1]);
			draw_key(temp_buffer, 145, 152, wormy_config.controls[INPUT_WEAPON_2]);

			draw_key(temp_buffer, 238, 41, wormy_config.secondary_controls[INPUT_MAP]);
			draw_key(temp_buffer, 238, 54, wormy_config.secondary_controls[INPUT_STATS]);

			draw_key(temp_buffer, 238, 74, wormy_config.secondary_controls[INPUT_WEAPON_NEXT]);
			draw_key(temp_buffer, 238, 87, wormy_config.secondary_controls[INPUT_WEAPON_LAST]);
			draw_key(temp_buffer, 238, 100, wormy_config.secondary_controls[INPUT_WEAPON_BEST]);
			draw_key(temp_buffer, 238, 113, wormy_config.secondary_controls[INPUT_CHANGE_AMMO_TYPE]);
			draw_key(temp_buffer, 238, 126, wormy_config.secondary_controls[INPUT_WEAPON_0]);
			draw_key(temp_buffer, 238, 139, wormy_config.secondary_controls[INPUT_WEAPON_1]);
			draw_key(temp_buffer, 238, 152, wormy_config.secondary_controls[INPUT_WEAPON_2]);

			// Draw on the final swap_buffer, and display mouse
			stretch_blit(temp_buffer, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			savedisplay();
			show_mouse(screen);

			// Make sure that we don't endlessly redraw.
			redraw_controls_menu = FALSE;
		}

		// Primiary map key
		if (region_clicked(145, 41, 208, 52) == TRUE) {
			wormy_config.controls[INPUT_MAP] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary stats key
		if (region_clicked(145, 54, 208, 65) == TRUE) {
			wormy_config.controls[INPUT_STATS] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary next weapon key
		if (region_clicked(145, 74, 208, 85) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_NEXT] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary previous weapon key
		if (region_clicked(145, 87, 208, 98) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_LAST] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary best weapon key
		if (region_clicked(145, 100, 208, 111) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_BEST] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary change ammo type key
		if (region_clicked(145, 113, 208, 124) == TRUE) {
			wormy_config.controls[INPUT_CHANGE_AMMO_TYPE] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 0 key
		if (region_clicked(145, 126, 208, 137) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_0] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 1 key
		if (region_clicked(145, 139, 208, 150) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_1] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 2 key
		if (region_clicked(145, 152, 208, 163) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_2] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary map key
		if (region_clicked(238, 41, 301, 52) == TRUE) {
			wormy_config.secondary_controls[INPUT_MAP] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary stats key
		if (region_clicked(238, 54, 301, 65) == TRUE) {
			wormy_config.secondary_controls[INPUT_STATS] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary next weapon key
		if (region_clicked(238, 74, 301, 85) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_NEXT] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary previous weapon key
		if (region_clicked(238, 87, 301, 98) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_LAST] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary best weapon key
		if (region_clicked(238, 100, 301, 111) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_BEST] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary change ammo type key
		if (region_clicked(238, 113, 301, 124) == TRUE) {
			wormy_config.secondary_controls[INPUT_CHANGE_AMMO_TYPE] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 0 key
		if (region_clicked(238, 126, 301, 137) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_0] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 1 key
		if (region_clicked(238, 139, 301, 150) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_1] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 2 key
		if (region_clicked(238, 152, 301, 163) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_2] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Left arrow
		if (mouse_clicked_region(173, 178, 206, 194) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(173, 178, 206, 194) == TRUE) {
				close_controls_menu = TRUE; return_value = -1;
			}
		}

		// Right arrow
		if (mouse_clicked_region(218, 178, 251, 194) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(218, 178, 251, 194) == TRUE) {
				close_controls_menu = TRUE; return_value = 1;
			}
		}

		// Ok button
		if (mouse_clicked_region(275, 178, 308, 194) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(275, 178, 308, 194) == TRUE) {
				close_controls_menu = TRUE; return_value = 0;
			}
		}

		// Optional enter keypress
		if (key[KEY_ENTER]) {
			while (key[KEY_ENTER]);
			close_controls_menu = TRUE;
		}

		if (idle_speed_counter == 0) rest(1);
	} while (close_controls_menu == FALSE);

	destroy_bitmap(temp_buffer);
	return return_value;
}

/***************************\
|** WORMY CONTROLS MENU 3 **|
\***************************/
int wormy_controls_menu_3()
{
	BITMAP *temp_buffer = create_bitmap_ex(8, 320, 200);
	char close_controls_menu = FALSE;
	char redraw_controls_menu = TRUE;
	int return_value = 0;

	// Pause the game while looking at the options menu
	game_is_running = FALSE;

	// This is the core snapshot loop
	do {
		idle_speed_counter = 0;
		// Re-draw the options menu if nessecary
		if (redraw_controls_menu == TRUE) {
			show_mouse(NULL);
			blit(worminator_data_file[WORMY_CONTROLS_MENU_3].dat, temp_buffer, 0, 0, 0, 0, 320, 200);

			draw_key(temp_buffer, 145, 41, wormy_config.controls[INPUT_WEAPON_3]);
			draw_key(temp_buffer, 145, 54, wormy_config.controls[INPUT_WEAPON_4]);
			draw_key(temp_buffer, 145, 67, wormy_config.controls[INPUT_WEAPON_5]);
			draw_key(temp_buffer, 145, 80, wormy_config.controls[INPUT_WEAPON_6]);
			draw_key(temp_buffer, 145, 93, wormy_config.controls[INPUT_WEAPON_7]);
			draw_key(temp_buffer, 145, 106, wormy_config.controls[INPUT_WEAPON_8]);
			draw_key(temp_buffer, 145, 119, wormy_config.controls[INPUT_WEAPON_9]);
			draw_key(temp_buffer, 145, 132, wormy_config.controls[INPUT_WEAPON_10]);
			draw_key(temp_buffer, 145, 145, wormy_config.controls[INPUT_WEAPON_11]);
			draw_key(temp_buffer, 145, 158, wormy_config.controls[INPUT_WEAPON_12]);

			draw_key(temp_buffer, 238, 41, wormy_config.secondary_controls[INPUT_WEAPON_3]);
			draw_key(temp_buffer, 238, 54, wormy_config.secondary_controls[INPUT_WEAPON_4]);
			draw_key(temp_buffer, 238, 67, wormy_config.secondary_controls[INPUT_WEAPON_5]);
			draw_key(temp_buffer, 238, 80, wormy_config.secondary_controls[INPUT_WEAPON_6]);
			draw_key(temp_buffer, 238, 93, wormy_config.secondary_controls[INPUT_WEAPON_7]);
			draw_key(temp_buffer, 238, 106, wormy_config.secondary_controls[INPUT_WEAPON_8]);
			draw_key(temp_buffer, 238, 119, wormy_config.secondary_controls[INPUT_WEAPON_9]);
			draw_key(temp_buffer, 238, 132, wormy_config.secondary_controls[INPUT_WEAPON_10]);
			draw_key(temp_buffer, 238, 145, wormy_config.secondary_controls[INPUT_WEAPON_11]);
			draw_key(temp_buffer, 238, 158, wormy_config.secondary_controls[INPUT_WEAPON_12]);

			// Draw on the final swap_buffer, and display mouse
			stretch_blit(temp_buffer, swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			savedisplay();
			show_mouse(screen);

			// Make sure that we don't endlessly redraw.
			redraw_controls_menu = FALSE;
		}

		// Primiary weapon 3 key
		if (region_clicked(145, 41, 208, 52) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_3] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 4 key
		if (region_clicked(145, 54, 208, 65) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_4] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 5 key
		if (region_clicked(145, 67, 208, 78) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_5] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 6 key
		if (region_clicked(145, 80, 208, 91) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_6] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 7 key
		if (region_clicked(145, 93, 208, 104) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_7] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 8 key
		if (region_clicked(145, 106, 208, 117) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_8] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 9 key
		if (region_clicked(145, 119, 208, 130) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_9] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 10 key
		if (region_clicked(145, 132, 208, 143) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_10] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 11 key
		if (region_clicked(145, 145, 208, 156) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_11] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Primiary weapon 12 key
		if (region_clicked(145, 158, 208, 169) == TRUE) {
			wormy_config.controls[INPUT_WEAPON_12] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 3 key
		if (region_clicked(238, 41, 301, 52) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_3] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 4 key
		if (region_clicked(238, 54, 301, 65) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_4] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 5 key
		if (region_clicked(238, 67, 301, 78) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_5] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 6 key
		if (region_clicked(238, 80, 301, 91) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_6] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 7 key
		if (region_clicked(238, 93, 301, 104) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_7] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 8 key
		if (region_clicked(238, 106, 301, 117) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_8] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 9 key
		if (region_clicked(238, 119, 301, 130) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_9] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 10 key
		if (region_clicked(238, 132, 301, 143) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_10] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 11 key
		if (region_clicked(238, 145, 301, 156) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_11] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Secondary weapon 12 key
		if (region_clicked(238, 158, 301, 169) == TRUE) {
			wormy_config.secondary_controls[INPUT_WEAPON_12] = get_new_key();
			redraw_controls_menu = TRUE;
		}

		// Left arrow
		if (mouse_clicked_region(173, 178, 206, 194) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(173, 178, 206, 194) == TRUE) {
				close_controls_menu = TRUE; return_value = -1;
			}
		}

		// Ok button
		if (mouse_clicked_region(275, 178, 308, 194) == TRUE) {
			while (mouse_b & 1);
			if (mouse_in_region(275, 178, 308, 194) == TRUE) {
				close_controls_menu = TRUE; return_value = 0;
			}
		}

		// Optional enter keypress
		if (key[KEY_ENTER]) {
			while (key[KEY_ENTER]);
			close_controls_menu = TRUE;
		}

		if (idle_speed_counter == 0) rest(1);
	} while (close_controls_menu == FALSE);

	destroy_bitmap(temp_buffer);
	return return_value;
}

/*************************\
|** WORMY CONTROLS MENU **|
\*************************/
void wormy_controls_menu()
{
	BITMAP *backup = create_bitmap_ex(8, screen_width, screen_height);
	int page_num = 0;
	char done = FALSE;

	// Backup the screen
	show_mouse(NULL);
	blit(swap_buffer, backup, 0, 0, 0, 0, screen_width, screen_height);
	show_mouse(screen);
	blit_to_screen(swap_buffer);

	while (!done) {
		if (page_num == 0) {
			page_num += wormy_controls_menu_1();
			if (page_num == 0) done = TRUE;
		} else if (page_num == 1) {
			page_num += wormy_controls_menu_2();
			if (page_num == 1) done = TRUE;
		} else if (page_num == 2) {
			page_num += wormy_controls_menu_3();
			if (page_num == 2) done = TRUE;
		}
	}

	// Restore the screen
	show_mouse(NULL);
	blit(backup, swap_buffer, 0, 0, 0, 0, screen_width, screen_height);
	savedisplay();
	show_mouse(screen);
	blit_to_screen(swap_buffer);

	destroy_bitmap(backup);
}

void save_screenshot()
{
	BITMAP *backup;
	int shotnum = -1;
	//char *fname = "fuck this shit"; 
	char fname[32]; // More compiler friendly version.

	do {
		shotnum++;
		sprintf(fname, "shot%d.bmp", shotnum);
	} while (exists(fname));

	set_color_depth(8);
	backup = create_bitmap_ex(8, screen_width, screen_height);
	blit(swap_buffer, backup, 0, 0, 0, 0, screen_width, screen_height);
	save_bmp(fname, backup, worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	destroy_bitmap(backup);
	while (key[KEY_PRTSCR]);
}
