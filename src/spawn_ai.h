/**********************\
|** SPARK SPAWNER AI **|
\**********************/
void spark_spawner_ai(int sprite_number)
{
	unsigned char spawn_loop;

	if (sprites[sprite_number].timer <= 0) {
		sprites[sprite_number].timer = (rand() % 1024) + 128;
		for (spawn_loop = 0; spawn_loop < 8; spawn_loop++) {
			if (sprites[sprite_number].sprite_type == BLUE_SPARK_SPAWNER_SPRITE) spawn_sprite(SMALL_BLUE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			else if (sprites[sprite_number].sprite_type == RED_SPARK_SPAWNER_SPRITE) spawn_sprite(SMALL_RED_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			else if (sprites[sprite_number].sprite_type == WHITE_SPARK_SPAWNER_SPRITE) spawn_sprite(SMALL_WHITE_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
			else spawn_sprite(SMALL_GOLD_SPARK_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
		}
	} else {
		sprites[sprite_number].timer--;
	}
}

/***************************\
|** WATER DROP SPAWNER AI **|
\***************************/
void water_drop_spawner_ai(int sprite_number)
{
	if (sprites[sprite_number].timer <= 0) {
		sprites[sprite_number].timer = (rand() % 1024) + 128;
		spawn_sprite(WATER_DROP_SPRITE, sprites[sprite_number].x_position_in_pixels + 6, sprites[sprite_number].y_position_in_pixels, 0, 0, 0);
	} else {
		sprites[sprite_number].timer--;
	}
}

/*****************************\
|** NUCLEAR BOMB SPAWNER AI **|
\*****************************/
void nuclear_bomb_spawner_ai(int sprite_number)
{
	if (sprites[sprite_number].timer <= 0) {
		sprites[sprite_number].timer = (rand() % 1024) + 896;
		if (rand() % 16 < 8) spawn_sprite(BIG_NUCLEAR_BOMB_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, 0);
		else spawn_sprite(SMALL_NUCLEAR_BOMB_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels, 0, 0, 0);
	} else {
		sprites[sprite_number].timer--;
	}
}

/*****************\
|** ARCSPARK AI **|
\*****************/
void arcspark_ai(int sprite_number)
{
	if (sprites[sprite_number].timer % 2 == 0) spawn_stardust(sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels, 0, 0, sprites[sprite_number].direction, sprites[sprite_number].direction + 4, 1, 1);
	sprites[sprite_number].timer++;

	// Move the arcspark
	if (sprites[sprite_number].action_state == 0) sprites[sprite_number].y_acceleration += 0.025;	// Move the particle
	sprites[sprite_number].x_position_in_pixels += sprites[sprite_number].x_acceleration + (float)((rand() % 7) - 4.00) / 10.00;
	sprites[sprite_number].y_position_in_pixels += sprites[sprite_number].y_acceleration + (float)((rand() % 7) - 4.00) / 10.00;

	// Destory the arcspark after a bit
	if (sprites[sprite_number].timer >= 128) destroy_sprite(sprite_number);
}

/*******************************\
|** CIGARETTE BUTT SPAWNER AI **|
\*******************************/
void cigarette_butt_spawner_ai(int sprite_number)
{
	if (sprites[sprite_number].timer <= 0) {
		sprites[sprite_number].timer = (rand() % 384) + 128;
		spawn_sprite(CIGARETTE_BUTT_ENEMY_SPRITE, sprites[sprite_number].x_position_in_pixels + 8, sprites[sprite_number].y_position_in_pixels + 8, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00 - 1, 0);
	} else {
		sprites[sprite_number].timer--;
	}
}

/*************************\
|** FIREWORK SPAWNER AI **|
\*************************/
void firework_spawner_ai(int sprite_number)
{
	if (sprites[sprite_number].timer <= 0) {
		sprites[sprite_number].timer = (rand() % 1024) + 1024;
		if (sprites[sprite_number].sprite_type == FIREWORK_SPAWNER_SPRITE) spawn_sprite(FIREWORK_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 16, 0, 0, 0);
		else spawn_sprite(BOTTLE_ROCKET_SPRITE, sprites[sprite_number].x_position_in_pixels, sprites[sprite_number].y_position_in_pixels - 16, 0, 0, 0);
	} else {
		sprites[sprite_number].timer--;
	}
}
