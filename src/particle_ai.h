/*********************\
|** FLYUP SPRITE AI **|
\*********************/
void flyup_sprite_ai(int sprite_number)
{
	// See if the sprite is getting old
	sprites[sprite_number].timer--;
	if (sprites[sprite_number].timer == 0) {
		destroy_sprite(sprite_number);
		return;
	}

	// Rotate the fly up sprite
	if (sprites[sprite_number].rotate_factor == 254) sprites[sprite_number].rotate_factor = 0;
	else sprites[sprite_number].rotate_factor += 2;

	// Move the sprite
	sprites[sprite_number].y_position_in_pixels -= 0.25;
}

/********************\
|** SMALL SPARK AI **|
\********************/
void small_spark_ai(int sprite_number)
{
	// Decrease the spark timer
	sprites[sprite_number].timer--;

	// Do some time checks and change frame
	if (sprites[sprite_number].timer == 0) {
		if (sprites[sprite_number].sprite_type == SMALL_WHITE_SPARK_SPRITE) {
			if (sprites[sprite_number].frame == SMALL_WHITE_SPARK_01) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_WHITE_SPARK_02;
			else if (sprites[sprite_number].frame == SMALL_WHITE_SPARK_02) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_WHITE_SPARK_03;
			else destroy_sprite(sprite_number);
			return;
		} else if (sprites[sprite_number].sprite_type == SMALL_BLUE_SPARK_SPRITE) {
			if (sprites[sprite_number].frame == SMALL_BLUE_SPARK_01) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_BLUE_SPARK_02;
			else if (sprites[sprite_number].frame == SMALL_BLUE_SPARK_02) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_BLUE_SPARK_03;
			else destroy_sprite(sprite_number);
			return;
		} else if (sprites[sprite_number].sprite_type == SMALL_RED_SPARK_SPRITE) {
			if (sprites[sprite_number].frame == SMALL_RED_SPARK_01) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_RED_SPARK_02;
			else if (sprites[sprite_number].frame == SMALL_RED_SPARK_02) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_RED_SPARK_03;
			else destroy_sprite(sprite_number);
			return;
		} else if (sprites[sprite_number].sprite_type == SMALL_GOLD_SPARK_SPRITE) {
			if (sprites[sprite_number].frame == SMALL_GOLD_SPARK_01) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_GOLD_SPARK_02;
			else if (sprites[sprite_number].frame == SMALL_GOLD_SPARK_02) sprites[sprite_number].timer = 32, sprites[sprite_number].frame = SMALL_GOLD_SPARK_03;
			else destroy_sprite(sprite_number);
			return;
		}
	}

	// Gravity effects sparks
	sprites[sprite_number].y_acceleration += 0.025;

	// Move the sparks
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
}

/*********************\
|** WATER BUBBLE AI **|
\*********************/
void water_bubble_ai(int sprite_number)
{
	// Check the timer and pop the bubble if nessicary
	if (sprites[sprite_number].hit_points_left == -666) {
		if (sprites[sprite_number].timer == 0) {
			destroy_sprite(sprite_number);
			return;
		} else {
			sprites[sprite_number].timer--;
		}
	}
	// Check to see if the bubble is still in the water
	else if (worminator_map.info_tile_grid[(int)(sprites[sprite_number].x_position_in_pixels) / 16][(int)(sprites[sprite_number].y_position_in_pixels) / 16] != water_info_tile) {
		sprites[sprite_number].hit_points_left = -666, sprites[sprite_number].timer = 64;
	}

	// Move the bubble around
	sprites[sprite_number].y_position_in_pixels -= .5;
	sprites[sprite_number].x_position_in_pixels += ((rand() % 40) - 20) / 20.0;
}

