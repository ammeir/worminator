/******************\
|** WORM GIRL AI **|
\******************/
void worm_girl_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// Death animation and null frame
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == WORM_GIRL_DIE_08) {
				return;
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
			if (sprites[sprite_number].frame == WORM_GIRL_DIE_03) {
				if (player.x_position_in_pixels + 16 <= sprites[sprite_number].x_position_in_pixels) spawn_sprite(ENEMY_MORTAR_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 1, -1, 0);
				else spawn_sprite(ENEMY_MORTAR_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, -1, -1, 0);
				sprites[number_of_sprites - 1].frame = WORM_GIRL_HEAD;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// Check to see if the player is sick enough to kill the girl
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 96; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 16, 0, 4);
		spawn_sprite(PERVERT_MESSAGE_SPRITE, 0, 0, 0, 0, 0);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = WORM_GIRL_DIE_01;
		play_sample(worminator_data_file[FEMALE_SCREAM_01].dat, 255, 128, 1000, FALSE);
		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].timer == 0) {
		if (sprites[sprite_number].action_state == 0) {
			if ((rand() % 10) >= 6) {
				sprites[sprite_number].action_state = 1;
				sprites[sprite_number].timer = 64 + (rand() % 16);
				sprites[sprite_number].frame = WORM_GIRL_BLINK;
			} else {
				sprites[sprite_number].action_state = 2, sprites[sprite_number].timer = 8;
			}
		} else if (sprites[sprite_number].action_state == 1) {
			sprites[sprite_number].frame = WORM_GIRL_STAND;
			sprites[sprite_number].timer = 64 + (rand() % 64);
			sprites[sprite_number].action_state = 0;
		} else if (sprites[sprite_number].action_state == 2) {
			if (sprites[sprite_number].frame == WORM_GIRL_TALK_01) sprites[sprite_number].frame = WORM_GIRL_TALK_02;
			else sprites[sprite_number].frame = WORM_GIRL_TALK_01;
			if ((rand() % 32) == 7) {
				sprites[sprite_number].frame = WORM_GIRL_STAND;
				sprites[sprite_number].timer = 64 + (rand() % 64);
				sprites[sprite_number].action_state = 0;
			} else {
				sprites[sprite_number].timer = 8;
			}
		}
	}
}

/*****************\
|** WORM BOY AI **|
\*****************/
void worm_boy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// Check to see if the player killed the boy
	if (sprites[sprite_number].hit_points_left <= 0) {
		for (spawn_loop = 0; spawn_loop < 128; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_sprite(PERVERT_MESSAGE_SPRITE, 0, 0, 0, 0, 0);
		destroy_sprite(sprite_number);
		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].timer == 0) {
		if (sprites[sprite_number].action_state == 0) {
			if ((rand() % 10) >= 6) {
				sprites[sprite_number].action_state = 1;
				sprites[sprite_number].timer = 64 + (rand() % 16);
				sprites[sprite_number].frame = WORM_BOY_BLINK;
			} else {
				sprites[sprite_number].action_state = 2, sprites[sprite_number].timer = 8;
			}
		} else if (sprites[sprite_number].action_state == 1) {
			sprites[sprite_number].frame = WORM_BOY_STAND;
			sprites[sprite_number].timer = 64 + (rand() % 64);
			sprites[sprite_number].action_state = 0;
		} else if (sprites[sprite_number].action_state == 2) {
			if (sprites[sprite_number].frame == WORM_BOY_TALK_01) sprites[sprite_number].frame = WORM_BOY_TALK_02;
			else sprites[sprite_number].frame = WORM_BOY_TALK_01;
			if ((rand() % 32) == 7) {
				sprites[sprite_number].frame = WORM_BOY_STAND;
				sprites[sprite_number].timer = 64 + (rand() % 64);
				sprites[sprite_number].action_state = 0;
			} else {
				sprites[sprite_number].timer = 8;
			}
		}
	}
}

/**************************\
|** WORM STRIPPER ONE AI **|
\**************************/
void worm_stripper_one_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// Death animation and null frame
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == WORM_STRIPPER_ONE_DIE_15) {
				return;
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// Check to see if the player is sick enough to kill the stripper
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 96; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 16, 0, 4);
		spawn_sprite(PERVERT_MESSAGE_SPRITE, 0, 0, 0, 0, 0);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = WORM_STRIPPER_ONE_DIE_01;
		play_sample(worminator_data_file[FEMALE_SCREAM_01].dat, 255, 128, 1000, FALSE);
		return;
	}

	// Check to see if the player is sick enough to kill the stripper
	if (sprites[sprite_number].hit_points_left <= 0)
		for (spawn_loop = 0; spawn_loop < 128; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].timer == 0) {
		if (sprites[sprite_number].animation_state == 1) {
			if (sprites[sprite_number].frame == WORM_STRIPPER_ONE_REST) sprites[sprite_number].frame = WORM_STRIPPER_ONE_BOUNCE_01;
			else sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 16;
			if (sprites[sprite_number].frame > WORM_STRIPPER_ONE_BOUNCE_01 + 7) {
				sprites[sprite_number].frame = WORM_STRIPPER_ONE_REST;
				sprites[sprite_number].animation_state = 0;
				sprites[sprite_number].timer = 16 + rand() % 64;
			}
		} else {
			sprites[sprite_number].animation_state = 1, sprites[sprite_number].timer = 16;
		}
	}
}

/*****************************\
|** WORM BLONDE NUDE ONE AI **|
\*****************************/
void worm_blonde_nude_one_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// Death animation and null frame
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == WORM_BLONDE_NUDE_ONE_DIE_16) {
				return;
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// Check to see if the player is sick enough to kill the stripper
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 96; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_sprite(PERVERT_MESSAGE_SPRITE, 0, 0, 0, 0, 0);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = WORM_BLONDE_NUDE_ONE_DIE_01;
		return;
	}

	// Check to see if the player is sick enough to kill the stripper
	if (sprites[sprite_number].hit_points_left <= 0)
		for (spawn_loop = 0; spawn_loop < 128; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].timer == 0) {
		if (sprites[sprite_number].animation_state == 1) {
			if (sprites[sprite_number].frame == WORM_BLONDE_NUDE_ONE_REST) sprites[sprite_number].frame = WORM_BLONDE_NUDE_ONE_SLIDE_01;
			else if (sprites[sprite_number].frame == WORM_BLONDE_NUDE_ONE_SLIDE_04) sprites[sprite_number].animation_state = 2;
			else sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 16;
		} else if (sprites[sprite_number].animation_state == 2) {
			if (sprites[sprite_number].frame == WORM_BLONDE_NUDE_ONE_SLIDE_01) {
				sprites[sprite_number].frame = WORM_BLONDE_NUDE_ONE_REST;
				sprites[sprite_number].animation_state = 0;
				sprites[sprite_number].timer = 16 + rand() % 64;
			} else {
				sprites[sprite_number].frame--;
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].animation_state = 1, sprites[sprite_number].timer = 16;
		}
	}
}
