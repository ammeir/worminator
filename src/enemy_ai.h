/************************\
|** CHECK ENEMY RANGES **|
\************************/
void check_enemy_ranges()
{
	int sprite_loop;

	LOCK_VARIABLE(sprite_loop);

	if (game_is_running == FALSE) return;
	game_is_running = FALSE;

	for (sprite_loop = 0; sprite_loop < number_of_sprites; sprite_loop++) {
		if (sprites[sprite_loop].hit_points_left > 0) {
			if (sprites[sprite_loop].x_position_in_pixels < player.x_position_in_pixels && player.x_position_in_pixels - sprites[sprite_loop].x_position_in_pixels + sprites[sprite_loop].width > 640) sprites[sprite_loop].in_range = FALSE;
			else if (sprites[sprite_loop].x_position_in_pixels > player.x_position_in_pixels && sprites[sprite_loop].x_position_in_pixels - player.x_position_in_pixels + 32 > 640) sprites[sprite_loop].in_range = FALSE;
			else if (sprites[sprite_loop].y_position_in_pixels < player.y_position_in_pixels && player.y_position_in_pixels - sprites[sprite_loop].y_position_in_pixels + sprites[sprite_loop].width > 512) sprites[sprite_loop].in_range = FALSE;
			else if (sprites[sprite_loop].y_position_in_pixels > player.y_position_in_pixels && sprites[sprite_loop].y_position_in_pixels - player.y_position_in_pixels + 32 > 512) sprites[sprite_loop].in_range = FALSE;
			else sprites[sprite_loop].in_range = TRUE;
		}
		if (sprites[sprite_loop].x_position_in_pixels < -64 || sprites[sprite_loop].y_position_in_pixels < -64 || sprites[sprite_loop].x_position_in_pixels > 4160 || sprites[sprite_loop].y_position_in_pixels > 3440) {
			destroy_sprite(sprite_loop);
			sprite_loop--;
		}
	}
	game_is_running = TRUE;
}

END_OF_FUNCTION(check_enemy_ranges);

/************************\
|** CHECK ENEMY SLOPES **|
\************************/
char check_enemy_slopes(int sprite_number)
{
	int slope_test_tile;

	// Primary slope check
	slope_test_tile = worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2)) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height)) / 16)];
	if (slope_test_tile >= slope_up_left_to_right_45_info_tile && slope_test_tile <= slope_filler_info_tile) {
		if (slope_test_tile == slope_up_left_to_right_45_info_tile) {
			sprites[sprite_number].y_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		} else if (slope_test_tile == slope_up_right_to_left_45_info_tile) {
			sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		} else if (slope_test_tile == slope_up_left_to_right_225_right_info_tile || slope_test_tile == slope_up_left_to_right_225_left_info_tile) {
			sprites[sprite_number].y_position_in_pixels -= sprites[sprite_number].x_acceleration / 2;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		} else if (slope_test_tile == slope_up_right_to_left_225_right_info_tile || slope_test_tile == slope_up_right_to_left_225_left_info_tile) {
			sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].x_acceleration / 2;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		}
	}

	// Secondary slope check
	slope_test_tile = worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2)) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height - 4)) / 16)];
	if (slope_test_tile >= slope_up_left_to_right_45_info_tile && slope_test_tile <= slope_filler_info_tile) {
		if (slope_test_tile == slope_up_left_to_right_45_info_tile) {
			sprites[sprite_number].y_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		} else if (slope_test_tile == slope_up_right_to_left_45_info_tile) {
			sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		} else if (slope_test_tile == slope_up_left_to_right_225_right_info_tile || slope_test_tile == slope_up_left_to_right_225_left_info_tile) {
			sprites[sprite_number].y_position_in_pixels -= sprites[sprite_number].x_acceleration / 2;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		} else if (slope_test_tile == slope_up_right_to_left_225_right_info_tile || slope_test_tile == slope_up_right_to_left_225_left_info_tile) {
			sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].x_acceleration / 2;
			sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
			return TRUE;
		}
	}
	return FALSE;
}

/***********************\
|** CHECK ENEMY TURNS **|
\***********************/
char check_enemy_turns(int sprite_number)
{
	int test_tile;

	if (sprites[sprite_number].direction == 0) {
		test_tile = worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height - 8)) / 16)];
		if (test_tile == solid_info_tile || test_tile == block_enemy_info_tile || test_tile == breakable_wall_info_tile) return TRUE;
		test_tile = worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2) + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height) / 16)];
		if (test_tile == blank_info_tile) return TRUE;
	} else {
		test_tile = worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height - 8)) / 16)];
		if (test_tile == solid_info_tile || test_tile == block_enemy_info_tile || test_tile == breakable_wall_info_tile) return TRUE;
		test_tile = worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2) - 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height) / 16)];
		if (test_tile == blank_info_tile) return TRUE;
	}

	return FALSE;
}

/**********************************\
|** CHECK ENEMY SLOPE EXCLUSIONS **|
\**********************************/
char check_enemy_slope_exclusions(int sprite_number)
{
	int test_tile;
	char number_of_loops = sprites[sprite_number].width / 16;
	char looper;

	if (sprites[sprite_number].width <= 32) return FALSE;

	for (looper = 0; looper <= number_of_loops; looper++) {
		test_tile = worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels) / 16) + looper][(int)((sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height - 8)) / 16)];
		if (test_tile >= slope_up_left_to_right_45_info_tile && test_tile <= slope_filler_info_tile) return TRUE;
	}

	return FALSE;
}

/**********************\
|** RUNNING ENEMY AI **|
\**********************/
void running_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == SMALL_RUNNER_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(SMALL_RUNNER_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 8, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 12, 16, 0);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = SMALL_RUNNER_DIE_01;

		// This gives the player some points for killing the runner and updates the score display
		player.score += 10;
		update_player_stats();

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// This is the ground action state used for the basic action, commonly walking
	if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > SMALL_RUNNER_RUN_05) sprites[sprite_number].frame = SMALL_RUNNER_RUN_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/**************************\
|** ROBOTIC DOG ENEMY AI **|
\**************************/
void robotic_dog_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == ROBOTIC_DOG_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(ROBOTIC_DOG_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 8, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 0);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = ROBOTIC_DOG_DIE_01;

		// This gives the player some points for killing the robotic dog and updates the score display
		player.score += 25;
		update_player_stats();

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// This makes the dog enter the biteing state when it should
	if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) {
		if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels + 32 >= sprites[sprite_number].x_position_in_pixels + 16) sprites[sprite_number].action_state = 1;
		else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels <= sprites[sprite_number].x_position_in_pixels + 16) sprites[sprite_number].action_state = 1;
	}

	if (sprites[sprite_number].action_state == 1) {
		// Animate the enemy's biteing state
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == ROBOTIC_DOG_BITE_02) {
				sprites[sprite_number].frame = ROBOTIC_DOG_WALK_01;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame == ROBOTIC_DOG_BITE_01) {
				sprites[sprite_number].frame = ROBOTIC_DOG_BITE_02;
				if (sprites[sprite_number].direction == 0 && player.frame != 0 && sprite_is_touching_player(sprite_number) == TRUE && player.x_position_in_pixels + 15 >= sprites[sprite_number].x_position_in_pixels) hurt_player(-2);
				else if (sprites[sprite_number].direction == 1 && player.frame != 0 && sprite_is_touching_player(sprite_number) == TRUE && player.x_position_in_pixels <= sprites[sprite_number].x_position_in_pixels + 15) hurt_player(-2);
				play_relative_sound(DOG_BARK_SOUND, sprite_number);
			} else {
				sprites[sprite_number].frame = ROBOTIC_DOG_BITE_01;
			}
			sprites[sprite_number].timer = 24;
		} else {
			sprites[sprite_number].timer--;
		}

		return;
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > ROBOTIC_DOG_WALK_04) sprites[sprite_number].frame = ROBOTIC_DOG_WALK_01;
			sprites[sprite_number].timer = 24;
		} else {
			sprites[sprite_number].timer--;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/*************************\
|** SMALL GUN TURRET AI **|
\*************************/
void small_gun_turret_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			return;
		} else {
			if (sprites[sprite_number].timer / 8.0 == (int)(sprites[sprite_number].timer / 8)) {
				if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels - 8 + ((float)((rand() % 320) - 160.00) / 10.00), 0, 0, 0);
				else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 8 + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 320) - 160.00) / 10.00), 0, 0, 0);
			}

			sprites[sprite_number].timer--;
			return;
		}
	}

	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, 16, 0);
		sprites[sprite_number].frame = SMALL_GUN_TURRET_BUSTED;
		sprites[sprite_number].timer = 64;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the small gun turret and updates the score display
		player.score += 25;
		update_player_stats();

		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].animation_state == 2) sprites[sprite_number].frame = SMALL_GUN_TURRET_REST;
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].animation_state == 2) {
			sprites[sprite_number].animation_state = 0;
			sprites[sprite_number].timer = 16;
		} else if (sprites[sprite_number].animation_state == 1) {
			if (sprites[sprite_number].frame == SMALL_GUN_TURRET_REST) sprites[sprite_number].frame = SMALL_GUN_TURRET_FIRE_01;
			else sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].frame > SMALL_GUN_TURRET_FIRE_01 + 2) {
				sprites[sprite_number].frame = SMALL_GUN_TURRET_REST;
				sprites[sprite_number].animation_state = 0;
				sprites[sprite_number].timer = (rand() % 256) + 128;
			}
		} else {
			sprites[sprite_number].animation_state = 1, sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels + 29, sprites[sprite_number].y_position_in_pixels + 2, 1, 0, 0);
			else spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels - 9, sprites[sprite_number].y_position_in_pixels + 2, -1, 0, 1);
		}
	}
}

