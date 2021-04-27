/* -- Player.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by David Layne
 *
 * For function descriptions and explanations, see proto.h
 * For a detailed explanation of how the engine works, see
 * the documents that came with this source code package. */

/*****************************\
|** CHECK WEAPON AUTOSWITCH **|
\*****************************/
int check_weapon_autoswitch()
{
	if (override_autoswitch) return -666;

	switch (player.current_weapon) {
	case 1:
		if (player.ammo_type == 0 && player.bullets_left == 0) return best_weapon_safe();
		else if (player.ammo_type == 1 && player.plasma_bullets_left == 0) return best_weapon_safe();
		break;
	case 2:
		if (player.ammo_type == 0 && player.shells_left == 0) return best_weapon_safe();
		else if (player.ammo_type == 1 && player.explosive_shells_left == 0) return best_weapon_safe();
		break;
	case 3:
		if (player.ammo_type == 0 && player.rockets_left == 0) return best_weapon_safe();
		else if (player.ammo_type == 1 && player.micro_nukes_left == 0) return best_weapon_safe();
		break;
	case 4:
		if (player.rockets_left == 0) return best_weapon_safe();
		break;
	case 5:
		if (player.cells_left == 0) return best_weapon_safe();
		break;
	case 6:
		if (player.gasoline_left == 0) return best_weapon_safe();
		break;
	case 7:
		if (player.mortars_left == 0) return best_weapon_safe();
		break;
	case 8:
		if (player.plasma_left == 0) return best_weapon_safe();
		break;
	case 9:
		if (player.ammo_type == 0 && player.mines_left == 0) return best_weapon_safe();
		else if (player.ammo_type == 1 && player.walking_mines_left == 0) return best_weapon_safe();
		break;
	case 10:
		if (player.chainsaw_duration_left == 0) return best_weapon_safe();
		break;
	case 11:
		if (player.rails_left == 0) return best_weapon_safe();
		break;
	case 12:
		if (player.spikestars_left == 0) return best_weapon_safe();
		break;
	}
	return -666;
}

/**********************\
|** BEST WEAPON SAFE **|
\**********************/
int best_weapon_safe()
{
	char done = FALSE;
	int new_weap = 13;

	do {
		new_weap--;
		if (new_weap == 0) return 0;
		switch (new_weap) {
		case 1:
			if (player.plasma_left > 0) return 8;
			break;
		case 2:
			if (player.mines_left > 0) return 9;
			break;
		case 3:
			if (player.gasoline_left > 0) return 6;
			break;
		case 4:
			if (player.chainsaw_duration_left > 0) return 10;
			break;
		case 5:
			if (player.rockets_left > 0) return 4;
			break;
		case 6:
			if (player.bullets_left > 0) return 1;
			break;
		case 7:
			if (player.shells_left > 0) return 2;
			break;
		case 8:
			if (player.rockets_left > 0) return 3;
			break;
		case 9:
			if (player.cells_left > 0) return 5;
			break;
		case 10:
			if (player.mortars_left > 0) return 7;
			break;
		case 11:
			if (player.rails_left > 0) return 11;
			break;
		case 12:
			if (player.spikestars_left > 0) return 12;
			break;
		}
	} while (done == FALSE);

	return 0;
}

/*****************\
|** BEST WEAPON **|
\*****************/
int best_weapon()
{
	char done = FALSE;
	int new_weap = 13;

	do {
		new_weap--;
		//if (new_weap < 1) new_weap = 12;
		if (new_weap == 0) return 0;
		switch (new_weap) {
		case 1:
			if (player.bullets_left > 0) return new_weap;
			break;
		case 2:
			if (player.shells_left > 0) return new_weap;
			break;
		case 3:
			if (player.rockets_left > 0) return new_weap;
			break;
		case 4:
			if (player.rockets_left > 0) return new_weap;
			break;
		case 5:
			if (player.cells_left > 0) return new_weap;
			break;
		case 6:
			if (player.gasoline_left > 0) return new_weap;
			break;
		case 7:
			if (player.mortars_left > 0) return new_weap;
			break;
		case 8:
			if (player.plasma_left > 0) return new_weap;
			break;
		case 9:
			if (player.mines_left > 0) return new_weap;
			break;
		case 10:
			if (player.chainsaw_duration_left > 0) return new_weap;
			break;
		case 11:
			if (player.rails_left > 0) return new_weap;
			break;
		case 12:
			if (player.spikestars_left > 0) return new_weap;
			break;
		}
	} while (done == FALSE);

	return 0;
}

/*****************\
|** LAST WEAPON **|
\*****************/
int last_weapon()
{
	char done = FALSE;
	int new_weap = player.current_weapon;

	override_autoswitch = TRUE;

	do {
		new_weap--;
		if (new_weap < 0) new_weap = 12;
		if (new_weap == player.current_weapon) return 0;
		switch (new_weap) {
		case 1:
			if (player.bullets_left > 0) return new_weap;
			break;
		case 2:
			if (player.shells_left > 0) return new_weap;
			break;
		case 3:
			if (player.rockets_left > 0) return new_weap;
			break;
		case 4:
			if (player.rockets_left > 0) return new_weap;
			break;
		case 5:
			if (player.cells_left > 0) return new_weap;
			break;
		case 6:
			if (player.gasoline_left > 0) return new_weap;
			break;
		case 7:
			if (player.mortars_left > 0) return new_weap;
			break;
		case 8:
			if (player.plasma_left > 0) return new_weap;
			break;
		case 9:
			if (player.mines_left > 0) return new_weap;
			break;
		case 10:
			if (player.chainsaw_duration_left > 0) return new_weap;
			break;
		case 11:
			if (player.rails_left > 0) return new_weap;
			break;
		case 12:
			if (player.spikestars_left > 0) return new_weap;
			break;
		}
	} while (done == FALSE);

	return 0;
}

/*****************\
|** NEXT WEAPON **|
\*****************/
int next_weapon()
{
	char done = FALSE;
	int new_weap = player.current_weapon;

	override_autoswitch = TRUE;

	do {
		new_weap++;
		if (new_weap > 12) new_weap = 0;
		if (new_weap == player.current_weapon) return 0;
		switch (new_weap) {
		case 1:
			if (player.bullets_left > 0) return new_weap;
			break;
		case 2:
			if (player.shells_left > 0) return new_weap;
			break;
		case 3:
			if (player.rockets_left > 0) return new_weap;
			break;
		case 4:
			if (player.rockets_left > 0) return new_weap;
			break;
		case 5:
			if (player.cells_left > 0) return new_weap;
			break;
		case 6:
			if (player.gasoline_left > 0) return new_weap;
			break;
		case 7:
			if (player.mortars_left > 0) return new_weap;
			break;
		case 8:
			if (player.plasma_left > 0) return new_weap;
			break;
		case 9:
			if (player.mines_left > 0) return new_weap;
			break;
		case 10:
			if (player.chainsaw_duration_left > 0) return new_weap;
			break;
		case 11:
			if (player.rails_left > 0) return new_weap;
			break;
		case 12:
			if (player.spikestars_left > 0) return new_weap;
			break;
		}
	} while (done == FALSE);

	return 0;
}

