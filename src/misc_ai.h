/**************************\
|** FLOATING PLATFORM AI **|
\**************************/
void floating_platform_ai(int sprite_number)
{
	char done_looping = FALSE;
	char blocked = FALSE;
	char exponent_loop;
	int tile_hit;
	int blocked_test;
	int sx = sprites[sprite_number].x_position_in_pixels;
	int sy = sprites[sprite_number].y_position_in_pixels;
	int delay = 8;

	// If the platform is waiting, then return
	if (sprites[sprite_number].timer > -16) sprites[sprite_number].timer--;

	// Check directional arrows and reset the direction in case it was changed
	if (sprites[sprite_number].timer <= 0) {
		check_sprite_turning_arrows(sprite_number, 0, .5, .5);
		sprites[sprite_number].direction = 0;
	}

	// Check for pause info tiles
	if (sprites[sprite_number].timer == -16) {
		tile_hit = worminator_map.info_tile_grid[(int)(sx / 16)][(int)(sy / 16)];
		if (worminator_map.info_tile_grid[(int)((sx + 15) / 16)][(int)(sy / 16)] != tile_hit) tile_hit = 0;
		if (worminator_map.info_tile_grid[(int)(sx / 16)][(int)((sy + 15) / 16)] != tile_hit) tile_hit = 0;

		if (tile_hit >= wait_16_info_tile && tile_hit <= wait_2048_info_tile) {
			for (exponent_loop = 0; exponent_loop <= (tile_hit - wait_16_info_tile); exponent_loop++) delay = delay * 2;
			sprites[sprite_number].timer = delay;
			//return;
		}
	}

	// Move the platform
	if (sprites[sprite_number].timer <= 0) {
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
		sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
	}

	// Move the player if nessecary
	if (input_buffer[INPUT_JUMP] && input_buffer[INPUT_DOWN]) return;
	if (sprites[sprite_number].y_acceleration > 0 && player.on_ground == TRUE) return;
	if (player.y_acceleration < sprites[sprite_number].y_acceleration) return;

	sprites[sprite_number].y_position_in_pixels--;
	if (sprite_is_touching_player(sprite_number) == TRUE && player.y_position_in_pixels + 32 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels + 32 < sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height) {
		while (done_looping == FALSE) {
			if (player.y_position_in_pixels + 32 > sprites[sprite_number].y_position_in_pixels - 1 && player.y_position_in_pixels + 32 < sprites[sprite_number].y_position_in_pixels + 4) {
				blocked_test = (int)player.y_position_in_pixels;
				push_player(0, -1);
				if (blocked_test == (int)player.y_position_in_pixels) done_looping = TRUE, blocked = TRUE;
			} else {
				done_looping = TRUE;
			}
		}
		;
		push_player(0, 1);
		push_player(0, 1);
		if (sprites[sprite_number].timer <= 0) {
			if (sprites[sprite_number].y_acceleration > 0) push_player(0, 1);
			push_player(sprites[sprite_number].x_acceleration, 0);
		}
		player.on_ground = TRUE;
		player.y_acceleration = 1;
	}
	sprites[sprite_number].y_position_in_pixels++;
}

/************************\
|** SMALL EXPLOSION AI **|
\************************/
void small_explosion_ai(int sprite_number)
{
	if (sprites[sprite_number].frame >= SMALL_EXPLOSION_01 && sprites[sprite_number].frame <= SMALL_EXPLOSION_08) {
		// Rotate the explosion sprite
		sprites[sprite_number].rotate_factor = rand() % 256;

		sprites[sprite_number].timer--;
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].animation_state == 0) {
				sprites[sprite_number].frame++;
				sprites[sprite_number].timer = 4;
				if (sprites[sprite_number].frame == SMALL_EXPLOSION_08) sprites[sprite_number].animation_state = 1;
			} else {
				sprites[sprite_number].frame--;
				sprites[sprite_number].timer = 4;
				if (sprites[sprite_number].frame < SMALL_EXPLOSION_01) {
					destroy_sprite(sprite_number);
					return;
				}
			}
		}
	} else {
		sprites[sprite_number].timer--;
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].animation_state == 0) {
				sprites[sprite_number].frame++;
				sprites[sprite_number].timer = 8;
				if (sprites[sprite_number].frame > NEW_SMALL_EXPLOSION_12) {
					destroy_sprite(sprite_number);
					return;
				}
			}
		}
	}
}

