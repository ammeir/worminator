/**************************\
|** WORMINATOR BULLET AI **|
\**************************/
void worminator_bullet_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	int sprite_hit;

	// Check the lifetime timer
	if (sprites[sprite_number].timer > 0) sprites[sprite_number].timer--;
	else sprite_busted = TRUE;

	// Move the bullet right or left
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

	// Check to see if the bullet hit an enemy and take approprate action if so
	sprite_hit = sprite_is_touching_sprite(sprite_number, 1);
	if (sprite_hit >= 0) {
		sprite_busted = TRUE;
		sprites[sprite_hit].hit_points_left -= .20;
	}

	// See if there are solid tiles in the way of the bullet
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels, 1);
	else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels, 1);

	// If the bullet hit anything at all, then destroy it
	if (sprite_busted == TRUE) {
		spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (sprites[sprite_number].x_acceleration / 2) * -1, (float)((rand() % 20) - 10.00) / 10.00, 0);
		destroy_sprite(sprite_number);
		return;
	}
}

/**************************\
|** WORMINATOR ROCKET AI **|
\**************************/
void worminator_rocket_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;
	int phb = player.health_bars_left, ptb = player.timer, bsn = number_of_sprites;

	// Move the rocket right or left, and up or down if the rocket is a split missle rocket
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Apply gravity if this is a tank shell
	if (sprites[sprite_number].frame != WORMINATOR_ROCKET_FLY) sprites[sprite_number].y_acceleration += .002;

	// Check to see if the rocket hit an enemy and take appropriate action if so
	if (sprite_is_touching_sprite(sprite_number, 1) >= 0) sprite_busted = TRUE;

	// See if there are solid tiles in the way of the rocket and adjust the rocket's speed
	if (sprites[sprite_number].direction == 0) {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 1);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		sprites[sprite_number].x_acceleration += .025;
		if (sprites[sprite_number].x_acceleration > 2) sprites[sprite_number].x_acceleration = 2;
	} else {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 1);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		sprites[sprite_number].x_acceleration -= .025;
		if (sprites[sprite_number].x_acceleration < -2) sprites[sprite_number].x_acceleration = -2;
	}

	// Stardust smoke trail
	if (out_of_player_range(sprite_number) == FALSE) {
		if (sprites[sprite_number].frame == WORMINATOR_ROCKET_FLY) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 255, 240, 4, 1);
		else if (sprites[sprite_number].frame == SUPER_TURKEY_MASHED_POTATOES) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 255, 250, 4, 1);
		else spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 2, 6, 4, 1);
	}

	// Animate if this is a tank shell
	if (sprites[sprite_number].frame != WORMINATOR_ROCKET_FLY && sprites[sprite_number].frame != SUPER_TURKEY_MASHED_POTATOES) {
		sprites[sprite_number].timer--;
		if (sprites[sprite_number].timer <= 0) {
			sprites[sprite_number].frame++;
			if (sprites[sprite_number].frame > TANK_SHELL_FLY_04) sprites[sprite_number].frame = TANK_SHELL_FLY_01;
			sprites[sprite_number].timer = 6;
		}
	}

	// If the rocket hit anything at all, then destyoy it
	if (sprite_busted == TRUE) {
		if (sprites[sprite_number].frame == WORMINATOR_ROCKET_FLY) {
			for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
			for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
			spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 3, 0, 0, 0);
			spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 3, 0, 0, 0);
			play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
			destroy_sprite(sprite_number);
		} else {
			if (sprites[sprite_number].frame == SUPER_TURKEY_MASHED_POTATOES && sprites[sprite_number].x_acceleration >= 0) sprites[sprite_number].x_position_in_pixels -= 24;
			for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
			for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
			for (spawn_loop = 0; spawn_loop < 10; spawn_loop++) {
				hurt_player(10);
				bsn++;
				if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 8 + 8 * sprites[sprite_number].x_acceleration + ((float)((rand() % 480) - 240.00) / 20.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 480) - 240.00) / 20.00), 0, 0, 0);
				else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + 16 + 8 * sprites[sprite_number].x_acceleration + ((float)((rand() % 480) - 240.00) / 20.00), sprites[sprite_number].y_position_in_pixels + 8 + ((float)((rand() % 480) - 240.00) / 20.00), 0, 0, 0);
				while (number_of_sprites > bsn) destroy_sprite(number_of_sprites - 1);
			}
			play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
			destroy_sprite(sprite_number);
			while (player.health_bars_left > phb) hurt_player(-1);
			while (player.health_bars_left < phb) hurt_player(1);
			while (number_of_sprites > bsn) destroy_sprite(number_of_sprites - 1);
			player.timer = ptb;
		}
		return;
	}
}

