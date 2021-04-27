// Load/Save utility functions for loading/saving intel generated binary
// files on non intel.

void read_byte(unsigned char *data, FILE *f)
{
	if (fread(data, 1, 1, f) != 1) {
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		alert("Fatal error: short read while", "reading a map-, savegame- or demo-file.", "Truncated / invalid file?  Aborting!", "OK", 0, 0, 0);
		exit(1);
	}
}

void read_bytes(unsigned char *data, unsigned int count, FILE *f)
{
	if (fread(data, 1, count, f) != count) {
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		alert("Fatal error: short read while", "reading a map-, savegame- or demo-file.", "Truncated / invalid file?  Aborting!", "OK", 0, 0, 0);
		exit(1);
	}
}

#ifndef READ_STRUCTS_AT_ONCE
void read_word(unsigned short *data, FILE *f)
{
	unsigned char a;
	unsigned char b;

	read_byte(&a, f);
	read_byte(&b, f);
	*data = ((unsigned short)a) | (((unsigned short)b) << 8);
}

void read_words(unsigned short *data, int count, FILE *f)
{
	int i;

	for (i = 0; i < count; i++)
		read_word(data + i, f);
}
#endif

void read_dword(unsigned int *data, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fread(data, sizeof(unsigned int), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: short read while", "reading a map-, savegame- or demo-file.", "Truncated / invalid file?  Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char a, b, c, d;
	read_byte(&a, f);
	read_byte(&b, f);
	read_byte(&c, f);
	read_byte(&d, f);
	*data = ((unsigned int)a) | (((unsigned int)b) << 8) |
		(((unsigned int)c) << 16) | (((unsigned int)d) << 24);
#endif
}

void read_worminator_map(WORMINATOR_MAP_FORMAT *map, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fread(map, sizeof(WORMINATOR_MAP_FORMAT), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: short read while", "reading a map-, savegame- or demo-file.", "Truncated / invalid file?  Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char padding[3];
	int i;

	read_word(&map->map_width, f);
	read_word(&map->map_height, f);
	read_bytes((unsigned char *)map->map_name, 80, f);
	read_words((unsigned short *)map->background_tile_grid[0], 256 * 192, f);
	read_words((unsigned short *)map->foreground_tile_grid[0], 256 * 192, f);
	read_words((unsigned short *)map->info_tile_grid[0], 256 * 192, f);
	read_bytes((unsigned char *)map->graphical_properties_tile_grid[0], 256 * 192, f);
	for (i = 0; i < 256; i++) {
		read_word((unsigned short *)&map->starting_sprites[i].sprite_type, f);
		read_word((unsigned short *)padding, f);
		read_dword((unsigned int *)
			   &map->starting_sprites[i].x_position_in_pixels, f);
		read_dword((unsigned int *)
			   &map->starting_sprites[i].y_position_in_pixels, f);
	}
	read_byte((unsigned char *)&map->weather, f);
	read_bytes(padding, 3, f);
#endif
}

void read_sprite(SPRITE *sprite, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fread(sprite, sizeof(SPRITE), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: short read while", "reading a map-, savegame- or demo-file.", "Truncated / invalid file?  Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char padding[3];
	union { float f; unsigned int u; } un;

	read_dword((unsigned int *)&sprite->sprite_active, f);
	read_dword((unsigned int *)&sprite->sprite_type, f);
	read_byte((unsigned char *)&sprite->in_range, f);
	read_bytes(padding, 3, f);
	read_dword((unsigned int *)&sprite->timer, f);
	read_byte((unsigned char *)&sprite->animation_state, f);
	read_bytes(padding, 3, f);
	read_dword((unsigned int *)&sprite->width, f);
	read_dword((unsigned int *)&sprite->height, f);
	read_dword((unsigned int *)&sprite->action_state, f);
	read_dword((unsigned int *)&sprite->frame, f);
	read_dword(&un.u, f);
	sprite->hit_points_left = un.f;
	read_dword(&un.u, f);
	sprite->x_position_in_pixels = un.f;
	read_dword(&un.u, f);
	sprite->y_position_in_pixels = un.f;
	read_dword(&un.u, f);
	sprite->x_acceleration = un.f;
	read_dword(&un.u, f);
	sprite->y_acceleration = un.f;
	read_byte((unsigned char *)&sprite->direction, f);
	read_byte(&sprite->rotate_factor, f);
	read_bytes(padding, 2, f);
#endif
}

void read_sprites(SPRITE *sprite, int count, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fread(sprite, sizeof(SPRITE), count, f) != count)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: short read while", "reading a map-, savegame- or demo-file.", "Truncated / invalid file?  Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	int i;
	for (i = 0; i < count; i++)
		read_sprite(sprite + i, f);
#endif
}

void read_player(PLAYER *player, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fread(player, sizeof(PLAYER), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: short read while", "reading a map-, savegame- or demo-file.", "Truncated / invalid file?  Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char padding[1];
	union { float f; unsigned int u; } un;

	read_dword((unsigned int *)&player->timer, f);
	read_dword((unsigned int *)&player->width, f);
	read_dword((unsigned int *)&player->height, f);
	read_dword((unsigned int *)&player->frame, f);
	read_byte((unsigned char *)&player->animation_timer, f);
	read_byte((unsigned char *)&player->direction, f);
	read_byte((unsigned char *)&player->current_weapon, f);
	read_byte((unsigned char *)&player->ammo_type, f);
	read_byte((unsigned char *)&player->on_ground, f);
	read_byte((unsigned char *)&player->in_water, f);
	read_byte((unsigned char *)&player->is_ducking, f);
	read_byte((unsigned char *)&player->letter_order, f);
	read_dword((unsigned int *)&player->score, f);
	read_byte((unsigned char *)&player->health_bars_left, f);
	read_byte((unsigned char *)&player->has_red_keycard, f);
	read_byte((unsigned char *)&player->has_green_keycard, f);
	read_byte((unsigned char *)&player->has_blue_keycard, f);
	read_dword((unsigned int *)&player->bullets_left, f);
	read_dword((unsigned int *)&player->shells_left, f);
	read_dword((unsigned int *)&player->rockets_left, f);
	read_dword((unsigned int *)&player->cells_left, f);
	read_dword((unsigned int *)&player->gasoline_left, f);
	read_dword((unsigned int *)&player->mortars_left, f);
	read_dword((unsigned int *)&player->plasma_left, f);
	read_dword((unsigned int *)&player->mines_left, f);
	read_dword((unsigned int *)&player->chainsaw_duration_left, f);
	read_dword((unsigned int *)&player->plasma_bullets_left, f);
	read_dword((unsigned int *)&player->explosive_shells_left, f);
	read_dword((unsigned int *)&player->micro_nukes_left, f);
	read_dword((unsigned int *)&player->walking_mines_left, f);
	read_dword((unsigned int *)&player->spikestars_left, f);
	read_dword((unsigned int *)&player->rails_left, f);
	read_dword(&un.u, f);
	player->x_position_in_pixels = un.f;
	read_dword(&un.u, f);
	player->y_position_in_pixels = un.f;
	read_dword(&un.u, f);
	player->x_acceleration = un.f;
	read_dword(&un.u, f);
	player->y_acceleration = un.f;
	read_byte((unsigned char *)&player->skin, f);
	read_byte((unsigned char *)&player->turbo_launch, f);
	read_byte((unsigned char *)&player->no_control, f);
	read_byte(padding, f);
#endif
}

/**************************************************************************/

void write_byte(const unsigned char *data, FILE *f)
{
	if (fwrite(data, 1, 1, f) != 1) {
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		alert("Fatal error: while writing a", "map-, savegame- or demo-file, disk full?", "Aborting!", "OK", 0, 0, 0);
		exit(1);
	}
}

void write_bytes(const unsigned char *data, unsigned int count, FILE *f)
{
	if (fwrite(data, 1, count, f) != count) {
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
		alert("Fatal error: while writing a", "map-, savegame- or demo-file, disk full?", "Aborting!", "OK", 0, 0, 0);
		exit(1);
	}
}

#ifndef READ_STRUCTS_AT_ONCE
void write_word(const unsigned short *data, FILE *f)
{
	unsigned char a = *data;
	unsigned char b = *data >> 8;

	write_byte(&a, f);
	write_byte(&b, f);
}

void write_words(const unsigned short *data, int count, FILE *f)
{
	int i;

	for (i = 0; i < count; i++)
		write_word(data + i, f);
}
#endif

void write_dword(const unsigned int *data, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fwrite(data, sizeof(unsigned int), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: while writing a", "map-, savegame- or demo-file, disk full?", "Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char a = *data;
	unsigned char b = *data >> 8;
	unsigned char c = *data >> 16;
	unsigned char d = *data >> 24;
	write_byte(&a, f);
	write_byte(&b, f);
	write_byte(&c, f);
	write_byte(&d, f);
#endif
}

void write_worminator_map(const WORMINATOR_MAP_FORMAT *map, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fwrite(map, sizeof(WORMINATOR_MAP_FORMAT), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: while writing a", "map-, savegame- or demo-file, disk full?", "Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char padding[3] = { 0, 0, 0 };
	int i;

	write_word(&map->map_width, f);
	write_word(&map->map_height, f);
	write_bytes((const unsigned char *)map->map_name, 80, f);
	write_words((const unsigned short *)map->background_tile_grid[0], 256 * 192, f);
	write_words((const unsigned short *)map->foreground_tile_grid[0], 256 * 192, f);
	write_words((const unsigned short *)map->info_tile_grid[0], 256 * 192, f);
	write_bytes((const unsigned char *)map->graphical_properties_tile_grid[0], 256 * 192, f);
	for (i = 0; i < 256; i++) {
		write_word((const unsigned short *)&map->starting_sprites[i].sprite_type, f);
		write_word((const unsigned short *)padding, f);
		write_dword((const unsigned int *)
			    &map->starting_sprites[i].x_position_in_pixels, f);
		write_dword((const unsigned int *)
			    &map->starting_sprites[i].y_position_in_pixels, f);
	}
	write_byte((const unsigned char *)&map->weather, f);
	write_bytes(padding, 3, f);
#endif
}

void write_sprite(const SPRITE *sprite, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fwrite(sprite, sizeof(SPRITE), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: while writing a", "map-, savegame- or demo-file, disk full?", "Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char padding[3] = { 0, 0, 0 };
	union { float f; unsigned int u; } un;

	write_dword((const unsigned int *)&sprite->sprite_active, f);
	write_dword((const unsigned int *)&sprite->sprite_type, f);
	write_byte((const unsigned char *)&sprite->in_range, f);
	write_bytes(padding, 3, f);
	write_dword((const unsigned int *)&sprite->timer, f);
	write_byte((const unsigned char *)&sprite->animation_state, f);
	write_bytes(padding, 3, f);
	write_dword((const unsigned int *)&sprite->width, f);
	write_dword((const unsigned int *)&sprite->height, f);
	write_dword((const unsigned int *)&sprite->action_state, f);
	write_dword((const unsigned int *)&sprite->frame, f);
	un.f = sprite->hit_points_left;
	write_dword(&un.u, f);
	un.f = sprite->x_position_in_pixels;
	write_dword(&un.u, f);
	un.f = sprite->y_position_in_pixels;
	write_dword(&un.u, f);
	un.f = sprite->x_acceleration;
	write_dword(&un.u, f);
	un.f = sprite->y_acceleration;
	write_dword(&un.u, f);
	write_byte((const unsigned char *)&sprite->direction, f);
	write_byte(&sprite->rotate_factor, f);
	write_bytes(padding, 2, f);
#endif
}

void write_sprites(const SPRITE *sprite, int count, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fwrite(sprite, sizeof(SPRITE), count, f) != count)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: while writing a", "map-, savegame- or demo-file, disk full?", "Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	int i;
	for (i = 0; i < count; i++)
		write_sprite(sprite + i, f);
#endif
}

void write_player(const PLAYER *player, FILE *f)
{
#ifdef READ_STRUCTS_AT_ONCE
	if (fwrite(player, sizeof(PLAYER), 1, f) != 1)
		// Ugly but better then no error handling at all
		set_pallete(worminator_data_file[DEFAULT_WORMINATOR_PALLETE].dat);
	alert("Fatal error: while writing a", "map-, savegame- or demo-file, disk full?", "Aborting!", "OK", 0, 0, 0);
	exit(1);
}
#else
	unsigned char padding[1] = { 0 };
	union { float f; unsigned int u; } un;

	write_dword((const unsigned int *)&player->timer, f);
	write_dword((const unsigned int *)&player->width, f);
	write_dword((const unsigned int *)&player->height, f);
	write_dword((const unsigned int *)&player->frame, f);
	write_byte((const unsigned char *)&player->animation_timer, f);
	write_byte((const unsigned char *)&player->direction, f);
	write_byte((const unsigned char *)&player->current_weapon, f);
	write_byte((const unsigned char *)&player->ammo_type, f);
	write_byte((const unsigned char *)&player->on_ground, f);
	write_byte((const unsigned char *)&player->in_water, f);
	write_byte((const unsigned char *)&player->is_ducking, f);
	write_byte((const unsigned char *)&player->letter_order, f);
	write_dword((const unsigned int *)&player->score, f);
	write_byte((const unsigned char *)&player->health_bars_left, f);
	write_byte((const unsigned char *)&player->has_red_keycard, f);
	write_byte((const unsigned char *)&player->has_green_keycard, f);
	write_byte((const unsigned char *)&player->has_blue_keycard, f);
	write_dword((const unsigned int *)&player->bullets_left, f);
	write_dword((const unsigned int *)&player->shells_left, f);
	write_dword((const unsigned int *)&player->rockets_left, f);
	write_dword((const unsigned int *)&player->cells_left, f);
	write_dword((const unsigned int *)&player->gasoline_left, f);
	write_dword((const unsigned int *)&player->mortars_left, f);
	write_dword((const unsigned int *)&player->plasma_left, f);
	write_dword((const unsigned int *)&player->mines_left, f);
	write_dword((const unsigned int *)&player->chainsaw_duration_left, f);
	write_dword((const unsigned int *)&player->plasma_bullets_left, f);
	write_dword((const unsigned int *)&player->explosive_shells_left, f);
	write_dword((const unsigned int *)&player->micro_nukes_left, f);
	write_dword((const unsigned int *)&player->walking_mines_left, f);
	write_dword((const unsigned int *)&player->spikestars_left, f);
	write_dword((const unsigned int *)&player->rails_left, f);
	un.f = player->x_position_in_pixels;
	write_dword(&un.u, f);
	un.f = player->y_position_in_pixels;
	write_dword(&un.u, f);
	un.f = player->x_acceleration;
	write_dword(&un.u, f);
	un.f = player->y_acceleration;
	write_dword(&un.u, f);
	write_byte((const unsigned char *)&player->skin, f);
	write_byte((const unsigned char *)&player->turbo_launch, f);
	write_byte((const unsigned char *)&player->no_control, f);
	write_byte(padding, f);
#endif
}