/*************************\
|** MEDIUM EXPLOSION AI **|
\*************************/
void medium_explosion_ai(int sprite_number)
{
	if (sprites[sprite_number].frame >= MEDIUM_EXPLOSION_01 && sprites[sprite_number].frame <= MEDIUM_EXPLOSION_10) {
		// Rotate the explosion sprite
		sprites[sprite_number].rotate_factor = rand() % 256;

		sprites[sprite_number].timer--;
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].animation_state == 0) {
				sprites[sprite_number].frame++;
				sprites[sprite_number].timer = 4;
				if (sprites[sprite_number].frame == MEDIUM_EXPLOSION_10) sprites[sprite_number].animation_state = 1;
			} else {
				sprites[sprite_number].frame--;
				sprites[sprite_number].timer = 4;
				if (sprites[sprite_number].frame < MEDIUM_EXPLOSION_01) {
					destroy_sprite(sprite_number);
					return;
				}
			}
		}
	} else {
		sprites[sprite_number].timer--;
		if (sprites[sprite_number].timer == 0) {
			if (sprites[sprite_number].animation_state == 0) {
				sprites[sprite_number].frame++;
				sprites[sprite_number].timer = 8;
				if (sprites[sprite_number].frame > NEW_MEDIUM_EXPLOSION_12) {
					destroy_sprite(sprite_number);
					return;
				}
			}
		}
	}
}

/**********************************\
|** BFG PLASMA BALL EXPLOSION AI **|
\**********************************/
void bfg_plasma_ball_explosion_ai(int sprite_number)
{
	int i;

	sprites[sprite_number].timer--;
	if (sprites[sprite_number].timer == 0) {
		sprites[sprite_number].frame++;
		sprites[sprite_number].timer = 12;
		if (sprites[sprite_number].frame == BFG_PLASMA_BALL_EXPLODE_08) {
			destroy_sprite(sprite_number);
			return;
		}
	}

	// Cast out evil green rays 'o death
	for (i = 0; i < number_of_sprites; i++) if (sprites[i].hit_points_left > 0) if (sqrt((sprites[sprite_number].x_position_in_pixels - sprites[i].x_position_in_pixels) * (sprites[sprite_number].x_position_in_pixels - sprites[i].x_position_in_pixels) + (sprites[sprite_number].y_position_in_pixels - sprites[i].y_position_in_pixels) * (sprites[sprite_number].y_position_in_pixels - sprites[i].y_position_in_pixels)) < 212) {
				sprites[i].hit_points_left -= .04;
				spawn_stardust(sprites[i].x_position_in_pixels + sprites[i].width / 2, sprites[i].y_position_in_pixels + sprites[i].height / 2, 0, 0, 32 + 10, 32 + 6, 4, 1);
			}
}

/*******************************\
|** SMALL EXPLOSIVE BARREL AI **|
\*******************************/
void small_explosive_barrel_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].hit_points_left != -666) {
		sprites[sprite_number].timer = 64;
		sprites[sprite_number].hit_points_left = -666;
	}

	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer / 8.0 == (int)(sprites[sprite_number].timer / 8)) {
			if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 7 + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 640) - 320.00) / 10.00), 0, 0, 0);
			else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 7 + ((float)((rand() % 320) - 160.00) / 10.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 640) - 320.00) / 10.00), 0, 0, 0);
		}

		if (sprites[sprite_number].timer / 16.0 == (int)(sprites[sprite_number].timer / 16)) {
			for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 7, sprites[sprite_number].y_position_in_pixels + 15, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 7, sprites[sprite_number].y_position_in_pixels + 15, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 7, sprites[sprite_number].y_position_in_pixels + 15, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		}

		if (sprites[sprite_number].timer == 0) {
			destroy_sprite(sprite_number);
			return;
		}

		sprites[sprite_number].timer--;
	}
}

