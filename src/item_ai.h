/***********************\
|** GETABLE HEALTH AI **|
\***********************/
void getable_health_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// Animate the life essence sprite
	if (sprites[sprite_number].sprite_type == LIFE_ESSENCE_SPRITE && sprites[sprite_number].timer == 0) {
		sprites[sprite_number].frame++;
		sprites[sprite_number].timer = 24;
		if (sprites[sprite_number].frame >= ZZ_ITEM_LIFE_ESSENCE_04) sprites[sprite_number].frame = ZZ_ITEM_LIFE_ESSENCE_01;
	} else if (sprites[sprite_number].sprite_type == LIFE_ESSENCE_SPRITE) {
		sprites[sprite_number].timer--;
	}

	if ((player.health_bars_left < 10 && sprite_is_touching_player(sprite_number) == TRUE) || (sprite_is_touching_player(sprite_number) == TRUE && wormy_config.difficulty == DIFFICULTY_NIGHTMARE)) {
		// Make some sparks so it looks fancy
		if (sprites[sprite_number].sprite_type == LIFE_ESSENCE_SPRITE) {
			for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		} else {
			for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		}

		// This gives the player 1, 2, 5, or 10 health
		if (sprites[sprite_number].sprite_type == LIFE_ESSENCE_SPRITE) {
			hurt_player(1 + rand() % 2);
		} else if (wormy_config.difficulty == DIFFICULTY_WIMP) {
			hurt_player(10);
		} else if (wormy_config.difficulty == DIFFICULTY_EASY) {
			if (sprites[sprite_number].frame == ITEM_HEALTH_1) hurt_player(2);
			else if (sprites[sprite_number].frame == ITEM_HEALTH_2) hurt_player(4);
			else hurt_player(10);
		} else if (wormy_config.difficulty == DIFFICULTY_HARD) {
			hurt_player(1);
		} else if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE) {
			hurt_player(0);
		} else {
			if (sprites[sprite_number].frame == ITEM_HEALTH_1) hurt_player(1);
			else if (sprites[sprite_number].frame == ITEM_HEALTH_2) hurt_player(2);
			else if (sprites[sprite_number].frame == ITEM_HEALTH_5) hurt_player(5);
			else hurt_player(10);
		}

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting a health pack
		player.score += 5;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].sprite_type == LIFE_ESSENCE_SPRITE) {
			add_text_message("You picked up a life essence");
		} else if (wormy_config.difficulty == DIFFICULTY_WIMP) {
			add_text_message("You got full health, wimp");
		} else if (wormy_config.difficulty == DIFFICULTY_EASY) {
			if (sprites[sprite_number].frame == ITEM_HEALTH_10) add_text_message("You got full 10 health");
			else add_text_message("You got extra health, sissy");
		} else if (wormy_config.difficulty == DIFFICULTY_HARD) {
			add_text_message("You got 1 health, tough guy");
		} else if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE) {
			add_text_message("No health for you");
		} else {
			if (sprites[sprite_number].frame == ITEM_HEALTH_1) add_text_message("You got 1 health");
			else if (sprites[sprite_number].frame == ITEM_HEALTH_2) add_text_message("You got 2 health");
			else if (sprites[sprite_number].frame == ITEM_HEALTH_5) add_text_message("You got 5 health");
			else add_text_message("You got 10 health");
		}

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/****************\
|** MINIGUN AI **|
\****************/
void minigun_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.bullets_left < 400 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the minigun avalible as a weapon
		if (player.bullets_left < 0) player.bullets_left = 0;

		// This gives the player 75 bullets and checks to make sure that you have no more than the limit of 400 bullets
		player.bullets_left += 75;
		if (player.bullets_left > 400) player.bullets_left = 400;

		// This picks the mini gun if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 1;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the minigun
		player.score += 25;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Minigun");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/************************\
|** GETABLE BULLETS AI **|
\************************/
void getable_bullets_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.bullets_left < 400 && player.bullets_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 40 or 80 bullets and checks to make sure that you have no more than the limit of 400 bullets
		if (sprites[sprite_number].frame == ITEM_BULLETS_SMALL) player.bullets_left += 40;
		else player.bullets_left += 80;
		if (player.bullets_left > 400) player.bullets_left = 400;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the bullets
		player.score += 10;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_BULLETS_SMALL) add_text_message("You got 40 bullets");
		else add_text_message("You got 80 bullets");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/****************\
|** SHOTGUN AI **|
\****************/
void shotgun_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.shells_left < 50 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the shotgun avalible as a weapon
		if (player.shells_left < 0) player.shells_left = 0;

		// This gives the player 15 shells and checks to make sure that you have no more than the limit of 50 shells
		player.shells_left += 15;
		if (player.shells_left > 50) player.shells_left = 50;

		// This picks the shotgun if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 2;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the shotgun
		player.score += 25;

		// This will refresh the stats to display the new shell total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Shotgun");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/***********************\