/**************************\
|** WORMINATOR MORTAR AI **|
\**************************/
void worminator_mortar_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	// Update lifespan timer
	if (sprites[sprite_number].timer == 0) sprite_busted = TRUE;
	else sprites[sprite_number].timer--;

	// Rotate the mortar
	if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;
	else sprites[sprite_number].rotate_factor++;

	// Move the mortar
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Check to see if the mortar hit an enemy and take appropriate action if so
	if (sprite_is_touching_sprite(sprite_number, 1) >= 0) sprite_busted = TRUE;

	// Check x direction
	if (sprites[sprite_number].x_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, 2) == TRUE) sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
	else if (sprites[sprite_number].x_acceleration < 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, 2) == TRUE) sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;

	// Check y direction
	if (sprites[sprite_number].y_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2) == TRUE) sprites[sprite_number].y_acceleration = sprites[sprite_number].y_acceleration * -1;
	else if (sprites[sprite_number].y_acceleration < 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels, 2) == TRUE) sprites[sprite_number].y_acceleration = sprites[sprite_number].y_acceleration * -1;

	// Apply gravity to the mortar
	sprites[sprite_number].y_acceleration += .025;

	// Trail if in fried chicken mode
	if (out_of_player_range(sprite_number) == FALSE && sprites[sprite_number].frame == SUPER_TURKEY_FRIED_CHICKEN) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 101, 107, 4, 1);

	// If the mortar hit anything damageable, then destyoy it
	if (sprite_busted == TRUE) {
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 10, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 10, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 10, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/***********************************\
|** WORMINATOR BFG PLASMA BALL AI **|
\***********************************/
void worminator_bfg_plasma_ball_ai(int sprite_number)
{
	int i;
	char sprite_busted = FALSE;

	// Animate the plasma ball
	if (sprites[sprite_number].timer == 0) {
		if (sprites[sprite_number].animation_state == 0) {
			sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].frame == BFG_PLASMA_BALL_FLY_01 + 2) sprites[sprite_number].animation_state = 1;
		} else {
			sprites[sprite_number].frame--;
			sprites[sprite_number].timer = 8;
			if (sprites[sprite_number].frame == BFG_PLASMA_BALL_FLY_01) sprites[sprite_number].animation_state = 0;
		}
	} else {
		sprites[sprite_number].timer--;
	}

	// Move the plasma ball right or left
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

	// Cast out evil green rays 'o death
	for (i = 0; i < number_of_sprites; i++) if (sprites[i].hit_points_left > 0) if (sqrt((sprites[sprite_number].x_position_in_pixels - sprites[i].x_position_in_pixels) * (sprites[sprite_number].x_position_in_pixels - sprites[i].x_position_in_pixels) + (sprites[sprite_number].y_position_in_pixels - sprites[i].y_position_in_pixels) * (sprites[sprite_number].y_position_in_pixels - sprites[i].y_position_in_pixels)) < 212) {
				sprites[i].hit_points_left -= .01;
				spawn_stardust(sprites[i].x_position_in_pixels + sprites[i].width / 2, sprites[i].y_position_in_pixels + sprites[i].height / 2, 0, 0, 32 + 10, 32 + 6, 4, 1);
			}

	// Check to see if the plasma ball hit an enemy and take appropriate action if so
	if (sprite_is_touching_sprite(sprite_number, 1) >= 0) sprite_busted = TRUE;

	// See if there are solid tiles in the way of the plasma ball and adjust it's speed

	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 1);
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 1);
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);

	if (sprites[sprite_number].direction == 0) {
		sprites[sprite_number].x_acceleration += .025;
		if (sprites[sprite_number].x_acceleration > 4) sprites[sprite_number].x_acceleration = 4;
	} else {
		sprites[sprite_number].x_acceleration -= .025;
		if (sprites[sprite_number].x_acceleration < -4) sprites[sprite_number].x_acceleration = -4;
	}

	// If the plasma ball hit anything at all, then destyoy it
	if (sprite_busted == TRUE) {
		spawn_sprite(BFG_PLASMA_BALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 8, 0, 0, 0);
		for (i = 0; i < 24; i++) spawn_sprite(ARCSPARK_SPRITE, (sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 38);
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/************************\
|** WORMINATOR MINE AI **|
\************************/
void worminator_mine_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// See if there are solid tiles in the way of the mine and adjust the y acceleration
	if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1) == TRUE) {
		sprites[sprite_number].y_acceleration = .025;
	} else if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 0) == TRUE) {
		sprites[sprite_number].y_acceleration = .025;
	} else {
		sprites[sprite_number].y_acceleration += .025;
		if (sprites[sprite_number].y_acceleration > 1) sprites[sprite_number].y_acceleration = 1;

		// Apply gravity
		sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
	}

	// Do animations and stuff
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].frame == WORMINATOR_MINE_ARM) {
			sprites[sprite_number].frame = WORMINATOR_MINE_REST, sprites[sprite_number].timer = 32;
			sprites[sprite_number].hit_points_left = .25;
		} else {
			if (sprites[sprite_number].frame == WORMINATOR_MINE_REST) sprites[sprite_number].frame = WORMINATOR_MINE_FLASH;
			else sprites[sprite_number].frame = WORMINATOR_MINE_REST;
			sprites[sprite_number].timer = 32;
		}
	} else {
		sprites[sprite_number].timer--;
	}

	// Check to see if the mine hit an enemy and take approprate action if so
	if ((sprites[sprite_number].frame != WORMINATOR_MINE_ARM && (sprite_is_touching_sprite(sprite_number, 1) >= 0 || sprite_is_touching_player(sprite_number) == TRUE)) || (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].frame != WORMINATOR_MINE_ARM)) {
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 2, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 2, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 2, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		destroy_sprite(sprite_number);
	}
}