/**************************************\
|** SMALL EXPLOSIVE NUCLEAR CRATE AI **|
\**************************************/
void small_explosive_nuclear_crate_ai(int sprite_number)
{
	if (sprites[sprite_number].hit_points_left <= 0) {
		spawn_sprite(MUSHROOM_CLOUD_SPRITE, sprites[sprite_number].x_position_in_pixels - 48, sprites[sprite_number].y_position_in_pixels - 96, 0, 0, 0);
		destroy_sprite(sprite_number);
		return;
	}
}

/***********************\
|** MUSHROOM CLOUD AI **|
\***********************/
void mushroom_cloud_ai(int sprite_number)
{
	sprites[sprite_number].timer--;
	if (sprites[sprite_number].timer == 0) {
		sprites[sprite_number].frame++;
		sprites[sprite_number].timer = 16;
		if (sprites[sprite_number].frame == MUSHROOM_CLOUD_01 + 24) {
			destroy_sprite(sprite_number);
			return;
		}
	}
}

/***********************\
|** PUSHABLE CRATE AI **|
\***********************/
void pushable_crate_ai(int sprite_number)
{
	char done_looping = FALSE;
	char blocked = FALSE;
	int blocked_test;
	// These are true or false for the three points nessecary for collision detection
	char p1, p2, p3;

	// The box is immortal, so max out it's health
	sprites[sprite_number].hit_points_left = 666;

	// This is the gravity code
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
	p1 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + 31, 4);
	p2 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 15, sprites[sprite_number].y_position_in_pixels + 31, 4);
	p3 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 31, sprites[sprite_number].y_position_in_pixels + 31, 4);
	if (p1 == FALSE && p2 == FALSE && p3 == FALSE) {
		sprites[sprite_number].y_acceleration += .025;
		if (sprites[sprite_number].y_acceleration > 1.5) sprites[sprite_number].y_acceleration = 1.5;
	} else {
		sprites[sprite_number].y_position_in_pixels -= sprites[sprite_number].y_acceleration;
		sprites[sprite_number].y_acceleration = .025;
	}

	// Cheat and mess up the collision detection a little :)
	sprites[sprite_number].x_position_in_pixels--;
	sprites[sprite_number].width += 2;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.y_position_in_pixels + 28 >= sprites[sprite_number].y_position_in_pixels) {
		// Check x boundaries
		if (player.x_position_in_pixels <= sprites[sprite_number].x_position_in_pixels + 16 && player.direction == 0) {
			sprites[sprite_number].x_position_in_pixels += player.x_acceleration;
			p1 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 32, sprites[sprite_number].y_position_in_pixels, 4);
			p2 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 32, sprites[sprite_number].y_position_in_pixels + 15, 4);
			p3 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 32, sprites[sprite_number].y_position_in_pixels + 31, 4);
			if (p1 == TRUE || p2 == TRUE || p3 == TRUE) {
				sprites[sprite_number].x_position_in_pixels -= player.x_acceleration;
				player.x_acceleration = 0;
			}
		} else if (player.x_position_in_pixels >= sprites[sprite_number].x_position_in_pixels + 16 && player.direction == 1) {
			sprites[sprite_number].x_position_in_pixels += player.x_acceleration;
			p1 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 1, sprites[sprite_number].y_position_in_pixels, 4);
			p2 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 1, sprites[sprite_number].y_position_in_pixels + 15, 4);
			p3 = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + 1, sprites[sprite_number].y_position_in_pixels + 31, 4);
			if (p1 == TRUE || p2 == TRUE || p3 == TRUE) {
				sprites[sprite_number].x_position_in_pixels -= player.x_acceleration;
				player.x_acceleration = 0;
			}
		}
	}

	// restore the collision detection to its original state
	sprites[sprite_number].x_position_in_pixels++;
	sprites[sprite_number].width -= 2;

	// Move the player if nessecary
	//if ((key[KEY_LCONTROL] || key[KEY_RCONTROL]) && key[KEY_DOWN]) return;
	if (sprites[sprite_number].y_acceleration > 0 && player.on_ground == TRUE) return;
	if (player.y_acceleration < sprites[sprite_number].y_acceleration) return;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.y_position_in_pixels + 32 > sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels + 32 < sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height) {
		while (done_looping == FALSE) {
			if (player.y_position_in_pixels + 32 > sprites[sprite_number].y_position_in_pixels - 1 && player.y_position_in_pixels + 32 < sprites[sprite_number].y_position_in_pixels + 4) {
				blocked_test = (int)player.y_position_in_pixels;
				push_player(0, -1);
				if (blocked_test == (int)player.y_position_in_pixels) done_looping = TRUE, blocked = TRUE;
			} else {
				done_looping = TRUE;
			}
		}
		;
		push_player(0, 1);
		push_player(0, 1);
		if (sprites[sprite_number].timer <= 0) {
			if (sprites[sprite_number].y_acceleration > 0) push_player(0, 1);
			push_player(sprites[sprite_number].x_acceleration, 0);
		}
		player.on_ground = TRUE;
		player.y_acceleration = 1;
	}
}