/**************************\
|** MORTAR GUN TURRET AI **|
\**************************/
void mortar_gun_turret_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			return;
		} else {
			if (sprites[sprite_number].timer / 8.0 == (int)(sprites[sprite_number].timer / 8)) {
				if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 8 + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 320) - 160.00) / 10.00), 0, 0, 0);
				else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 16 + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels + 8 + ((float)((rand() % 320) - 160.00) / 10.00), 0, 0, 0);
			}

			sprites[sprite_number].timer--;
			return;
		}
	}

	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 16, 16, 0);
		sprites[sprite_number].frame = MORTAR_GUN_TURRET_BUSTED;
		sprites[sprite_number].timer = 128;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the mortar turret and updates the score display
		player.score += 50;
		update_player_stats();

		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].animation_state == 2) sprites[sprite_number].frame = MORTAR_GUN_TURRET_REST;
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].animation_state == 2) {
			sprites[sprite_number].animation_state = 0;
			sprites[sprite_number].timer = 16;
		} else if (sprites[sprite_number].animation_state == 1) {
			if (sprites[sprite_number].frame == MORTAR_GUN_TURRET_REST) sprites[sprite_number].frame = MORTAR_GUN_TURRET_FIRE_01;
			else sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 16;
			if (sprites[sprite_number].frame == MORTAR_GUN_TURRET_FIRE_03) {
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_MORTAR_SPRITE, sprites[sprite_number].x_position_in_pixels + 37, sprites[sprite_number].y_position_in_pixels - 7, 1, -1, 0);
				else spawn_sprite(ENEMY_MORTAR_SPRITE, sprites[sprite_number].x_position_in_pixels - 1, sprites[sprite_number].y_position_in_pixels - 7, -1, -1, 1);
			}
			if (sprites[sprite_number].frame > MORTAR_GUN_TURRET_FIRE_01 + 5) {
				sprites[sprite_number].frame = MORTAR_GUN_TURRET_REST;
				sprites[sprite_number].animation_state = 0;
				sprites[sprite_number].timer = (rand() % 512) + 128;
			}
		} else {
			sprites[sprite_number].animation_state = 1, sprites[sprite_number].timer = 16;
		}
	}
}

/***********************\
|** BIG GUN TURRET AI **|
\***********************/
void big_gun_turret_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			return;
		} else {
			if (sprites[sprite_number].timer / 8.0 == (int)(sprites[sprite_number].timer / 8)) {
				if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 8 + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 320) - 160.00) / 10.00), 0, 0, 0);
				else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 16 + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels + 8 + ((float)((rand() % 320) - 160.00) / 10.00), 0, 0, 0);
			}

			sprites[sprite_number].timer--;
			return;
		}
	}

	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 16, 16, 0);
		sprites[sprite_number].frame = BIG_GUN_TURRET_BUSTED;
		sprites[sprite_number].timer = 128;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the bug gun turret and updates the score display
		player.score += 75;
		update_player_stats();

		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].animation_state == 2) sprites[sprite_number].frame = BIG_GUN_TURRET_REST;
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].animation_state == 2) {
			sprites[sprite_number].animation_state = 0;
			sprites[sprite_number].timer = 16;
		} else if (sprites[sprite_number].animation_state == 1) {
			if (sprites[sprite_number].frame == BIG_GUN_TURRET_REST) sprites[sprite_number].frame = BIG_GUN_TURRET_FIRE_01;
			else sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].frame > BIG_GUN_TURRET_FIRE_01 + 2) {
				sprites[sprite_number].frame = BIG_GUN_TURRET_REST;
				sprites[sprite_number].animation_state = 0;
				sprites[sprite_number].timer = (rand() % 256) + 128;
			}
		} else {
			sprites[sprite_number].animation_state = 1, sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].direction == 0) spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels + 45, sprites[sprite_number].y_position_in_pixels + 2, 1, 0, 0);
			else spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels - 29, sprites[sprite_number].y_position_in_pixels + 2, -1, 0, 1);
		}
	}
}

/***************************\
|** FLYING PROBE ENEMY AI **|
\***************************/
void flying_probe_enemy_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	if (sprites[sprite_number].hit_points_left == -666) {
		// Rotate the sprite
		if (sprites[sprite_number].rotate_factor == 254) sprites[sprite_number].rotate_factor = 0;
		else sprites[sprite_number].rotate_factor += 2;

		//  Move the sprite
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
		sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

		// Apply gravity to the sprite
		sprites[sprite_number].y_acceleration += .01;
		if (sprites[sprite_number].y_acceleration > 1) sprites[sprite_number].y_acceleration = 1;

		// Test for a collision with the player
		if (sprite_is_touching_player(sprite_number) == TRUE) sprite_busted = TRUE;

		// Test for a collision with a wall or something
		else if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 12, sprites[sprite_number].y_position_in_pixels + 12, 0)) sprite_busted = TRUE;

		// Blow up the probe
		if (sprite_busted == TRUE) {
			spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 4, sprites[sprite_number].y_position_in_pixels - 4, 0, 0, 0);
			spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 12, sprites[sprite_number].y_position_in_pixels + 12, 8, 0);
			for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 12, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			destroy_sprite(sprite_number);
			return;
		}
		return;
	}

	// This triggers the death animation
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 12, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		if (player.x_position_in_pixels + 16 <= sprites[sprite_number].x_position_in_pixels + 12) sprites[sprite_number].x_acceleration = (float)(rand() % 10) / 10.00 + .25;
		else sprites[sprite_number].x_acceleration = -(float)(rand() % 10) / 10.00 - .25;
		sprites[sprite_number].y_acceleration = -.25;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the flying probe and updates the score display
		player.score += 15;
		update_player_stats();

		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].frame == FLYING_PROBE_FIRE) {
			sprites[sprite_number].frame = FLYING_PROBE_FLY;
			sprites[sprite_number].timer = (rand() % 512) + 256;
		} else {
			sprites[sprite_number].frame = FLYING_PROBE_FIRE, sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels + 23, sprites[sprite_number].y_position_in_pixels + 15, 1, 0, 0);
			else spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels - 7, sprites[sprite_number].y_position_in_pixels + 15, -1, 0, 1);
		}
	}

	// Damage the player on contact
	if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

	// Check turning arrows and turn the sprite if nessicary
	check_sprite_turning_arrows(sprite_number, 0, .5, .5);

	// Move the probe
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
}