|** GETABLE SHELLS AI **|
\***********************/
void getable_shells_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.shells_left < 50 && player.shells_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 10 or 25 shells and checks to make sure that you have no more than the limit of 50 shells
		if (sprites[sprite_number].frame == ITEM_SHELLS_SMALL) player.shells_left += 10;
		else player.shells_left += 25;
		if (player.shells_left > 50) player.shells_left = 50;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the shells
		player.score += 10;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_SHELLS_SMALL) add_text_message("You got 5 shells");
		else add_text_message("You got 25 shells");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/************************\
|** ROCKET LAUNCHER AI **|
\************************/
void rocket_launcher_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.rockets_left < 75 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the rocket launcher avalible as a weapon
		if (player.rockets_left < 0) player.rockets_left = 0;

		// This gives the player 10 rockets and checks to make sure that you have no more than the limit of 75 rockets
		player.rockets_left += 10;
		if (player.rockets_left > 75) player.rockets_left = 75;

		// This picks the rocket launcher if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 3;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the rocket launcher
		player.score += 25;

		// This will refresh the stats to display the new rocket total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Rocket Launcher");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/************************\
|** GETABLE ROCKETS AI **|
\************************/
void getable_rockets_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.rockets_left < 75 && player.rockets_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 1 or 5 rockets and checks to make sure that you have no more than the limit of 75 rockets
		if (sprites[sprite_number].frame == ITEM_ROCKETS_SMALL) player.rockets_left += 1;
		else player.rockets_left += 5;
		if (player.rockets_left > 75) player.rockets_left = 75;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the rockets
		player.score += 10;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_ROCKETS_SMALL) add_text_message("You got 1 rocket");
		else add_text_message("You got 5 rockets");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/******************\
|** LASER GUN AI **|
\******************/
void laser_gun_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.cells_left < 75 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the laser gun avalible as a weapon
		if (player.cells_left < 0) player.cells_left = 0;

		// This gives the player 25 cells and checks to make sure that you have no more than the limit of 75 cells
		player.cells_left += 25;
		if (player.cells_left > 75) player.cells_left = 75;

		// This picks the laser gun if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 5;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the laser gun
		player.score += 25;

		// This will refresh the stats to display the new cell total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Laser Gun");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/**********************\
|** GETABLE CELLS AI **|
\**********************/
void getable_cells_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.cells_left < 75 && player.cells_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 25 or 50 cells and checks to make sure that you have no more than the limit of 75 cells
		if (sprites[sprite_number].frame == ITEM_CELLS_SMALL) player.cells_left += 25;
		else player.cells_left += 50;
		if (player.cells_left > 75) player.cells_left = 75;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the cells
		player.score += 10;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_CELLS_SMALL) add_text_message("You got 25 cells");
		else add_text_message("You got 50 cells");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*********************\
|** FLAMETHROWER AI **|
\*********************/
void flamethrower_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.gasoline_left < 100 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the flame thrower avalible as a weapon
		if (player.gasoline_left < 0) player.gasoline_left = 0;

		// This gives the player 25 gasoline and checks to make sure that you have no more than the limit of 25 gasoline
		player.gasoline_left += 25;
		if (player.gasoline_left > 100) player.gasoline_left = 100;

		// This picks the flamethrower if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 6;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the flamethrower
		player.score += 25;

		// This will refresh the stats to display the new gasoline total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Flamethrower");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*************************\