/*****************\
|** PUSH PLAYER **|
\*****************/
void push_player(float x_amount, float y_amount)
{
	unsigned char info_value_1, info_value_2, info_value_3;
	int x_rel, y_rel;

	if (player.no_control == TRUE) return;	// If the player has no control, we shouldnt be here

	// Drop down test
	if (x_amount == 512 && y_amount == -512) {
		if (player.on_ground == FALSE || player.skin == TANK_SKIN) return;
		player.y_position_in_pixels++;
		info_value_1 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 4) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];
		info_value_2 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];
		info_value_3 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 27) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];
		if ((info_value_1 == solid_from_top_allow_ammo_info_tile || info_value_2 == solid_from_top_allow_ammo_info_tile || info_value_3 == solid_from_top_allow_ammo_info_tile) || (info_value_1 == solid_from_top_no_ammo_info_tile || info_value_2 == solid_from_top_no_ammo_info_tile || info_value_3 == solid_from_top_no_ammo_info_tile)) {
			if (info_value_1 == solid_info_tile || info_value_2 == solid_info_tile || info_value_3 == solid_info_tile) player.y_position_in_pixels--;
			else if (info_value_1 == solid_from_top_no_drop_info_tile || info_value_2 == solid_from_top_no_drop_info_tile || info_value_3 == solid_from_top_no_drop_info_tile) player.y_position_in_pixels--;
			else if (info_value_1 == painful_from_top_info_tile || info_value_2 == painful_from_top_info_tile || info_value_3 == painful_from_top_info_tile) player.y_position_in_pixels--;
			else if (info_value_1 == painful_from_bottom_info_tile || info_value_2 == painful_from_bottom_info_tile || info_value_3 == painful_from_bottom_info_tile) player.y_position_in_pixels--;
			else if (info_value_1 == breakable_wall_info_tile || info_value_2 == breakable_wall_info_tile || info_value_3 == breakable_wall_info_tile) player.y_position_in_pixels--;
			else player.on_ground = FALSE;
		} else {
			(player.y_position_in_pixels--);
		}
		return;
	}

	if (x_amount > 1) x_amount = 1;
	else if (x_amount < -1) x_amount = -1;
	if (y_amount > 1) y_amount = 1;
	else if (y_amount < -1) y_amount = -1;

	// Right
	if (x_amount > 0) {
		player.x_position_in_pixels += x_amount;
		if (player.x_position_in_pixels > 4064) player.x_position_in_pixels = 4064;
		if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_left_to_right_45_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 15 - x_rel) {
				player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_left_to_right_225_left_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 15 - x_rel / 2) {
				player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_left_to_right_225_right_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 7 - x_rel / 2) {
				player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
			}
		}

		if (player.skin == TANK_SKIN) {
			if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_left_to_right_45_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 15 - x_rel) {
					player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_left_to_right_225_left_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 15 - x_rel / 2) {
					player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_left_to_right_225_right_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 7 - x_rel / 2) {
					player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
				}
			}
		}

		info_value_1 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 27) / 16)][(int)((player.y_position_in_pixels + 2) / 16)];
		info_value_2 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 27) / 16)][(int)((player.y_position_in_pixels + 15) / 16)];
		info_value_3 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 27) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];

		if (info_value_1 == blank_info_tile && info_value_2 == blank_info_tile && info_value_3 == blank_info_tile) return;

		if (info_value_1 == solid_info_tile || info_value_2 == solid_info_tile || info_value_3 == solid_info_tile) player.x_position_in_pixels -= x_amount;
		else if (player.skin == TANK_SKIN && (info_value_1 == tank_solid_info_tile || info_value_2 == tank_solid_info_tile || info_value_3 == tank_solid_info_tile)) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == solid_from_left_allow_ammo_info_tile || info_value_2 == solid_from_left_allow_ammo_info_tile || info_value_3 == solid_from_left_allow_ammo_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == solid_from_left_no_ammo_info_tile || info_value_2 == solid_from_left_no_ammo_info_tile || info_value_3 == solid_from_left_no_ammo_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == painful_from_top_info_tile || info_value_2 == painful_from_top_info_tile || info_value_3 == painful_from_top_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == painful_from_bottom_info_tile || info_value_2 == painful_from_bottom_info_tile || info_value_3 == painful_from_bottom_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == breakable_wall_info_tile || info_value_2 == breakable_wall_info_tile || info_value_3 == breakable_wall_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == conveyor_belt_right_info_tile || info_value_2 == conveyor_belt_right_info_tile || info_value_3 == conveyor_belt_right_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == conveyor_belt_left_info_tile || info_value_2 == conveyor_belt_left_info_tile || info_value_3 == conveyor_belt_left_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == half_slippery_info_tile || info_value_2 == half_slippery_info_tile || info_value_3 == half_slippery_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == mostly_slippery_info_tile || info_value_2 == mostly_slippery_info_tile || info_value_3 == mostly_slippery_info_tile) player.x_position_in_pixels -= x_amount;

		player_touched((player.x_position_in_pixels + 27) / 16, (player.y_position_in_pixels + 2) / 16);
		player_touched((player.x_position_in_pixels + 27) / 16, (player.y_position_in_pixels + 15) / 16);
		player_touched((player.x_position_in_pixels + 27) / 16, (player.y_position_in_pixels + 31) / 16);
		return;
	}
	// Left
	else if (x_amount < 0) {
		player.x_position_in_pixels += x_amount;
		if (player.x_position_in_pixels < 0) player.x_position_in_pixels = 0;
		if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_right_to_left_45_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= x_rel) {
				player.y_position_in_pixels -= 1; player.on_ground = TRUE;	/*return;*/
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_right_to_left_225_left_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= x_rel / 2) {
				player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_right_to_left_225_right_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 7 + x_rel / 2) {
				player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
			}
		}

		if (player.skin == TANK_SKIN) {
			if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_right_to_left_45_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= x_rel) {
					player.y_position_in_pixels -= 1; player.on_ground = TRUE;	/*return;*/
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_right_to_left_225_left_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= x_rel / 2) {
					player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_right_to_left_225_right_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 7 + x_rel / 2) {
					player.y_position_in_pixels -= 1; player.on_ground = TRUE;		/*return;*/
				}
			}
		}

		info_value_1 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 4) / 16)][(int)((player.y_position_in_pixels + 2) / 16)];
		info_value_2 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 4) / 16)][(int)((player.y_position_in_pixels + 15) / 16)];
		info_value_3 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 4) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];

		if (info_value_1 == blank_info_tile && info_value_2 == blank_info_tile && info_value_3 == blank_info_tile) return;

		if (info_value_1 == solid_info_tile || info_value_2 == solid_info_tile || info_value_3 == solid_info_tile) player.x_position_in_pixels -= x_amount;
		else if (player.skin == TANK_SKIN && (info_value_1 == tank_solid_info_tile || info_value_2 == tank_solid_info_tile || info_value_3 == tank_solid_info_tile)) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == solid_from_right_allow_ammo_info_tile || info_value_2 == solid_from_right_allow_ammo_info_tile || info_value_3 == solid_from_right_allow_ammo_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == solid_from_right_no_ammo_info_tile || info_value_2 == solid_from_right_no_ammo_info_tile || info_value_3 == solid_from_right_no_ammo_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == painful_from_top_info_tile || info_value_2 == painful_from_top_info_tile || info_value_3 == painful_from_top_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == painful_from_bottom_info_tile || info_value_2 == painful_from_bottom_info_tile || info_value_3 == painful_from_bottom_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == breakable_wall_info_tile || info_value_2 == breakable_wall_info_tile || info_value_3 == breakable_wall_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == conveyor_belt_right_info_tile || info_value_2 == conveyor_belt_right_info_tile || info_value_3 == conveyor_belt_right_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == conveyor_belt_left_info_tile || info_value_2 == conveyor_belt_left_info_tile || info_value_3 == conveyor_belt_left_info_tile) player.x_position_in_pixels -= x_amount;

		else if (info_value_1 == half_slippery_info_tile || info_value_2 == half_slippery_info_tile || info_value_3 == half_slippery_info_tile) player.x_position_in_pixels -= x_amount;
		else if (info_value_1 == mostly_slippery_info_tile || info_value_2 == mostly_slippery_info_tile || info_value_3 == mostly_slippery_info_tile) player.x_position_in_pixels -= x_amount;

		player_touched((player.x_position_in_pixels + 4) / 16, (player.y_position_in_pixels + 2) / 16);
		player_touched((player.x_position_in_pixels + 4) / 16, (player.y_position_in_pixels + 15) / 16);
		player_touched((player.x_position_in_pixels + 4) / 16, (player.y_position_in_pixels + 31) / 16);
		return;
	}
	// Down
	else if (y_amount > 0) {
		player.y_position_in_pixels += y_amount;
		if (player.y_position_in_pixels > 3040) player.y_position_in_pixels = 3040, player.on_ground = TRUE;
		if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_left_to_right_45_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 15 - x_rel) {
				player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
			} else {
				return;
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_left_to_right_225_left_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 15 - x_rel / 2) {
				player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
			} else {
				return;
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_left_to_right_225_right_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 7 - x_rel / 2) {
				player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
			} else {
				return;
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_right_to_left_45_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= x_rel) {
				player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
			} else {
				return;
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_right_to_left_225_left_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= x_rel / 2) {
				player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
			} else {
				return;
			}
		} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == slope_up_right_to_left_225_right_info_tile) {
			x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
			y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
			if (y_rel >= 8 + x_rel / 2) {
				player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
			} else {
				return;
			}
		}

		if (player.skin == TANK_SKIN) {
			if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_left_to_right_45_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 15 - x_rel) {
					player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
				} else {
					return;
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_left_to_right_225_left_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 15 - x_rel / 2) {
					player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
				} else {
					return;
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_left_to_right_225_right_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 7 - x_rel / 2) {
					player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
				} else {
					return;
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_right_to_left_45_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= x_rel) {
					player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
				} else {
					return;
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_right_to_left_225_left_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= x_rel / 2) {
					player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
				} else {
					return;
				}
			} else if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)] == tank_slope_up_right_to_left_225_right_info_tile) {
				x_rel = abs((((int)player.x_position_in_pixels + 15) / 16 * 16) - player.x_position_in_pixels - 15);
				y_rel = abs((((int)player.y_position_in_pixels + 31) / 16 * 16) - player.y_position_in_pixels - 31);
				if (y_rel >= 8 + x_rel / 2) {
					player.y_position_in_pixels -= y_amount; player.on_ground = TRUE; return;
				} else {
					return;
				}
			}
		}

		info_value_1 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 4) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];
		info_value_2 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];
		info_value_3 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 27) / 16)][(int)((player.y_position_in_pixels + 31) / 16)];

		if (info_value_1 == blank_info_tile && info_value_2 == blank_info_tile && info_value_3 == blank_info_tile) return;

		if (info_value_1 == solid_info_tile || info_value_2 == solid_info_tile || info_value_3 == solid_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if (player.skin == TANK_SKIN && (info_value_1 == tank_solid_info_tile || info_value_2 == tank_solid_info_tile || info_value_3 == tank_solid_info_tile)) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if (player.skin == TANK_SKIN && (info_value_1 == tank_solid_from_top_no_drop_info_tile || info_value_2 == tank_solid_from_top_no_drop_info_tile || info_value_3 == tank_solid_from_top_no_drop_info_tile) && (int)player.y_position_in_pixels + 31 == (int)((player.y_position_in_pixels + 31) / 16) * 16) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if ((info_value_1 == solid_from_top_no_drop_info_tile || info_value_2 == solid_from_top_no_drop_info_tile || info_value_3 == solid_from_top_no_drop_info_tile) && (int)player.y_position_in_pixels + 31 == (int)((player.y_position_in_pixels + 31) / 16) * 16) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if ((info_value_1 == solid_from_top_allow_ammo_info_tile || info_value_2 == solid_from_top_allow_ammo_info_tile || info_value_3 == solid_from_top_allow_ammo_info_tile) && (int)player.y_position_in_pixels + 31 == (int)((player.y_position_in_pixels + 31) / 16) * 16) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if ((info_value_1 == solid_from_top_no_ammo_info_tile || info_value_2 == solid_from_top_no_ammo_info_tile || info_value_3 == solid_from_top_no_ammo_info_tile) && (int)player.y_position_in_pixels + 31 == (int)((player.y_position_in_pixels + 31) / 16) * 16) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if ((info_value_1 == slope_filler_info_tile || info_value_2 == slope_filler_info_tile || info_value_3 == slope_filler_info_tile) && (int)player.y_position_in_pixels + 31 == (int)((player.y_position_in_pixels + 31) / 16) * 16) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == painful_from_bottom_info_tile || info_value_2 == painful_from_bottom_info_tile || info_value_3 == painful_from_bottom_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == painful_from_top_info_tile || info_value_2 == painful_from_top_info_tile || info_value_3 == painful_from_top_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
			if (player.timer == 0) hurt_player(-1);
			if (player.health_bars_left < 1) return;
		} else if (info_value_1 == breakable_wall_info_tile || info_value_2 == breakable_wall_info_tile || info_value_3 == breakable_wall_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == conveyor_belt_right_info_tile || info_value_2 == conveyor_belt_right_info_tile || info_value_3 == conveyor_belt_right_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount, push_player(.615, 0);
		} else if (info_value_1 == conveyor_belt_left_info_tile || info_value_2 == conveyor_belt_left_info_tile || info_value_3 == conveyor_belt_left_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount, push_player(-.615, 0);
		} else if (info_value_1 == half_slippery_info_tile || info_value_2 == half_slippery_info_tile || info_value_3 == half_slippery_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == mostly_slippery_info_tile || info_value_2 == mostly_slippery_info_tile || info_value_3 == mostly_slippery_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		}

		player_touched((player.x_position_in_pixels + 4) / 16, (player.y_position_in_pixels + 31) / 16);
		player_touched((player.x_position_in_pixels + 15) / 16, (player.y_position_in_pixels + 31) / 16);
		player_touched((player.x_position_in_pixels + 27) / 16, (player.y_position_in_pixels + 31) / 16);
		return;
	}
	// Up
	else if (y_amount < 0) {
		player.y_position_in_pixels += y_amount;
		if (player.y_position_in_pixels < 0) player.y_position_in_pixels = 0, player.y_acceleration = 0;

		info_value_1 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 4) / 16)][(int)((player.y_position_in_pixels + 2) / 16)];
		info_value_2 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)((player.y_position_in_pixels + 2) / 16)];
		info_value_3 = worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 27) / 16)][(int)((player.y_position_in_pixels + 2) / 16)];

		if (info_value_1 == blank_info_tile && info_value_2 == blank_info_tile && info_value_3 == blank_info_tile) return;

		if (info_value_1 == solid_info_tile || info_value_2 == solid_info_tile || info_value_3 == solid_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (player.skin == TANK_SKIN && (info_value_1 == tank_solid_info_tile || info_value_2 == tank_solid_info_tile || info_value_3 == tank_solid_info_tile)) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == solid_from_bottom_allow_ammo_info_tile || info_value_2 == solid_from_bottom_allow_ammo_info_tile || info_value_3 == solid_from_bottom_allow_ammo_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == solid_from_bottom_no_ammo_info_tile || info_value_2 == solid_from_bottom_no_ammo_info_tile || info_value_3 == solid_from_bottom_no_ammo_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == painful_from_top_info_tile || info_value_2 == painful_from_top_info_tile || info_value_3 == painful_from_top_info_tile) {
			player.on_ground = TRUE, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == painful_from_bottom_info_tile || info_value_2 == painful_from_bottom_info_tile || info_value_3 == painful_from_bottom_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
			if (player.timer == 0) hurt_player(-1);
			if (player.health_bars_left < 1) return;
		} else if (info_value_1 == breakable_wall_info_tile || info_value_2 == breakable_wall_info_tile || info_value_3 == breakable_wall_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == conveyor_belt_right_info_tile || info_value_2 == conveyor_belt_right_info_tile || info_value_3 == conveyor_belt_right_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == conveyor_belt_left_info_tile || info_value_2 == conveyor_belt_left_info_tile || info_value_3 == conveyor_belt_left_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == half_slippery_info_tile || info_value_2 == half_slippery_info_tile || info_value_3 == half_slippery_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		} else if (info_value_1 == mostly_slippery_info_tile || info_value_2 == mostly_slippery_info_tile || info_value_3 == mostly_slippery_info_tile) {
			player.y_acceleration = 0, player.y_position_in_pixels -= y_amount;
		}

		player_touched((player.x_position_in_pixels + 4) / 16, (player.y_position_in_pixels + 2) / 16);
		player_touched((player.x_position_in_pixels + 15) / 16, (player.y_position_in_pixels + 2) / 16);
		player_touched((player.x_position_in_pixels + 27) / 16, (player.y_position_in_pixels + 2) / 16);
		return;
	}
}