/************************\
|** PERVERT MESSAGE AI **|
\************************/
void pervert_message_ai(int sprite_number)
{
	// See if its time to remove the pervert message
	if (sprites[sprite_number].timer == 0) {
		destroy_sprite(sprite_number);
		return;
	} else {
		sprites[sprite_number].timer--;
	}

	// Make sure the pervert message is still centered
	sprites[sprite_number].x_position_in_pixels = screen_x_position + (int)(screen_width * .8) / 2 - sprites[sprite_number].width / 2;
	sprites[sprite_number].y_position_in_pixels = screen_y_position + (int)(screen_width * .96) / 2 - sprites[sprite_number].height / 2;
}

/******************\
|** GREY DOOR AI **|
\******************/
void grey_door_ai(int sprite_number)
{
	// Paused at the bottom
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE) {
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 128;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 24) / 16)] = 0;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 40) / 16)] = 0;
			play_relative_sound(DOOR_OPEN_SOUND, sprite_number);
		} else {
			return;
		}
	}

	// Moving up
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels -= .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/*****************\
|** RED DOOR AI **|
\*****************/
void red_door_ai(int sprite_number)
{
	// Paused at the bottom
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE && player.has_red_keycard == TRUE) {
			player.has_red_keycard = FALSE;
			update_player_stats();
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 128;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 24) / 16)] = 0;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 40) / 16)] = 0;
			play_relative_sound(DOOR_OPEN_SOUND, sprite_number);
		} else if (sprite_is_touching_player(sprite_number) == TRUE) {
			add_text_message_if_new("You need a red key to open this door");
		} else {
			return;
		}
	}

	// Moving up
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels -= .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/*********************\
|** RED DOOR TOP AI **|
\*********************/
void red_door_top_ai(int sprite_number)
{
	// Paused at the center
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE && player.has_red_keycard == TRUE) {
			player.has_red_keycard = 69;	// This is a special case so that the bottom half will be triggered too
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 64;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 24) / 16)] = 0;
			play_relative_sound(DOOR_OPEN_SOUND, sprite_number);
		} else if (sprite_is_touching_player(sprite_number) == TRUE) {
			add_text_message_if_new("You need a red key to open this door");
		} else {
			return;
		}
	}

	// Moving up
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels -= .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/************************\
|** RED DOOR BOTTOM AI **|
\************************/
void red_door_bottom_ai(int sprite_number)
{
	// Paused at the center
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE && player.has_red_keycard == 69) {
			player.has_red_keycard = FALSE;
			update_player_stats();
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 64;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 40) / 16)] = 0;
		} else {
			return;
		}
	}

	// Moving down
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels += .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the bottom
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/*******************\
|** GREEN DOOR AI **|
\*******************/
void green_door_ai(int sprite_number)
{
	// Paused at the bottom
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE && player.has_green_keycard == TRUE) {
			player.has_green_keycard = FALSE;
			update_player_stats();
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 128;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 24) / 16)] = 0;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 40) / 16)] = 0;
			play_relative_sound(DOOR_OPEN_SOUND, sprite_number);
		} else if (sprite_is_touching_player(sprite_number) == TRUE) {
			add_text_message_if_new("You need a green key to open this door");
		} else {
			return;
		}
	}

	// Moving up
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels -= .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/***********************\
|** GREEN DOOR TOP AI **|
\***********************/
void green_door_top_ai(int sprite_number)
{
	// Paused at the center
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE && player.has_green_keycard == TRUE) {
			player.has_green_keycard = 69;	// This is a special case so that the bottom half will be triggered too
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 64;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 24) / 16)] = 0;
			play_relative_sound(DOOR_OPEN_SOUND, sprite_number);
		} else if (sprite_is_touching_player(sprite_number) == TRUE) {
			add_text_message_if_new("You need a green key to open this door");
		} else {
			return;
		}
	}

	// Moving up
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels -= .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/**************************\
|** GREEN DOOR BOTTOM AI **|
\**************************/
void green_door_bottom_ai(int sprite_number)
{
	// Paused at the center
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE && player.has_green_keycard == 69) {
			player.has_green_keycard = FALSE;
			update_player_stats();
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 64;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 40) / 16)] = 0;
		} else {
			return;
		}
	}

	// Moving down
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels += .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the bottom
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/******************\
|** BLUE DOOR AI **|
\******************/
void blue_door_ai(int sprite_number)
{
	// Paused at the bottom
	if (sprites[sprite_number].action_state == 0) {
		if (sprite_is_touching_player(sprite_number) == TRUE && player.has_blue_keycard == TRUE) {
			player.has_blue_keycard = FALSE;
			update_player_stats();
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 128;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 24) / 16)] = 0;
			worminator_map.info_tile_grid[(int)((sprites[sprite_number].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_number].y_position_in_pixels + 40) / 16)] = 0;
			play_relative_sound(DOOR_OPEN_SOUND, sprite_number);
		} else if (sprite_is_touching_player(sprite_number) == TRUE) {
			add_text_message_if_new("You need a blue key to open this door");
		} else {
			return;
		}
	}

	// Moving up
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].action_state = 2;
		} else {
			sprites[sprite_number].y_position_in_pixels -= .25;
			sprites[sprite_number].timer--;
		}
	}
	// Paused at the top
	else if (sprites[sprite_number].action_state == 2) {
		return;
	}
}