|** GETABLE GASOLINE AI **|
\*************************/
void getable_gasoline_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.gasoline_left < 100 && player.gasoline_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 15 or 30 gasoline and checks to make sure that you have no more than the limit of 25 gasoline
		if (sprites[sprite_number].frame == ITEM_GASOLINE_SMALL) player.gasoline_left += 15;
		else player.gasoline_left += 30;
		if (player.gasoline_left > 100) player.gasoline_left = 100;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the gasoline
		player.score += 10;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_GASOLINE_SMALL) add_text_message("You got 5 gasoline");
		else add_text_message("You got 10 gasoline");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/**********************\
|** MORTAR CANNON AI **|
\**********************/
void mortar_cannon_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.mortars_left < 25 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the mortar cannon avalible as a weapon
		if (player.mortars_left < 0) player.mortars_left = 0;

		// This gives the player 5 mortars and checks to make sure that you have no more than the limit of 25 mortars
		player.mortars_left += 5;
		if (player.mortars_left > 25) player.mortars_left = 25;

		// This picks the mortar cannon if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 7;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the mortar cannon
		player.score += 25;

		// This will refresh the stats to display the new plasma total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Mortar Cannon");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/************************\
|** GETABLE MORTARS AI **|
\************************/
void getable_mortars_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.mortars_left < 25 && player.mortars_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 1 or 5 mortars and checks to make sure that you have no more than the limit of 25 mortars
		if (sprites[sprite_number].frame == ITEM_MORTARS_SMALL) player.mortars_left += 1;
		else player.mortars_left += 5;
		if (player.mortars_left > 25) player.mortars_left = 25;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the mortars
		player.score += 10;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_MORTARS_SMALL) add_text_message("You got 1 mortar");
		else add_text_message("You got 5 mortars");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/************\
|** BFG AI **|
\************/
void bfg_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.plasma_left < 10 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the bfg avalible as a weapon
		if (player.plasma_left < 0) player.plasma_left = 0;

		// This gives the player 2 plasma and checks to make sure that you have no more than the limit of 10 plasma
		player.plasma_left += 2;
		if (player.plasma_left > 10) player.plasma_left = 10;

		// This picks the bfg if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 8;

		// This plays the bfg pickup weapon sound
		play_sample(worminator_data_file[PICKUP_BFG_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the bfg
		player.score += 500;

		// This will refresh the stats to display the new plasma total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the BFG 1574");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/***********************\
|** GETABLE PLASMA AI **|
\***********************/
void getable_plasma_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.plasma_left < 10 && player.plasma_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 1 or 2 plasma and checks to make sure that you have no more than the limit of 10 plasma
		if (sprites[sprite_number].frame == ITEM_PLASMA_SMALL) player.plasma_left++;
		else player.plasma_left += 2;
		if (player.plasma_left > 10) player.plasma_left = 10;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the plasma
		player.score += 50;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_PLASMA_SMALL) add_text_message("You got 1 plasma");
		else add_text_message("You got 2 plasma");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/******************\
|** MINELAYER AI **|
\******************/
void minelayer_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.mines_left < 15 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the minelayer avalible as a weapon
		if (player.mines_left < 0) player.mines_left = 0;

		// This gives the player 5 mines and checks to make sure that you have no more than the limit of 15 mines
		player.mines_left += 5;
		if (player.mines_left > 15) player.mines_left = 15;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the minelayer
		player.score += 25;

		// This picks the minelayer if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 9;

		// This will refresh the stats to display the new plasma total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the MineLayer");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/**********************\
|** GETABLE MINES AI **|
\**********************/
void getable_mines_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.mines_left < 15 && player.mines_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 1 or 5 mines and checks to make sure that you have no more than the limit of 15 mines
		if (sprites[sprite_number].frame == ITEM_MINES_SMALL) player.mines_left++;
		else player.mines_left += 5;
		if (player.mines_left > 15) player.mines_left = 15;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the mines
		player.score += 10;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_MINES_SMALL) add_text_message("You got 1 mine");
		else add_text_message("You got 2 mines");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/********************\
|** RED KEYCARD AI **|
\********************/
void red_keycard_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.has_red_keycard == FALSE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the red keycard
		player.has_red_keycard = TRUE;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the red keycard
		player.score += 50;

		// This will refresh the stats to show the keycard
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the red keycard");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/**********************\
|** GREEN KEYCARD AI **|
\**********************/
void green_keycard_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.has_green_keycard == FALSE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the green keycard
		player.has_green_keycard = TRUE;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the green keycard
		player.score += 50;

		// This will refresh the stats to show the keycard
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the green keycard");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*********************\
|** BLUE KEYCARD AI **|
\*********************/
void blue_keycard_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.has_blue_keycard == FALSE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the blue keycard
		player.has_blue_keycard = TRUE;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the blue keycard
		player.score += 50;

		// This will refresh the stats to show the keycard
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the blue keycard");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/***************\
|** LETTER AI **|
\***************/
void letter_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the letter
		player.score += 500;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a wormy letter");

		// This considers the letter order and order bonus cases
		if (sprites[sprite_number].sprite_type == LETTER_W_SPRITE) {
			if (player.letter_order == 0) player.letter_order = 1;
			else player.letter_order = -1;
		} else if (sprites[sprite_number].sprite_type == LETTER_O_SPRITE) {
			if (player.letter_order == 1) player.letter_order = 2;
			else player.letter_order = -1;
		} else if (sprites[sprite_number].sprite_type == LETTER_R_SPRITE) {
			if (player.letter_order == 2) player.letter_order = 3;
			else player.letter_order = -1;
		} else if (sprites[sprite_number].sprite_type == LETTER_M_SPRITE) {
			// Award the actual bonus
			if (player.letter_order == 3) {
				// Make some extra sparks so it looks fancy
				for (spawn_loop = 0; spawn_loop < 24; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 60.00) / 20.00, 0);
				for (spawn_loop = 0; spawn_loop < 24; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 60.00) / 20.00, 0);
				for (spawn_loop = 0; spawn_loop < 24; spawn_loop++) spawn_sprite(SMALL_BLUE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 60.00) / 20.00, 0);

				// Spawn flyup sprites
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels - 16, sprites[sprite_number].y_position_in_pixels + 5 - 16, 0, 0, 0);
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels - 16, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels - 16, sprites[sprite_number].y_position_in_pixels + 5 + 16, 0, 0, 0);
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5 - 16, 0, 0, 0);
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5 + 16, 0, 0, 0);
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 5 - 16, 0, 0, 0);
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);
				spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 5 + 16, 0, 0, 0);

				// This gives the player a ton of points for getting the word worm spelled properly
				player.score += 4000;

				// This will refresh the stats to show the new point total
				update_player_stats();

				// This displays a message to the player
				add_text_message("You got the superwormy spelling bonus");

				// Reset the leter_order counter just in case
				player.letter_order = 0;
			} else {
				player.letter_order = -1;
			}
		}

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*********************\
|** SODA SIXPACK AI **|
\*********************/
void soda_sixpack_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_50_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the soda
		player.score += 50;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a soda sixpack");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/******************\
|** DOUGHNUTS AI **|
\******************/
void doughnuts_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_100_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the doughnuts
		player.score += 100;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got some doughnuts");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*************************\
|** BOX OF DOUGHNUTS AI **|
\*************************/
void box_of_doughnuts_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_250_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the box of doughnuts
		player.score += 250;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a box of doughnuts");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*****************\
|** MUD BEER AI **|
\*****************/
void mud_beer_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the mud beer
		player.score += 500;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a bottle of mud beer");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*******************\
|** DIRT CHIPS AI **|
\*******************/
void dirt_chips_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_50_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the dirt chips
		player.score += 50;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a bag of dirt chips");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/**********************\
|** MUD ICE CREAM AI **|
\**********************/
void mud_ice_cream_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_100_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the mud ice cream
		player.score += 100;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a cone of mud ice cream");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/************************\
|** MINI MOUNT N DO AI **|
\************************/
void mini_mount_n_do_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_250_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the mini mount n do
		player.score += 250;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a can of mini mount n do");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/****************\
|** MUD PIE AI **|
\****************/
void mud_pie_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the mud pie
		player.score += 500;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a slice of mud pie");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/******************************\
|** BIG TASTY BALL O DIRT AI **|
\******************************/
void big_tasty_ball_o_dirt_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// Spawn a flyup sprite
		spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);
		spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 5, 0, 0, 0);
		spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 21, 0, 0, 0);
		spawn_sprite(BONUS_FLYUP_500_POINT_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 21, 0, 0, 0);

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the ball of dirt
		player.score += 2000;

		// This will refresh the stats to show the new point total
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a big tasty ball o dirt!");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*****************\
|** CHAINSAW AI **|
\*****************/
void chainsaw_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.chainsaw_duration_left < 666 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the chainsaw avalible as a weapon
		if (player.chainsaw_duration_left < 0) player.chainsaw_duration_left = 0;

		// This gives the player a full chainsaw duration counter
		player.chainsaw_duration_left = 666;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the chainsaw
		player.score += 50;

		// This picks the chainsaw if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 10;

		// This will refresh the stats to display the new duration total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Chainsaw");
		add_text_message("Go find some meat");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*******************************\