/****************************\
|** ENEMY PLASMA BULLET AI **|
\****************************/
void enemy_plasma_bullet_ai(int sprite_number)
{
	char sprite_busted = FALSE;

	// Check the lifetime timer
	if (sprites[sprite_number].timer > 0) sprites[sprite_number].timer--;
	else sprite_busted = TRUE;

	// Move the bullet right or left
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

	// Check for a collision with the player
	if (sprite_is_touching_player(sprite_number) == TRUE && player.health_bars_left != 0 && player.timer == 0) {
		/*if (rand() % 4 == 2) */ hurt_player(-1);
		sprite_busted = TRUE;
	}

	// See if there are solid tiles in the way of the bullet
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels, 1);

	// If the bullet hit anything at all, then destroy it
	if (sprite_busted == TRUE) {
		spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (sprites[sprite_number].x_acceleration / 2) * -1, (float)((rand() % 20) - 10.00) / 10.00, 0);
		destroy_sprite(sprite_number);
		return;
	}
}

/*************************\
|** ENEMY LASER BOLT AI **|
\*************************/
void enemy_laser_bolt_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	// Check to see if the laser has hit the range limit
	sprites[sprite_number].timer--;
	if (sprites[sprite_number].timer == 0) sprite_busted = TRUE;

	// Check to see if the laser hit the player
	if (sprite_is_touching_player(sprite_number) == TRUE && player.health_bars_left != 0) hurt_player(-1), sprite_busted = TRUE;

	// Move the laser right or left
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

	// See if there are solid tiles in the way of the laser
	if (sprites[sprite_number].direction == 0) {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
	} else {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
	}

	if (sprite_busted == TRUE) {
		//poof_out(sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, sprites[sprite_number].frame);
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		//	play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/***************************\
|** SMALL ENEMY ROCKET AI **|
\***************************/
void small_enemy_rocket_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.health_bars_left != 0) sprite_busted = TRUE;

	// Adjust the egg's trajectory
	if (sprites[sprite_number].frame == ENEMY_EGG_FLY)
		sprites[sprite_number].y_acceleration += .002;

	// Move the rocket right or left, and up or down if the rocket is a split missle rocket
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// See if there are solid tiles in the way of the rocket and adjust the rocket's speed
	if (sprites[sprite_number].direction == 0) {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		sprites[sprite_number].x_acceleration += .025;
		if (sprites[sprite_number].x_acceleration > 2) sprites[sprite_number].x_acceleration = 2;
	} else {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		sprites[sprite_number].x_acceleration -= .025;
		if (sprites[sprite_number].x_acceleration < -2) sprites[sprite_number].x_acceleration = -2;
	}

	if (sprite_busted == TRUE) {
		if (sprites[sprite_number].frame == SMALL_ENEMY_ROCKET_FLY) {
			for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
			spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 1, sprites[sprite_number].y_position_in_pixels - 4, 0, 0, 0);
			play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		} else if (sprites[sprite_number].frame == ENEMY_EGG_FLY) {
			spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 120, 125, 4, 24);
			spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 228, 235, 4, 16);
			if (sprite_is_touching_player(sprite_number) == TRUE) hurt_player(-1);
			play_relative_sound(EGG_SPLAT_SOUND, sprite_number);
		}
		destroy_sprite(sprite_number);
		return;
	}
}

