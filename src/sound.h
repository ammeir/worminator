/* -- Sound.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * For function descriptions and explanations, see proto.h
 * For a detailed explanation of how the engine works, see
 * the documents that came with this source code package. */
void play_relative_sound(int sound_number, int sprite_number)
{
	unsigned char volume, sound_direction;
	int player_center_x, player_center_y;
	int sprite_center_x, sprite_center_y;

	player_center_x = player.x_position_in_pixels + 16;
	player_center_y = player.y_position_in_pixels + 16;

	sprite_center_x = sprites[sprite_number].x_position_in_pixels + (sprites[sprite_number].width / 2);
	sprite_center_y = sprites[sprite_number].y_position_in_pixels + (sprites[sprite_number].height / 2);

	// Check to see if the sprite is in range of the player
	if (abs(player_center_x - sprite_center_x) <= 384 && abs(player_center_y - sprite_center_y) <= 384) {
		volume = (abs(player_center_x - sprite_center_x) + abs(player_center_y - sprite_center_y)) / 2;
		volume = (volume * -1 * 2 / 3) + 255;
		sound_direction = ((sprite_center_x - player_center_x) / 2) * 2 / 3 + 128;
		if (player.in_water) play_sample(worminator_data_file[sound_number].dat, volume, sound_direction, 750, FALSE);
		else play_sample(worminator_data_file[sound_number].dat, volume, sound_direction, 1000, FALSE);
	}
}