|** GETABLE PLASMA BULLETS AI **|
\*******************************/
void getable_plasma_bullets_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.plasma_bullets_left < 250 && player.bullets_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 10 or 25 plasma bullets and checks to make sure that you have no more than the limit of 100 plasma bullets
		if (sprites[sprite_number].frame == ITEM_PLASMA_BULLETS_SMALL) player.plasma_bullets_left += 25;
		else player.plasma_bullets_left += 50;
		if (player.plasma_bullets_left > 250) player.plasma_bullets_left = 250;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the plasma bullets
		player.score += 5;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_PLASMA_BULLETS_SMALL) add_text_message("You got 25 plasma bullets");
		else add_text_message("You got 50 plasma bullets");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*********************************\
|** GETABLE EXPLOSIVE SHELLS AI **|
\*********************************/
void getable_explosive_shells_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.explosive_shells_left < 25 && player.shells_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 5 or 10 explosive shells and checks to make sure that you have no more than the limit of 25 explosive shells
		if (sprites[sprite_number].frame == ITEM_EXPLOSIVE_SHELLS_SMALL) player.explosive_shells_left += 5;
		else player.explosive_shells_left += 10;
		if (player.explosive_shells_left > 25) player.explosive_shells_left = 25;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the explosive shells
		player.score += 15;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_EXPLOSIVE_SHELLS_SMALL) add_text_message("You got 5 explosive shells");
		else add_text_message("You got 10 explosive shells");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/***************************\
|** GETABLE MICRO NUKE AI **|
\***************************/
void getable_micro_nuke_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.micro_nukes_left < 1 && player.rockets_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player a micro nuke
		player.micro_nukes_left++;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the micro nuke
		player.score += 75;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got a Micro Nuke");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/**********************\
|** GETABLE RAILS AI **|
\**********************/
void getable_rails_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.rails_left < 50 && player.rails_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 10 or 20 rails and checks to make sure that you have no more than the limit of 50 rails
		if (sprites[sprite_number].frame == ITEM_RAILS_SMALL) player.rails_left += 10;
		else player.rails_left += 20;
		if (player.rails_left > 50) player.rails_left = 50;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the rails
		player.score += 20;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_RAILS_SMALL) add_text_message("You got 10 rails");
		else add_text_message("You got 20 rails");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/******************************\
|** GETABLE WALKING MINES AI **|
\******************************/
void getable_walking_mines_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.walking_mines_left < 10 && player.mines_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player 1 or 5 walking mines and checks to make sure that you have no more than the limit of 10 walking mines
		if (sprites[sprite_number].frame == ITEM_WALKING_MINES_SMALL) player.walking_mines_left++;
		else player.walking_mines_left += 5;
		if (player.walking_mines_left > 10) player.walking_mines_left = 10;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the walking mines
		player.score += 25;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		if (sprites[sprite_number].frame == ITEM_WALKING_MINES_SMALL) add_text_message("You got 1 walking mine");
		else add_text_message("You got 5 walking mines");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/****************\
|** RAILGUN AI **|
\****************/
void railgun_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.rails_left < 50 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the railgun avalible as a weapon
		if (player.rails_left < 0) player.rails_left = 0;

		// This gives the player 5 rails and checks to see that you have no more than the limit of 50 rails
		player.rails_left += 5;
		if (player.rails_left > 50) player.spikestars_left = 50;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the railgun
		player.score += 200;

		// This picks the railgun if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 11;

		// This will refresh the stats to display the new duration total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Railgun");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*******************\