/***************************\
|** LARGE ENEMY ROCKET AI **|
\***************************/
void large_enemy_rocket_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	if (sprite_is_touching_player(sprite_number) == TRUE && player.health_bars_left != 0) sprite_busted = TRUE;

	// This is some basic animation code -- IMPROVE IT ASAP
	if (sprites[sprite_number].frame == LARGE_ENEMY_ROCKET_FLY_01) sprites[sprite_number].frame = LARGE_ENEMY_ROCKET_FLY_02;
	else sprites[sprite_number].frame = LARGE_ENEMY_ROCKET_FLY_01;

	// Move the rocket right or left, and up or down if the rocket is a split missle rocket
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// See if there are solid tiles in the way of the rocket and adjust the rocket's speed
	if (sprites[sprite_number].direction == 0) {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		sprites[sprite_number].x_acceleration += .025;
		if (sprites[sprite_number].x_acceleration > 2) sprites[sprite_number].x_acceleration = 2;
	} else {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 2);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2);
		sprites[sprite_number].x_acceleration -= .025;
		if (sprites[sprite_number].x_acceleration < -2) sprites[sprite_number].x_acceleration = -2;
	}

	if (sprite_busted == TRUE) {
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/*********************\
|** ENEMY MORTAR AI **|
\*********************/
void enemy_mortar_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	// Update lifespan timer
	if (sprites[sprite_number].timer == 0) sprite_busted = TRUE;
	else sprites[sprite_number].timer--;

	// Rotate the mortar
	if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;
	else sprites[sprite_number].rotate_factor++;

	// Move the mortar
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Check to see if the mortar hit an enemy and take appropriate action if so
	if (sprite_is_touching_player(sprite_number) == TRUE) sprite_busted = TRUE;

	// Check x direction
	if (sprites[sprite_number].x_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, 2) == TRUE) sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
	else if (sprites[sprite_number].x_acceleration < 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2, 2) == TRUE) sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;

	// Check y direction
	if (sprites[sprite_number].y_acceleration > 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 2) == TRUE) sprites[sprite_number].y_acceleration = sprites[sprite_number].y_acceleration * -1;
	else if (sprites[sprite_number].y_acceleration < 0 && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2, sprites[sprite_number].y_position_in_pixels, 2) == TRUE) sprites[sprite_number].y_acceleration = sprites[sprite_number].y_acceleration * -1;

	// Apply gravity to the mortar
	sprites[sprite_number].y_acceleration += .025;

	// If the mortar hit anything damageable, then destyoy it
	if (sprite_busted == TRUE) {
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 10, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 10, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		//spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 10, sprites[sprite_number].y_position_in_pixels - 10, 0, 0, 0);
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/********************\
|** ENEMY TOXIN AI **|
\********************/
void enemy_toxin_ai(int sprite_number)
{
	char sprite_busted = FALSE;

	// Move the toxin right or left
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

	// Check for a collision with the player
	if (sprite_is_touching_player(sprite_number) == TRUE && player.health_bars_left != 0) {
		hurt_player(-2);
		sprite_busted = TRUE;
		spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (sprites[sprite_number].x_acceleration / 2) * -1, (float)((rand() % 20) - 10.00) / 10.00, 0);
	}

	// See if there are solid tiles in the way of the bullet
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + ((sprites[sprite_number].width) / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1);
	else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + ((sprites[sprite_number].width) / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1);

	// If the toxin hit anything at all, then destroy it
	if (sprite_busted == TRUE) {
		spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 41, 36, 4, 24);
		destroy_sprite(sprite_number);
		return;
	}
}

/*********************************\
|** WORMINATOR PLASMA BULLET AI **|
\*********************************/
void worminator_plasma_bullet_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	int sprite_hit;

	// Check the lifetime timer
	if (sprites[sprite_number].timer > 0) sprites[sprite_number].timer--;
	else sprite_busted = TRUE;

	// Move the plasma bullet right or left
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

	// Check to see if the plasma bullet hit an enemy and take approprate action if so
	sprite_hit = sprite_is_touching_sprite(sprite_number, 1);
	if (sprite_hit >= 0) {
		sprite_busted = TRUE;
		sprites[sprite_hit].hit_points_left -= .45;
	}

	// See if there are solid tiles in the way of the plasma bullet
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1);
	else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1);

	// If the plasma bullet hit anything at all, then destroy it
	if (sprite_busted == TRUE) {
		spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 41, 36, 4, 16);
		destroy_sprite(sprite_number);
		return;
	}
}