/*******************\
|** WATER DROP AI **|
\*******************/
void water_drop_ai(int sprite_number)
{
	// Dropping down
	if (sprites[sprite_number].action_state == 0) {
		sprites[sprite_number].y_acceleration += .025;
		if (sprites[sprite_number].y_acceleration > 2) sprites[sprite_number].y_acceleration = 2;
		sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1) == TRUE) {
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].x_position_in_pixels -= 6;
			sprites[sprite_number].timer = 16;
			sprites[sprite_number].frame = WATER_DROP_SPLASH_01;
		}
	}
	// Splashing
	else {
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 16;
			if (sprites[sprite_number].frame > WATER_DROP_SPLASH_08) {
				destroy_sprite(sprite_number);
				return;
			}
		} else {
			sprites[sprite_number].timer--;
		}
	}
}

/*********************\
|** BORED SPRITE AI **|
\*********************/
void bored_sprite_ai(int sprite_number)
{
	int val;

	if (sprites[sprite_number].frame == 0) {
		val = rand() % 6;
		sprites[sprite_number].frame = BORED_FRAME_01 + val;
		switch (val) {
		case 0:
			sprites[sprite_number].width = 320;  sprites[sprite_number].height = 200;
			sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 144;
			sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 84;
			sprites[sprite_number].timer = 512;
			break;
		case 1:
			sprites[sprite_number].width = 96;  sprites[sprite_number].height = 96;
			sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 32;
			sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 96;
			sprites[sprite_number].timer = 384;
			break;
		case 2:
			sprites[sprite_number].width = 75;  sprites[sprite_number].height = 75;
			sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 21;
			sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 75;
			sprites[sprite_number].timer = 384;
			break;
		case 3:
			sprites[sprite_number].width = 75;  sprites[sprite_number].height = 75;
			sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 21;
			sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 75;
			sprites[sprite_number].timer = 384;
			break;
		case 4:
			sprites[sprite_number].width = 50;  sprites[sprite_number].height = 50;
			sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 9;
			sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 50;
			sprites[sprite_number].timer = 256;
			break;
		case 5:
			sprites[sprite_number].width = 148;  sprites[sprite_number].height = 132;
			sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 404;
			sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 50;
			sprites[sprite_number].timer = 692;
			break;
		}
	}

	switch (sprites[sprite_number].frame) {
	case BORED_FRAME_01:
		sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 144;
		sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 84;
		break;
	case BORED_FRAME_02:
		sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 32;
		sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 96;
		break;
	case BORED_FRAME_03:
		sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 21;
		sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 75;
		break;
	case BORED_FRAME_04:
		sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 21;
		sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 75;
		break;
	case BORED_FRAME_05:
		sprites[sprite_number].x_position_in_pixels = player.x_position_in_pixels - 9;
		sprites[sprite_number].y_position_in_pixels = player.y_position_in_pixels - 50;
	case BORED_FRAME_06:
		sprites[sprite_number].x_position_in_pixels++;
	}

	sprites[sprite_number].timer--;
	if (sprites[sprite_number].timer <= 0) destroy_sprite(sprite_number);
}