/********************\
|** PLAYER TOUCHED **|
\********************/
void player_touched(int x_tile_location, int y_tile_location)
{
	int touched_info_tile;
	int x_tile_loop, y_tile_loop;

	if (player.health_bars_left <= 0) return;

	// This variable helps shorten the if statements and prevent redundant calculations
	touched_info_tile = worminator_map.info_tile_grid[x_tile_location][y_tile_location];

	// Turbo launch tiles
	if (player.turbo_launch == FALSE && touched_info_tile >= turbo_launch_1_info_tile && touched_info_tile <= turbo_launch_4_info_tile) {
		if (touched_info_tile == turbo_launch_1_info_tile) player.y_acceleration = -2.5;
		if (touched_info_tile == turbo_launch_2_info_tile) player.y_acceleration = -3;
		if (touched_info_tile == turbo_launch_3_info_tile) player.y_acceleration = -5;
		if (touched_info_tile == turbo_launch_4_info_tile) player.y_acceleration = -7;
		player.turbo_launch = TRUE;
		add_text_message_if_new("Turbo Launch");
	}

	// Lose control tile
	if (touched_info_tile == lose_control_info_tile && player.no_control == FALSE) {
		player.no_control = TRUE;
		player.x_position_in_pixels = (int)((player.x_position_in_pixels + 4) / 16) * 16;
		player.y_position_in_pixels = (int)((player.y_position_in_pixels + 4) / 16) * 16;
		/*player.x_acceleration = 0;
		*  player.y_acceleration = 0;*/
		control_player();
	}

	// A switch info tile
	if (touched_info_tile >= switch_1_info_tile && touched_info_tile <= switch_16_info_tile) {
		for (x_tile_loop = 0; x_tile_loop < worminator_map.map_width; x_tile_loop++) {
			for (y_tile_loop = 0; y_tile_loop < worminator_map.map_height; y_tile_loop++) {
				if (worminator_map.info_tile_grid[x_tile_loop][y_tile_loop] == touched_info_tile + 16) {
					trigger_switch(x_tile_loop, y_tile_loop);
					if (worminator_map.info_tile_grid[x_tile_loop][y_tile_loop] == touched_info_tile + 16) worminator_map.info_tile_grid[x_tile_loop][y_tile_loop] = 0;
				}
			}
		}
		if (worminator_map.info_tile_grid[x_tile_location][y_tile_location - 1] == worminator_map.info_tile_grid[x_tile_location][y_tile_location + 1]) worminator_map.info_tile_grid[x_tile_location][y_tile_location] = worminator_map.info_tile_grid[x_tile_location][y_tile_location + 1];
		else worminator_map.info_tile_grid[x_tile_location][y_tile_location] = blank_info_tile;

		// Animate the switch flipping
		if (worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] >= 40 && worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] <= 47) {
			worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] += 16;
			if (worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] == 58) {
				worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] = 43;
				worminator_map.foreground_tile_grid[x_tile_location][y_tile_location + 1] = 59;
			}
			if (worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] == 59) {
				worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] = 42;
				worminator_map.foreground_tile_grid[x_tile_location][y_tile_location + 1] = 58;
			}
		} else if (worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] >= 56 && worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] <= 63) {
			worminator_map.foreground_tile_grid[x_tile_location][y_tile_location] -= 16;
		}
	}

	// A lethal tile that will always kill the Worminator
	if (touched_info_tile == lethal_info_tile) {
		player.timer = 0; hurt_player(-10); return;
	}

	// A very painful info tile that will hurt the Worminator a lot
	if (touched_info_tile == very_painful_info_tile && player.timer == 0) {
		hurt_player(-5); return;
	}

	// A painful info tile that will hurt the Worminator
	if (touched_info_tile == painful_info_tile && player.timer == 0) {
		hurt_player(-1); return;
	}

	// Quick teleport info tiles
	if (touched_info_tile >= quick_teleporter_1_info_tile && touched_info_tile <= quick_teleporter_16_info_tile) {
		for (x_tile_loop = 0; x_tile_loop < worminator_map.map_width; x_tile_loop++) {
			for (y_tile_loop = 0; y_tile_loop < worminator_map.map_height; y_tile_loop++) {
				if (worminator_map.info_tile_grid[x_tile_loop][y_tile_loop] == touched_info_tile + 80) {
					player.x_position_in_pixels = x_tile_loop * 16;
					player.y_position_in_pixels = y_tile_loop * 16;
					player.y_acceleration = 0;
					check_enemy_ranges();
					return;
				}
			}
		}
	}

	// Quick exit level info tiles
	if (touched_info_tile >= quick_exit_to_level_1_info_tile && touched_info_tile <= quick_exit_to_level_16_info_tile) {
		load_map((touched_info_tile - quick_exit_to_level_1_info_tile) + 1, NULL, 0);
		return;
	}
	if (touched_info_tile >= quick_exit_to_level_17_info_tile && touched_info_tile <= quick_exit_to_level_24_info_tile) {
		load_map((touched_info_tile - quick_exit_to_level_17_info_tile) + 17, NULL, 0);
		return;
	}
}

/********************\
|** CONTROL PLAYER **|
\********************/
void control_player()
{
	int tile_hit = worminator_map.info_tile_grid[(int)(player.x_position_in_pixels / 16)][(int)(player.y_position_in_pixels / 16)];

	if (worminator_map.info_tile_grid[(int)((player.x_position_in_pixels + 15) / 16)][(int)(player.y_position_in_pixels / 16)] != tile_hit) tile_hit = -1;
	if (worminator_map.info_tile_grid[(int)(player.x_position_in_pixels / 16)][(int)((player.y_position_in_pixels + 15) / 16)] != tile_hit) tile_hit = -1;

	if (tile_hit == up_turning_point_info_tile) {
		player.x_acceleration = 0;  player.y_acceleration = -1;
	}
	if (tile_hit == down_turning_point_info_tile) {
		player.x_acceleration = 0;  player.y_acceleration = 1;
	}
	if (tile_hit == right_turning_point_info_tile) {
		player.x_acceleration = 1;  player.y_acceleration = 0;
	}
	if (tile_hit == left_turning_point_info_tile) {
		player.x_acceleration = -1;  player.y_acceleration = 0;
	}
	if (tile_hit == up_right_turning_point_info_tile) {
		player.x_acceleration = 1;  player.y_acceleration = -1;
	}
	if (tile_hit == up_left_turning_point_info_tile) {
		player.x_acceleration = -1;  player.y_acceleration = -1;
	}
	if (tile_hit == down_right_turning_point_info_tile) {
		player.x_acceleration = 1;  player.y_acceleration = 1;
	}
	if (tile_hit == down_left_turning_point_info_tile) {
		player.x_acceleration = -1;  player.y_acceleration = 1;
	}
	if (tile_hit == regain_control_info_tile) {
		player.no_control = FALSE;  return;
	}

	player.x_position_in_pixels += player.x_acceleration;
	player.y_position_in_pixels += player.y_acceleration;
}

/********************\
|** TRIGGER SWITCH **|
\********************/
void trigger_switch(int x, int y)
{
	int sprite_loop;

	// Fists scan for sprites on the trigger
	for (sprite_loop = 0; sprite_loop < number_of_sprites; sprite_loop++) if (sprites[sprite_loop].x_position_in_pixels == x * 16 && sprites[sprite_loop].y_position_in_pixels == y * 16) {
			// Open doors
			if (sprites[sprite_loop].sprite_type >= GREY_DOOR_SPRITE && sprites[sprite_loop].sprite_type <= BLUE_DOOR_SPRITE) {
				sprites[sprite_loop].action_state = 1;
				sprites[sprite_loop].timer = 128;
				worminator_map.info_tile_grid[(int)((sprites[sprite_loop].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_loop].y_position_in_pixels + 24) / 16)] = 0;
				worminator_map.info_tile_grid[(int)((sprites[sprite_loop].x_position_in_pixels + 8) / 16)][(int)((sprites[sprite_loop].y_position_in_pixels + 40) / 16)] = 0;
				play_relative_sound(DOOR_OPEN_SOUND, sprite_loop);
				add_text_message_if_new("Door opened");
			}

			// Move down floating platforms
			if (sprites[sprite_loop].sprite_type == FLOATING_PLATFORM_SPRITE) {
				sprites[sprite_loop].y_position_in_pixels += 16;
				add_text_message_if_new("Platform activated");
			}

			// Disable gun turrets
			if (sprites[sprite_loop].sprite_type >= SMALL_GUN_TURRET_ENEMY_SPRITE && sprites[sprite_loop].sprite_type <= BIG_GUN_TURRET_ENEMY_SPRITE) {
				sprites[sprite_loop].animation_state = 2;
				sprites[sprite_loop].timer = 2048;
				add_text_message_if_new("Gun turret offline for a short time");
			}
		}

	// Remove pathing arrows
	if (worminator_map.info_tile_grid[x][y - 1] >= up_turning_point_info_tile && worminator_map.info_tile_grid[x][y - 1] <= down_right_turning_point_info_tile) worminator_map.info_tile_grid[x][y - 1] = blank_info_tile;

	// Make platforms appear
	if (worminator_map.graphical_properties_tile_grid[x][y] == hide_grph_tile) {
		worminator_map.info_tile_grid[x][y] = solid_info_tile;
		worminator_map.graphical_properties_tile_grid[x][y] = masked_foreground_grph_tile;
		add_text_message_if_new("Platform created!");
	}
}