/*****************************\
|** WORMINATOR SPIKESTAR AI **|
\*****************************/
void worminator_spikestar_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	int sprite_hit;

	// Rotate the spikestar
	if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;
	else sprites[sprite_number].rotate_factor += 4;

	// Move the spikestar right or left, and up or down if nessicary
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Check to see if the spikestar hit an enemy and take approprate action if so
	sprite_hit = sprite_is_touching_sprite(sprite_number, 1);
	if (sprite_hit >= 0) {
		sprite_busted = TRUE;
		sprites[sprite_hit].hit_points_left -= 15;
	}

	// The following updates are based on the type of spikestar (red - sine, blue - negative sine, yellow - straight)
	if (sprites[sprite_number].frame == WORMINATOR_SPIKESTAR_RED_FLY) {
		// Adjust the spikestar's y velocity to conform to a sine wave path
		sprites[sprite_number].y_acceleration = fixtof(fsin(itofix(sprites[sprite_number].rotate_factor)));

		// Stardust smoke trail
		if (out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, .05, 77, 66, 2, 1);
	} else if (sprites[sprite_number].frame == WORMINATOR_SPIKESTAR_BLUE_FLY) {
		// Adjust the spikestar's y velocity to conform to an inverse sine wave path
		sprites[sprite_number].y_acceleration = -fixtof(fsin(itofix(sprites[sprite_number].rotate_factor)));

		// Stardust smoke trail
		if (out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, -.05, 157, 146, 2, 1);
	} else if (sprites[sprite_number].frame == WORMINATOR_SPIKESTAR_YELLOW_FLY) {
		// Adjust the spikestar's y velocity to conform to a linear
		sprites[sprite_number].y_acceleration = 0;

		// Stardust smoke trail
		if (out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 237, 226, 1, 1);
	}

	// See if there are solid tiles in the way of the spikestar
	if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1);
	else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1);

	// If the spikestar hit anything at all, then destroy it
	if (sprite_busted == TRUE) {
		spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 41, 36, 4, 16);
		destroy_sprite(sprite_number);
		return;
	}
}

/******************************\
|** WORMINATOR MICRO NUKE AI **|
\******************************/
void worminator_micro_nuke_ai(int sprite_number)
{
	char sprite_busted = FALSE;

	// Move the micro nuke right or left, and up or down
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Check to see if the micro nuke hit an enemy and take appropriate action if so
	if (sprite_is_touching_sprite(sprite_number, 1) >= 0) sprite_busted = TRUE;

	// See if there are solid tiles in the way of the micro nuke and adjust the micro nuke's speed
	if (sprites[sprite_number].direction == 0) {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels, 1);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		sprites[sprite_number].x_acceleration += .015;
		if (sprites[sprite_number].x_acceleration > 2) sprites[sprite_number].x_acceleration = 2;
	} else {
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 1);
		if (sprite_busted == FALSE) sprite_busted = check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		else check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1);
		sprites[sprite_number].x_acceleration -= .015;
		if (sprites[sprite_number].x_acceleration < -2) sprites[sprite_number].x_acceleration = -2;
	}

	// Stardust smoke trail
	if (out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 2, 6, 4, 2);

	// If the micro nuke hit anything at all, then destyoy it
	if (sprite_busted == TRUE) {
		spawn_sprite(MUSHROOM_CLOUD_SPRITE, sprites[sprite_number].x_position_in_pixels - 56, sprites[sprite_number].y_position_in_pixels - 128 + sprites[sprite_number].height, 0, 0, 0);
		destroy_sprite(sprite_number);
		return;
	}
}