/*********************\
|** WATER SPLASH AI **|
\*********************/
void water_splash_ai(int sprite_number)
{
	unsigned char spawn_loop;

	// If the sprite was just spawned, make some water droplets
	if (sprites[sprite_number].timer == 8 && sprites[sprite_number].frame == WATER_SPLASH_01) for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) spawn_sprite(SMALL_BLUE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 16, sprites[sprite_number].y_position_in_pixels + 16, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);

	// Check the timer and change the frame or destroy the sprite if nessicary
	if (sprites[sprite_number].timer == 0) {
		if (sprites[sprite_number].frame == WATER_SPLASH_16) {
			destroy_sprite(sprite_number);
			return;
		} else {
			sprites[sprite_number].timer = 8, sprites[sprite_number].frame++;
		}
	} else {
		sprites[sprite_number].timer--;
	}
}

/*****************\
|** SHRAPGIB AI **|
\*****************/
void shrapgib_ai(int sprite_number)
{
	// Update the timer and destroy sprite if time is up
	sprites[sprite_number].timer--;
	if (sprites[sprite_number].timer == 0) {
		destroy_sprite(sprite_number);
		return;
	}

	// Rotate the gib or shrapnel
	if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;
	else sprites[sprite_number].rotate_factor++;

	// Gravity effects shrapnel
	sprites[sprite_number].y_acceleration += 0.025;

	// Move the shrapnel
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
}

/******************************\
|** BASIC SPINNING SPRITE AI **|
\******************************/
void basic_spinning_sprite_ai(int sprite_number)
{
	// Rotate the fly up sprite
	if (sprites[sprite_number].rotate_factor == 255) sprites[sprite_number].rotate_factor = 0;
	else sprites[sprite_number].rotate_factor++;
}

/**************************\
|** STARDUST PARTICLE AI **|
\**************************/
void stardust_particle_ai(int sprite_number)
{
	/* Special stardust info:
	 *      frame - current color pallete index
	 *      rotate_factor - final color
	 *      action_state - movement path type (gravity, no gravity, random horizontal, random vertical, total random, sine wave horizontal, sine wave)
	 *      direction - used for movement path state */

	// Do some time checks and change frame
	if (sprites[sprite_number].timer == 0) {
		// Change the color as nessicary
		if (sprites[sprite_number].rotate_factor > sprites[sprite_number].frame) sprites[sprite_number].frame++;
		else if (sprites[sprite_number].rotate_factor < sprites[sprite_number].frame) sprites[sprite_number].frame--;
		if (sprites[sprite_number].frame == sprites[sprite_number].rotate_factor) {
			destroy_sprite(sprite_number);
			return;
		}
		sprites[sprite_number].timer = 8;
	} else {
		sprites[sprite_number].timer--;
	}

	// If gravity effects sparks, then apply it.  Also, handle all the special movement path code here
	if (sprites[sprite_number].action_state == 0) {
		sprites[sprite_number].y_acceleration += 0.025;
	} else if (sprites[sprite_number].action_state == 2) {
		sprites[sprite_number].x_acceleration = (float)((rand() % 20) - 10.00) / 10.00;
	} else if (sprites[sprite_number].action_state == 3) {
		sprites[sprite_number].y_acceleration = (float)((rand() % 20) - 10.00) / 10.00;
	} else if (sprites[sprite_number].action_state == 4) {
		sprites[sprite_number].x_acceleration = (float)((rand() % 20) - 10.00) / 10.00;
		sprites[sprite_number].y_acceleration = (float)((rand() % 20) - 10.00) / 10.00;
	} else if (sprites[sprite_number].action_state == 5) {
		sprites[sprite_number].x_acceleration = fixtof(fsin(itofix(sprites[sprite_number].direction)));
		if (sprites[sprite_number].direction == -1) sprites[sprite_number].direction = 0;
		else sprites[sprite_number].direction += 8;
	} else if (sprites[sprite_number].action_state == 6) {
		sprites[sprite_number].y_acceleration = fixtof(fsin(itofix(sprites[sprite_number].direction)));
		if (sprites[sprite_number].direction == -1) sprites[sprite_number].direction = 0;
		else sprites[sprite_number].direction += 8;
	}

	// Move the particle
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration;
}