/***********************\
|** PICK PLAYER FRAME **|
\***********************/
void pick_player_frame()
{
	int damage_frame_shift = ((10 - player.health_bars_left) / 2.5);

	if (player.skin == DAMAGED_WORM_1_SKIN) damage_frame_shift = 1;
	else if (player.skin == DAMAGED_WORM_2_SKIN) damage_frame_shift = 2;
	else if (player.skin == DAMAGED_WORM_3_SKIN) damage_frame_shift = 3;
	else if (damage_frame_shift > 3) damage_frame_shift = 3;

	if (wormy_config.super_turkey_mode == TRUE) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_BAKED_TURKEY_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_BAKED_TURKEY_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_BAKED_TURKEY_WALK_01 || player.frame > SKIN_BAKED_TURKEY_WALK_04) player.frame = SKIN_BAKED_TURKEY_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_BAKED_TURKEY_WALK_04) player.frame = SKIN_BAKED_TURKEY_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_BAKED_TURKEY_STAND;
		}
	} else if (player.skin == WORMINATOR_SKIN || player.skin == DAMAGED_WORM_1_SKIN || player.skin == DAMAGED_WORM_2_SKIN || player.skin == DAMAGED_WORM_3_SKIN) {
		if (player.in_water == FALSE) {
			if (player.is_ducking == TRUE) {
				player.frame = WORMINATOR_DUCK_SET_01 + damage_frame_shift;
			} else if (player.on_ground == FALSE) {
				player.frame = WORMINATOR_JUMP_SET_01 + damage_frame_shift;
			} else if (player.x_acceleration != 0) {
				if (player.on_ground == TRUE) {
					player.animation_timer++;
					if (player.animation_timer == 16) {
						if (player.frame < WORMINATOR_WALK_01_SET_01 || player.frame > WORMINATOR_WALK_04_SET_04) player.frame = WORMINATOR_WALK_01_SET_01 + damage_frame_shift;
						else player.frame += 4;
						if (player.frame > WORMINATOR_WALK_04_SET_04) player.frame = WORMINATOR_WALK_01_SET_01 + damage_frame_shift;
						player.animation_timer = 0;
					}
				}
			} else {
				player.frame = WORMINATOR_STAND_SET_01 + damage_frame_shift;
			}
		} else {
			player.animation_timer++;
			if (player.animation_timer == 16) {
				if (player.frame >= WORMINATOR_SWIM_01_SET_01 && player.frame <= WORMINATOR_SWIM_03_SET_04) {
					player.frame += 4;
					if (player.frame > WORMINATOR_SWIM_04_SET_04) player.frame = WORMINATOR_SWIM_01_SET_01 + damage_frame_shift;
				} else {
					player.frame = WORMINATOR_SWIM_01_SET_01 + damage_frame_shift;
				}
				player.animation_timer = 0;
			}
		}
	} else if (player.skin == THE_BRAIN_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_THE_BRAIN_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_THE_BRAIN_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_THE_BRAIN_WALK_01 || player.frame > SKIN_THE_BRAIN_WALK_04) player.frame = SKIN_THE_BRAIN_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_THE_BRAIN_WALK_04) player.frame = SKIN_THE_BRAIN_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_THE_BRAIN_STAND;
		}
	} else if (player.skin == THE_YIPNIK_MIKIMBO_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_THE_YIPNIK_MIKIMBO_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_THE_YIPNIK_MIKIMBO_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_THE_YIPNIK_MIKIMBO_WALK_01 || player.frame > SKIN_THE_YIPNIK_MIKIMBO_WALK_04) player.frame = SKIN_THE_YIPNIK_MIKIMBO_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_THE_YIPNIK_MIKIMBO_WALK_04) player.frame = SKIN_THE_YIPNIK_MIKIMBO_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_THE_YIPNIK_MIKIMBO_STAND;
		}
	} else if (player.skin == ORIGINAL_WORMINATOR_SKIN) {
		player.frame = SKIN_ORIGINAL_WORMINATOR_STAND;
	} else if (player.skin == HOTDOG_HERO_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_HOTDOG_HERO_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_HOTDOG_HERO_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_HOTDOG_HERO_WALK_01 || player.frame > SKIN_HOTDOG_HERO_WALK_02) player.frame = SKIN_HOTDOG_HERO_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_HOTDOG_HERO_WALK_02) player.frame = SKIN_HOTDOG_HERO_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_HOTDOG_HERO_STAND;
		}
	} else if (player.skin == VOODOO_BADIE_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_VOODOO_BADIE_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_VOODOO_BADIE_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_VOODOO_BADIE_WALK_01 || player.frame > SKIN_VOODOO_BADIE_WALK_02) player.frame = SKIN_VOODOO_BADIE_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_VOODOO_BADIE_WALK_02) player.frame = SKIN_VOODOO_BADIE_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_VOODOO_BADIE_STAND;
		}
	} else if (player.skin == YUANTI_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_YUANTI_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_YUANTI_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_YUANTI_WALK_01 || player.frame > SKIN_YUANTI_WALK_04) player.frame = SKIN_YUANTI_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_YUANTI_WALK_04) player.frame = SKIN_YUANTI_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_YUANTI_STAND;
		}
	} else if (player.skin == MELON_O_DEATH_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_MELON_O_DEATH_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_MELON_O_DEATH_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_MELON_O_DEATH_WALK_01 || player.frame > SKIN_MELON_O_DEATH_WALK_04) player.frame = SKIN_MELON_O_DEATH_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_MELON_O_DEATH_WALK_04) player.frame = SKIN_MELON_O_DEATH_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_MELON_O_DEATH_STAND;
		}
	} else if (player.skin == BAKED_TURKEY_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_BAKED_TURKEY_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_BAKED_TURKEY_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_BAKED_TURKEY_WALK_01 || player.frame > SKIN_BAKED_TURKEY_WALK_04) player.frame = SKIN_BAKED_TURKEY_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_BAKED_TURKEY_WALK_04) player.frame = SKIN_BAKED_TURKEY_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_BAKED_TURKEY_STAND;
		}
	} else if (player.skin == WATERMELON_WONDER_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_WATERMELON_WONDER_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_WATERMELON_WONDER_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_WATERMELON_WONDER_WALK_01 || player.frame > SKIN_WATERMELON_WONDER_WALK_04) player.frame = SKIN_WATERMELON_WONDER_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_WATERMELON_WONDER_WALK_04) player.frame = SKIN_WATERMELON_WONDER_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_WATERMELON_WONDER_STAND;
		}
	} else if (player.skin == THE_INVERTEBRATOR_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_THE_INVERTEBRATOR_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_THE_INVERTEBRATOR_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_THE_INVERTEBRATOR_WALK_01 || player.frame > SKIN_THE_INVERTEBRATOR_WALK_04) player.frame = SKIN_THE_INVERTEBRATOR_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_THE_INVERTEBRATOR_WALK_04) player.frame = SKIN_THE_INVERTEBRATOR_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_THE_INVERTEBRATOR_STAND;
		}
	} else if (player.skin == BROWNIE_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_BROWNIE_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_BROWNIE_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_BROWNIE_WALK_01 || player.frame > SKIN_BROWNIE_WALK_04) player.frame = SKIN_BROWNIE_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_BROWNIE_WALK_04) player.frame = SKIN_BROWNIE_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_BROWNIE_STAND;
		}
	} else if (player.skin == EVIL_CUBE_O_DEATH_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_EVIL_CUBE_DUCK;
		} else {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_EVIL_CUBE_WALK_01 || player.frame > SKIN_EVIL_CUBE_WALK_16) player.frame = SKIN_EVIL_CUBE_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_EVIL_CUBE_WALK_16) player.frame = SKIN_EVIL_CUBE_WALK_01;
					player.animation_timer = 0;
				}
			}
		}
	} else if (player.skin == ENDO_SKELETON_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_ENDO_SKELETON_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_ENDO_SKELETON_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_ENDO_SKELETON_WALK_01 || player.frame > SKIN_ENDO_SKELETON_WALK_04) player.frame = SKIN_ENDO_SKELETON_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_ENDO_SKELETON_WALK_04) player.frame = SKIN_ENDO_SKELETON_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_ENDO_SKELETON_STAND;
		}
	} else if (player.skin == MUMMY_WORM_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_MUMMY_WORM_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_MUMMY_WORM_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_MUMMY_WORM_WALK_01 || player.frame > SKIN_MUMMY_WORM_WALK_04) player.frame = SKIN_MUMMY_WORM_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_MUMMY_WORM_WALK_04) player.frame = SKIN_MUMMY_WORM_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_MUMMY_WORM_STAND;
		}
	} else if (player.skin == THE_ANGEL_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_THE_ANGEL_DUCK;
		} else {
			player.animation_timer++;
			if (player.animation_timer == 16) {
				if (player.frame < SKIN_THE_ANGEL_WALK_01 || player.frame > SKIN_THE_ANGEL_WALK_04) player.frame = SKIN_THE_ANGEL_WALK_01;
				else player.frame++;
				if (player.frame > SKIN_THE_ANGEL_WALK_04) player.frame = SKIN_THE_ANGEL_WALK_01;
				player.animation_timer = 0;
			}
		}
	} else if (player.skin == THE_DEVIL_SKIN) {
		if (player.is_ducking == TRUE) {
			player.frame = SKIN_THE_DEVIL_DUCK;
		} else if (player.on_ground == FALSE) {
			player.frame = SKIN_THE_DEVIL_JUMP;
		} else if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer == 16) {
					if (player.frame < SKIN_THE_DEVIL_WALK_01 || player.frame > SKIN_THE_DEVIL_WALK_04) player.frame = SKIN_THE_DEVIL_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_THE_DEVIL_WALK_04) player.frame = SKIN_THE_DEVIL_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_THE_DEVIL_STAND;
		}
	} else if (player.skin == TANK_SKIN) {
		if (player.frame < SKIN_TANK_WALK_01 || player.frame > SKIN_TANK_WALK_16) player.frame = SKIN_TANK_WALK_01;
		if (player.x_acceleration != 0) {
			if (player.on_ground == TRUE) {
				player.animation_timer++;
				if (player.animation_timer >= 4) {
					if (player.frame < SKIN_TANK_WALK_01 || player.frame > SKIN_TANK_WALK_16) player.frame = SKIN_TANK_WALK_01;
					else player.frame++;
					if (player.frame > SKIN_TANK_WALK_16) player.frame = SKIN_TANK_WALK_01;
					player.animation_timer = 0;
				}
			}
		} else {
			player.frame = SKIN_TANK_WALK_01;
		}
	}
}