/************************\
|** DECIMATER ENEMY AI **|
\************************/
void decimater_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == DECIMATER_DIE_20) {
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


	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 0);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = DECIMATER_DIE_01;

		// This gives the player some points for killing the decimater and updates the score display
		player.score += 75;
		update_player_stats();

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// Shooting state
	if (sprites[sprite_number].action_state > 0) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == DECIMATER_FIRE_03) {
				sprites[sprite_number].frame = DECIMATER_WALK_01;
				if (sprites[sprite_number].action_state == 3) sprites[sprite_number].action_state = 0;
				else sprites[sprite_number].action_state++;
			} else if (sprites[sprite_number].frame == DECIMATER_FIRE_02) {
				sprites[sprite_number].frame = DECIMATER_FIRE_03;
			} else if (sprites[sprite_number].frame == DECIMATER_FIRE_01) {
				sprites[sprite_number].frame = DECIMATER_FIRE_02;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_TOXIN_SPRITE, sprites[sprite_number].x_position_in_pixels + 30, sprites[sprite_number].y_position_in_pixels + 18, 1.5, 0, 0);
				else spawn_sprite(ENEMY_TOXIN_SPRITE, sprites[sprite_number].x_position_in_pixels + 3, sprites[sprite_number].y_position_in_pixels + 18, -1.5, 0, 1);
			} else {
				sprites[sprite_number].frame = DECIMATER_FIRE_01;
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > DECIMATER_WALK_06) sprites[sprite_number].frame = DECIMATER_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with toxin, turn around and shoot
		if (rand() % 64 == 32) {
			if (player.y_position_in_pixels + 48 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels - 48 < sprites[sprite_number].y_position_in_pixels + 48) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 48) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 512 == 256) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.75;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .75;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/**************************\
|** GIANT ROBOT ENEMY AI **|
\**************************/
void giant_robot_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;
	int sprite_starter;

	// This is the actual death animation script
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer / 8.0 == (int)(sprites[sprite_number].timer / 8)) {
			if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 16 + ((float)((rand() % 640) - 320.00) / 10.00), sprites[sprite_number].y_position_in_pixels + 15 + ((float)((rand() % 640) - 320.00) / 10.00), 0, 0, 0);
			else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 24 + ((float)((rand() % 640) - 320.00) / 10.00), sprites[sprite_number].y_position_in_pixels + 24 + ((float)((rand() % 640) - 320.00) / 10.00), 0, 0, 0);
		}

		if (sprites[sprite_number].timer == 0) {
			destroy_sprite(sprite_number);
			return;
		}

		// This is the actual explosion part
		if (sprites[sprite_number].timer == 128) {
			sprites[sprite_number].frame = 0;

			// This makes the shrapenel buffbott shrapnel
			sprite_starter = number_of_sprites;
			spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 32, sprites[sprite_number].y_position_in_pixels + 32, 32, 0);
			if (sprite_starter < 1024) sprites[sprite_starter].frame = GIANT_ROBOT_SHRAPNEL_01;
			if (sprite_starter + 1 < 1024) sprites[sprite_starter + 1].frame = GIANT_ROBOT_SHRAPNEL_02;
			if (sprite_starter + 2 < 1024) sprites[sprite_starter + 2].frame = GIANT_ROBOT_SHRAPNEL_03;
			if (sprite_starter + 3 < 1024) sprites[sprite_starter + 3].frame = GIANT_ROBOT_SHRAPNEL_03;
			if (sprite_starter + 4 < 1024) sprites[sprite_starter + 4].frame = GIANT_ROBOT_SHRAPNEL_04;
			if (sprite_starter + 5 < 1024) sprites[sprite_starter + 5].frame = GIANT_ROBOT_SHRAPNEL_04;
			if (sprite_starter + 6 < 1024) sprites[sprite_starter + 6].frame = GIANT_ROBOT_SHRAPNEL_05;
			if (sprite_starter + 7 < 1024) sprites[sprite_starter + 7].frame = GIANT_ROBOT_SHRAPNEL_06;
			if (sprite_starter + 8 < 1024) sprites[sprite_starter + 8].frame = GIANT_ROBOT_SHRAPNEL_07;

			// Adjust the shrapnel speed so that they last longer
			for (spawn_loop = 0; spawn_loop < 9; spawn_loop++) if (sprite_starter + spawn_loop < 1024) sprites[spawn_loop + sprite_starter].y_acceleration -= .75;
		}

		sprites[sprite_number].timer--;
		return;
	}

	// This triggers the death animation
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 64; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 32, sprites[sprite_number].y_position_in_pixels + 32, 16, 0);
		sprites[sprite_number].frame = GIANT_ROBOT_DAMAGED;
		sprites[sprite_number].timer = 256;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the giant robot and updates the score display
		player.score += 250;
		update_player_stats();

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == GIANT_ROBOT_FIRE_02) {
				sprites[sprite_number].frame = GIANT_ROBOT_WALK_03;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame == GIANT_ROBOT_FIRE_01) {
				sprites[sprite_number].frame = GIANT_ROBOT_FIRE_02;
				if (sprites[sprite_number].direction == 0) {
					spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels + 30, sprites[sprite_number].y_position_in_pixels + 30, .25, -.15, 0);
					spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels + 30, sprites[sprite_number].y_position_in_pixels + 30, .25, 0, 0);
					spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels + 30, sprites[sprite_number].y_position_in_pixels + 30, .25, .15, 0);
				} else {
					spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels - 30, sprites[sprite_number].y_position_in_pixels + 30, .25, -.15, 1);
					spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels - 30, sprites[sprite_number].y_position_in_pixels + 30, .25, 0, 1);
					spawn_sprite(LARGE_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels - 30, sprites[sprite_number].y_position_in_pixels + 30, .25, .15, 1);
				}
			} else {
				sprites[sprite_number].frame = GIANT_ROBOT_FIRE_01;
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].animation_state == 1) sprites[sprite_number].frame++;
			else sprites[sprite_number].frame--;
			if (sprites[sprite_number].frame == GIANT_ROBOT_WALK_01) sprites[sprite_number].animation_state = 1;
			else if (sprites[sprite_number].frame == GIANT_ROBOT_WALK_05) sprites[sprite_number].animation_state = 0;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with rockets, turn around and shoot
		if (rand() % 128 == 64) {
			if (player.y_position_in_pixels + 96 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels - 64 < sprites[sprite_number].y_position_in_pixels + 64) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 1024 == 512) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.25;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .25;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-5);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/*********************************\
|** MECHANICAL CRUSHER ENEMY AI **|
\*********************************/
void mechanical_crusher_enemy_ai(int sprite_number)
{
	// The mechanical crusher is immortal, so max out it's health
	sprites[sprite_number].hit_points_left = 666;

	// Moving up
	if (sprites[sprite_number].animation_state == 0) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 1;
			sprites[sprite_number].timer = 8 + (rand() % 24);
		} else {
			sprites[sprite_number].y_position_in_pixels -= .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].animation_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 2;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// Slamming down
	else if (sprites[sprite_number].animation_state == 2) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 3;
			sprites[sprite_number].timer = 8 + (rand() & 24);
		} else {
			sprites[sprite_number].y_position_in_pixels += 4;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the bottom
	else if (sprites[sprite_number].animation_state == 3) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 0;
			sprites[sprite_number].timer = 256;
		} else {
			sprites[sprite_number].timer--;
		}
	}

	// This is the damage control stuff
	if (sprite_is_touching_player(sprite_number) == TRUE) {
		// See if the player was crushed
		if (player.y_position_in_pixels > sprites[sprite_number].y_position_in_pixels + 90) {
			if (sprites[sprite_number].animation_state == 2) {
				if (player.timer == 0) hurt_player(-5);
			} else if (player.y_acceleration < 0) {
				player.y_acceleration = 0;
				if (player.timer == 0) hurt_player(-5);
			}
		} else {
			if (player.x_position_in_pixels + 32 >= sprites[sprite_number].x_position_in_pixels && player.x_position_in_pixels + 32 <= sprites[sprite_number].x_position_in_pixels + 2) {
				player.x_position_in_pixels = (sprites[sprite_number].x_position_in_pixels - 31);
				player.x_acceleration = 0;
			} else if (player.x_position_in_pixels >= sprites[sprite_number].x_position_in_pixels + 29 && player.x_position_in_pixels <= sprites[sprite_number].x_position_in_pixels + 31) {
				player.x_position_in_pixels = (sprites[sprite_number].x_position_in_pixels + 31);
				player.x_acceleration = 0;
			} else {
				push_player(0, 1);
				player.y_acceleration = 0;
			}
		}
	}
}