|** STARDUSTER AI **|
\*******************/
void starduster_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.spikestars_left < 9 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This makes the starduster avalible as a weapon
		if (player.spikestars_left < 0) player.spikestars_left = 0;

		// This gives the player a single blast of 3 spikestars and checks to see that you have no more than the limit of 3 spikestars
		player.spikestars_left += 3;
		if (player.spikestars_left > 9) player.spikestars_left = 9;

		// Play the pickup weapon sound
		play_sample(worminator_data_file[PICKUP_WEAPON_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the starduster
		player.score += 250;

		// This picks the starduster if the player does not have a different gun selected
		if (player.current_weapon == 0 || wormy_config.switch_weapons_on_pickup) desired_weapon = 12;

		// This will refresh the stats to display the new duration total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got the Starduster");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/**************************\
|** GETABLE SPIKESTARS AI **|
\**************************/
void getable_spikestars_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (player.spikestars_left < 9 && player.spikestars_left >= 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		// Make some sparks so it looks fancy
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 10, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

		// This gives the player a single blast of 3 spikestars and checks to see that you have no more than the limit of 3 spikestars
		player.spikestars_left += 3;
		if (player.spikestars_left > 9) player.spikestars_left = 9;

		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// This gives the player some points for getting the spikestars
		player.score += 150;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("You got 3 spikestars");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}

/*************\
|** TANK AI **|
\*************/
void tank_ai(int sprite_number)
{
	if (wormy_config.super_turkey_mode == TRUE) return;	// Turkeys cant enter tanks silly!
	if (sprites[sprite_number].timer > 0) sprites[sprite_number].timer--;
	if (!sprite_is_touching_player(sprite_number) && player.y_position_in_pixels + 0 >= sprites[sprite_number].y_position_in_pixels) sprites[sprite_number].timer = 0;
	else if (sprite_is_touching_player(sprite_number) && sprites[sprite_number].timer > 0 && player.y_position_in_pixels + 16 >= sprites[sprite_number].y_position_in_pixels && sprites[sprite_number].timer > 16) sprites[sprite_number].timer = 16;
	else if (sprite_is_touching_player(sprite_number) && sprites[sprite_number].timer > 0) sprites[sprite_number].timer++;
	if (sprites[sprite_number].timer == 0 && sprite_is_touching_player(sprite_number) == TRUE && player.y_acceleration >= 0) {
		// Play the pickup item sound
		play_sample(worminator_data_file[PICKUP_ITEM_SOUND].dat, 255, 128, 1000, FALSE);

		// Set the player position and skin
		player.skin = TANK_SKIN;
		player.x_position_in_pixels = sprites[sprite_number].x_position_in_pixels;
		player.y_position_in_pixels = sprites[sprite_number].y_position_in_pixels;

		// This will refresh the stats to display the new bullet total and highlight the weapon
		update_player_stats();

		// This displays a message to the player
		add_text_message("Tank Time!");

		// Kill this sprite
		destroy_sprite(sprite_number);
	}
}