/*****************\
|** DRAW PLAYER **|
\*****************/
void draw_player()
{
	int sx_anchor = player.x_position_in_pixels - screen_x_position, sy_anchor = player.y_position_in_pixels - screen_y_position;

	// Don't draw any frame if there is no frame selected
	if (player.frame != 0) {
		// This makes the worminator flash when he has been hurt
		if (((int)((int)(player.timer / 24) / 2) == ((int)(player.timer / 24) / 2.0)) || player.timer == 0) {
			if (player.skin == TANK_SKIN) {
				if (player.x_acceleration == 0 || (player.x_acceleration > 0 && player.direction == 0) || (player.x_acceleration < 0 && player.direction == 1)) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[player.frame].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[player.frame].dat, sx_anchor, sy_anchor);
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[(SKIN_TANK_WALK_16 - player.frame) + SKIN_TANK_WALK_01].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[(SKIN_TANK_WALK_16 - player.frame) + SKIN_TANK_WALK_01].dat, sx_anchor, sy_anchor);
				}
			} else {
				if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[player.frame].dat, sx_anchor, sy_anchor);
				else draw_sprite_h_flip(double_buffer, worminator_data_file[player.frame].dat, sx_anchor, sy_anchor);
			}

			// Draw super turkey cape and the cool shirt
			if (wormy_config.super_turkey_mode == TRUE) {
				if (global_timer_2 > 23) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_04].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_04].dat, sx_anchor, sy_anchor);
				} else if (global_timer_2 > 15) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_03].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_03].dat, sx_anchor, sy_anchor);
				} else if (global_timer_2 > 7) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_02].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_02].dat, sx_anchor, sy_anchor);
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_01].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SUPER_TURKEY_CAPE_01].dat, sx_anchor, sy_anchor);
				}
			} else if (wormy_config.cool_mode == TRUE && player.skin < 4) {
				if (player.frame == WORMINATOR_WALK_04_SET_01 || player.frame == WORMINATOR_WALK_04_SET_02 || player.frame == WORMINATOR_WALK_04_SET_03 || player.frame == WORMINATOR_WALK_04_SET_04) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[COOL_SHIRT_04].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[COOL_SHIRT_04].dat, sx_anchor, sy_anchor);
				} else if (player.frame == WORMINATOR_WALK_03_SET_01 || player.frame == WORMINATOR_WALK_03_SET_02 || player.frame == WORMINATOR_WALK_03_SET_03 || player.frame == WORMINATOR_WALK_03_SET_04) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[COOL_SHIRT_03].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[COOL_SHIRT_03].dat, sx_anchor, sy_anchor);
				} else if (player.frame == WORMINATOR_WALK_02_SET_01 || player.frame == WORMINATOR_WALK_02_SET_02 || player.frame == WORMINATOR_WALK_02_SET_03 || player.frame == WORMINATOR_WALK_02_SET_04) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[COOL_SHIRT_02].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[COOL_SHIRT_02].dat, sx_anchor, sy_anchor);
				} else if (player.frame == WORMINATOR_WALK_01_SET_01 || player.frame == WORMINATOR_WALK_01_SET_02 || player.frame == WORMINATOR_WALK_01_SET_03 || player.frame == WORMINATOR_WALK_01_SET_04 || player.frame == WORMINATOR_JUMP_SET_01 || player.frame == WORMINATOR_JUMP_SET_02 || player.frame == WORMINATOR_JUMP_SET_03 || player.frame == WORMINATOR_JUMP_SET_04 || player.frame == WORMINATOR_STAND_SET_01 || player.frame == WORMINATOR_STAND_SET_02 || player.frame == WORMINATOR_STAND_SET_03 || player.frame == WORMINATOR_STAND_SET_04) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[COOL_SHIRT_01].dat, sx_anchor, sy_anchor);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[COOL_SHIRT_01].dat, sx_anchor, sy_anchor);
				}
			}


			// No weapons are drawn if the Worminator is ducking or dead
			if (player.is_ducking == TRUE || player.health_bars_left == 0) return;

			// No weapons are drawn in super turkey mode
			if (wormy_config.super_turkey_mode == TRUE) return;

			if (player.skin == TANK_SKIN) {
				if (fire_key_state > 60) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_06].dat, sx_anchor + 54, sy_anchor + 18);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_06].dat, sx_anchor + 1, sy_anchor + 18);
				} else if (fire_key_state > 50) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_05].dat, sx_anchor + 54, sy_anchor + 18);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_05].dat, sx_anchor, sy_anchor + 18);
				} else if (fire_key_state > 40) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_04].dat, sx_anchor + 54, sy_anchor + 14);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_04].dat, sx_anchor, sy_anchor + 14);
				} else if (fire_key_state > 30) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_03].dat, sx_anchor + 54, sy_anchor + 18);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_03].dat, sx_anchor, sy_anchor + 18);
				} else if (fire_key_state > 20) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_02].dat, sx_anchor + 54, sy_anchor + 18);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_02].dat, sx_anchor, sy_anchor + 18);
				} else if (fire_key_state > 10) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_01].dat, sx_anchor + 54, sy_anchor + 18);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_01].dat, sx_anchor + 1, sy_anchor + 18);
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_01].dat, sx_anchor + 54, sy_anchor + 18);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[SKIN_TANK_FIRE_PATCH_01].dat, sx_anchor + 1, sy_anchor + 18);
				}
				return;
			}

			// This is the minigun code
			if (player.current_weapon == 1 && player.bullets_left >= 0) {
				if (((player.ammo_type == 0 && player.bullets_left > 0) || (player.ammo_type == 1 && player.plasma_bullets_left > 0)) && fire_key_state != 0) {
					if (fire_key_state > 6) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_TURN].dat, sx_anchor + 15, sy_anchor + 12);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_TURN].dat, sx_anchor - 4, sy_anchor + 12);
					} else if (fire_key_state > 4) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_FIRE_03].dat, sx_anchor + 15, sy_anchor + 12);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_FIRE_03].dat, sx_anchor - 10, sy_anchor + 12);
					} else if (fire_key_state > 2) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_FIRE_02].dat, sx_anchor + 15, sy_anchor + 12);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_FIRE_02].dat, sx_anchor - 10, sy_anchor + 12);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_FIRE_01].dat, sx_anchor + 15, sy_anchor + 12);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_FIRE_01].dat, sx_anchor - 9, sy_anchor + 12);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINIGUN].dat, sx_anchor + 15, sy_anchor + 12);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINIGUN].dat, sx_anchor - 4, sy_anchor + 12);
				}
				if (player.ammo_type == 1) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_PLASMA_BULLET_MASK].dat, sx_anchor + 20, sy_anchor + 12);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINIGUN_PLASMA_BULLET_MASK].dat, sx_anchor + 1, sy_anchor + 12);
				}
			}

			// This is the shotgun code
			if (player.current_weapon == 2 && player.shells_left >= 0) {
				if (((player.ammo_type == 0 && player.shells_left > 0) || (player.ammo_type == 1 && player.explosive_shells_left > 0)) && fire_key_state != 0) {
					if (fire_key_state > 120) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 108) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_01].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_01].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 96) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_02].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_02].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 84) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_03].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_03].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 72) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_02].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_02].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 60) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_01].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_COCK_01].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 48) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 36) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor - 5, sy_anchor + 11);
					} else if (fire_key_state > 24) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_FIRE_03].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_FIRE_03].dat, sx_anchor - 9, sy_anchor + 11);
					} else if (fire_key_state > 12) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_FIRE_02].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_FIRE_02].dat, sx_anchor - 9, sy_anchor + 11);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_FIRE_01].dat, sx_anchor + 13, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_FIRE_01].dat, sx_anchor - 9, sy_anchor + 11);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor + 13, sy_anchor + 11);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN].dat, sx_anchor - 5, sy_anchor + 11);
				}
				if (player.ammo_type == 1) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_EXPLOSIVE_SHELL_MASK].dat, sx_anchor + 21, sy_anchor + 7);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_SHOTGUN_EXPLOSIVE_SHELL_MASK].dat, sx_anchor + 5, sy_anchor + 7);
				}
			}

			// Only the first two weapons are avalible under water
			if (player.in_water == TRUE) return;

			// This is the rocket launcher code
			if (player.current_weapon == 3 && player.rockets_left >= 0) {
				if (((player.ammo_type == 0 && player.rockets_left > 0) || (player.ammo_type == 1 && player.micro_nukes_left > 0)) && fire_key_state != 0) {
					if (fire_key_state > 25) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER].dat, sx_anchor + 11, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER].dat, sx_anchor - 3, sy_anchor + 11);
					} else if (fire_key_state > 20) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor + 11, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor - 6, sy_anchor + 11);
					} else if (fire_key_state > 15) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor + 11, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor - 7, sy_anchor + 11);
					} else if (fire_key_state > 10) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_03].dat, sx_anchor + 11, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_03].dat, sx_anchor - 7, sy_anchor + 11);
					} else if (fire_key_state > 5) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor + 11, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor - 7, sy_anchor + 11);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor + 11, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor - 6, sy_anchor + 11);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER].dat, sx_anchor + 11, sy_anchor + 11);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER].dat, sx_anchor - 3, sy_anchor + 11);
				}
				if (player.ammo_type == 1) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_MICRO_NUKE_MASK].dat, sx_anchor + 13, sy_anchor + 17);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_ROCKET_LAUNCHER_MICRO_NUKE_MASK].dat, sx_anchor - 5, sy_anchor + 17);
				}
			}

			// This is the dual rocket launcher code
			if (player.current_weapon == 4 && player.rockets_left >= 0) {
				if (player.rockets_left > 1 && fire_key_state != 0) {
					if (fire_key_state > 35) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER].dat, sx_anchor + 12, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER].dat, sx_anchor - 2, sy_anchor + 10);
					} else if (fire_key_state > 30) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor + 12, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor - 8, sy_anchor + 10);
					} else if (fire_key_state > 25) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor + 12, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor - 9, sy_anchor + 10);
					} else if (fire_key_state > 20) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_03].dat, sx_anchor + 12, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_03].dat, sx_anchor - 12, sy_anchor + 9);
					} else if (fire_key_state > 15) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_04].dat, sx_anchor + 12, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_04].dat, sx_anchor - 14, sy_anchor + 8);
					} else if (fire_key_state > 10) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_03].dat, sx_anchor + 12, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_03].dat, sx_anchor - 12, sy_anchor + 9);
					} else if (fire_key_state > 5) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor + 12, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_02].dat, sx_anchor - 9, sy_anchor + 10);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor + 12, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER_FIRE_01].dat, sx_anchor - 8, sy_anchor + 10);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER].dat, sx_anchor + 12, sy_anchor + 10);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_DUAL_ROCKET_LAUNCHER].dat, sx_anchor - 2, sy_anchor + 10);
				}
			}

			// This is the laser gun code
			if (player.current_weapon == 5 && player.cells_left >= 0) {
				if (player.cells_left > 0 && fire_key_state != 0) {
					if (fire_key_state > 15) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN].dat, sx_anchor + 11, sy_anchor + 12);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN].dat, sx_anchor - 6, sy_anchor + 12);
					} else if (fire_key_state > 7) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN_FIRE_01].dat, sx_anchor + 11, sy_anchor + 12);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN_FIRE_01].dat, sx_anchor - 11, sy_anchor + 12);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN_FIRE_02].dat, sx_anchor + 11, sy_anchor + 11);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN_FIRE_02].dat, sx_anchor - 17, sy_anchor + 11);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN].dat, sx_anchor + 11, sy_anchor + 12);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_LASER_GUN].dat, sx_anchor - 6, sy_anchor + 12);
				}
			}

			// This is the flamethrower code
			if (player.current_weapon == 6 && player.gasoline_left >= 0) {
				if (player.gasoline_left > 0 && fire_key_state != 0) {
					if (fire_key_state > 44) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_01].dat, sx_anchor + 10, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_01].dat, sx_anchor - 15, sy_anchor + 10);
					} else if (fire_key_state > 42) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_02].dat, sx_anchor + 10, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_02].dat, sx_anchor - 22, sy_anchor + 8);
					} else if (fire_key_state > 40) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_03].dat, sx_anchor + 10, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_03].dat, sx_anchor - 28, sy_anchor + 9);
					} else if (fire_key_state > 38) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_04].dat, sx_anchor + 10, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_04].dat, sx_anchor - 37, sy_anchor + 8);
					} else if (fire_key_state > 36) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_05].dat, sx_anchor + 10, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_05].dat, sx_anchor - 39, sy_anchor + 9);
					} else if (fire_key_state > 34) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_06].dat, sx_anchor + 10, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_06].dat, sx_anchor - 44, sy_anchor + 9);
					} else if (fire_key_state > 32) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_07].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_07].dat, sx_anchor - 56, sy_anchor + 7);
					} else if (fire_key_state > 30) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_08].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_08].dat, sx_anchor - 62, sy_anchor + 7);
					} else if (fire_key_state > 28) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_09].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_09].dat, sx_anchor - 65, sy_anchor + 7);
					} else if (fire_key_state > 26) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_10].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_10].dat, sx_anchor - 65, sy_anchor + 7);
						if (input_buffer[INPUT_FIRE] && desired_weapon == 6 && player.gasoline_left > 0) {
							player.gasoline_left--;
							update_player_stats();
							fire_key_state = 15;
						}
					} else if (fire_key_state > 24) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_11].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_11].dat, sx_anchor - 65, sy_anchor + 7);
					} else if (fire_key_state > 22) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_12].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_12].dat, sx_anchor - 65, sy_anchor + 7);
					} else if (fire_key_state > 20) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_11].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_11].dat, sx_anchor - 65, sy_anchor + 7);
					} else if (fire_key_state > 18) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_10].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_10].dat, sx_anchor - 65, sy_anchor + 7);
					} else if (fire_key_state > 16) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_09].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_09].dat, sx_anchor - 65, sy_anchor + 7);
					} else if (fire_key_state > 14) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_08].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_08].dat, sx_anchor - 62, sy_anchor + 7);
					} else if (fire_key_state > 12) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_07].dat, sx_anchor + 10, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_07].dat, sx_anchor - 56, sy_anchor + 7);
					} else if (fire_key_state > 10) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_06].dat, sx_anchor + 10, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_06].dat, sx_anchor - 44, sy_anchor + 9);
					} else if (fire_key_state > 8) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_05].dat, sx_anchor + 10, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_05].dat, sx_anchor - 39, sy_anchor + 9);
					} else if (fire_key_state > 6) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_04].dat, sx_anchor + 10, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_04].dat, sx_anchor - 37, sy_anchor + 8);
					} else if (fire_key_state > 4) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_03].dat, sx_anchor + 10, sy_anchor + 9);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_03].dat, sx_anchor - 28, sy_anchor + 9);
					} else if (fire_key_state > 2) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_02].dat, sx_anchor + 10, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_02].dat, sx_anchor - 22, sy_anchor + 8);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_01].dat, sx_anchor + 10, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER_FIRE_01].dat, sx_anchor - 15, sy_anchor + 10);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER].dat, sx_anchor + 10, sy_anchor + 12);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_FLAMETHROWER].dat, sx_anchor - 1, sy_anchor + 12);
				}
			}

			// This is the mortar cannon code
			if (player.current_weapon == 7 && player.mortars_left >= 0) {
				if (player.mortars_left > 0 && fire_key_state != 0) {
					if (fire_key_state > 50) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON].dat, sx_anchor + 8, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON].dat, sx_anchor + 3, sy_anchor + 8);
					} else if (fire_key_state > 40) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_01].dat, sx_anchor + 8, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_01].dat, sx_anchor + 3, sy_anchor + 8);
					} else if (fire_key_state > 30) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_02].dat, sx_anchor + 8, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_02].dat, sx_anchor + 1, sy_anchor + 7);
					} else if (fire_key_state > 20) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_03].dat, sx_anchor + 8, sy_anchor + 6);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_03].dat, sx_anchor + 0, sy_anchor + 6);
					} else if (fire_key_state > 10) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_02].dat, sx_anchor + 8, sy_anchor + 7);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_02].dat, sx_anchor + 1, sy_anchor + 7);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_01].dat, sx_anchor + 8, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON_FIRE_01].dat, sx_anchor + 3, sy_anchor + 8);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON].dat, sx_anchor + 8, sy_anchor + 8);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MORTAR_CANNON].dat, sx_anchor + 3, sy_anchor + 8);
				}
			}

			// This is the BFG code
			if (player.current_weapon == 8 && player.plasma_left >= 0) {
				if (player.plasma_left > 0 && fire_key_state != 0) {
					if (fire_key_state > 160) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG].dat, sx_anchor + 3, sy_anchor + 2);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG].dat, sx_anchor, sy_anchor + 2);
					} else if (fire_key_state > 150) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_06].dat, sx_anchor + 2, sy_anchor);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_06].dat, sx_anchor - 32, sy_anchor);
					} else if (fire_key_state > 120) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_05].dat, sx_anchor + 2, sy_anchor + 2);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_05].dat, sx_anchor, sy_anchor + 2);
					} else if (fire_key_state > 90) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_04].dat, sx_anchor + 3, sy_anchor + 2);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_04].dat, sx_anchor, sy_anchor + 2);
					} else if (fire_key_state > 60) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_03].dat, sx_anchor + 3, sy_anchor + 2);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_03].dat, sx_anchor, sy_anchor + 2);
					} else if (fire_key_state > 30) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_02].dat, sx_anchor + 3, sy_anchor + 2);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_02].dat, sx_anchor, sy_anchor + 2);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_01].dat, sx_anchor + 3, sy_anchor + 2);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG_FIRE_01].dat, sx_anchor, sy_anchor + 2);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_BFG].dat, sx_anchor + 3, sy_anchor + 2);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_BFG].dat, sx_anchor, sy_anchor + 2);
				}
			}

			// This is the Minelayer code
			if (player.current_weapon == 9 && player.mines_left >= 0) {
				if (((player.ammo_type == 0 && player.mines_left > 0) || (player.ammo_type == 1 && player.walking_mines_left > 0)) && fire_key_state != 0) {
					if (fire_key_state > 100) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_11].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_11].dat, sx_anchor + 12, sy_anchor + 4);
					} else if (fire_key_state > 90) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_10].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_10].dat, sx_anchor + 11, sy_anchor + 4);
					} else if (fire_key_state > 80) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_09].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_09].dat, sx_anchor + 10, sy_anchor + 4);
					} else if (fire_key_state > 70) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_08].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_08].dat, sx_anchor + 9, sy_anchor + 4);
					} else if (fire_key_state > 60) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_07].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_07].dat, sx_anchor + 9, sy_anchor + 4);
					} else if (fire_key_state > 50) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_06].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_06].dat, sx_anchor + 8, sy_anchor + 4);
					} else if (fire_key_state > 40) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_05].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_05].dat, sx_anchor + 1, sy_anchor + 4);
					} else if (fire_key_state > 30) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_04].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_04].dat, sx_anchor + 2, sy_anchor + 4);
					} else if (fire_key_state > 20) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_03].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_03].dat, sx_anchor + 2, sy_anchor + 4);
					} else if (fire_key_state > 10) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_02].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_02].dat, sx_anchor + 3, sy_anchor + 4);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_01].dat, sx_anchor + 6, sy_anchor + 4);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_LAY_01].dat, sx_anchor + 4, sy_anchor + 4);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER].dat, sx_anchor + 6, sy_anchor + 4);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER].dat, sx_anchor + 5, sy_anchor + 4);
				}
				if (player.ammo_type == 1) {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_WALKING_MINE_MASK].dat, sx_anchor + 3, sy_anchor + 5);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_MINE_LAYER_WALKING_MINE_MASK].dat, sx_anchor + 20, sy_anchor + 5);
				}
			}

			// This is the chainsaw code
			if (player.current_weapon == 10 && player.chainsaw_duration_left >= 0) {
				if (player.chainsaw_duration_left > 0 && fire_key_state != 0) {
					if (fire_key_state > 6) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW_FIRE_03].dat, sx_anchor + 5, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW_FIRE_03].dat, sx_anchor - 14, sy_anchor + 10);
					} else if (fire_key_state > 3) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW_FIRE_02].dat, sx_anchor + 5, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW_FIRE_02].dat, sx_anchor - 14, sy_anchor + 10);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW_FIRE_01].dat, sx_anchor + 5, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW_FIRE_01].dat, sx_anchor - 14, sy_anchor + 10);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW].dat, sx_anchor + 5, sy_anchor + 10);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_CHAINSAW].dat, sx_anchor - 14, sy_anchor + 10);
				}
			}

			// This is the railgun code
			if (player.current_weapon == 11 && player.rails_left >= 0) {
				if (player.rails_left > 0 && fire_key_state != 0) {
					if (fire_key_state > 64) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_RAILGUN].dat, sx_anchor + 10, sy_anchor + 12);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_RAILGUN].dat, sx_anchor, sy_anchor + 12);
					} else if (fire_key_state > 48) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_02].dat, sx_anchor + 10, sy_anchor + 5);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_02].dat, sx_anchor - 5, sy_anchor + 5);
					} else if (fire_key_state > 32) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_01].dat, sx_anchor + 10, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_01].dat, sx_anchor - 3, sy_anchor + 8);
					} else if (fire_key_state > 16) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_03].dat, sx_anchor + 10, sy_anchor + 1);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_03].dat, sx_anchor - 8, sy_anchor + 1);
					} else if (fire_key_state > 8) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_02].dat, sx_anchor + 10, sy_anchor + 5);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_02].dat, sx_anchor - 5, sy_anchor + 5);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_01].dat, sx_anchor + 10, sy_anchor + 8);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_RAILGUN_FIRE_01].dat, sx_anchor - 3, sy_anchor + 8);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_RAILGUN].dat, sx_anchor + 10, sy_anchor + 12);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_RAILGUN].dat, sx_anchor, sy_anchor + 12);
				}
			}

			// This is the starduster code
			if (player.current_weapon == 12 && player.spikestars_left >= 0) {
				if (player.spikestars_left > 0 && fire_key_state != 0) {
					if (fire_key_state > 90) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER].dat, sx_anchor, sy_anchor + 10);
					} else if (fire_key_state > 80) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_01].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_01].dat, sx_anchor, sy_anchor + 10);
					} else if (fire_key_state > 70) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_02].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_02].dat, sx_anchor, sy_anchor + 10);
					} else if (fire_key_state > 60) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_03].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_03].dat, sx_anchor - 5, sy_anchor + 10);
					} else if (fire_key_state > 50) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_04].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_04].dat, sx_anchor - 7, sy_anchor + 10);
					} else if (fire_key_state > 40) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_05].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_05].dat, sx_anchor - 8, sy_anchor + 10);
					} else if (fire_key_state > 30) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_04].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_04].dat, sx_anchor - 7, sy_anchor + 10);
					} else if (fire_key_state > 20) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_03].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_03].dat, sx_anchor - 5, sy_anchor + 10);
					} else if (fire_key_state > 10) {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_02].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_02].dat, sx_anchor, sy_anchor + 10);
					} else {
						if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_01].dat, sx_anchor + 6, sy_anchor + 10);
						else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER_FIRE_01].dat, sx_anchor, sy_anchor + 10);
					}
				} else {
					if (player.direction == 0) draw_sprite(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER].dat, sx_anchor + 6, sy_anchor + 10);
					else draw_sprite_h_flip(double_buffer, worminator_data_file[WORMINATOR_STARDUSTER].dat, sx_anchor, sy_anchor + 10);
				}
			}
		}
	}
}