/***************************\
|** NUCLEAR WORM ENEMY AI **|
\***************************/
void nuclear_worm_enemy_ai(int sprite_number)
{
	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// This makes the worm explode when it should
	if (sprite_is_touching_player(sprite_number) == TRUE || sprites[sprite_number].hit_points_left <= 0) {
		spawn_sprite(MUSHROOM_CLOUD_SPRITE, sprites[sprite_number].x_position_in_pixels - 52, sprites[sprite_number].y_position_in_pixels - 96, 0, 0, 0);
		destroy_sprite(sprite_number);

		// This gives the player some points for killing the nuclear worm and updates the score display
		player.score += 50;
		update_player_stats();

		return;
	}

	// This is the ground action state used for the basic action, commonly walking
	if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == NUCLEAR_WORM_WALK_01) sprites[sprite_number].frame = NUCLEAR_WORM_WALK_02;
			else sprites[sprite_number].frame = NUCLEAR_WORM_WALK_01;
			sprites[sprite_number].timer = 24;
		} else {
			sprites[sprite_number].timer--;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		// if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/*******************************\
|** LASER REBEL WORM ENEMY AI **|
\*******************************/
void laser_rebel_worm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == LASER_REBEL_WORM_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(LASER_REBEL_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 8);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = LASER_REBEL_WORM_DIE_01;

		// This gives the player some points for killing the laser rebel worm and updates the score display
		player.score += 15;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 4 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// Shooting state
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == LASER_REBEL_WORM_FIRE_03) {
				sprites[sprite_number].frame = LASER_REBEL_WORM_WALK_02;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame == LASER_REBEL_WORM_FIRE_02) {
				sprites[sprite_number].frame = LASER_REBEL_WORM_FIRE_03;
			} else if (sprites[sprite_number].frame == LASER_REBEL_WORM_FIRE_01) {
				sprites[sprite_number].frame = LASER_REBEL_WORM_FIRE_02;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels + 31, sprites[sprite_number].y_position_in_pixels + 12, 1, 0, 0);
				else spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels - 7, sprites[sprite_number].y_position_in_pixels + 12, -1, 0, 1);
			} else {
				sprites[sprite_number].frame = LASER_REBEL_WORM_FIRE_01;
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > LASER_REBEL_WORM_WALK_04) sprites[sprite_number].frame = LASER_REBEL_WORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with some bullets, turn around and shoot
		if (rand() % 64 == 32) {
			if (player.y_position_in_pixels + 30 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels + 2 < sprites[sprite_number].y_position_in_pixels + 32) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
				return;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 1024 == 512) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.75;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .75;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/********************************\
|** ROCKET REBEL WORM ENEMY AI **|
\********************************/
void rocket_rebel_worm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == ROCKET_REBEL_WORM_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(ROCKET_REBEL_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 8);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = ROCKET_REBEL_WORM_DIE_01;

		// This gives the player some points for killing the rocket rebel worm and updates the score display
		player.score += 15;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 3 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// Shooting state
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == ROCKET_REBEL_WORM_FIRE_03) {
				sprites[sprite_number].frame = ROCKET_REBEL_WORM_WALK_02;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame == ROCKET_REBEL_WORM_FIRE_02) {
				sprites[sprite_number].frame = ROCKET_REBEL_WORM_FIRE_03;
			} else if (sprites[sprite_number].frame == ROCKET_REBEL_WORM_FIRE_01) {
				sprites[sprite_number].frame = ROCKET_REBEL_WORM_FIRE_02;
				if (sprites[sprite_number].direction == 0) spawn_sprite(SMALL_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 12, 1.5, 0, 0);
				else spawn_sprite(SMALL_ENEMY_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels - 8, sprites[sprite_number].y_position_in_pixels + 12, -1.5, 0, 1);
			} else {
				sprites[sprite_number].frame = ROCKET_REBEL_WORM_FIRE_01;
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > ROCKET_REBEL_WORM_WALK_04) sprites[sprite_number].frame = ROCKET_REBEL_WORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with some rockets, turn around and shoot
		if (rand() % 64 == 32) {
			if (player.y_position_in_pixels + 30 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels + 2 < sprites[sprite_number].y_position_in_pixels + 32) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
				return;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 1024 == 512) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.75;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .75;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/*******************************\
|** RIFLE REBEL WORM ENEMY AI **|
\*******************************/
void rifle_rebel_worm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == RIFLE_REBEL_WORM_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(RIFLE_REBEL_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 8);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = RIFLE_REBEL_WORM_DIE_01;

		// This gives the player some points for killing the laser rebel worm and updates the score display
		player.score += 15;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 4 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// Shooting state
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == RIFLE_REBEL_WORM_FIRE_03) {
				sprites[sprite_number].frame = RIFLE_REBEL_WORM_WALK_02;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame == RIFLE_REBEL_WORM_FIRE_02) {
				sprites[sprite_number].frame = RIFLE_REBEL_WORM_FIRE_03;
			} else if (sprites[sprite_number].frame == RIFLE_REBEL_WORM_FIRE_01) {
				sprites[sprite_number].frame = RIFLE_REBEL_WORM_FIRE_02;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_PLASMA_BULLET_SPRITE, sprites[sprite_number].x_position_in_pixels + 28, sprites[sprite_number].y_position_in_pixels + 13, 2, 0, 0);
				else spawn_sprite(ENEMY_PLASMA_BULLET_SPRITE, sprites[sprite_number].x_position_in_pixels - 4, sprites[sprite_number].y_position_in_pixels + 13, -2, 0, 1);
			} else {
				sprites[sprite_number].frame = RIFLE_REBEL_WORM_FIRE_01;
			}
			sprites[sprite_number].timer = 8;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > RIFLE_REBEL_WORM_WALK_04) sprites[sprite_number].frame = RIFLE_REBEL_WORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with some bullets, turn around and shoot
		if (rand() % 8 == 4) {
			if (player.y_position_in_pixels + 30 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels + 2 < sprites[sprite_number].y_position_in_pixels + 32) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
				return;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 1024 == 512) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.75;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .75;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/***********************************\
|** BIG GREEN SLIME BLOB ENEMY AI **|
\***********************************/
void big_green_slime_blob_enemy_ai(int sprite_number)
{
	// This prevents the slime blob from ever being killed
	sprites[sprite_number].hit_points_left = 666;

	// This is the ground action state used for the basic action, commonly walking
	if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > BIG_GREEN_SLIME_BLOB_08) sprites[sprite_number].frame = BIG_GREEN_SLIME_BLOB_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-10);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/**********************\
|** GUMDROP ENEMY AI **|
\**********************/
void gumdrop_enemy_ai(int sprite_number)
{
	int sprite_starter;
	unsigned char spawn_loop;

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0) {
		// This makes the shrapenel buffbott shrapnel
		sprite_starter = number_of_sprites;
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, 32, 0);
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) if (sprite_starter + spawn_loop < 1024) sprites[sprite_starter + spawn_loop].frame = GUMDROP_GIB_01;
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) if (sprite_starter + spawn_loop + 16 < 1024) sprites[sprite_starter + spawn_loop + 16].frame = GUMDROP_GIB_02;
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		destroy_sprite(sprite_number);

		// This gives the player some points for killing the gumdrop and updates the score display
		player.score += 2;
		update_player_stats();

		return;
	}

	// This is the ground action state used for the basic action, commonly walking
	if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > GUMDROP_MOVE_08) sprites[sprite_number].frame = GUMDROP_MOVE_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/***************************\
|** NUCLEAR BOMB ENEMY AI **|
\***************************/
void nuclear_bomb_enemy_ai(int sprite_number)
{
	// Accelelerate and move the bomb
	sprites[sprite_number].y_acceleration += .02;
	if (sprites[sprite_number].y_acceleration > 2) sprites[sprite_number].y_acceleration = 2;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// This is what triggers the explosion
	if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 0) == TRUE) {
		spawn_sprite(MUSHROOM_CLOUD_SPRITE, sprites[sprite_number].x_position_in_pixels - 64 + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels - 128 + sprites[sprite_number].height, 0, 0, 0);
		destroy_sprite(sprite_number);
		return;
	}
}

