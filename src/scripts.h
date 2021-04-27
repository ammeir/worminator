/* -- Scripts.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * For function descriptions and explanations, see proto.h
 * For a detailed explanation of how the engine works, see
 * the documents that came with this source code package. */

/***********************\
|** PLAY INTRODUCTION **|
\***********************/
void play_introduction()
{
	//PSV_DEBUG("play_introduction()");
	// Set the pallete to the worminator pallete
	set_pallete(black_pallete);

	// Fill the screen with black
	rectfill(swap_buffer, 0, 0, screen_width, screen_height, 15);

	// Draw, fade in, rest, and fade out the first frame
	stretch_blit(worminator_data_file[INTRODUCTION_FRAME_01].dat,
		     swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
	blit_to_screen(swap_buffer);
	rest(1000);
	fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
	blit_to_screen(swap_buffer);
	rest(2500);
	fade_out(8);

	// Draw, fade in, rest, and fade out the second frame
	stretch_blit(worminator_data_file[INTRODUCTION_FRAME_02].dat,
		    swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
	blit_to_screen(swap_buffer);
	rest(1000);
	fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
	blit_to_screen(swap_buffer);
	rest(2500);
}

/*****************\
|** RUN SCRIPTS **|
\*****************/
void run_scripts()
{
	int script_loop;

	// Check and run all scripts

	// Level 1 (Worminator Factory) campaign scripts
	if (current_level == 1) {
		// Secret passage message
		if (worminator_map.info_tile_grid[0][0] == 0 && player.x_position_in_pixels + 16 >= 2016 && player.x_position_in_pixels + 16 < 2048 && player.y_position_in_pixels + 16 >= 2624 && player.y_position_in_pixels + 16 < 2656) {
			worminator_map.info_tile_grid[0][0] = 164;
			add_text_message("A secret passage");
		}
		// Small gun turret ambush
		if (worminator_map.info_tile_grid[1][0] == 0 && player.x_position_in_pixels + 16 >= 1872 && player.x_position_in_pixels + 16 < 1904 && player.y_position_in_pixels + 16 >= 2656 && player.y_position_in_pixels + 16 < 2688) {
			worminator_map.info_tile_grid[1][0] = 164;
			worminator_map.foreground_tile_grid[123][167] = 0;
			worminator_map.foreground_tile_grid[124][167] = 0;
			spawn_sprite(SMALL_GUN_TURRET_ENEMY_SPRITE, 1968, 2672, 0, 0, 1);
			play_sample(worminator_data_file[ALARM_SOUND].dat, 255, 128, 1000, FALSE);
			add_text_message("Ambush");
		}
		// Bridge extention switch
		if (worminator_map.info_tile_grid[2][0] == 0 && player.x_position_in_pixels + 16 >= 1744 && player.x_position_in_pixels + 16 < 1760 && player.y_position_in_pixels + 16 >= 2832 && player.y_position_in_pixels + 16 < 2880) {
			worminator_map.info_tile_grid[2][0] = 164;
			worminator_map.background_tile_grid[110][178] = 57;
			for (script_loop = 0; script_loop < 7; script_loop++) {
				worminator_map.background_tile_grid[script_loop + 104][165] = 306;
				worminator_map.info_tile_grid[script_loop + 104][165] = solid_info_tile;
			}
			play_sample(worminator_data_file[GONG_SOUND].dat, 255, 128, 1000, FALSE);
			add_text_message("Bridge extended");
		}
		// Nuclear strike
		if (worminator_map.info_tile_grid[3][0] == 0 && player.x_position_in_pixels + 16 >= 2144 && player.x_position_in_pixels + 16 < 2167 && player.y_position_in_pixels + 16 >= 2672 && player.y_position_in_pixels + 16 < 2704) {
			worminator_map.info_tile_grid[3][0] = 164;
			spawn_sprite(BIG_NUCLEAR_BOMB_ENEMY_SPRITE, 2240, 1856, 0, 0, 0);
			play_sample(worminator_data_file[ALARM_SOUND].dat, 255, 128, 1000, FALSE);
			add_text_message("Nuclear launch detected");
		}
		// Special bridge repair kit item
		if (worminator_map.info_tile_grid[4][0] == 0 && player.x_position_in_pixels + 16 >= 3184 && player.x_position_in_pixels + 16 < 3232 && player.y_position_in_pixels + 16 >= 3008 && player.y_position_in_pixels + 16 < 3056) {
			worminator_map.info_tile_grid[4][0] = 164;
			worminator_map.foreground_tile_grid[200][189] = 0;
			worminator_map.graphical_properties_tile_grid[200][189] = masked_background_grph_tile;
			for (script_loop = 0; script_loop < 4; script_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, 3208, 3032, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			for (script_loop = 0; script_loop < 4; script_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, 3208, 3032, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			add_text_message("You got a bridge repair kit");
			play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);
		}
		// Repair bridge event
		if (worminator_map.info_tile_grid[4][0] == 164 && worminator_map.info_tile_grid[5][0] == 0 && player.x_position_in_pixels + 16 >= 2336 && player.x_position_in_pixels + 16 < 2400 && player.y_position_in_pixels + 16 >= 2928 && player.y_position_in_pixels + 16 < 2992) {
			worminator_map.info_tile_grid[5][0] = 164;
			for (script_loop = 0; script_loop < 13; script_loop++) {
				worminator_map.foreground_tile_grid[script_loop + 135][186] = 2036;
				worminator_map.info_tile_grid[script_loop + 135][186] = solid_info_tile;
			}
			add_text_message("You repaired the lava bridge");
		}
		return;
	}
	// Level 2 (Sewers) campaign scripts
	else if (current_level == 2) {
		// Lift activation switch
		if (worminator_map.info_tile_grid[0][64] == 0 && player.x_position_in_pixels + 16 >= 1328 && player.x_position_in_pixels + 16 < 1344 && player.y_position_in_pixels + 16 >= 304 && player.y_position_in_pixels + 16 < 352) {
			worminator_map.info_tile_grid[0][64] = 164;
			worminator_map.background_tile_grid[84][20] = 56;
			worminator_map.info_tile_grid[36][32] = up_turning_point_info_tile;
			play_sample(worminator_data_file[GONG_SOUND].dat, 255, 128, 1000, FALSE);
			add_text_message("Lift activated");
		}
		// Turbo launch tube
		if (player.turbo_launch == FALSE && player.x_position_in_pixels + 16 >= 768 && player.x_position_in_pixels + 16 < 848 && player.y_position_in_pixels + 16 >= 864 && player.y_position_in_pixels + 16 < 896) {
			player.y_acceleration = -3.5;
			player.turbo_launch = TRUE;
			add_text_message("Turbo Launch");
		}

		return;
	}
	// Level 3 (Cyberworm Building) campaign scripts
	else if (current_level == 3) {
		// Drop red key if nessicary
		if (worminator_map.info_tile_grid[0][88] == 0 && player.x_position_in_pixels + 16 >= 48 && player.x_position_in_pixels + 16 < 80 && player.y_position_in_pixels + 16 >= 208 && player.y_position_in_pixels + 16 < 240) {
			worminator_map.info_tile_grid[0][88] = 164;
			if (player.has_red_keycard == TRUE) {
				player.has_red_keycard = FALSE;
				update_player_stats();
				add_text_message("You just dropped the red keycard back");
				add_text_message("down into the sewer");
			}
		}
		return;
	}
	// Level 4 (Airship) campaign scripts
	else if (current_level == 4) {
		// Nuclear strike halt switch
		if (worminator_map.info_tile_grid[0][0] == 0 && player.x_position_in_pixels + 16 >= 784 && player.x_position_in_pixels + 16 < 800 && player.y_position_in_pixels + 16 >= 1312 && player.y_position_in_pixels + 16 < 1360) {
			worminator_map.info_tile_grid[0][0] = 164;
			worminator_map.background_tile_grid[48][83] = 40;
			for (script_loop = 0; script_loop < 1024; script_loop++) {
				if (sprites[script_loop].sprite_type == NUCLEAR_BOMB_SPAWNER_SPRITE) {
					destroy_sprite(script_loop);
					script_loop--;
				}
			}
			play_sample(worminator_data_file[INITIATE_SHUTDOWN_SEQUENCE_SOUND].dat, 255, 128, 1000, FALSE);
			add_text_message("Nuclear strike halted");
		}
		// Dynamite blast lever
		if (worminator_map.info_tile_grid[1][0] == 0 && player.x_position_in_pixels + 16 >= 1744 && player.x_position_in_pixels + 16 < 1792 && player.y_position_in_pixels + 16 >= 1536 && player.y_position_in_pixels + 16 < 1600) {
			worminator_map.info_tile_grid[1][0] = 164;
			worminator_map.foreground_tile_grid[110][97] = 43;
			worminator_map.foreground_tile_grid[110][98] = 59;
			worminator_map.foreground_tile_grid[131][92] = 0;
			worminator_map.foreground_tile_grid[133][92] = 0;
			worminator_map.foreground_tile_grid[130][93] = 0;
			worminator_map.foreground_tile_grid[131][93] = 0;
			worminator_map.foreground_tile_grid[132][93] = 0;
			worminator_map.foreground_tile_grid[133][93] = 0;
			worminator_map.foreground_tile_grid[134][93] = 0;
			worminator_map.foreground_tile_grid[131][94] = 0;
			worminator_map.foreground_tile_grid[132][94] = 0;
			worminator_map.foreground_tile_grid[133][94] = 0;
			worminator_map.foreground_tile_grid[132][95] = 0;
			worminator_map.foreground_tile_grid[133][95] = 0;
			worminator_map.info_tile_grid[131][92] = 0;
			worminator_map.info_tile_grid[133][92] = 0;
			worminator_map.info_tile_grid[130][93] = 0;
			worminator_map.info_tile_grid[131][93] = 0;
			worminator_map.info_tile_grid[132][93] = 0;
			worminator_map.info_tile_grid[133][93] = 0;
			worminator_map.info_tile_grid[134][93] = 0;
			worminator_map.info_tile_grid[131][94] = 0;
			worminator_map.info_tile_grid[132][94] = 0;
			worminator_map.info_tile_grid[133][94] = 0;
			worminator_map.info_tile_grid[132][95] = 0;
			worminator_map.info_tile_grid[133][95] = 0;
			spawn_sprite(MEDIUM_EXPLOSION_SPRITE, 2112, 1488, 0, 0, 0);
			spawn_shrapgibs(2112, 1488, 64, 0);
			play_sample(worminator_data_file[GONG_SOUND].dat, 255, 128, 1000, FALSE);
			quake_timer = 256;
			add_text_message("Dynamite blast clears the passage");
		}
		return;
	}
	// Level 6 (Don't Know) campaign scripts
	else if (current_level == 6) {
		// Apply a patch to fix a fireworm bug (Thanks a lot Jeff!!!)
		worminator_map.info_tile_grid[117][66] = 19;
		// Seal door and steal weapons
		if (worminator_map.info_tile_grid[0][0] == 0 && player.x_position_in_pixels + 16 >= 160 && player.x_position_in_pixels + 16 < 192 && player.y_position_in_pixels + 16 >= 1296 && player.y_position_in_pixels + 16 < 1328) {
			worminator_map.info_tile_grid[0][0] = 164;
			worminator_map.info_tile_grid[10][81] = 0;
			player.current_weapon = 0;
			player.has_red_keycard = FALSE, player.has_green_keycard = FALSE, player.has_blue_keycard = FALSE;
			player.bullets_left = -1, player.shells_left = -1, player.rockets_left = -1, player.cells_left = -1, player.gasoline_left = -1, player.mortars_left = -1, player.plasma_left = -1, player.mines_left = -1;
			desired_weapon = 0;
			update_player_stats();
			add_text_message("Something stole your weapons and then");
			add_text_message("locked the door behind you");
		}
		// Turbo launch tube
		if (player.turbo_launch == FALSE && player.x_position_in_pixels + 16 >= 1248 && player.x_position_in_pixels + 16 < 1280 && player.y_position_in_pixels + 16 >= 1760 && player.y_position_in_pixels + 16 < 1792) {
			player.y_acceleration = -5.5;
			player.turbo_launch = TRUE;
			add_text_message("Turbo Launch");
		}
		// Game ending
		if (player.x_position_in_pixels + 16 >= 1872 && player.x_position_in_pixels + 16 < 1904 && player.y_position_in_pixels + 16 >= 1648 && player.y_position_in_pixels + 16 < 1680) {
			game_is_running = FALSE;
			stop_midi();
			display_snapshot(12, FALSE);	// Worm scientist message

			// Show the endgame snapshots
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_FINALE_PAGE_01].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			fade_in_pal(worminator_data_file[WORMY_FINALE_PAGE_01_PALLETE].dat, 2);
			blit_to_screen(swap_buffer);
			rest(5000);
			fade_out(2);
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_FINALE_PAGE_02].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			fade_in_pal(worminator_data_file[WORMY_FINALE_PAGE_02_PALLETE].dat, 2);
			blit_to_screen(swap_buffer);
			rest(5000);
			fade_out(2);
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_FINALE_PAGE_03].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			fade_in_pal(worminator_data_file[WORMY_FINALE_PAGE_03_PALLETE].dat, 2);
			blit_to_screen(swap_buffer);
			rest(5000);
			fade_out(2);
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_FINALE_PAGE_04].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			fade_in_pal(worminator_data_file[WORMY_FINALE_PAGE_04_PALLETE].dat, 2);
			blit_to_screen(swap_buffer);
			rest(5000);
			fade_out(2);

			// These screens are common to both versions of the game
			set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
			display_snapshot(13, FALSE);	// Famous last words
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_THE_END_OF_IT_ALL].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			fade_in_pal(worminator_data_file[WORMY_THE_END_OF_IT_ALL_PALLETE].dat, 2);
			blit_to_screen(swap_buffer);
			rest(10000);
			fade_out(2);

			// Restore the main menu again a really cheap way :)
			set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
			stretch_blit(worminator_data_file[MAIN_TITLE_SCREEN].dat,
					swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);

			// Check for a high score
			check_high_score();

			current_level = -69;
			wormy_menu();
		}
		return;
	}

	// Level 7 (Stomach) campaign scripts
	if (current_level == 7) {
		// Update counters
		if (worminator_map.info_tile_grid[0][0] > 0) {
			worminator_map.info_tile_grid[0][0] = worminator_map.info_tile_grid[0][0] + 1;
			if (worminator_map.info_tile_grid[0][0] > 1024) worminator_map.info_tile_grid[0][0] = 0;
		}
		if (worminator_map.info_tile_grid[1][0] > 0) {
			worminator_map.info_tile_grid[1][0] = worminator_map.info_tile_grid[1][0] + 1;
			if (worminator_map.info_tile_grid[1][0] > 1024) worminator_map.info_tile_grid[1][0] = 0;
		}
		if (worminator_map.info_tile_grid[3][0] > 0) {
			worminator_map.info_tile_grid[3][0] = worminator_map.info_tile_grid[3][0] + 1;
			if (worminator_map.info_tile_grid[3][0] > 1024) worminator_map.info_tile_grid[3][0] = 0;
		}

		// Rival village elder's message
		if (worminator_map.info_tile_grid[0][0] == 0 && !playing_demo && !recording_demo && player.x_position_in_pixels + 16 >= 2560 && player.x_position_in_pixels <= 2624 && player.y_position_in_pixels + 16 >= 1616 && player.y_position_in_pixels + 16 <= 1680) {
			game_is_running = FALSE;
			masked_stretch_blit(worminator_data_file[STOMACH_LEVEL_MESSAGE_01].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(1000);
			masked_stretch_blit(worminator_data_file[PRESS_ANY_KEY_MESSAGE].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			clear_keybuf();
			while (!keypressed()) {
			}
			worminator_map.info_tile_grid[0][0] = 1;
			game_is_running = TRUE;
		}

		// Village elder messages
		if (worminator_map.info_tile_grid[0][0] == 0 && !playing_demo && !recording_demo && player.x_position_in_pixels + 16 >= 3600 && player.x_position_in_pixels <= 3664 && player.y_position_in_pixels + 16 >= 2144 && player.y_position_in_pixels + 16 <= 2208) {
			game_is_running = FALSE;
			if (worminator_map.info_tile_grid[2][0] == 1)
				masked_stretch_blit(worminator_data_file[STOMACH_LEVEL_MESSAGE_02].dat,
						swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			if (worminator_map.info_tile_grid[2][0] == 2)
				masked_stretch_blit(worminator_data_file[STOMACH_LEVEL_MESSAGE_03].dat,
						swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			if (worminator_map.info_tile_grid[2][0] == 3)
				masked_stretch_blit(worminator_data_file[STOMACH_LEVEL_MESSAGE_04].dat,
						swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			if (worminator_map.info_tile_grid[2][0] == 4)
				masked_stretch_blit(worminator_data_file[STOMACH_LEVEL_MESSAGE_05].dat,
						swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(1000);
			masked_stretch_blit(worminator_data_file[PRESS_ANY_KEY_MESSAGE].dat,
				swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			clear_keybuf();
			while (!keypressed());
			worminator_map.info_tile_grid[0][0] = 1;
			if (worminator_map.info_tile_grid[2][0] == 1)
				worminator_map.info_tile_grid[2][0] = 2;	// Change the village elder message
			game_is_running = TRUE;
		}

		// Surf shop owner's message
		if (worminator_map.info_tile_grid[3][0] == 0 && !playing_demo && !recording_demo && player.x_position_in_pixels + 16 >= 384 && player.x_position_in_pixels <= 448 && player.y_position_in_pixels + 16 >= 2752 && player.y_position_in_pixels + 16 <= 2816) {
			game_is_running = FALSE;
			masked_stretch_blit(worminator_data_file[STOMACH_LEVEL_MESSAGE_06].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			rest(1000);
			masked_stretch_blit(worminator_data_file[PRESS_ANY_KEY_MESSAGE].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);
			clear_keybuf();
			while (!keypressed());
			worminator_map.info_tile_grid[3][0] = 1;
			game_is_running = TRUE;
		}

		// Holy peach pit grab
		if (worminator_map.background_tile_grid[240][153] != 0 && player.x_position_in_pixels + 16 >= 3824 && player.x_position_in_pixels <= 3888 && player.y_position_in_pixels + 16 >= 2432 && player.y_position_in_pixels + 16 <= 2496) {
			add_text_message("You got the Holy Peach Pit!");
			worminator_map.background_tile_grid[240][153] = 0;
			worminator_map.background_tile_grid[241][153] = 0;
			worminator_map.background_tile_grid[240][154] = 0;
			worminator_map.background_tile_grid[241][154] = 0;
			worminator_map.graphical_properties_tile_grid[240][153] = 0;
			worminator_map.graphical_properties_tile_grid[241][153] = 0;
			worminator_map.graphical_properties_tile_grid[240][154] = 0;
			worminator_map.graphical_properties_tile_grid[241][154] = 0;
			for (script_loop = 0; script_loop < 16; script_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, 3856, 2464, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			for (script_loop = 0; script_loop < 16; script_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, 3856, 2464, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			play_sample(worminator_data_file[HOLY_MUSIC_SOUND].dat, 255, 128, 1000, FALSE);	// Play some holy music
			player.score += 2000;								// Award some points
			worminator_map.info_tile_grid[2][0] = 3;					// Set new village elder message
		}

		// Holy peach pit place
		if (worminator_map.background_tile_grid[241][133] == 0 && worminator_map.background_tile_grid[240][153] == 0 && player.x_position_in_pixels + 16 >= 3840 && player.x_position_in_pixels <= 3904 && player.y_position_in_pixels + 16 >= 2112 && player.y_position_in_pixels + 16 <= 2176) {
			add_text_message("Holy Peach Pit recovered!");
			add_text_message("The village's pride is restored!");
			worminator_map.background_tile_grid[241][133] = 8381;
			worminator_map.background_tile_grid[242][133] = 8382;
			worminator_map.background_tile_grid[241][134] = 8397;
			worminator_map.background_tile_grid[242][134] = 8398;
			worminator_map.graphical_properties_tile_grid[241][133] = masked_background_grph_tile;
			worminator_map.graphical_properties_tile_grid[242][133] = masked_background_grph_tile;
			worminator_map.graphical_properties_tile_grid[241][134] = masked_background_grph_tile;
			worminator_map.graphical_properties_tile_grid[242][134] = masked_background_grph_tile;
			for (script_loop = 0; script_loop < 16; script_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, 3872, 2144, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			for (script_loop = 0; script_loop < 16; script_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, 3872, 2144, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			//spawn_sprite(FIREWORK_SPRITE, 3824, 2176, 0, 0, 0);
			spawn_sprite(BOTTLE_ROCKET_SPRITE, 3840, 2176, 0, 0, 0);
			spawn_sprite(FIREWORK_SPRITE, 3888, 2176, 0, 0, 0);
			//spawn_sprite(FIREWORK_SPRITE, 3904, 2176, 0, 0, 0);
			worminator_map.info_tile_grid[2][0] = 4;					// Set new village elder message
			worminator_map.info_tile_grid[0][0] = 0;					// Reset elder speech timer
			play_sample(worminator_data_file[HOLY_MUSIC_SOUND].dat, 255, 128, 1000, FALSE);	// Play some holy music

			// destroy rock
			for (script_loop = 0; script_loop < 30; script_loop++) {
				worminator_map.background_tile_grid[52 + (script_loop % 10)][133 + (script_loop / 10)] = worminator_map.background_tile_grid[52 + (script_loop % 10)][125 + (script_loop / 10)];
				worminator_map.info_tile_grid[52 + (script_loop % 10)][133 + (script_loop / 10)] = worminator_map.info_tile_grid[52 + (script_loop % 10)][125 + (script_loop / 10)];
				worminator_map.graphical_properties_tile_grid[52 + (script_loop % 10)][133 + (script_loop / 10)] = worminator_map.graphical_properties_tile_grid[52 + (script_loop % 10)][125 + (script_loop / 10)];
			}
			for (script_loop = 0; script_loop < 80; script_loop++) {
				worminator_map.background_tile_grid[52 + (script_loop % 10)][125 + (script_loop / 10)] = 0;
				worminator_map.info_tile_grid[52 + (script_loop % 10)][125 + (script_loop / 10)] = 0;
				worminator_map.graphical_properties_tile_grid[52 + (script_loop % 10)][125 + (script_loop / 10)] = 0;
			}
		}

		// Rock/stomach wall messages
		if (worminator_map.info_tile_grid[35][129] != marker_info_tile && player.x_position_in_pixels + 16 >= 576 && player.x_position_in_pixels + 16 <= 640 && player.y_position_in_pixels + 16 >= 2048 && player.y_position_in_pixels + 16 <= 2128) add_text_message_if_new("Blast through this stomach wall");
		if (worminator_map.info_tile_grid[54][128] != blank_info_tile && player.x_position_in_pixels + 16 >= 976 && player.x_position_in_pixels + 16 <= 1040 && player.y_position_in_pixels + 16 >= 1984 && player.y_position_in_pixels + 16 <= 2176) {
			add_text_message_if_new("You can not move these rocks on your own");
			add_text_message_if_new("Perhaps the local tapeworms can help you");
		}

		// Final tunnel explosion
		for (script_loop = 0; script_loop < 6; script_loop++) {
			if (worminator_map.info_tile_grid[35 + (script_loop % 2)][129 + (script_loop / 2)] == 0) {
				for (script_loop = 0; script_loop < 6; script_loop++) worminator_map.info_tile_grid[35 + (script_loop % 2)][129 + (script_loop / 2)] = marker_info_tile;
				for (script_loop = 0; script_loop < 527; script_loop++) {
					if (worminator_map.background_tile_grid[11 + (script_loop % 31)][105 + (script_loop / 31)] != 0) worminator_map.foreground_tile_grid[11 + (script_loop % 31)][122 + (script_loop / 31)] = worminator_map.background_tile_grid[11 + (script_loop % 31)][105 + (script_loop / 31)];
					else worminator_map.foreground_tile_grid[11 + (script_loop % 31)][122 + (script_loop / 31)] = 0;
					worminator_map.background_tile_grid[11 + (script_loop % 31)][122 + (script_loop / 31)] = 0;
					//worminator_map.foreground_tile_grid[11 + (script_loop % 31)][105 + (script_loop / 31)] = worminator_map.background_tile_grid[0][0];
				}
				for (script_loop = 0; script_loop < 50; script_loop++) worminator_map.background_tile_grid[14 + (script_loop % 5)][126 + (script_loop / 5)] = worminator_map.background_tile_grid[script_loop % 5][126 + (script_loop / 5)];
				for (script_loop = 0; script_loop < 32; script_loop++) {
					if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, 464 + ((float)((rand() % 1920) - 960.00) / 20.00), 2064 + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
					else spawn_sprite(SMALL_EXPLOSION_SPRITE, 464 + ((float)((rand() % 1920) - 960.00) / 20.00), 2064 + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
				}
				quake_timer += 512;
				add_text_message("Stomach lining destroyed!");
				add_text_message("Procede down the tunnel to exit.");
				break;
			}
		}
	}

	// Level 9 (jeph's level) campaign scripts (184, 44) 2*2 - crapatana message
	if (current_level == 9) {
		// Crapatana briefing
		if (worminator_map.info_tile_grid[0][0] != marker_info_tile && player.x_position_in_pixels + 16 >= 2928 && player.x_position_in_pixels + 16 < 2992 && player.y_position_in_pixels + 16 >= 688 && player.y_position_in_pixels + 16 < 752) {
			worminator_map.info_tile_grid[0][0] = marker_info_tile;
			// Display the crapatana message
			display_snapshot(28, FALSE);
			game_is_running = TRUE;
		}
	}

	// Level 11 (Submarine Base) campaign scripts
	if (current_level == 11) {
		// Blast doors message
		if (worminator_map.info_tile_grid[22][91] == solid_info_tile && player.x_position_in_pixels + 16 >= 336 && player.x_position_in_pixels + 16 < 384 && player.y_position_in_pixels + 16 >= 1408 && player.y_position_in_pixels + 16 < 1488) {
			// Display a message saying doors aren't gonna open yet
			text_messages[0].message = "Blast doors locked until missiles armed";
			text_messages[0].timer = 1024;
		}

		// Blast doors switch
		if (worminator_map.background_tile_grid[115][135] == 41 && player.x_position_in_pixels + 16 >= 1824 && player.x_position_in_pixels + 16 < 1872 && player.y_position_in_pixels + 16 >= 2144 && player.y_position_in_pixels + 16 < 2192) {
			worminator_map.background_tile_grid[115][135] = 57;

			// Move the doors up
			worminator_map.foreground_tile_grid[22][89] = 818;
			worminator_map.foreground_tile_grid[24][89] = 818;
			worminator_map.foreground_tile_grid[27][89] = 818;
			worminator_map.foreground_tile_grid[29][89] = 818;
			worminator_map.foreground_tile_grid[22][90] = 0;
			worminator_map.foreground_tile_grid[24][90] = 0;
			worminator_map.foreground_tile_grid[27][90] = 0;
			worminator_map.foreground_tile_grid[29][90] = 0;
			worminator_map.foreground_tile_grid[22][91] = 0;
			worminator_map.foreground_tile_grid[24][91] = 0;
			worminator_map.foreground_tile_grid[27][91] = 0;
			worminator_map.foreground_tile_grid[29][91] = 0;
			worminator_map.foreground_tile_grid[22][90] = 0;
			worminator_map.foreground_tile_grid[24][90] = 0;
			worminator_map.foreground_tile_grid[27][90] = 0;
			worminator_map.foreground_tile_grid[29][90] = 0;
			worminator_map.info_tile_grid[22][90] = 0;
			worminator_map.info_tile_grid[24][90] = 0;
			worminator_map.info_tile_grid[27][90] = 0;
			worminator_map.info_tile_grid[29][90] = 0;
			worminator_map.info_tile_grid[22][91] = 0;
			worminator_map.info_tile_grid[24][91] = 0;
			worminator_map.info_tile_grid[27][91] = 0;
			worminator_map.info_tile_grid[29][91] = 0;

			play_sample(worminator_data_file[GONG_SOUND].dat, 255, 128, 1000, FALSE);
			add_text_message("Missiles armed");
			add_text_message("Blast doors opened");
		}

		// Missile launch event
		if (worminator_map.info_tile_grid[22][91] == 0 && player.x_position_in_pixels + 16 >= 272 && player.x_position_in_pixels + 16 < 288 && player.y_position_in_pixels + 16 >= 1408 && player.y_position_in_pixels + 16 < 1488) {
			worminator_map.info_tile_grid[22][91] = marker_info_tile;
			for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if (sprites[script_loop].sprite_type == GIANT_ROCKET_SPRITE && sprites[script_loop].y_position_in_pixels == 1392) sprites[script_loop].hit_points_left = 0;
			add_text_message("Missiles launched");
		}

		return;
	}

	// Level 12 (Hospital) campaign scripts
	if (current_level == 12) {
		// Wrom and the scientist damage area
		if (worminator_map.info_tile_grid[1][0] == 0) {
			worminator_map.info_tile_grid[1][0] = marker_info_tile;
			spawn_sprite(DAMAGE_MARKER, 2448, 2384, 80, 32, 0);
		}

		// Forcefield warning message
		if (worminator_map.info_tile_grid[0][0] == 0 && player.x_position_in_pixels + 16 >= 1936 && player.x_position_in_pixels <= 2032 && player.y_position_in_pixels + 16 >= 2034 && player.y_position_in_pixels + 16 <= 2368) add_text_message_if_new("Warning, deadly forcefield!");

		// Worm cannon frame hack
		if (worminator_map.info_tile_grid[0][1] == marker_info_tile) for (script_loop = 0; script_loop < number_of_sprites; script_loop++) {
				if (sprites[script_loop].sprite_type == WORM_CANNON_ENEMY_SPRITE && sprites[script_loop].x_position_in_pixels == 94 * 16 && sprites[script_loop].y_position_in_pixels == 103 * 16) sprites[script_loop].frame = 0;
				worminator_map.info_tile_grid[0][1] = blank_info_tile;
			}

		// Satellite dish
		if (worminator_map.info_tile_grid[0][0] == blank_info_tile) {
			for (script_loop = 0; script_loop < 25; script_loop++) if ((player.x_position_in_pixels + 16 >= 1504 && player.x_position_in_pixels + 16 <= 1552 && player.y_position_in_pixels + 16 >= 1744 && player.y_position_in_pixels + 16 <= 1792) || worminator_map.info_tile_grid[94 + script_loop / 5][103 + script_loop % 5] != breakable_wall_info_tile) {
					add_text_message("Radar dish destroyed!");
					add_text_message("Forcefield deactivated");
					for (script_loop = 0; script_loop < 25; script_loop++) {// Destroy the dish
						worminator_map.background_tile_grid[94 + script_loop / 5][103 + script_loop % 5] = 0;
						worminator_map.foreground_tile_grid[94 + script_loop / 5][103 + script_loop % 5] = 0;
						worminator_map.info_tile_grid[94 + script_loop / 5][103 + script_loop % 5] = blank_info_tile;
						worminator_map.graphical_properties_tile_grid[94 + script_loop / 5][103 + script_loop % 5] = 0;
					}
					spawn_sprite(MUSHROOM_CLOUD_SPRITE, 94 * 16 - 24, 103 * 16 - 48, 0, 0, 0);
					spawn_sprite(WORM_CANNON_ENEMY_SPRITE, 94 * 16, 103 * 16, 0, 0, 0);
					sprites[number_of_sprites - 1].hit_points_left = 0;
					worminator_map.foreground_tile_grid[124][145] = 0;
					worminator_map.foreground_tile_grid[125][145] = 0;
					worminator_map.foreground_tile_grid[124][146] = 0;
					worminator_map.foreground_tile_grid[125][146] = 0;
					worminator_map.info_tile_grid[124][145] = blank_info_tile;
					worminator_map.info_tile_grid[125][145] = blank_info_tile;
					worminator_map.info_tile_grid[124][146] = blank_info_tile;
					worminator_map.info_tile_grid[125][146] = blank_info_tile;
					worminator_map.info_tile_grid[0][0] = marker_info_tile;
					worminator_map.info_tile_grid[0][1] = marker_info_tile;
					tile_animations[0][0] = 255;	// Disable animations
					break;
				}
		}

		// Kill Wrom connor message
		if (player.x_position_in_pixels + 16 >= 2256 && player.x_position_in_pixels <= 2528 && player.y_position_in_pixels + 16 >= 2288 && player.y_position_in_pixels + 16 <= 2400) add_text_message_if_new("It's Wrom and the scientist, kill them!");

		// Wrom and the scientist killed script
		for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if (sprites[script_loop].sprite_type == DAMAGE_MARKER && sprites[script_loop].hit_points_left <= 0) {
				worminator_map.foreground_tile_grid[153][149] = 8;
				worminator_map.foreground_tile_grid[154][149] = 9;
				worminator_map.foreground_tile_grid[153][150] = 24;
				worminator_map.foreground_tile_grid[154][150] = 25;

				worminator_map.graphical_properties_tile_grid[153][149] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[154][149] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[153][150] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[154][150] = masked_foreground_grph_tile;

				worminator_map.foreground_tile_grid[156][149] = 474;
				worminator_map.foreground_tile_grid[157][149] = 475;
				worminator_map.foreground_tile_grid[156][150] = 490;
				worminator_map.foreground_tile_grid[157][150] = 491;

				worminator_map.graphical_properties_tile_grid[156][149] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[157][149] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[156][150] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[157][150] = masked_foreground_grph_tile;
				quake_timer = 32;
				if (worminator_map.info_tile_grid[3][0] == 24) {
					load_map(13, NULL, 0);
					return;
				} else {
					worminator_map.info_tile_grid[2][0] = worminator_map.info_tile_grid[2][0] + 1;
					if (worminator_map.info_tile_grid[2][0] > 48) {
						worminator_map.info_tile_grid[2][0] = 0; worminator_map.info_tile_grid[3][0] = worminator_map.info_tile_grid[3][0] + 1;
					}
					spawn_shrapgibs(2464, 2400, 0, 1);
					spawn_shrapgibs(2512, 2400, 0, 1);
				}
			}
	}

	// Level 13  (city level)
	if (current_level == 13) {
		// Wrom damage area
		if (worminator_map.info_tile_grid[0][0] != marker_info_tile) {
			worminator_map.info_tile_grid[0][0] = marker_info_tile;
			spawn_sprite(DAMAGE_MARKER, 3632, 656, 32, 32, 0);
		}

		// Kill Wrom connor message
		if (player.x_position_in_pixels + 16 >= 3552 && player.x_position_in_pixels <= 4000 && player.y_position_in_pixels + 16 >= 592 && player.y_position_in_pixels + 16 <= 704) add_text_message_if_new("It's young Wrom, kill him!");

		// Wrom and the scientist killed script
		for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if (sprites[script_loop].sprite_type == DAMAGE_MARKER && sprites[script_loop].hit_points_left <= 0) {
				worminator_map.foreground_tile_grid[227][41] = 604;
				worminator_map.foreground_tile_grid[228][41] = 605;
				worminator_map.foreground_tile_grid[227][42] = 620;
				worminator_map.foreground_tile_grid[228][42] = 621;
				worminator_map.graphical_properties_tile_grid[227][41] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[228][41] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[227][42] = masked_foreground_grph_tile;
				worminator_map.graphical_properties_tile_grid[228][42] = masked_foreground_grph_tile;
				quake_timer = 32;
				if (worminator_map.info_tile_grid[3][0] == 24) {
					load_map(14, NULL, 0);
					return;
				} else {
					worminator_map.info_tile_grid[2][0] = worminator_map.info_tile_grid[2][0] + 1;
					if (worminator_map.info_tile_grid[2][0] > 48) {
						worminator_map.info_tile_grid[2][0] = 0; worminator_map.info_tile_grid[3][0] = worminator_map.info_tile_grid[3][0] + 1;
					}
					spawn_shrapgibs(3648, 672, 0, 1);
				}
			}
	}

	// Level 14 (Tank ride) campaign scripts
	if (current_level == 14) {
		// Building demolition
		if (worminator_map.info_tile_grid[99][57] != blank_info_tile && player.x_position_in_pixels + 16 >= 1760 && player.x_position_in_pixels <= 1824 && player.y_position_in_pixels + 16 >= 800 && player.y_position_in_pixels + 16 <= 860) {
			for (script_loop = 0; script_loop < 60; script_loop++) {
				worminator_map.foreground_tile_grid[103 + (script_loop % 12)][56 + (script_loop / 12)] = 0;
				worminator_map.graphical_properties_tile_grid[103 + (script_loop % 12)][56 + (script_loop / 12)] = 0;
				if (56 + (script_loop / 12) != 59) worminator_map.info_tile_grid[103 + (script_loop % 12)][56 + (script_loop / 12)] = blank_info_tile;
				else worminator_map.info_tile_grid[103 + (script_loop % 12)][56 + (script_loop / 12)] = tank_solid_from_top_no_drop_info_tile;
			}
			worminator_map.info_tile_grid[99][57] = blank_info_tile;
			worminator_map.info_tile_grid[99][58] = blank_info_tile;

			// Flip the switch
			worminator_map.foreground_tile_grid[111][51] = worminator_map.foreground_tile_grid[111][51] - 2;
			worminator_map.foreground_tile_grid[112][51] = worminator_map.foreground_tile_grid[112][51] - 2;
			worminator_map.foreground_tile_grid[111][52] = worminator_map.foreground_tile_grid[111][52] - 2;
			worminator_map.foreground_tile_grid[112][52] = worminator_map.foreground_tile_grid[112][52] - 2;

			for (script_loop = 0; script_loop < 32; script_loop++) {
				if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, 1744 + ((float)((rand() % 1920) - 960.00) / 20.00), 936 + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
				else spawn_sprite(SMALL_EXPLOSION_SPRITE, 1744 + ((float)((rand() % 1920) - 960.00) / 20.00), 936 + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
			}
			quake_timer += 256;
			play_sample(worminator_data_file[NUCLEAR_EXPLOSION_SOUND].dat, 255, 128, 1000, FALSE);
		}
		// Open red door while in tank
		if (player.skin == TANK_SKIN && player.has_red_keycard == TRUE && player.x_position_in_pixels + 16 >= 3120 && player.x_position_in_pixels <= 3184 && player.y_position_in_pixels + 16 >= 848 && player.y_position_in_pixels + 16 <= 912) {
			for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if (sprites[script_loop].sprite_type == RED_DOOR_SPRITE && sprites[script_loop].action_state == 0) {
					player.has_red_keycard = FALSE;
					sprites[script_loop].action_state = 1;
					sprites[script_loop].timer = 128;
					worminator_map.info_tile_grid[(int)((sprites[script_loop].x_position_in_pixels + 8) / 16)][(int)((sprites[script_loop].y_position_in_pixels + 24) / 16)] = 0;
					worminator_map.info_tile_grid[(int)((sprites[script_loop].x_position_in_pixels + 8) / 16)][(int)((sprites[script_loop].y_position_in_pixels + 40) / 16)] = 0;
					play_relative_sound(DOOR_OPEN_SOUND, script_loop);
				}
		}
		// Red door open
		for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if (sprites[script_loop].sprite_type == RED_DOOR_SPRITE && sprites[script_loop].action_state != 0) {
				worminator_map.info_tile_grid[198][54] = blank_info_tile;
				worminator_map.info_tile_grid[198][55] = blank_info_tile;
			}
	}

	// Level 16 (defend map) campaign scripts
	if (current_level == 16) {
		// Tank door 6, 171
		for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if (sprites[script_loop].sprite_type == BLUE_DOOR_SPRITE && sprites[script_loop].action_state != 0) {
				worminator_map.info_tile_grid[6][171] = blank_info_tile;
				worminator_map.info_tile_grid[6][172] = blank_info_tile;
				worminator_map.foreground_tile_grid[6][171] = 0;
				worminator_map.foreground_tile_grid[6][172] = 0;
			}
		// Timelock
		worminator_map.info_tile_grid[0][0] = worminator_map.info_tile_grid[0][0] + 1;
		if (worminator_map.info_tile_grid[0][0] >= 255) {
			worminator_map.info_tile_grid[1][0] = worminator_map.info_tile_grid[1][0] + 1;
			worminator_map.info_tile_grid[0][0] = 0;
		}
		if (worminator_map.info_tile_grid[1][0] >= 48) {
			worminator_map.info_tile_grid[1][0] = 0;
			for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if (sprites[script_loop].sprite_type == BLUE_DOOR_SPRITE && sprites[script_loop].action_state == 0) {
					sprites[script_loop].action_state = 1;
					sprites[script_loop].timer = 128;
					worminator_map.info_tile_grid[(int)((sprites[script_loop].x_position_in_pixels + 8) / 16)][(int)((sprites[script_loop].y_position_in_pixels + 24) / 16)] = 0;
					worminator_map.info_tile_grid[(int)((sprites[script_loop].x_position_in_pixels + 8) / 16)][(int)((sprites[script_loop].y_position_in_pixels + 40) / 16)] = 0;
					play_relative_sound(DOOR_OPEN_SOUND, script_loop);
					add_text_message("Timelocked door opened");
				}
		}
		// End of game
		for (script_loop = 0; script_loop < number_of_sprites; script_loop++) if ((sprites[script_loop].sprite_type == LASER_REBEL_WORM_ENEMY_SPRITE || sprites[script_loop].sprite_type == ROCKET_REBEL_WORM_ENEMY_SPRITE || sprites[script_loop].sprite_type == RIFLE_REBEL_WORM_ENEMY_SPRITE) && sprites[script_loop].hit_points_left > 0) script_loop = 6666;
		if (script_loop != 6667) {
			game_is_running = FALSE;
			play_midi(worminator_data_file[WORMINATOR_MUSIC_10].dat, TRUE);
			fade_out(256);

			// Show the dead worm background
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_CUTSCENE_06].dat,
					swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			fade_in_pal(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat, 8);
			blit_to_screen(swap_buffer);
			rest(500);
			dofinalescroll();
			fade_out(8);

			// These screens are common to both versions of the game
			set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
			display_snapshot(29, FALSE);	// Famous last words
			set_pallete(black_pallete);
			stretch_blit(worminator_data_file[WORMY_THE_END_OF_IT_ALL].dat,
					swap_buffer, 0, 0, 320, 200, 0, 0, screen_width, screen_height);
			fade_in_pal(worminator_data_file[WORMY_THE_END_OF_IT_ALL_PALLETE].dat, 2);
			blit_to_screen(swap_buffer);
			rest(5000);
			fade_out(2);

			// Restore the main menu again a really cheap way :)
			set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
			stretch_blit(worminator_data_file[MAIN_TITLE_SCREEN].dat,
					swap_buffer, 0, 0, 640, 480, 0, 0, screen_width, screen_height);
			blit_to_screen(swap_buffer);

			// Check for a high score
			check_high_score();

			current_level = -69;
			wormy_menu();
		}
	}
}