/********************************\
|** WORMINATOR WALKING MINE AI **|
\********************************/
void worminator_walking_mine_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// See if there are solid tiles in the way of the mine and adjust the y acceleration
	if (sprites[sprite_number].frame == WORMINATOR_WALKING_MINE_ARM) {
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 1) == TRUE) {
			sprites[sprite_number].y_acceleration = .025;
		} else if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width, sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height, 0) == TRUE) {
			sprites[sprite_number].y_acceleration = .025;
		} else {
			sprites[sprite_number].y_acceleration += .025;
			if (sprites[sprite_number].y_acceleration > 1) sprites[sprite_number].y_acceleration = 1;

			// Apply gravity
			sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
		}
	}

	// Do animations and stuff
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].frame == WORMINATOR_WALKING_MINE_ARM) {
			sprites[sprite_number].frame = WORMINATOR_WALKING_MINE_WALK_01, sprites[sprite_number].timer = 16;
		} else {
			if (sprites[sprite_number].frame == WORMINATOR_WALKING_MINE_WALK_01) sprites[sprite_number].frame = WORMINATOR_WALKING_MINE_WALK_02;
			else sprites[sprite_number].frame = WORMINATOR_WALKING_MINE_WALK_01;
			sprites[sprite_number].timer = 16;
		}
	} else {
		sprites[sprite_number].timer--;
	}

	// Walk
	if (sprites[sprite_number].frame != WORMINATOR_WALKING_MINE_ARM) {
		// Call a handy little function to process slopes for us
		if (check_enemy_slopes(sprite_number) == TRUE) return;

		// If the mine is in the ground, get it unstuck
		sprites[sprite_number].y_acceleration += 10;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height), 0) == TRUE) sprites[sprite_number].y_position_in_pixels--;
		sprites[sprite_number].y_acceleration -= 10;

		// Apply gravity and move the walking mine
		sprites[sprite_number].y_position_in_pixels++;
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;

		// Turn around the mine if nessicary
		if (check_enemy_turns(sprite_number) == TRUE && check_enemy_slope_exclusions(sprite_number) == FALSE) {
			if (sprites[sprite_number].direction == 0) sprites[sprite_number].direction = 1;
			else sprites[sprite_number].direction = 0;
			sprites[sprite_number].x_position_in_pixels -= sprites[sprite_number].x_acceleration;
			sprites[sprite_number].x_acceleration = sprites[sprite_number].x_acceleration * -1;
		}
	}

	// Check to see if the mine hit an enemy and take approprate action if so
	if ((sprites[sprite_number].frame != WORMINATOR_WALKING_MINE_ARM && (sprite_is_touching_sprite(sprite_number, 1) >= 0 || sprite_is_touching_player(sprite_number) == TRUE)) || sprites[sprite_number].hit_points_left <= 0) {
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 2, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 2, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 2, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 11, sprites[sprite_number].y_position_in_pixels - 13, 0, 0, 0);
		destroy_sprite(sprite_number);
	}
}

/**********************\
|** ENEMY MISSILE AI **|
\**********************/
void enemy_missile_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;
	double theta;

	// Animate the missile
	if (sprites[sprite_number].timer == 0) {
		sprites[sprite_number].frame++;
		sprites[sprite_number].timer = 8;
		if (sprites[sprite_number].frame > ENEMY_MISSILE_FLY_04) sprites[sprite_number].frame = ENEMY_MISSILE_FLY_01;
	} else {
		sprites[sprite_number].timer--;
	}

	// Destroy the missile if it was shot down
	/*if (sprites[sprite_number].hit_points_left <= 0) {
	 *      player.score++;
	 *      sprite_busted = TRUE;
	 *      }*/

	// Re-aim the missle
	theta = atan2((sprites[sprite_number].y_position_in_pixels + 4) - (player.y_position_in_pixels + 16), (sprites[sprite_number].x_position_in_pixels + 8) - (player.x_position_in_pixels + 16));
	sprites[sprite_number].x_acceleration = -.85 * cos(theta);
	sprites[sprite_number].y_acceleration = -.85 * sin(theta);

	// Cap the missile's velocity
	/*if (sprites[sprite_number].x_acceleration > .85) sprites[sprite_number].x_acceleration = .85;
	 * if (sprites[sprite_number].x_acceleration < -.85) sprites[sprite_number].x_acceleration = -.85;
	 * if (sprites[sprite_number].y_acceleration > .85) sprites[sprite_number].y_acceleration = .85;
	 * if (sprites[sprite_number].y_acceleration < -.85) sprites[sprite_number].y_acceleration = -.85;*/

	// Rotate the missile
	theta = theta * 180 / 3.14159265;
	theta += 180;
	if (theta < 0) theta += 360;
	while (theta > 360) theta -= 360; sprites[sprite_number].rotate_factor = (unsigned char)(theta * 256 / 360);
	if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;

	// Move the missile
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Stardust smoke trail
	if (out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 255, 240, 4, 1);

	// Check to see if the missile hit the player and take appropriate action if so
	if (sprite_is_touching_player(sprite_number) == TRUE) sprite_busted = TRUE;

	// If the missile has hit a wall, make it 'poof out'
	if (sprite_busted == FALSE && check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1) == TRUE) {
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_stardust(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 255, 240, 4, 16);
		spawn_stardust(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 2, 6, 4, 32);
		destroy_sprite(sprite_number);
		return;
	}

	// If the missile hit the player or was shot down, make it explode
	if (sprite_busted == TRUE) {
		for (spawn_loop = 0; spawn_loop < 4; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 8, 0, 0, 0);
		spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 8, 0, 0, 0);
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		destroy_sprite(sprite_number);
		return;
	}
}