/******************************\
|** FIREWORM SEMENT ENEMY AI **|
\******************************/
void fireworm_segment_enemy_ai(int sprite_number)
{
	// Animate the enemy's ground action state
	if (sprites[sprite_number].timer == 0) {
		sprites[sprite_number].frame++;
		if (sprites[sprite_number].frame > SMALL_FLAME_BURN_04) sprites[sprite_number].frame = SMALL_FLAME_BURN_01;
		sprites[sprite_number].timer = 8;
	} else {
		sprites[sprite_number].timer--;
	}

	// Damage the player on contact
	if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-2);

	// Check turning arrows and turn the sprite if nessicary
	check_sprite_turning_arrows(sprite_number, 0, .5, .5);

	// Move the fireworm segment
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
}

/************************\
|** FLAME JET ENEMY AI **|
\************************/
void flame_jet_enemy_ai(int sprite_number)
{
	// If the flame jet is not burning, reduce timer and activate burn if nessicary.
	if (sprites[sprite_number].timer == 0 && sprites[sprite_number].frame == 0) {
		sprites[sprite_number].frame = FLAME_JET_01;
		sprites[sprite_number].timer = 4;
	} else if (sprites[sprite_number].frame == 0) {
		sprites[sprite_number].timer--;
	}

	// Animate the flame jet flame if currently burning
	if (sprites[sprite_number].timer == 0 && sprites[sprite_number].frame != 0) {
		sprites[sprite_number].frame++;
		if (sprites[sprite_number].frame > FLAME_JET_22) {
			sprites[sprite_number].frame = 0;
			sprites[sprite_number].timer = 96 + (rand() % 96);
		} else {
			sprites[sprite_number].timer = 4;
		}
	} else if (sprites[sprite_number].frame != 0) {
		sprites[sprite_number].timer--;
	}

	// Damage the player on contact if in flame jet state
	if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE && sprites[sprite_number].frame > FLAME_JET_04 && sprites[sprite_number].frame < FLAME_JET_19) hurt_player(-4);
}

/**********************\
|** BIG BUG ENEMY AI **|
\**********************/
void big_bug_enemy_ai(int sprite_number)
{
	// Action states:
	// 0:  Walking in normal state
	// 1:  Morphing to ball state
	// 2:  Rolling in ball state
	// 3:  Morphing to normal state
}

/*********************\
|** W-1000 ENEMY AI **|
\*********************/
void w_1000_enemy_ai(int sprite_number)
{
	// Action states:
	// 0:  Resting in statue state
	// 1:  Morphing from statue to normal standing state
	// 2:  Walking in normal state
	// 3:  Jumping in normal state

	// 4:  Melting down - set timer to 8
	// 5:  Moving in melted down state - set timer to 8
	// 6:  Reforming from melted down state - set timer to 8

	// 7:  Morphing a sword
	// 8:  Attacking with sword
	// 9:  Walking with sword
	// 10:  Jumping with sword
	// 11:  Morphing from sword to normal state

	// 12:  Morphing an axe
	// 13:  Attacking with an axe
	// 14:  Walking with an axe
	// 15:  Jumping with an axe
	// 16:  Morphing from axe to normal state
}

/********************\
|** WORM CANNON AI **|
\********************/
void worm_cannon_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			destroy_sprite(sprite_number);
			return;
		} else {
			if (sprites[sprite_number].timer / 4.0 == (int)(sprites[sprite_number].timer / 4)) {
				if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 32 + ((float)((rand() % 1920) - 960.00) / 20.00), sprites[sprite_number].y_position_in_pixels - 8 + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
				else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 40 + ((float)((rand() % 1920) - 960.00) / 20.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
			}

			sprites[sprite_number].timer--;
			return;
		}
	}

	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, 16, 0);
		sprites[sprite_number].frame = WORM_CANNON_BUSTED;
		sprites[sprite_number].timer = 512;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the worm cannon and updates the score display
		player.score += 100;
		update_player_stats();

		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].animation_state == 2) {
			if (sprites[sprite_number].frame == WORM_CANNON_REST) sprites[sprite_number].frame = WORM_CANNON_FIRE_01;
			else sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].frame > WORM_CANNON_FIRE_04) {
				sprites[sprite_number].frame = WORM_CANNON_REST;
				sprites[sprite_number].animation_state = 0;
				sprites[sprite_number].timer = (rand() % 256) + 256;
			}
		} else if (sprites[sprite_number].animation_state == 1) {
			if (sprites[sprite_number].frame == WORM_CANNON_REST) sprites[sprite_number].frame = WORM_CANNON_SUCK_01;
			else sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 16;
			if (sprites[sprite_number].frame > WORM_CANNON_SUCK_16) {
				sprites[sprite_number].frame = WORM_CANNON_REST;
				sprites[sprite_number].animation_state = 2, sprites[sprite_number].timer = 8;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ARAB_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels + 64, sprites[sprite_number].y_position_in_pixels + 8, 1, -.25, 0);
				else spawn_sprite(ARAB_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, -1, -.25, 1);
			}
		} else {
			sprites[sprite_number].animation_state = 1, sprites[sprite_number].timer = 16;
		}
	}
}

/************************\
|** ARAB WORM ENEMY AI **|
\************************/
void arab_worm_enemy_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.health_bars_left != 0) sprite_busted = TRUE;

	// This is some basic animation code -- IMPROVE IT ASAP
	if (sprites[sprite_number].frame == ARAB_WORM_FLY_01) sprites[sprite_number].frame = ARAB_WORM_FLY_04;
	else sprites[sprite_number].frame = ARAB_WORM_FLY_01;

	// Move the arab worm right or left and up or down
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Apply gravity to the arab worm
	sprites[sprite_number].y_acceleration += .015;

	// See if there are solid tiles in the way of the arab worm and adjust the rocket's speed
	if (sprites[sprite_number].direction == 0) {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		sprites[sprite_number].x_acceleration += .025;
		if (sprites[sprite_number].x_acceleration > 3) sprites[sprite_number].x_acceleration = 3;
	} else {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		sprites[sprite_number].x_acceleration -= .025;
		if (sprites[sprite_number].x_acceleration < -3) sprites[sprite_number].x_acceleration = -3;
	}

	// Stardust smoke trail
	if (out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 255, 240, 4, 1);

	// If the arab worm hit anything at all, then destroy it
	if (sprite_busted == TRUE) {
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 8, 16, 16);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 3, 0, 0, 0);
		spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 32, sprites[sprite_number].y_position_in_pixels - 3, 0, 0, 0);
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/**********************\
|** WERMAID ENEMY AI **|
\**********************/
void wermaid_enemy_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	if (sprites[sprite_number].hit_points_left == -666) {
		// Rotate the sprite
		if (sprites[sprite_number].rotate_factor == 254) sprites[sprite_number].rotate_factor = 0;
		else sprites[sprite_number].rotate_factor += 2;

		//  Move the sprite
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
		sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

		// Apply gravity to the sprite
		sprites[sprite_number].y_acceleration += .01;
		if (sprites[sprite_number].y_acceleration > 1) sprites[sprite_number].y_acceleration = 1;

		// Test for a collision with the player
		//if (sprite_is_touching_player(sprite_number) == TRUE) sprite_busted = TRUE;

		// Test for a collision with a wall or something
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 12, sprites[sprite_number].y_position_in_pixels + 12, 0)) sprite_busted = TRUE;

		// Blow up the wermaid
		if (sprite_busted == TRUE) {
			//spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 4, sprites[sprite_number].y_position_in_pixels - 4, 0, 0, 0);
			spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 48, sprites[sprite_number].y_position_in_pixels + 16, 0, 32);
			for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			destroy_sprite(sprite_number);
			return;
		}
		return;
	}

	// This triggers the death animation
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 16, 0, 16);
		sprites[sprite_number].x_acceleration = (float)((rand() % 20) - 10.00) / 10.00;
		sprites[sprite_number].y_acceleration = -.25;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the wermaid and updates the score display
		player.score += 25;
		update_player_stats();

		return;
	}

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Update animations
	if (sprites[sprite_number].timer <= 0) {
		sprites[sprite_number].frame++;
		if (sprites[sprite_number].frame > WERMAID_SWIM_08) sprites[sprite_number].frame = WERMAID_SWIM_01;
		sprites[sprite_number].timer = 8;
	}

	// Damage the player on contact
	if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

	// Check turning arrows and turn the sprite if nessicary
	check_sprite_turning_arrows(sprite_number, 0, 1, 1);

	// Move the wermaid
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
}