/***************************\
|** CHECK PLAYER TELEPORT **|
\***************************/
char check_player_teleport()
{
	unsigned char x_start = 0, y_start = 0;
	unsigned char test_tile_number, teleport_tile_number = 0;

	int x_tile_loop, y_tile_loop;

	for (x_tile_loop = player.x_position_in_pixels - 4; x_tile_loop < player.x_position_in_pixels + 19; x_tile_loop += 11) {
		for (y_tile_loop = player.y_position_in_pixels - 8; y_tile_loop <= player.y_position_in_pixels + 24; y_tile_loop += 16) {
			test_tile_number = worminator_map.info_tile_grid[(int)(x_tile_loop / 16)][(int)(y_tile_loop / 16)];
			if (test_tile_number >= teleporter_1_info_tile && test_tile_number <= teleporter_16_info_tile) {
				x_start = (int)(x_tile_loop / 16), y_start = (int)(y_tile_loop / 16);
				teleport_tile_number = test_tile_number;
			} else if (test_tile_number >= exit_to_level_1_info_tile && test_tile_number <= exit_to_level_16_info_tile) {
				game_is_running = FALSE;
				if (alert("Do you really want", "to exit this level", "right now?", "Yes", "No", 0, 0) == 1)
					if (load_map((test_tile_number - exit_to_level_1_info_tile) + 1, NULL, 0))
						return TRUE;
				game_is_running = TRUE;
				return FALSE;
			} else if (test_tile_number >= exit_to_level_17_info_tile && test_tile_number <= exit_to_level_24_info_tile) {
				game_is_running = FALSE;
				if (alert("Do you really want", "to exit this level", "right now?", "Yes", "No", 0, 0) == 1)
					if (load_map((test_tile_number - exit_to_level_17_info_tile) + 1, NULL, 0))
						return TRUE;
				game_is_running = TRUE;
				return FALSE;
			}
		}
	}

	if (teleport_tile_number) {
		for (x_tile_loop = 0; x_tile_loop < worminator_map.map_width; x_tile_loop++) {
			for (y_tile_loop = 0; y_tile_loop < worminator_map.map_height; y_tile_loop++) {
				if (worminator_map.info_tile_grid[x_tile_loop][y_tile_loop] == teleport_tile_number && (x_tile_loop != x_start || y_tile_loop != y_start)) {
					player.x_position_in_pixels = x_tile_loop * 16;	/* + (player.x_position_in_pixels - x_start * 16);*/
					player.y_position_in_pixels = y_tile_loop * 16;	/* + (player.y_position_in_pixels - y_start * 16);*/
					player.y_acceleration = 0;
					check_enemy_ranges();
				}
			}
		}
	}
	return FALSE;
}