/*****************\
|** FIREWORK AI **|
\*****************/
void firework_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;
	double theta;

	// Count down to the explosion
	if (sprites[sprite_number].timer <= 0) {
		if (sprites[sprite_number].action_state == 1) {
			sprite_busted = TRUE;
		} else {
			play_relative_sound(FIREWORK_LAUNCH_SOUND, sprite_number);
			sprites[sprite_number].action_state = 1;
			sprites[sprite_number].timer = 128 + rand() % 128;
		}
	} else {
		sprites[sprite_number].timer--;
	}

	// Animate the firework
	if (sprites[sprite_number].action_state == 0) {
		if (sprites[sprite_number].timer < 64) {
			if (sprites[sprite_number].frame >= BOTTLE_ROCKET_FLY && sprites[sprite_number].frame <= BOTTLE_ROCKET_REST) sprites[sprite_number].frame = BOTTLE_ROCKET_FUSE_04 - sprites[sprite_number].timer / 16;
			if (sprites[sprite_number].frame >= FIREWORK_FLY && sprites[sprite_number].frame <= FIREWORK_REST) sprites[sprite_number].frame = FIREWORK_FUSE_04 - sprites[sprite_number].timer / 16;
		} else {
			if (sprites[sprite_number].frame >= BOTTLE_ROCKET_FLY && sprites[sprite_number].frame <= BOTTLE_ROCKET_REST) sprites[sprite_number].frame = BOTTLE_ROCKET_REST;
			if (sprites[sprite_number].frame >= FIREWORK_FLY && sprites[sprite_number].frame <= FIREWORK_REST) sprites[sprite_number].frame = FIREWORK_REST;
		}
	} else {
		if (sprites[sprite_number].frame >= BOTTLE_ROCKET_FLY && sprites[sprite_number].frame <= BOTTLE_ROCKET_REST) sprites[sprite_number].frame = BOTTLE_ROCKET_FLY;
		if (sprites[sprite_number].frame >= FIREWORK_FLY && sprites[sprite_number].frame <= FIREWORK_REST) sprites[sprite_number].frame = FIREWORK_FLY;
	}

	if (sprites[sprite_number].action_state == 1) {
		// Destroy the firework if it was shot down
		if (sprites[sprite_number].hit_points_left <= 0) {
			player.score++;
			sprite_busted = TRUE;
		}

		// Re-aim the firework
		theta = atan2(sprites[sprite_number].y_acceleration, sprites[sprite_number].x_acceleration) + 3.14159265;
		//theta = atan2((sprites[sprite_number].y_position_in_pixels + 4) - (player.y_position_in_pixels + 16), (sprites[sprite_number].x_position_in_pixels + 8) - (player.x_position_in_pixels + 16));
		if (sprites[sprite_number].animation_state * 3.14159265 / 180 > theta) theta += .001;
		else if (sprites[sprite_number].animation_state * 3.14159265 / 180 < theta) theta -= .001;
		sprites[sprite_number].x_acceleration = -.95 * cos(theta);
		sprites[sprite_number].y_acceleration = -.95 * sin(theta);

		// Rotate the firework
		theta = theta * 180 / 3.14159265;
		theta += 270;
		if (theta < 0) theta += 360;
		while (theta > 360) theta -= 360; sprites[sprite_number].rotate_factor = (unsigned char)(theta * 256 / 360);
		if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;

		// Move the firework
		sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
		sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

		// Stardust smoke trail
		if (out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, 2, 6, 4, 1);

		// Check to see if the firework hit the player or a wall
		//if (sprite_is_touching_player(sprite_number) == TRUE) sprite_busted = TRUE;
		if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), 1) == TRUE) sprite_busted = TRUE;

		// If the firework hit the player or was shot down, make it explode
		if (sprite_busted == TRUE) {
			for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
			for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 20) - 10.00) / 10.00, (float)((rand() % 20) - 10.00) / 10.00, 0);
			spawn_stardust(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 255, 240, 4, 16);
			spawn_stardust(sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 8, 0, 0, 2, 6, 4, 32);
			spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 8, sprites[sprite_number].y_position_in_pixels, 0, 0, 0);

			// Choose a color
			theta = (int)(rand() % 13);
			for (spawn_loop = 0; spawn_loop < 24; spawn_loop++) spawn_sprite(ARCSPARK_SPRITE, (sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height / 2), 0, 0, (38 + theta * 16));

			play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
			destroy_sprite(sprite_number);
			return;
		}
	}
}