/*************************\
|** BLUE SLIME ENEMY AI **|
\*************************/
void blue_slime_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		// Let the blob fall if nessicary
		if (sprites[sprite_number].action_state == 1) {
			if (sprites[sprite_number].y_acceleration >= -sprites[sprite_number].animation_state && sprites[sprite_number].y_acceleration <= sprites[sprite_number].animation_state) sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
			else if (sprites[sprite_number].y_acceleration < -sprites[sprite_number].animation_state) sprites[sprite_number].y_position_in_pixels -= sprites[sprite_number].animation_state;
			else sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].animation_state;
			sprites[sprite_number].y_acceleration += 0.02;
			if (sprites[sprite_number].y_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 0) == TRUE) sprites[sprite_number].action_state = 0;
		}

		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == BLUE_SLIME_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(BLUE_SLIME_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels + 4, sprites[sprite_number].y_position_in_pixels + 4, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 24;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_BLUE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_stardust(sprites[sprite_number].x_position_in_pixels + 12, sprites[sprite_number].y_position_in_pixels + 12, 0, 0, 16, 24, 4, 64);
		sprites[sprite_number].frame = BLUE_SLIME_DIE_01;
		sprites[sprite_number].timer = 24;
		sprites[sprite_number].hit_points_left = -666;

		// This gives the player some points for killing the blue slime and updates the score display
		player.score += 15;
		update_player_stats();

		return;
	}

	// Hurt the player on contact
	if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-2);

	// Update the timer and change animation state if time is up
	sprites[sprite_number].timer--;

	// Action states - 0 rest, 2 jumping
	// Update animations
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].action_state == 0) {
			sprites[sprite_number].frame = BLUE_SLIME_JUMP_01;
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 8;
			sprites[sprite_number].y_acceleration = -2.5;
		} else if (sprites[sprite_number].action_state == 1) {
			sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].frame > BLUE_SLIME_JUMP_09)
				sprites[sprite_number].frame = BLUE_SLIME_JUMP_09;
		}
	}

	// Update action states
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].y_acceleration >= -sprites[sprite_number].animation_state && sprites[sprite_number].y_acceleration <= sprites[sprite_number].animation_state) sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
		else if (sprites[sprite_number].y_acceleration < -sprites[sprite_number].animation_state) sprites[sprite_number].y_position_in_pixels -= sprites[sprite_number].animation_state;
		else sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].animation_state;
		sprites[sprite_number].y_acceleration += 0.02;
		if (sprites[sprite_number].y_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 0) == TRUE) {
			sprites[sprite_number].frame = BLUE_SLIME_REST;
			sprites[sprite_number].action_state = 0;
			sprites[sprite_number].timer = 256;
		}
	}
}

