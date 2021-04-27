void start_demo_recording(char *filename);
void stop_demo_recording();
char play_demo(char *filename);
void resynch();
void setdemoconfig();

void start_demo_recording(char *filename)
{
	demofile = fopen(filename, "wb");
	if (!demofile) {
		alert("Error could not create demofile:", filename, "Demo recording canceled", "OK", 0, 0, 0);
		return;
	}
	write_worminator_map(&worminator_map, demofile);
	write_dword((unsigned int *)&number_of_sprites, demofile);
	write_sprites(sprites, sizeof(sprites) / sizeof(SPRITE), demofile);
	write_player(&player, demofile);
	write_byte((unsigned char *)&current_level, demofile);
	recording_demo = TRUE;
	setdemoconfig();
	resynch();
}

void stop_demo_recording()
{
	int i = 1;

	write_dword((unsigned int *)&i, demofile);	// Indicate that demo is over
	fclose(demofile);
	load_config();
	recording_demo = FALSE;
}

/* retval:
 * 0 done nothing, game state unchanged
 * 1 partially loaded, but couldn't play game state has been changed
 * 2 successfully played, game state has been changed
 */
char play_demo(char *filename)
{
	int i;

	// Load up the demo
	if (!load_saved_game(filename))
		return 0;

	// Load the data from the saved game file
	demofile = fopen(filename, "rb");
	if (demofile) {
		i = fseek(demofile, 347224 /* sizeof(worminator_map) */ +
			  4 /* sizeof(number_of_sprites) */ +
			  (sizeof(sprites) / sizeof(SPRITE)) * 60 +
			  112 /* sizeof(player) */ +
			  1 /* sizeof(current_level)) */, SEEK_SET);
	}
	if (!demofile || (i != 0)) {
		alert("Error: reading demofile:", filename, "Aborting demo play!", "OK", 0, 0, 0);
		return 1;
	}
	playing_demo = TRUE;
	setdemoconfig();
	resynch();

	// Play the actual demo
	clear_keybuf();
	do {	// Start core demo loop
		game_is_running = TRUE;
		while (speed_counter > 0 && !time_to_quit) {
			game_is_running = TRUE;
			// End demo if need be
			read_dword((unsigned int *)&i, demofile);
			if (i == 1) time_to_quit = TRUE;
			else handle_input();
			if (quake_timer > 0) quake_timer--;
			if (player.timer > 0) player.timer--;
			speed_counter--;
			if (keypressed()) time_to_quit = TRUE;
		}
		render_map();
		if (keypressed()) time_to_quit = TRUE;
	} while (!time_to_quit);	// End core demo loop

	fclose(demofile);
	load_config();
	playing_demo = FALSE;
	time_to_quit = FALSE;	// Reset this to avoid ending the game
	clear_keybuf();
	bullet_time_engaged = FALSE;
	change_game_speed(wormy_config.game_speed);
	return 2;
}

void resynch()
{
	char weather_loop;

	game_is_running = FALSE;	// Pause the game

	srand(1);			// Reset the random number seed
	// Re-set the weather
	for (weather_loop = 0; weather_loop < 64; weather_loop++) {
		weather[weather_loop][0] = rand() % screen_width;
		weather[weather_loop][1] = rand() % screen_height;
	}

	// Clear out the timers and unpause the game
	speed_counter = 0;
	global_timer = 0;
	bullet_time = 4096;
	game_is_running = TRUE;
}

void setdemoconfig()
{
	save_config();

	// Set up demo config
	wormy_config.game_speed = GAME_SPEED_NORMAL;
	wormy_config.difficulty = DIFFICULTY_NORMAL;
	wormy_config.particle_detail = 0;
	wormy_config.may_tricks_mode = TRUE;
	wormy_config.autoswitch_weapons = TRUE;
	wormy_config.switch_weapons_on_pickup = FALSE;
	wormy_config.godly_mode = FALSE;
	wormy_config.freeze_mode = FALSE;
	wormy_config.rapid_fire_mode = FALSE;
	wormy_config.fly_mode = FALSE;
	wormy_config.wealth_mode = FALSE;
	wormy_config.super_turkey_mode = FALSE;
	wormy_config.ekg_mode = FALSE;
	wormy_config.cool_mode = FALSE;
	wormy_config.display_particles = TRUE;
	wormy_config.show_laser_sight = FALSE;
}