/*********************\
|** GIANT ROCKET AI **|
\*********************/
void giant_rocket_ai(int sprite_number)
{
	char sprite_busted = FALSE;
	unsigned char spawn_loop;

	// Do the big explosions and stuff
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			destroy_sprite(sprite_number);
			return;
		} else {
			if (sprites[sprite_number].timer / 4.0 == (int)(sprites[sprite_number].timer / 4)) {
				if (rand() % 10 > 4) spawn_sprite(MEDIUM_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels + ((float)((rand() % 1920) - 960.00) / 20.00), sprites[sprite_number].y_position_in_pixels - 8 + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
				else spawn_sprite(SMALL_EXPLOSION_SPRITE, sprites[sprite_number].x_position_in_pixels - 8 + ((float)((rand() % 1920) - 960.00) / 20.00), sprites[sprite_number].y_position_in_pixels + ((float)((rand() % 960) - 240.00) / 20.00), 0, 0, 0);
			}

			sprites[sprite_number].timer--;
			return;
		}
	}

	// Animate the missile
	if (sprites[sprite_number].action_state == 1) {
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 64;
			if (sprites[sprite_number].frame > GIANT_ROCKET_LAUNCH_05) {
				sprites[sprite_number].frame = GIANT_ROCKET_FLY_01;
				sprites[sprite_number].timer = 16;
				sprites[sprite_number].y_acceleration = -.05;
				sprites[sprite_number].action_state = 2;
				play_relative_sound(MISSILE_LAUNCH_SOUND, sprite_number);
				quake_timer += 32;
				if (quake_timer > 512) quake_timer = 512;
			}
		} else {
			sprites[sprite_number].timer--;
		}
	} else if (sprites[sprite_number].action_state == 2) {
		sprites[sprite_number].y_acceleration -= .02;
		if (sprites[sprite_number].y_acceleration < -2) sprites[sprite_number].y_acceleration = -2;
		if (sprites[sprite_number].timer == 0) {
			sprites[sprite_number].frame++;
			sprites[sprite_number].timer = 16;
			if (sprites[sprite_number].frame > GIANT_ROCKET_FLY_04) sprites[sprite_number].frame = GIANT_ROCKET_FLY_01;
		} else {
			sprites[sprite_number].timer--;
		}
	}

	// Launch the rocket if it is shot at
	if (sprites[sprite_number].hit_points_left <= 0 && sprites[sprite_number].action_state == 0) {
		player.score++;
		sprites[sprite_number].action_state = 1;
		sprites[sprite_number].frame = GIANT_ROCKET_LAUNCH_01;
		sprites[sprite_number].timer = 64;
		play_relative_sound(MISSILE_ARM_SOUND, sprite_number);
	}

	// Move the firework
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;

	// Stardust smoke trail
	if (sprites[sprite_number].action_state == 2 && out_of_player_range(sprite_number) == FALSE) spawn_stardust((sprites[sprite_number].x_position_in_pixels + sprites[sprite_number].width / 2), (sprites[sprite_number].y_position_in_pixels + sprites[sprite_number].height), 0, 0, 2, 6, 4, 1);

	// Check to see if the rocket hit a wall
	if (check_point_collisions_grid(sprite_number, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels, 1) == TRUE) sprite_busted = TRUE;

	// If the rocket hit a wall, blow it up
	if (sprite_busted == TRUE) {
		for (spawn_loop = 0; spawn_loop < 32; spawn_loop++) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2), sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2), (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		spawn_sprite(MUSHROOM_CLOUD_SPRITE, sprites[sprite_number].x_position_in_pixels - 56, sprites[sprite_number].y_position_in_pixels - 16, 0, 0, 1);
		sprites[sprite_number].frame = 0;
		sprites[sprite_number].timer = 512;
		sprites[sprite_number].hit_points_left = -666;
		play_relative_sound(ROCKET_EXPLODE_SOUND, sprite_number);
		return;
	}
}