/*******************\
|** BOSS ENEMY AI **|
\*******************/
void boss_enemy_ai(int sprite_number)
{
	//unsigned char spawn_loop;

	// This is the death animation code
	/*if (sprites[sprite_number].hit_points_left == -666) {
	 *      if (sprites[sprite_number].timer == 0) {
	 *              if (sprites[sprite_number].frame == DECIMATER_DIE_20) return;
	 *              else {
	 *                      sprites[sprite_number].timer = 16;
	 *                      sprites[sprite_number].frame++;
	 *                      }
	 *              }
	 *      else sprites[sprite_number].timer--;
	 *      return;
	 *      }*/


	// This is what triggers the death scene
	/*if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
	 *      for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
	 *      spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 0);
	 *      sprites[sprite_number].timer = 16;
	 *      sprites[sprite_number].hit_points_left = -666;
	 *      sprites[sprite_number].frame = DECIMATER_DIE_01;
	 *
	 *      // This gives the player some points for killing the decimater and updates the score display
	 *      player.score += 75;
	 *      update_player_stats();
	 *
	 *      return;
	 *      }*/

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// states:  0 = walking, 1 = phase out, 2 = invisible, 3 = phase in, 4 = shooting, 5 = hacking

	// Shooting state
	/*if (sprites[sprite_number].action_state > 0) {
	 *      if (sprites[sprite_number].timer == 0) {
	 *              if (sprites[sprite_number].frame == DECIMATER_FIRE_03) {
	 *                      sprites[sprite_number].frame = DECIMATER_WALK_01;
	 *                      if (sprites[sprite_number].action_state == 3) sprites[sprite_number].action_state = 0;
	 *                      else sprites[sprite_number].action_state++;
	 *                      }
	 *              else if (sprites[sprite_number].frame == DECIMATER_FIRE_02) sprites[sprite_number].frame = DECIMATER_FIRE_03;
	 *              else if (sprites[sprite_number].frame == DECIMATER_FIRE_01) {
	 *                      sprites[sprite_number].frame = DECIMATER_FIRE_02;
	 *                      if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_TOXIN_SPRITE, sprites[sprite_number].x_position_in_pixels + 30, sprites[sprite_number].y_position_in_pixels + 18, 1.5, 0, 0);
	 *                      else spawn_sprite(ENEMY_TOXIN_SPRITE, sprites[sprite_number].x_position_in_pixels + 3, sprites[sprite_number].y_position_in_pixels + 18, -1.5, 0, 1);
	 *                      }
	 *              else sprites[sprite_number].frame = DECIMATER_FIRE_01;
	 *              sprites[sprite_number].timer = 16;
	 *              }
	 *      else sprites[sprite_number].timer--;
	 *      }*/

	// This is the phasing in action state
	if (sprites[sprite_number].action_state == 3) {
		// Animate the enemy's phase in state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame--;
			if (sprites[sprite_number].frame < BOSS_PHASE_01) {
				sprites[sprite_number].frame = BOSS_WALK_01;
				sprites[sprite_number].action_state = 0;
				sprites[sprite_number].timer = 16;
			}
			sprites[sprite_number].timer = 24;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the invisible state
	else if (sprites[sprite_number].action_state == 2) {
		sprites[sprite_number].timer--;	// Decrement timer, when it expires, change states
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].frame = BOSS_PHASE_08;
			sprites[sprite_number].timer = 24;
			sprites[sprite_number].action_state = 3;
			sprites[sprite_number].hit_points_left = sprites[sprite_number].rotate_factor;
			sprites[sprite_number].rotate_factor = 0;
			return;
		}

		// Randomly turn around sometimes
		if (rand() % 512 == 256) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.5 - ((rand() % 5) / 10.0);
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .5 + ((rand() % 5) / 10.0);
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
	// This is the phasing out action state
	else if (sprites[sprite_number].action_state == 1) {
		// Animate the enemy's phase out state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > BOSS_PHASE_08) {
				sprites[sprite_number].frame = BOSS_PHASE_08;
				sprites[sprite_number].action_state = 2;
				sprites[sprite_number].timer = 256 + rand() % 256;
				sprites[sprite_number].rotate_factor = sprites[sprite_number].hit_points_left;
				sprites[sprite_number].hit_points_left = -1;
				return;
			}
			sprites[sprite_number].timer = 24;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > BOSS_WALK_06) sprites[sprite_number].frame = BOSS_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with toxin, turn around and shoot
		/*if (rand() % 64 == 32) {
		 *      if (player.y_position_in_pixels + 48 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels - 48 < sprites[sprite_number].y_position_in_pixels + 48) {
		 *              if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 48) sprites[sprite_number].action_state = 1;
		 *              else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
		 *              }
		 *      }*/

		// Randomly turn around sometimes
		if (rand() % 512 == 256) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.5 - ((rand() % 5) / 10.0);
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .5 + ((rand() % 5) / 10.0);
		}

		// Randomly phase shift sometimes
		if (rand() % 1024 == 512) {
			sprites[sprite_number].frame = BOSS_PHASE_01;
			sprites[sprite_number].timer = 24;
			sprites[sprite_number].action_state = 1;
			return;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/**************************************\
|** HEAVY WEAPON REBEL WORM ENEMY AI **|
\**************************************/
void heavy_weapon_rebel_worm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame >= HEAVY_WEAPON_REBEL_WORM_DIE_13) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(HEAVY_WEAPON_REBEL_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 8);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = HEAVY_WEAPON_REBEL_WORM_DIE_01;

		// This gives the player some points for killing the heavy weapon rebel worm and updates the score display
		player.score += 20;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 2 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// Shooting state
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == HEAVY_WEAPON_REBEL_WORM_FIRE_05) {
				sprites[sprite_number].frame = HEAVY_WEAPON_REBEL_WORM_WALK_01;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame == HEAVY_WEAPON_REBEL_WORM_FIRE_01) {
				sprites[sprite_number].frame = HEAVY_WEAPON_REBEL_WORM_FIRE_02;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_MISSILE_SPRITE, sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 12, .85, 0, 0);
				else spawn_sprite(ENEMY_MISSILE_SPRITE, sprites[sprite_number].x_position_in_pixels - 8, sprites[sprite_number].y_position_in_pixels + 12, -.85, 0, 1);
			} else if (sprites[sprite_number].frame >= HEAVY_WEAPON_REBEL_WORM_FIRE_02 && sprites[sprite_number].frame < HEAVY_WEAPON_REBEL_WORM_FIRE_05) {
				sprites[sprite_number].frame++;
			} else {
				sprites[sprite_number].frame = HEAVY_WEAPON_REBEL_WORM_FIRE_01;
				play_relative_sound(MISSILE_LAUNCH_SOUND, sprite_number);
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > HEAVY_WEAPON_REBEL_WORM_WALK_04) sprites[sprite_number].frame = HEAVY_WEAPON_REBEL_WORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with some missiles (or randomly), turn around and shoot
		if (rand() % 256 == 128) {
			if (rand() % 4 == 2 || (player.y_position_in_pixels + 30 > sprites[sprite_number].y_position_in_pixels - 64 && player.y_position_in_pixels + 2 < sprites[sprite_number].y_position_in_pixels + 32 + 64)) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
				return;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 1024 == 512) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.5;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .5;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/************************\
|** THUG WORM ENEMY AI **|
\************************/
void thug_worm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == THUG_WORM_DIE_04) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(THUG_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 8);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = THUG_WORM_DIE_01;

		// This gives the player some points for killing the laser rebel worm and updates the score display
		player.score += 15;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 4 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// Shooting state
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == THUG_WORM_FIRE) {
				sprites[sprite_number].frame = THUG_WORM_WALK_01;
				sprites[sprite_number].action_state = 0;
			} else {
				sprites[sprite_number].frame = THUG_WORM_FIRE;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_PLASMA_BULLET_SPRITE, sprites[sprite_number].x_position_in_pixels + 28, sprites[sprite_number].y_position_in_pixels + 13, 2, 0, 0);
				else spawn_sprite(ENEMY_PLASMA_BULLET_SPRITE, sprites[sprite_number].x_position_in_pixels - 4, sprites[sprite_number].y_position_in_pixels + 13, -2, 0, 1);
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > THUG_WORM_WALK_02) sprites[sprite_number].frame = THUG_WORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with some bullets, turn around and shoot
		if (rand() % 8 == 4) {
			if (player.y_position_in_pixels + 30 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels + 2 < sprites[sprite_number].y_position_in_pixels + 32) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
				return;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 1024 == 512) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.75;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .75;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/*****************************\
|** SPEAR TAPEWORM ENEMY AI **|
\*****************************/
void spear_tapeworm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == SPEAR_TAPEWORM_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(SPEAR_TAPEWORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 8, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 12, 0, 16);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = SPEAR_TAPEWORM_DIE_01;

		// This gives the player some points for killing the tapeworm and updates the score display
		player.score += 8;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 5 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// This is the ground action state used for the basic action, commonly walking
	if (sprites[sprite_number].action_state == 0) {
		// Animate the tapeworm's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > SPEAR_TAPEWORM_WALK_04) sprites[sprite_number].frame = SPEAR_TAPEWORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/***************************\
|** EGG TAPEWORM ENEMY AI **|
\***************************/
void egg_tapeworm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == EGG_TAPEWORM_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(EGG_TAPEWORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 8, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 12, 0, 16);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = EGG_TAPEWORM_DIE_01;

		// This gives the player some points for killing the tapeworm and updates the score display
		player.score += 12;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 4 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// Shooting state
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == EGG_TAPEWORM_FIRE_04) {
				sprites[sprite_number].frame = EGG_TAPEWORM_WALK_01;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame == EGG_TAPEWORM_FIRE_01) {
				sprites[sprite_number].frame = EGG_TAPEWORM_FIRE_02;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_EGG_SPRITE, sprites[sprite_number].x_position_in_pixels + 24, sprites[sprite_number].y_position_in_pixels + 6, .65, .05, 0);
				else spawn_sprite(ENEMY_EGG_SPRITE, sprites[sprite_number].x_position_in_pixels - 8, sprites[sprite_number].y_position_in_pixels + 6, -.65, .05, 1);
			} else if (sprites[sprite_number].frame >= EGG_TAPEWORM_FIRE_02 && sprites[sprite_number].frame < EGG_TAPEWORM_FIRE_04) {
				sprites[sprite_number].frame++;
			} else {
				sprites[sprite_number].frame = EGG_TAPEWORM_FIRE_01;
				play_relative_sound(EGG_FIRE_SOUND, sprite_number);
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}

	// This is the ground action state used for the basic action, commonly walking
	if (sprites[sprite_number].action_state == 0) {
		// Animate the tapeworm's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > EGG_TAPEWORM_WALK_04) sprites[sprite_number].frame = EGG_TAPEWORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with some eggs (or randomly), turn around and shoot
		if (rand() % 256 == 128) {
			if (rand() % 4 == 2 || (player.y_position_in_pixels + 30 > sprites[sprite_number].y_position_in_pixels - 64 && player.y_position_in_pixels + 2 < sprites[sprite_number].y_position_in_pixels + 32 + 64)) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
				return;
			}
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		//if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}

/**********************\
|** BOULDER ENEMY AI **|
\**********************/
void boulder_enemy_ai(int sprite_number)
{
	// Boulders can't be killed, so set health to max
	//sprites[sprite_number].hit_points_left = 666;

	// Freeze the boulder if its over a trigger
	if (worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels) / 16)][(int)((sprites[sprite_number].y_position_in_pixels) / 16)] >= trigger_1_info_tile && worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels) / 16)][(int)((sprites[sprite_number].y_position_in_pixels) / 16)] <= trigger_16_info_tile) return;

	// This is the ground action state used for the basic action, commonly walking
	if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].x_acceleration != 0 && sprites[sprite_number].direction == 0) {
			if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;
			else sprites[sprite_number].rotate_factor++;
		} else if (sprites[sprite_number].x_acceleration != 0 && sprites[sprite_number].direction == 1) {
			if (sprites[sprite_number].rotate_factor == 0) sprites[sprite_number].rotate_factor = 255;
			else sprites[sprite_number].rotate_factor--;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Hurt the player on contact if this is de-commented
		if (player.frame != 0 && player.timer == 0 && sprite_is_touching_player(sprite_number) == TRUE && sprites[sprite_number].x_acceleration != 0) hurt_player(-3);

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) {
			sprites[sprite_number].y_position_in_pixels--;
			if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
			sprites[sprite_number].y_acceleration -= 10;
			if (sprites[sprite_number].y_acceleration > 0) sprites[sprite_number].y_acceleration = -(sprites[sprite_number].y_acceleration / 1.5) + .02;
			if (sprites[sprite_number].y_acceleration > -.2) sprites[sprite_number].y_acceleration = -.02;
		} else {
			sprites[sprite_number].y_acceleration -= 10;
		}

		// Apply gravity and move the enemy
		sprites[sprite_number].y_acceleration += .02;
		if (sprites[sprite_number].y_acceleration > 2) sprites[sprite_number].y_acceleration = 2;
		sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE && worminator_map.info_tile_grid[(int)(sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2)) / 16][(int)(sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height) / 16] != blank_info_tile) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
			sprites[sprite_number].x_acceleration = 0;
		}
	}
}