/**********************\
|** EXPLODING CAR AI **|
\**********************/
void exploding_car_ai(int sprite_number)
{
	int spawn_loop, phb = player.health_bars_left, ptb = player.timer, bsn = number_of_sprites;

	if (sprites[sprite_number].action_state == 1) return;
	if (player.skin == TANK_SKIN && player.x_position_in_pixels + 96 >= sprites[sprite_number].x_position_in_pixels && player.x_position_in_pixels < sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width && player.y_position_in_pixels + 64 >= sprites[sprite_number].y_position_in_pixels && player.y_position_in_pixels < sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height) {
		sprites[sprite_number].y_position_in_pixels += 4;
		sprites[sprite_number].frame--;
		sprites[sprite_number].action_state = 1;
		quake_timer += 48;
		//if (sprites[sprite_number].timer / 4.0 == (int)(sprites[sprite_number].timer / 4)) {
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) {
			hurt_player(10);
			bsn++;
			if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 32 + ((float)((rand() % 1920) - 960.00) / 20.00), sprites[sprite_number].y_position_in_pixels - 8 + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
			else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 40 + ((float)((rand() % 1920) - 960.00) / 20.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
			while (number_of_sprites > bsn) destroy_sprite(number_of_sprites - 1);
			play_relative_sound(CAR_CRUSH_SOUND, sprite_number);
		}
		while (player.health_bars_left > phb) hurt_player(-1);
		while (player.health_bars_left < phb) hurt_player(1);
		while (number_of_sprites > bsn) destroy_sprite(number_of_sprites - 1);
		player.timer = ptb;
	}
}

/***********************\
|** PALETTE EFFECT AI **|
\***********************/
void palette_effect_ai(int sprite_number)
{
	if (sprite_is_touching_player(sprite_number) && !(wormy_config.cool_mode && bullet_time_engaged)) {
		if (sprites[sprite_number].sprite_type == DARK_PALETTE_EFFECT_SPRITE) darken(FALSE);
		if (sprites[sprite_number].sprite_type == LIGHT_PALETTE_EFFECT_SPRITE) restore_palette(FALSE);
		sprites[sprite_number].timer = 1024;
	}
}