/******************\
|** CLEAR PLAYER **|
\******************/
void clear_player()
{
	player.timer = 0;
	player.width = 0;
	player.height = 0;
	player.frame = 0;
	player.animation_timer = 0;
	player.direction = 0;
	player.current_weapon = 0, player.ammo_type = 0;
	player.on_ground = 0;
	player.in_water = FALSE;
	player.is_ducking = FALSE;
	player.letter_order = 0;
	player.health_bars_left = 10;
	player.has_red_keycard = FALSE, player.has_green_keycard = FALSE, player.has_blue_keycard = FALSE;
	player.bullets_left = -1, player.shells_left = -1, player.rockets_left = -1, player.cells_left = -1, player.gasoline_left = -1, player.mortars_left = -1, player.plasma_left = -1, player.mines_left = -1;
	player.chainsaw_duration_left = -1, player.plasma_bullets_left = 0, player.explosive_shells_left = 0, player.micro_nukes_left = 0, player.walking_mines_left = 0, player.spikestars_left = -1, player.rails_left = -1;
	player.x_position_in_pixels = 0;
	player.y_position_in_pixels = 0;
	player.x_acceleration = 0;
	player.y_acceleration = 0;
	if (wormy_config.remember_skin == TRUE) player.skin = wormy_config.skin;
	else player.skin = WORMINATOR_SKIN;
	player.turbo_launch = FALSE;
	player.no_control = FALSE;

	desired_weapon = 0;
}

/*************************\
|** UPDATE PLAYER STATS **|
\*************************/
void update_player_stats()
{
	// Refresh the ammo display
	blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 292, 140, 292, 140, 20, 8);
	if (player.current_weapon == 0) sprintf(message, "000");
	else if (player.current_weapon == 1 && player.ammo_type == 0 && player.bullets_left > 0) sprintf(message, "%-5d", player.bullets_left);
	else if (player.current_weapon == 1 && player.ammo_type == 0 && player.bullets_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 1 && player.ammo_type == 1 && player.plasma_bullets_left > 0) sprintf(message, "%-5d", player.plasma_bullets_left);
	else if (player.current_weapon == 1 && player.ammo_type == 1 && player.plasma_bullets_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 2 && player.ammo_type == 0 && player.shells_left > 0) sprintf(message, "%-5d", player.shells_left);
	else if (player.current_weapon == 2 && player.ammo_type == 0 && player.shells_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 2 && player.ammo_type == 1 && player.explosive_shells_left > 0) sprintf(message, "%-5d", player.explosive_shells_left);
	else if (player.current_weapon == 2 && player.ammo_type == 1 && player.explosive_shells_left <= 0) sprintf(message, "000");
	else if ((player.current_weapon == 3 || player.current_weapon == 4) && player.ammo_type == 0 && player.rockets_left > 0) sprintf(message, "%-5d", player.rockets_left);
	else if ((player.current_weapon == 3 || player.current_weapon == 4) && player.ammo_type == 0 && player.rockets_left <= 0) sprintf(message, "000");
	else if ((player.current_weapon == 3 || player.current_weapon == 4) && player.ammo_type == 1 && player.micro_nukes_left > 0) sprintf(message, "%-5d", player.micro_nukes_left);
	else if ((player.current_weapon == 3 || player.current_weapon == 4) && player.ammo_type == 1 && player.micro_nukes_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 5 && player.cells_left > 0) sprintf(message, "%-5d", player.cells_left);
	else if (player.current_weapon == 5 && player.cells_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 6 && player.gasoline_left > 0) sprintf(message, "%-5d", player.gasoline_left);
	else if (player.current_weapon == 6 && player.gasoline_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 7 && player.mortars_left > 0) sprintf(message, "%-5d", player.mortars_left);
	else if (player.current_weapon == 7 && player.mortars_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 8 && player.plasma_left > 0) sprintf(message, "%-5d", player.plasma_left);
	else if (player.current_weapon == 8 && player.plasma_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 9 && player.ammo_type == 0 && player.mines_left > 0) sprintf(message, "%-5d", player.mines_left);
	else if (player.current_weapon == 9 && player.ammo_type == 0 && player.mines_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 9 && player.ammo_type == 1 && player.walking_mines_left > 0) sprintf(message, "%-5d", player.walking_mines_left);
	else if (player.current_weapon == 9 && player.ammo_type == 1 && player.walking_mines_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 10 && player.chainsaw_duration_left > 0) sprintf(message, "%-5d", player.chainsaw_duration_left);
	else if (player.current_weapon == 10 && player.chainsaw_duration_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 11 && player.rails_left > 0) sprintf(message, "%-3d", player.rails_left);
	else if (player.current_weapon == 11 && player.rails_left <= 0) sprintf(message, "000");
	else if (player.current_weapon == 12 && player.spikestars_left > 0) sprintf(message, "%-3d", player.spikestars_left);
	else if (player.current_weapon == 12 && player.spikestars_left <= 0) sprintf(message, "000");

	if (player.ammo_type == 1 && (player.current_weapon == 1 || player.current_weapon == 2 || player.current_weapon == 3 || player.current_weapon == 10)) textout(screen_buffer, worminator_data_file[SMALL_NUMBERS_FONT].dat, message, 293, 140, 41);
	else textout(screen_buffer, worminator_data_file[SMALL_NUMBERS_FONT].dat, message, 293, 140, -1);

	// Highlight the weapons the Worminator has
	if (player.bullets_left > 0 && player.current_weapon == 1) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 129, 293, 129, 18, 9);
	else if (player.bullets_left == 0 && player.current_weapon == 1) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 129, 293, 129, 18, 9);
	else if (player.bullets_left == 0 && player.current_weapon != 1) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 129, 293, 129, 18, 9);
	else if (player.bullets_left > 0 && player.current_weapon != 1) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 129, 293, 129, 18, 9);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 129, 293, 129, 18, 9);

	if (player.shells_left > 0 && player.current_weapon == 2) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 121, 293, 121, 18, 8);
	else if (player.shells_left == 0 && player.current_weapon == 2) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 121, 293, 121, 18, 8);
	else if (player.shells_left == 0 && player.current_weapon != 2) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 121, 293, 121, 18, 8);
	else if (player.shells_left > 0 && player.current_weapon != 2) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 121, 293, 121, 18, 8);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 121, 293, 121, 18, 8);

	if (player.rockets_left > 0 && (player.current_weapon == 3 || player.current_weapon == 4)) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 113, 293, 113, 18, 8);
	else if (player.rockets_left == 0 && (player.current_weapon == 3 || player.current_weapon == 4)) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 113, 293, 113, 18, 8);
	else if (player.rockets_left == 0 && player.current_weapon != 3 && player.current_weapon != 4) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 113, 293, 113, 18, 8);
	else if (player.rockets_left > 0 && player.current_weapon != 3 && player.current_weapon != 4) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 113, 293, 113, 18, 8);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 113, 293, 113, 18, 8);

	if (player.cells_left > 0 && player.current_weapon == 5) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 103, 293, 103, 18, 9);
	else if (player.cells_left == 0 && player.current_weapon == 5) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 103, 293, 103, 18, 9);
	else if (player.cells_left == 0 && player.current_weapon != 5) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 103, 293, 103, 18, 9);
	else if (player.cells_left > 0 && player.current_weapon != 5) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 103, 293, 103, 18, 9);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 103, 293, 103, 18, 9);

	if (player.gasoline_left > 0 && player.current_weapon == 6) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 92, 293, 92, 18, 11);
	else if (player.gasoline_left == 0 && player.current_weapon == 6) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 92, 293, 92, 18, 11);
	else if (player.gasoline_left == 0 && player.current_weapon != 6) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 92, 293, 92, 18, 11);
	else if (player.gasoline_left > 0 && player.current_weapon != 6) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 92, 293, 92, 18, 11);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 92, 293, 92, 18, 11);

	if (player.mortars_left > 0 && player.current_weapon == 7) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 85, 293, 85, 18, 8);
	else if (player.mortars_left == 0 && player.current_weapon == 7) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 85, 293, 85, 18, 8);
	else if (player.mortars_left == 0 && player.current_weapon != 7) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 85, 293, 85, 18, 8);
	else if (player.mortars_left > 0 && player.current_weapon != 7) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 85, 293, 85, 18, 8);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 85, 293, 85, 18, 8);

	if (player.plasma_left > 0 && player.current_weapon == 8) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 76, 293, 76, 18, 9);
	else if (player.plasma_left == 0 && player.current_weapon == 8) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 76, 293, 76, 18, 9);
	else if (player.plasma_left == 0 && player.current_weapon != 8) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 76, 293, 76, 18, 9);
	else if (player.plasma_left > 0 && player.current_weapon != 8) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 76, 293, 76, 18, 9);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 76, 293, 76, 18, 9);

	if (player.mines_left > 0 && player.current_weapon == 9) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 37, 71, 297, 71, 10, 5);
	else if (player.mines_left == 0 && player.current_weapon == 9) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 37, 71, 297, 71, 10, 5);
	else if (player.mines_left == 0 && player.current_weapon != 9) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 37, 71, 297, 71, 10, 5);
	else if (player.mines_left > 0 && player.current_weapon != 9) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 37, 71, 297, 71, 10, 5);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 297, 71, 297, 71, 10, 5);

	if (player.chainsaw_duration_left > 0 && player.current_weapon == 10) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 64, 293, 64, 18, 6);
	else if (player.chainsaw_duration_left == 0 && player.current_weapon == 10) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 64, 293, 64, 18, 6);
	else if (player.chainsaw_duration_left == 0 && player.current_weapon != 10) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 64, 293, 64, 18, 6);
	else if (player.chainsaw_duration_left > 0 && player.current_weapon != 10) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 64, 293, 64, 18, 6);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 64, 293, 64, 18, 6);

	if (player.rails_left > 0 && player.current_weapon == 11) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 57, 293, 57, 18, 6);
	else if (player.rails_left == 0 && player.current_weapon == 11) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 57, 293, 57, 18, 6);
	else if (player.rails_left == 0 && player.current_weapon != 11) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 57, 293, 57, 18, 6);
	else if (player.rails_left > 0 && player.current_weapon != 11) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 57, 293, 57, 18, 6);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 57, 293, 57, 18, 6);

	if (player.spikestars_left > 0 && player.current_weapon == 12) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_GLOW].dat, screen_buffer, 33, 47, 293, 47, 18, 9);
	else if (player.spikestars_left == 0 && player.current_weapon == 12) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY_GLOW].dat, screen_buffer, 33, 47, 293, 47, 18, 9);
	else if (player.spikestars_left == 0 && player.current_weapon != 12) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH_EMPTY].dat, screen_buffer, 33, 47, 293, 47, 18, 9);
	else if (player.spikestars_left > 0 && player.current_weapon != 12) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 33, 47, 293, 47, 18, 9);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 293, 47, 293, 47, 18, 9);

	// Highlight the keycards the Worminator has
	if (player.has_red_keycard == TRUE) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 24, 159, 284, 159, 2, 4);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 284, 159, 284, 159, 2, 4);

	if (player.has_green_keycard == TRUE) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 29, 159, 289, 159, 2, 4);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 289, 159, 289, 159, 2, 4);

	if (player.has_blue_keycard == TRUE) blit(worminator_data_file[WORMINATOR_HUD_ITEMS_PATCH].dat, screen_buffer, 34, 159, 294, 159, 2, 4);
	else blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 294, 159, 294, 159, 2, 4);

	// Show the player's score

	// Wipe clean the score display area
	blit(worminator_data_file[WORMINATOR_HUD].dat, screen_buffer, 268, 180, 268, 180, 44, 11);

	// Draw in the new score
	if (player.score >= 1000000) {
		sprintf(message, "%-5d", player.score);
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	} else if (player.score >= 100000) {
		sprintf(message, "0%-5d", player.score);
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	} else if (player.score >= 10000) {
		sprintf(message, "00%-5d", player.score);
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	} else if (player.score >= 1000) {
		sprintf(message, "000%-5d", player.score);
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	} else if (player.score >= 100) {
		sprintf(message, "0000%-5d", player.score);
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	} else if (player.score >= 10) {
		sprintf(message, "00000%-5d", player.score);
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	} else if (player.score > 0) {
		sprintf(message, "000000%-5d", player.score);
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	} else if (player.score == 0) {
		sprintf(message, "0000000");
		textout(screen_buffer, worminator_data_file[LARGE_NUMBERS_FONT].dat, message, 269, 182, 167);
	}
	stats_dirty = TRUE;
}