/****************************\
|** STOMPING FOOT ENEMY AI **|
\****************************/
void stomping_foot_enemy_ai(int sprite_number)
{
	// The stomping foot is immortal, so max out it's health
	sprites[sprite_number].hit_points_left = 666;

	// Moving up
	if (sprites[sprite_number].animation_state == 0) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 1;
			sprites[sprite_number].timer = 8 + (rand() % 24);
		} else {
			sprites[sprite_number].y_position_in_pixels -= .5;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].animation_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 2;
			sprites[sprite_number].timer = 24;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// Slamming down
	else if (sprites[sprite_number].animation_state == 2) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 3;
			sprites[sprite_number].timer = 256 + (rand() % 256);
		} else {
			sprites[sprite_number].y_position_in_pixels += 8;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the bottom
	else if (sprites[sprite_number].animation_state == 3) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].animation_state = 0;
			sprites[sprite_number].timer = 384;
		} else {
			sprites[sprite_number].timer--;
		}
	}

	// This is the damage control stuff
	if (sprite_is_touching_player(sprite_number) == TRUE && player.x_position_in_pixels >= sprites[sprite_number].x_position_in_pixels && player.x_position_in_pixels <= sprites[sprite_number].x_position_in_pixels + 384 - 32) {
		// See if the player was crushed
		if (player.y_position_in_pixels > sprites[sprite_number].y_position_in_pixels + 592) {
			if (sprites[sprite_number].animation_state == 2) {
				if (player.timer == 0) hurt_player(-4);
			} else if (player.y_acceleration < 0) {
				player.y_acceleration = 0;
				if (player.timer == 0) hurt_player(-4);
			}
		} else {
			if (player.x_position_in_pixels + 32 >= sprites[sprite_number].x_position_in_pixels && player.x_position_in_pixels + 32 <= sprites[sprite_number].x_position_in_pixels + 32 + 2) {
				player.x_position_in_pixels = (sprites[sprite_number].x_position_in_pixels + 32 - 31);
				player.x_acceleration = 0;
			} else if (player.x_position_in_pixels >= sprites[sprite_number].x_position_in_pixels + 381 - 32 && player.x_position_in_pixels <= sprites[sprite_number].x_position_in_pixels + 383 - 32) {
				player.x_position_in_pixels = (sprites[sprite_number].x_position_in_pixels + 383 - 32);
				player.x_acceleration = 0;
			} else {
				push_player(0, 1);
				player.y_acceleration = 0;
			}
		}
	}
}

/***********************\
|** CIGARETTE BUTT AI **|
\***********************/
void cigarette_butt_enemy_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	// Update lifespan timer
	sprites[sprite_number].timer++;
	if (sprites[sprite_number].timer > 512) sprites[sprite_number].timer = 512;

	// Rotate the cig
	sprites[sprite_number].rotate_factor = (sprites[sprite_number].rotate_factor + 4) % 256;

	// Move the cig
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Check to see if the cig hit anything and take appropriate action if so
	/*if (sprite_is_touching_sprite(sprite_number, 1) >= 0) sprite_busted = TRUE;*/

	// Stardust trail
	spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 2, 6, 4, 1);

	// Check x direction
	if (sprites[sprite_number].x_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, 2) == TRUE) sprite_busted = TRUE;
	else if (sprites[sprite_number].x_acceleration < 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, 2) == TRUE) sprite_busted = TRUE;

	// Check y direction
	if (sprites[sprite_number].y_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2) == TRUE) sprite_busted = TRUE;
	else if (sprites[sprite_number].y_acceleration < 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels, 2) == TRUE) sprite_busted = TRUE;

	// Apply gravity to the mortar
	sprites[sprite_number].y_acceleration += .025;

	// If the mortar hit anything damageable, then destyoy it
	if (sprite_busted == TRUE) {
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 3; spawn_loop++) {
			if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 8 + 8 * sprites[sprite_number].x_acceleration + ((float)((rand() % 480) - 240.00) / 20.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 480) - 240.00) / 20.00), 0, 0, 0);
			else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 16 + 8 * sprites[sprite_number].x_acceleration + ((float)((rand() % 480) - 240.00) / 20.00), sprites[sprite_number].y_position_in_pixels + 8 + ((float)((rand() % 480) - 240.00) / 20.00), 0, 0, 0);
		}
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/************************\
|** PUNK WORM ENEMY AI **|
\************************/
void punk_worm_enemy_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// This is the death animation code
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == PUNK_WORM_DIE_08) {
				// Nightmare mode respawn
				if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && out_of_player_range(sprite_number) == TRUE) {
					spawn_sprite(PUNK_WORM_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, sprites[sprite_number].direction);
					destroy_sprite(sprite_number);
					return;
				} else {
					return;
				}
			} else {
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].frame++;
			}
		} else {
			sprites[sprite_number].timer--;
		}
		return;
	}

	// This is what triggers the death scene
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_shrapgibs(sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 12, 16, 8);
		sprites[sprite_number].timer = 16;
		sprites[sprite_number].hit_points_left = -666;
		sprites[sprite_number].frame = PUNK_WORM_DIE_01;

		// This gives the player some points for killing the punk worm and updates the score display
		player.score += 20;
		update_player_stats();

		// Spawn a life essence sometimes
		if (rand() % 3 == 0) spawn_sprite(LIFE_ESSENCE_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 0);

		return;
	}

	// Drop out immediately if the sprite is out of range
	//if (sprites[sprite_number].in_range == FALSE) return;

	// Shooting state
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].frame == PUNK_WORM_FIRE_05) {
				sprites[sprite_number].frame = PUNK_WORM_WALK_01;
				sprites[sprite_number].action_state = 0;
			} else if (sprites[sprite_number].frame >= PUNK_WORM_FIRE_02 && sprites[sprite_number].frame <= PUNK_WORM_FIRE_04) {
				sprites[sprite_number].frame++;
			} else if (sprites[sprite_number].frame == PUNK_WORM_FIRE_01) {
				sprites[sprite_number].frame = PUNK_WORM_FIRE_02;
				if (sprites[sprite_number].direction == 0) spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels + 31, sprites[sprite_number].y_position_in_pixels + 12, 1, 0, 0);
				else spawn_sprite(ENEMY_LASER_BOLT_SPRITE, sprites[sprite_number].x_position_in_pixels - 7, sprites[sprite_number].y_position_in_pixels + 12, -1, 0, 1);
			} else {
				sprites[sprite_number].frame = PUNK_WORM_FIRE_01;
			}
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// This is the ground action state used for the basic action, commonly walking
	else if (sprites[sprite_number].action_state == 0) {
		// Animate the enemy's ground action state
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > PUNK_WORM_WALK_02) sprites[sprite_number].frame = PUNK_WORM_WALK_01;
			sprites[sprite_number].timer = 16;
		} else {
			sprites[sprite_number].timer--;
		}

		// If there is a good chance of hitting the player with some bullets, turn around and shoot
		if (rand() % 64 == 32) {
			if (player.y_position_in_pixels + 30 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels + 2 < sprites[sprite_number].y_position_in_pixels + 32) {
				if (sprites[sprite_number].direction == 0 && player.x_position_in_pixels > sprites[sprite_number].x_position_in_pixels + 64) sprites[sprite_number].action_state = 1;
				else if (sprites[sprite_number].direction == 1 && player.x_position_in_pixels + 32 < sprites[sprite_number].x_position_in_pixels) sprites[sprite_number].action_state = 1;
				return;
			}
		}

		// Randomly turn around sometimes
		if (rand() % 1024 == 512) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1, sprites[sprite_number].x_acceleration = -.75;
			else sprites[sprite_number].direction = 0, sprites[sprite_number].x_acceleration = .75;
		}

		////////// Physics code for the enemy ///////////////////////////////////////////////////////////

		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the enemy is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the enemy
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the enemy if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}
}