/*****************\
|** HURT PLAYER **|
\*****************/
void hurt_player(char amount_to_hurt)
{
	unsigned char spawn_loop;

	// Return if god mode is enabled
	if (wormy_config.godly_mode == TRUE) return;

	// Reset the hp regenereation timer
	if (amount_to_hurt < 0) hp_regen_timer = 1;

	// Adjust damage for special modes
	if (wormy_config.difficulty == DIFFICULTY_WIMP && amount_to_hurt < -1) {
		player.timer = 128;
		if (amount_to_hurt == -1 && rand() % 10 < 5) {
			amount_to_hurt = 0;
		} else {
			amount_to_hurt /= 2;
			//amount_to_hurt++;
			if (amount_to_hurt > -1) amount_to_hurt = -1;
			if (amount_to_hurt == -1 && rand() % 10 < 5) amount_to_hurt = 0;
		}
	} else if (wormy_config.difficulty == DIFFICULTY_EASY && amount_to_hurt < -2) {
		amount_to_hurt++;
	} else if (wormy_config.difficulty == DIFFICULTY_HARD && amount_to_hurt <= -1) {
		amount_to_hurt--;
	} else if (wormy_config.difficulty == DIFFICULTY_NIGHTMARE && amount_to_hurt <= -1) {
		amount_to_hurt -= 2;
	}

	// Do actual damage
	if (player.frame != 0 && amount_to_hurt < 0) {
		player.health_bars_left += amount_to_hurt;
		player.timer = 128;
		for (spawn_loop = 0; spawn_loop < 16; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, player.x_position_in_pixels + 15, player.y_position_in_pixels + 15, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
	} else if (amount_to_hurt > 0) {
		player.health_bars_left += amount_to_hurt;
	}

	// Check for max/min health
	if (player.health_bars_left > 10) player.health_bars_left = 10;
	if (player.health_bars_left < 0) player.health_bars_left = 0;

	// Update health bars
	if (player.health_bars_left >= 10) rectfill(screen_buffer, 270, 50, 286, 55, 74);
	else rectfill(screen_buffer, 270, 50, 286, 55, 255);
	if (player.health_bars_left >= 9) rectfill(screen_buffer, 270, 60, 286, 65, 74);
	else rectfill(screen_buffer, 270, 60, 286, 65, 255);
	if (player.health_bars_left >= 8) rectfill(screen_buffer, 270, 70, 286, 75, 73);
	else rectfill(screen_buffer, 270, 70, 286, 75, 255);
	if (player.health_bars_left >= 7) rectfill(screen_buffer, 270, 80, 286, 85, 73);
	else rectfill(screen_buffer, 270, 80, 286, 85, 255);
	if (player.health_bars_left >= 6) rectfill(screen_buffer, 270, 90, 286, 95, 72);
	else rectfill(screen_buffer, 270, 90, 286, 95, 255);
	if (player.health_bars_left >= 5) rectfill(screen_buffer, 270, 100, 286, 105, 71);
	else rectfill(screen_buffer, 270, 100, 286, 105, 255);
	if (player.health_bars_left >= 4) rectfill(screen_buffer, 270, 110, 286, 115, 70);
	else rectfill(screen_buffer, 270, 110, 286, 115, 255);
	if (player.health_bars_left >= 3) rectfill(screen_buffer, 270, 120, 286, 125, 70);
	else rectfill(screen_buffer, 270, 120, 286, 125, 255);
	if (player.health_bars_left >= 2) rectfill(screen_buffer, 270, 130, 286, 135, 69);
	else rectfill(screen_buffer, 270, 130, 286, 135, 255);
	if (player.health_bars_left >= 1) rectfill(screen_buffer, 270, 140, 286, 145, 69);
	else rectfill(screen_buffer, 270, 140, 286, 145, 255);

	// Update worminator face
	if (player.health_bars_left > 0) {
		switch (player.skin) {
		case THE_BRAIN_SKIN:
			blit(worminator_data_file[SKIN_THE_BRAIN_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case THE_YIPNIK_MIKIMBO_SKIN:
			blit(worminator_data_file[SKIN_THE_YIPNIK_MIKIMBO_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case ORIGINAL_WORMINATOR_SKIN:
			blit(worminator_data_file[SKIN_ORIGINAL_WORMINATOR_FACE_PATCH].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case HOTDOG_HERO_SKIN:
			blit(worminator_data_file[SKIN_HOTDOG_HERO_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case VOODOO_BADIE_SKIN:
			blit(worminator_data_file[SKIN_VOODOO_BADIE_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case YUANTI_SKIN:
			blit(worminator_data_file[SKIN_YUANTI_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case MELON_O_DEATH_SKIN:
			blit(worminator_data_file[SKIN_MELON_O_DEATH_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case BAKED_TURKEY_SKIN:
			blit(worminator_data_file[SKIN_BAKED_TURKEY_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case WATERMELON_WONDER_SKIN:
			blit(worminator_data_file[SKIN_WATERMELON_WONDER_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case THE_INVERTEBRATOR_SKIN:
			blit(worminator_data_file[SKIN_THE_INVERTEBRATOR_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case BROWNIE_SKIN:
			blit(worminator_data_file[SKIN_BROWNIE_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case EVIL_CUBE_O_DEATH_SKIN:
			blit(worminator_data_file[SKIN_EVIL_CUBE_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case ENDO_SKELETON_SKIN:
			blit(worminator_data_file[SKIN_ENDO_SKELETON_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case MUMMY_WORM_SKIN:
			blit(worminator_data_file[SKIN_MUMMY_WORM_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case THE_ANGEL_SKIN:
			blit(worminator_data_file[SKIN_THE_ANGEL_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		case THE_DEVIL_SKIN:
			blit(worminator_data_file[SKIN_THE_DEVIL_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		default:
			blit(worminator_data_file[WORMINATOR_FACE_PATCH_01 + 10 - player.health_bars_left].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
			break;
		}
	}
	stats_dirty = TRUE;
}

/*****************\
|** KILL PLAYER **|
\*****************/
void kill_player()
{
	char done = FALSE;
	unsigned char spawn_loop;
	int frame_loop = 0;

	// Deduct points for dying
	player.score -= 5000;
	if (player.score < 0) player.score = 0;

	// Update worminator face
	switch (player.skin) {
	case THE_BRAIN_SKIN:
		blit(worminator_data_file[SKIN_THE_BRAIN_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case THE_YIPNIK_MIKIMBO_SKIN:
		blit(worminator_data_file[SKIN_THE_YIPNIK_MIKIMBO_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case ORIGINAL_WORMINATOR_SKIN:
		blit(worminator_data_file[SKIN_ORIGINAL_WORMINATOR_FACE_PATCH].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case HOTDOG_HERO_SKIN:
		blit(worminator_data_file[SKIN_HOTDOG_HERO_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case VOODOO_BADIE_SKIN:
		blit(worminator_data_file[SKIN_VOODOO_BADIE_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case YUANTI_SKIN:
		blit(worminator_data_file[SKIN_YUANTI_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case MELON_O_DEATH_SKIN:
		blit(worminator_data_file[SKIN_MELON_O_DEATH_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case BAKED_TURKEY_SKIN:
		blit(worminator_data_file[SKIN_BAKED_TURKEY_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case WATERMELON_WONDER_SKIN:
		blit(worminator_data_file[SKIN_WATERMELON_WONDER_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case THE_INVERTEBRATOR_SKIN:
		blit(worminator_data_file[SKIN_THE_INVERTEBRATOR_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case BROWNIE_SKIN:
		blit(worminator_data_file[SKIN_BROWNIE_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case EVIL_CUBE_O_DEATH_SKIN:
		blit(worminator_data_file[SKIN_EVIL_CUBE_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case ENDO_SKELETON_SKIN:
		blit(worminator_data_file[SKIN_ENDO_SKELETON_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case MUMMY_WORM_SKIN:
		blit(worminator_data_file[SKIN_MUMMY_WORM_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case THE_ANGEL_SKIN:
		blit(worminator_data_file[SKIN_THE_ANGEL_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	case THE_DEVIL_SKIN:
		blit(worminator_data_file[SKIN_THE_DEVIL_FACE_PATCH_01].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	default:
		blit(worminator_data_file[WORMINATOR_FACE_PATCH_DEAD].dat, screen_buffer, 0, 0, 271, 7, 38, 34);
		break;
	}

	// Create lots of blood spots and some gibs and shrapnel
	for (spawn_loop = 0; spawn_loop < 128; spawn_loop++) spawn_sprite(SMALL_RED_SPARK_SPRITE, player.x_position_in_pixels + 15, player.y_position_in_pixels + 15, (float)((rand() % 40) - 20.00) / 20.00, (float)((rand() % 40) - 20.00) / 20.00, 0);
	spawn_shrapgibs(player.x_position_in_pixels + 16, player.y_position_in_pixels + 16, 2, 8);

	// Don't let the player be drawn
	if (player.skin == WORMINATOR_SKIN) {
		player.frame = WORMINATOR_DIE_A_01;
		player.timer = 0;
	} else {
		player.frame = 0;
	}

	// Disable bullet time
	change_game_speed(wormy_config.game_speed);
	bullet_time_engaged = FALSE;

	// Run for 256 frames to allow the blood spots to move and the death animation to run
	do {
		while (speed_counter > 0 && done != TRUE) {
			// Gravity on dying worminator
			player.y_acceleration += 0.02;
			if (player.on_ground == TRUE && player.y_acceleration > 1) player.y_acceleration = 1;
			else if (player.y_acceleration > 2) player.y_acceleration = 2;
			push_player(0, player.y_acceleration);

			update_sprites();
			frame_loop++;
			if (player.skin == WORMINATOR_SKIN) {
				if ((frame_loop + 1) % 16 == 0) player.frame++;
				if (player.frame > WORMINATOR_DIE_A_12) player.frame = WORMINATOR_DIE_A_12;
			}
			if (frame_loop >= 256) done = TRUE;
			speed_counter--;
		}
		render_map();
	} while (done != TRUE);

	// Fade out the display and clear out some junk
	fade_out(32);
	clear_sprites();
	clear_player();

	// Re-load the level
	if (current_level != -1)
		player_just_died = TRUE;

	load_map(current_level, current_level_mapfile_path, 0);
}
