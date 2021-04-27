/* Worminator level editor */
/* This program copyright 2002 by David Layne */

/*****************\
|** GLOBAL DATA **|
\*****************/
//BITMAP *worminator_tiles;
BITMAP *worminator_info_tiles;
BITMAP *worminator_graphical_properties_tiles;
BITMAP *ed_double_buffer;

WORMINATOR_MAP_FORMAT editor_map;

unsigned char tile_width = 16, tile_height = 16;

unsigned char mouse_button_1_register = 0;
unsigned char show_grid = 0, show_background_layer = 1, show_foreground_layer = 1, show_info_layer = 0, show_graphical_properties_layer = 0;
unsigned char current_editing_layer = 0;

int tile_grid_attribute = 0;
char multi_select_width = 0, multi_select_height = 0;
int sprite_number_attribute = 0;
int top_tile_offset = 0;
int ed_control_x = 0, ed_control_y = 0;
int ed_number_of_sprites = -1;

// A characted string used to tell what action is being performed
char *current_action;

// Stuff to see what grid square the mouse is in
int mouse_x_position = -1, mouse_y_position = -1;
int old_mouse_x_position = -1, old_mouse_y_position = -1;

/***********************\
|** FUNCTION PROTOTYPE**|
\***********************/

unsigned char ed_handle_input();
void draw_ed_controls();
int popout_menu(BITMAP *bmp, int y, int delay);
void popin_menu(BITMAP *bmp, int y, int delay);
void ed_render_map();
void set_map_layer_value(int x_in_pixels, int y_in_pixels, char layer, int attribute);
void ed_load_map(char *file_name);
void ed_save_map(char *file_name);

char select_layer_button();
char select_tile_button();
char grid_on_off_button();
char tools_menu_button();
char fill_area_button(int x, int y);
char patterned_fill_button(int x, int y);
char copy_paste_button(int x, int y);
char background_to_foreground_button(int x, int y);
char foreground_to_background_button(int x, int y);
char resize_level_button();
char add_sprite_button();
char new_map_button();
char display_properties_button();
char load_map_button();
char save_map_button();
char exit_program_button();

char check_scroll();
void refresh_tile_display();
void draw_tile_box(unsigned char x_tile, unsigned char y_tile, unsigned char width, unsigned char height);
char done_button(char draw_flags, int y);

/*******************\
|** MAIN FUNCTION **|
\*******************/
int editor()
{
	BITMAP *backup = create_bitmap_ex(8, screen_width, screen_height);
	BITMAP *plax_backup = parallax_background;
	unsigned char done = 0;

	// Backup the current display
	show_mouse(NULL); blit(screen, backup, 0, 0, 0, 0, screen_width, screen_height); show_mouse(screen);

	// Perform some editor initialization
	screen_x_position = screen_y_position = 0;
	ed_number_of_sprites = -1;
	ed_control_x = screen_width - 60, ed_control_y = 0;

	// This is the default size for a worminator map
	editor_map.map_width = 32;
	editor_map.map_height = 32;

	// Begin with the default action
	current_action = "Editing Background Layer. . .";
	ed_render_map();

	do {	// Start core editor loop
		// This accepts any and all input from the user
		done = ed_handle_input();

		// We use allegro's dialog functions, so this is required because the default colors suck with our pallete.
		gui_fg_color = 122;
		gui_bg_color = 133;

		// This code checks to see if the mouse is on another grid square and if so, updates the display
		if (!mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200)) {
			mouse_x_position = (mouse_x + screen_x_position) / 16;
			mouse_y_position = (mouse_y + screen_y_position) / 16;
			if (mouse_x_position != old_mouse_x_position || mouse_y_position != old_mouse_y_position) {
				ed_render_map();
				old_mouse_x_position = mouse_x_position;
				old_mouse_y_position = mouse_y_position;
			}
		}
	} while (done != 1);	// End core editor loop

	// Restore the current display
	parallax_background = plax_backup;
	show_mouse(NULL); blit(backup, screen, 0, 0, 0, 0, screen_width, screen_height); show_mouse(screen);

	destroy_bitmap(backup);
	return 0;
}

/******************\
|** HANDLE INPUT **|
\******************/
unsigned char ed_handle_input()
{
	char render_it = FALSE;
	char multi_select_loop_x, multi_select_loop_y;
	int sprite_delete_loop;

	/****************************************************************************\
	|** Check the sidebar buttons ***********************************************|
	\****************************************************************************/

	// Update mouse button status
	if (mouse_b & 1) {
		if (mouse_button_1_register == 0) mouse_button_1_register = 1;
		else if (mouse_button_1_register == 1) mouse_button_1_register = 2;
	} else {
		mouse_button_1_register = 0;
	}

	// Select layer button ///////////////////////////////////////////////////////
	if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 4, ed_control_x + 55, ed_control_y + 19)) {
		render_it = select_layer_button();
	}
	// Select tile button ////////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 20, ed_control_x + 55, ed_control_y + 35)) {
		render_it = select_tile_button();
	}
	// Grid on/off button ////////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 36, ed_control_x + 55, ed_control_y + 51)) {
		render_it = grid_on_off_button();
	}
	// Tools menu button /////////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 52, ed_control_x + 55, ed_control_y + 67)) {
		render_it = tools_menu_button();
	}
	// Resize level button ///////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 68, ed_control_x + 55, ed_control_y + 83)) {
		render_it = resize_level_button();
	}
	// Add sprite button /////////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 84, ed_control_x + 55, ed_control_y + 99)) {
		render_it = add_sprite_button();
	}
	// New map button ////////////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 100, ed_control_x + 55, ed_control_y + 115)) {
		render_it = new_map_button();
	}
	// Display properties button /////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 116, ed_control_x + 55, ed_control_y + 131)) {
		render_it = display_properties_button();
	}
	// Load map button ///////////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 132, ed_control_x + 55, ed_control_y + 147)) {
		render_it = load_map_button();
	}
	// Save map button ///////////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 148, ed_control_x + 55, ed_control_y + 163)) {
		render_it = save_map_button();
	}
	// Exit program button ///////////////////////////////////////////////////////
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x + 4, ed_control_y + 164, ed_control_x + 55, ed_control_y + 179)) {
		return exit_program_button();
	}
	// Move the controls dialog box
	else if (mouse_button_1_register == 1 && mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(ed_control_x + 4, ed_control_y + 4, ed_control_x + 55, ed_control_y + 179)) {
		int mx_offset = mouse_x - ed_control_x;
		int my_offset = mouse_y - ed_control_y;

		while (mouse_b & 1) {
			ed_control_x = mouse_x - mx_offset;
			ed_control_y = mouse_y - my_offset;

			// Normalize the controls coordinates
			if (ed_control_x < 0) ed_control_x = 0;
			if (ed_control_y < 0) ed_control_y = 0;
			if (ed_control_x > screen_width - 60) ed_control_x = screen_width - 60;
			if (ed_control_y > screen_height - 200) ed_control_y = screen_height - 200;

			ed_render_map();
		}
	}

	/****************************************************************************\
	|** Check the main editor map ***********************************************|
	\****************************************************************************/

	// Mouse button 1 clicked while mouse is over the editor map ////////////////
	if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200)) {
		set_map_layer_value(mouse_x, mouse_y, current_editing_layer, tile_grid_attribute);
		if (multi_select_width > 0 || multi_select_height > 0) {
			for (multi_select_loop_x = 0; multi_select_loop_x < multi_select_width; multi_select_loop_x++)
				for (multi_select_loop_y = 0; multi_select_loop_y < multi_select_height; multi_select_loop_y++)
					set_map_layer_value((multi_select_loop_x * 16) + mouse_x, (multi_select_loop_y * 16) + mouse_y, current_editing_layer, tile_grid_attribute + multi_select_loop_x + (multi_select_loop_y * 16));
		}
		render_it = TRUE;
	}

	// Delete key pressed while mouse is over the editor map ////////////////
	if (key[KEY_DEL] && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200)) {
		for (sprite_delete_loop = 1; sprite_delete_loop < 255; sprite_delete_loop++) {
			if ((mouse_x + screen_x_position) >= editor_map.starting_sprites[sprite_delete_loop].x_position_in_pixels && (mouse_x + screen_x_position) <= editor_map.starting_sprites[sprite_delete_loop].x_position_in_pixels + 16) {
				if ((mouse_y + screen_y_position) >= editor_map.starting_sprites[sprite_delete_loop].y_position_in_pixels && (mouse_y + screen_y_position) <= editor_map.starting_sprites[sprite_delete_loop].y_position_in_pixels + 16) {
					if (editor_map.starting_sprites[sprite_delete_loop].sprite_type == 188) editor_map.weather = 0;
					else if (editor_map.starting_sprites[sprite_delete_loop].sprite_type == 189) editor_map.weather = 0;
					else if (editor_map.starting_sprites[sprite_delete_loop].sprite_type == 190) editor_map.weather = 0;
					else if (editor_map.starting_sprites[sprite_delete_loop].sprite_type == 191) editor_map.weather = 0;
					editor_map.starting_sprites[sprite_delete_loop].sprite_type = 0;
				}
			}
		}
		render_it = TRUE;
	}

	if (check_scroll() == TRUE) ed_render_map();

	else if (render_it == TRUE) ed_render_map();

	return 0;
}

void draw_ed_controls()
{
	// Normalize the controls coordinates
	if (ed_control_x < 0) ed_control_x = 0;
	if (ed_control_y < 0) ed_control_y = 0;
	if (ed_control_x > screen_width - 60) ed_control_x = screen_width - 60;
	if (ed_control_y > screen_height - 200) ed_control_y = screen_height - 200;

	// Redraw the editor tools
	blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, ed_double_buffer, 0, 0, ed_control_x, ed_control_y, 60, 200);

	// Display the grid button pressed down if appropriate
	if (show_grid == 1) blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, ed_double_buffer, 0, 32, ed_control_x + 4, ed_control_y + 36, 52, 16);

	// Display coordinate info
	sprintf(message, "X: %-5d", mouse_x_position);
	textout(ed_double_buffer, font, message, ed_control_x + 4, ed_control_y + 180, 15);
	sprintf(message, "Y: %-5d", mouse_y_position);
	textout(ed_double_buffer, font, message, ed_control_x + 4, ed_control_y + 188, 15);
}

// This function displays a popout menu at point y relative to the editor controls, and returns the final x coordinate
int popout_menu(BITMAP *bmp, int y, int delay)
{
	int i;		// Loop index
	int x, xf;	// Initial and final x coordinates

	delay = 0;

	// Determine initial and final x coordinates and perform scrolling
	if (ed_control_x <= screen_width - 60 - bmp->w) {
		x = ed_control_x + 60;
		xf = x + bmp->w;

		for (i = x; i <= xf; i++) {	// Scroll toward the right
			show_mouse(NULL);
			blit(bmp, screen, bmp->w - i + x, 0, x, y + ed_control_y, i - x, bmp->h);
			show_mouse(screen);
			rest(delay);
		}
		return x;	// Return the final x coordinate
	} else {
		x = ed_control_x;
		xf = x - bmp->w;

		for (i = x; i >= xf; i--) {	// Scroll toward the left
			show_mouse(NULL);
			blit(bmp, screen, 0, 0, i, y + ed_control_y, x - i, bmp->h);
			show_mouse(screen);
			rest(delay);
		}
		return xf;	// Return the final x coordinate
	}
}

// This function reverses the opreation of popout_menu
void popin_menu(BITMAP *bmp, int y, int delay)
{
	int i;		// Loop index
	int x, xf;	// Initial and final x coordinates

	delay = 0;

	// Determine initial and final x coordinates and perform scrolling
	if (ed_control_x <= screen_width - 60 - bmp->w) {
		x = ed_control_x + 60;
		xf = x + bmp->w;

		for (i = xf; i >= x; i--) {	// Scroll in toward the left
			show_mouse(NULL);
			blit(ed_double_buffer, screen, x, y + ed_control_y, x, y + ed_control_y, bmp->w, bmp->h);
			blit(bmp, screen, bmp->w - i + x, 0, x, y + ed_control_y, i - x, bmp->h);
			show_mouse(screen);
			rest(delay);
		}
	} else {
		x = ed_control_x;
		xf = x - bmp->w;

		for (i = xf; i <= x; i++) {	// Scroll in toward the right
			show_mouse(NULL);
			blit(ed_double_buffer, screen, xf, y + ed_control_y, xf, y + ed_control_y, bmp->w, bmp->h);
			blit(bmp, screen, 0, 0, i, y + ed_control_y, x - i, bmp->h);
			show_mouse(screen);
			rest(delay);
		}
	}

	while (mouse_b & 1) {
	}
}

/****************\
|** RENDER MAP **|
\****************/
void ed_render_map()
{
	char x_tile_loop, y_tile_loop;
	unsigned char weather_loop;
	int x_grid_loop, y_grid_loop;
	int sprite_loop;
	int sprite_tile_x, sprite_tile_y;
	int start_x_tile = 0, start_y_tile = 0;
	int sx = screen_x_position, sy = screen_y_position;
	int plax_x, plax_y;

	if (parallax_background->w == screen_width || editor_map.map_width == 16) plax_x = 0;
	else plax_x = (float)screen_x_position * (float)((float)(parallax_background->w - screen_width) / (editor_map.map_width * 16 - screen_width));

	if (parallax_background->h == screen_height || editor_map.map_height == 12) plax_y = 0;
	else plax_y = (float)screen_y_position * (float)((float)(parallax_background->h - screen_height) / (editor_map.map_height * 16 - screen_height));

	// Draw the parallaxing background
	if (parallax_background->w < screen_width || parallax_background->h < screen_height) stretch_blit(parallax_background, ed_double_buffer, 0, 0, parallax_background->w, parallax_background->h, 0, 0, screen_width, screen_height);
	else blit(parallax_background, ed_double_buffer, plax_x, plax_y, 0, 0, screen_width, screen_height);

	// Snow
	if (editor_map.weather == 1) {
		for (weather_loop = 0; weather_loop < 64; weather_loop++) putpixel(ed_double_buffer, weather[weather_loop][0], weather[weather_loop][1], 255);
	}
	// Rain
	else if (editor_map.weather == 2) {
		for (weather_loop = 0; weather_loop < 64; weather_loop++) {
			putpixel(ed_double_buffer, weather[weather_loop][0], weather[weather_loop][1], 20);
			putpixel(ed_double_buffer, weather[weather_loop][0], weather[weather_loop][1] - 1, 22);
			putpixel(ed_double_buffer, weather[weather_loop][0] + 1, weather[weather_loop][1] - 2, 24);
			putpixel(ed_double_buffer, weather[weather_loop][0] + 1, weather[weather_loop][1] - 3, 26);
		}
	}

	while (sx >= 16) {
		start_x_tile++;
		sx -= 16;
	}
	;

	while (sy >= 16) {
		start_y_tile++;
		sy -= 16;
	}
	;

	if (show_background_layer == 1) {
		for (x_tile_loop = 0; x_tile_loop < (screen_width / 16) + 2; x_tile_loop++) {
			for (y_tile_loop = 0; y_tile_loop < (screen_height / 16) + 2; y_tile_loop++) {
				if (start_x_tile + x_tile_loop >= editor_map.map_width || start_y_tile + y_tile_loop >= editor_map.map_height) rectfill(ed_double_buffer, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy, x_tile_loop * tile_width - sx + tile_width, y_tile_loop * tile_height - sy + tile_height, 0);
				else if (editor_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == masked_background_grph_tile || editor_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == masked_everything_grph_tile) masked_blit(worminator_tiles, ed_double_buffer, editor_map.background_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * tile_width, 0, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy, tile_width, tile_height);
				else if (editor_map.background_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] == 0) blit(parallax_background, ed_double_buffer, x_tile_loop * tile_width - sx + plax_x, y_tile_loop * tile_height - sy + plax_y, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy, tile_width, tile_height);
				else blit(worminator_tiles, ed_double_buffer, editor_map.background_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * tile_width, 0, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy, tile_width, tile_height);
			}
		}
	} else {
		rectfill(ed_double_buffer, 0, 0, screen_width, screen_height, 0);
	}

	// Render the Worminator's starting spot
	masked_blit(worminator_data_file[ED_EDITOR_SPRITE_CHART].dat, ed_double_buffer, 0, 0, editor_map.starting_sprites[0].x_position_in_pixels - screen_x_position, editor_map.starting_sprites[0].y_position_in_pixels - screen_y_position, tile_width, tile_height);

	for (sprite_loop = 1; sprite_loop < 256; sprite_loop++) {
		if (editor_map.starting_sprites[sprite_loop].sprite_type != 0) {
			sprite_tile_x = editor_map.starting_sprites[sprite_loop].sprite_type * 16;
			sprite_tile_y = 0;
			while (sprite_tile_x >= 256) {
				sprite_tile_x -= 256;
				sprite_tile_y += 16;
			}
			;
			masked_blit(worminator_data_file[ED_EDITOR_SPRITE_CHART].dat, ed_double_buffer, sprite_tile_x, sprite_tile_y, editor_map.starting_sprites[sprite_loop].x_position_in_pixels - screen_x_position, editor_map.starting_sprites[sprite_loop].y_position_in_pixels - screen_y_position, 16, 16);
		}
	}

	if (show_foreground_layer == 1) {
		for (x_tile_loop = 0; x_tile_loop < (screen_width / 16) + 2; x_tile_loop++)
			for (y_tile_loop = 0; y_tile_loop < (screen_height / 16) + 2; y_tile_loop++)
				if (editor_map.foreground_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != 0) masked_blit(worminator_tiles, ed_double_buffer, editor_map.foreground_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * tile_width, 0, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy, tile_width, tile_height);
	}

	if (show_info_layer == 1) {
		for (x_tile_loop = 0; x_tile_loop < (screen_width / 16) + 2; x_tile_loop++) {
			for (y_tile_loop = 0; y_tile_loop < (screen_height / 16) + 2; y_tile_loop++) {
				if (editor_map.info_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != 0) {
					if (show_background_layer == 1) {
						blit(worminator_info_tiles, spawned_sprite, editor_map.info_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * tile_width, 0, 0, 0, tile_width, tile_height);
						draw_trans_sprite(ed_double_buffer, spawned_sprite, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy);
					} else {
						blit(worminator_info_tiles, ed_double_buffer, editor_map.info_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * tile_width, 0, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy, tile_width, tile_height);
					}
				}
			}
		}
	}

	if (show_graphical_properties_layer == 1) {
		for (x_tile_loop = 0; x_tile_loop < (screen_width / 16) + 2; x_tile_loop++) {
			for (y_tile_loop = 0; y_tile_loop < (screen_height / 16) + 2; y_tile_loop++) {
				if (editor_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] != 0) {
					if (show_background_layer == 1) {
						blit(worminator_graphical_properties_tiles, spawned_sprite, editor_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * tile_width, 0, 0, 0, tile_width, tile_height);
						draw_trans_sprite(ed_double_buffer, spawned_sprite, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy);
					} else {
						blit(worminator_graphical_properties_tiles, ed_double_buffer, editor_map.graphical_properties_tile_grid[start_x_tile + x_tile_loop][start_y_tile + y_tile_loop] * tile_width, 0, x_tile_loop * tile_width - sx, y_tile_loop * tile_height - sy, tile_width, tile_height);
					}
				}
			}
		}
	}

	if (show_grid == 1) {
		for (x_grid_loop = 0; x_grid_loop <= screen_width; x_grid_loop += 16) {
			vline(ed_double_buffer, x_grid_loop - sx, 0, screen_height, 255);
			vline(ed_double_buffer, x_grid_loop - sx - 1, 0, screen_height, 255);
		}
		for (y_grid_loop = 0; y_grid_loop <= screen_height; y_grid_loop += 16) {
			hline(ed_double_buffer, 0, y_grid_loop - sy, screen_width, 255);
			hline(ed_double_buffer, 0, y_grid_loop - sy - 1, screen_width, 255);
		}
	}

	// This displays the current action message
	textout(ed_double_buffer, worminator_data_file[DEFAULT_WORMINATOR_FONT].dat, current_action, 0, 0, -1);

	// This is what draws the multi select box itself
	if (!mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && (multi_select_width > 0 || multi_select_height > 0)) {
		if (show_grid == 0) rect(ed_double_buffer, (mouse_x_position * 16) - screen_x_position, (mouse_y_position * 16) - screen_y_position, (mouse_x_position * 16) - screen_x_position + (multi_select_width * 16) - 1, (mouse_y_position * 16) - screen_y_position + (multi_select_height * 16) - 1, 255);
		else rect(ed_double_buffer, (mouse_x_position * 16) - screen_x_position, (mouse_y_position * 16) - screen_y_position, (mouse_x_position * 16) - screen_x_position + (multi_select_width * 16) - 1, (mouse_y_position * 16) - screen_y_position + (multi_select_height * 16) - 1, 24);
	}

	// Show the editor controls
	draw_ed_controls();

	// paste on the double buffer
	show_mouse(NULL);
	blit(ed_double_buffer, screen, 0, 0, 0, 0, screen_width, screen_height);
	show_mouse(screen);
}

/*************************\
|** SET MAP LAYER VALUE **|
\*************************/
void set_map_layer_value(int x_in_pixels, int y_in_pixels, char layer, int attribute)
{
	int x_location = 0, y_location = 0;

	x_in_pixels += screen_x_position;
	y_in_pixels += screen_y_position;

	while (x_in_pixels >= tile_width) {
		x_location++;
		x_in_pixels -= tile_width;
	}
	;

	while (y_in_pixels >= tile_width) {
		y_location++;
		y_in_pixels -= tile_width;
	}
	;

	if (layer == 0) editor_map.background_tile_grid[x_location][y_location] = attribute;
	else if (layer == 1) editor_map.foreground_tile_grid[x_location][y_location] = attribute;
	else if (layer == 2) editor_map.info_tile_grid[x_location][y_location] = attribute;
	else if (layer == 3) editor_map.graphical_properties_tile_grid[x_location][y_location] = attribute;
}

/**************\
|** LOAD MAP **|
\**************/
void ed_load_map(char *file_name)
{
	int sprite_spawn_loop;
	FILE *map_file = fopen(file_name, "rb");

	if (!map_file) {
		alert("Warning: could not open map-file:", file_name, "Ignoring load map request!", "OK", 0, 0, 0);
		return;
	}
	read_worminator_map(&editor_map, map_file);
	fclose(map_file);

	// Select the correct parallaxing background
	for (sprite_spawn_loop = 1; sprite_spawn_loop < 255; sprite_spawn_loop++)
		//////// Parallaxing Backgrounds //////////////////////////////////////////////////////////////
		if (editor_map.starting_sprites[sprite_spawn_loop].sprite_type >= 160 && editor_map.starting_sprites[sprite_spawn_loop].sprite_type < 170) parallax_background = worminator_data_file[PARALLAXING_BACKGROUND_01 + editor_map.starting_sprites[sprite_spawn_loop].sprite_type - 160].dat;

	check_scroll();

	// Re-load the tilesets in case the loaded map uses different tiles than were previously selected.
	load_tiles();

	ed_render_map();
}

/**************\
|** SAVE MAP **|
\**************/
void ed_save_map(char *file_name)
{
	FILE *map_file = fopen(file_name, "wb");

	if (!map_file) {
		alert("Warning: could not open map-file:", file_name, "for saving.  Ignoring save request!", "OK", 0, 0, 0);
		return;
	}
	write_worminator_map(&editor_map, map_file);
	fclose(map_file);
}

/*************************\
|** SELECT LAYER BUTTON **|
\*************************/
char select_layer_button()
{
	char done_looping = FALSE;
	BITMAP *ed_temp;
	int x, y;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 0, ed_control_x + 4, ed_control_y + 4, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 4, ed_control_x + 55, ed_control_y + 19) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 4, ed_control_x + 4, ed_control_y + 4, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 0, ed_control_x + 4, ed_control_y + 4, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 4, ed_control_x + 4, ed_control_y + 4, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Scroll out the popup select layer window
	ed_temp = create_bitmap_ex(8, 52, 64);
	if (current_editing_layer == 0) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 0, 0, 0, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 0, 0, 0, 52, 16);
	if (current_editing_layer == 1) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 16, 0, 16, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 16, 0, 16, 52, 16);
	if (current_editing_layer == 2) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 32, 0, 32, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 32, 0, 32, 52, 16);
	if (current_editing_layer == 3) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 48, 0, 48, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 48, 0, 48, 52, 16);
	x = popout_menu(ed_temp, 4, 5);
	y = ed_control_y + 4;
	destroy_bitmap(ed_temp);

	// Loop until one of the buttons is pressed
	do {
		// Update mouse button status
		if (mouse_b & 1) {
			if (mouse_button_1_register == 0) mouse_button_1_register = 1;
			else if (mouse_button_1_register == 1) mouse_button_1_register = 2;
		} else {
			mouse_button_1_register = 0;
		}

		// Select background popup menu button
		if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y, x + 52, y + 16)) {
			current_editing_layer = 0;
			done_looping = TRUE;
		}
		// Select foreground popup menu button
		else if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y + 16, x + 52, y + 32)) {
			current_editing_layer = 1;
			done_looping = TRUE;
		}
		// Select info popup menu button
		else if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y + 32, x + 52, y + 48)) {
			current_editing_layer = 2;
			top_tile_offset = 0;
			if (tile_grid_attribute >= 192) tile_grid_attribute = 0;
			done_looping = TRUE;
		}
		// Select graphical properties popup menu button
		else if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y + 48, x + 52, y + 64)) {
			current_editing_layer = 3;
			top_tile_offset = 0;
			if (tile_grid_attribute >= 192) tile_grid_attribute = 0;
			done_looping = TRUE;
		}
	} while (done_looping != TRUE);

	// The current action has to be editing if this button was pressed
	if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
	if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
	if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
	if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";

	// Refresh the map to show the new current action
	ed_render_map();

	// Scroll in the popup select layer window
	ed_temp = create_bitmap_ex(8, 52, 64);
	if (current_editing_layer == 0) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 0, 0, 0, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 0, 0, 0, 52, 16);
	if (current_editing_layer == 1) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 16, 0, 16, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 16, 0, 16, 52, 16);
	if (current_editing_layer == 2) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 32, 0, 32, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 32, 0, 32, 52, 16);
	if (current_editing_layer == 3) blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW_DOWN].dat, ed_temp, 0, 48, 0, 48, 52, 16);
	else blit(worminator_data_file[ED_POPUP_SELECT_LAYER_WINDOW].dat, ed_temp, 0, 48, 0, 48, 52, 16);
	popin_menu(ed_temp, 4, 5);
	destroy_bitmap(ed_temp);

	// Tell the editor to re-draw the scrolling display
	return TRUE;
}

/************************\
|** SELECT TILE BUTTON **|
\************************/
char select_tile_button()
{
	char done_looping = FALSE;
	BITMAP *tile_pointer = worminator_data_file[TILES].dat;
	int selected_x_tile_position = tile_grid_attribute * 16, selected_y_tile_position = 0;
	int x = screen_width / 2 - 128;
	int in_region = TRUE;	// Used to process buttion status

	while (selected_x_tile_position >= 256) {
		selected_y_tile_position += 16;
		selected_x_tile_position -= 256;
	}

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 16, ed_control_x + 4, ed_control_y + 20, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 20, ed_control_x + 55, ed_control_y + 35) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 20, ed_control_x + 4, ed_control_y + 20, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 16, ed_control_x + 4, ed_control_y + 20, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 20, ed_control_x + 4, ed_control_y + 20, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Display the tiles and the done button
	refresh_tile_display();

	// Wait for the user to click on a tile
	do {
		if (mouse_clicked_region_notran(x, 0, x + 256 - 1, screen_height)) {
			if (((mouse_x - x) / 16) + (((mouse_y + top_tile_offset) / 16) * 16) != tile_grid_attribute) {
				tile_grid_attribute = ((mouse_x - x) / 16) + (((mouse_y + top_tile_offset) / 16) * 16);
				selected_x_tile_position = tile_grid_attribute * 16, selected_y_tile_position = 0;
				while (selected_x_tile_position >= 256) {
					selected_y_tile_position += 16;
					selected_x_tile_position -= 256;
				}
				multi_select_width = 0, multi_select_height = 0;
				refresh_tile_display();
			}
		} else if (mouse_b & 2 && mouse_in_region_notran(x, 0, x + 256 - 1, screen_height)) {
			if (mouse_x - x > selected_x_tile_position && mouse_y + top_tile_offset > selected_y_tile_position) {
				if ((mouse_x - x - selected_x_tile_position) / 16 + 1 != multi_select_width || (mouse_y - selected_y_tile_position + top_tile_offset) / 16 + 1 != multi_select_height) {
					multi_select_width = (mouse_x - x - selected_x_tile_position) / 16 + 1;
					multi_select_height = (mouse_y - selected_y_tile_position + top_tile_offset) / 16 + 1;
					refresh_tile_display();
				}
			}
		}

		if (key[KEY_DOWN] || mouse_y == screen_height - 1) {
			top_tile_offset += 16;
			if (top_tile_offset > tile_pointer->h - 192) top_tile_offset = tile_pointer->h - 192;
			refresh_tile_display();
			rest(10);
		}

		if (key[KEY_UP] || mouse_y == 0) {
			top_tile_offset -= 16;
			if (top_tile_offset < 0) top_tile_offset = 0;
			refresh_tile_display();
			rest(10);
		}

		if (key[KEY_PGDN]) {
			top_tile_offset += 16 * 12;
			if (top_tile_offset > tile_pointer->h - 192) top_tile_offset = tile_pointer->h - 192;
			refresh_tile_display();
			rest(25);
		}

		if (key[KEY_PGUP]) {
			top_tile_offset -= 16 * 12;
			if (top_tile_offset < 0) top_tile_offset = 0;
			refresh_tile_display();
			rest(25);
		}

		if (mouse_clicked_region_notran(screen_width - 16, 0, screen_width, 32)) {
			top_tile_offset -= 16;
			if (top_tile_offset < 0) top_tile_offset = 0;
			refresh_tile_display();
			// Show pressed arrow
			show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS_DOWN].dat, screen, 0, 0, screen_width - 16, 0, 16, 32); show_mouse(screen);
			rest(10);
			show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS].dat, screen, 0, 0, screen_width - 16, 0, 16, 32); show_mouse(screen);
		}

		if (mouse_clicked_region_notran(screen_width - 16, screen_height - 32, screen_width, screen_height)) {
			top_tile_offset += 16;
			if (top_tile_offset > tile_pointer->h - 192) top_tile_offset = tile_pointer->h - 192;
			refresh_tile_display();
			// Show pressed arrow
			show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS_DOWN].dat, screen, 0, 32, screen_width - 16, screen_height - 32, 16, 32); show_mouse(screen);
			rest(10);
			show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS].dat, screen, 0, 32, screen_width - 16, screen_height - 32, 16, 32); show_mouse(screen);
		}

		if (mouse_clicked_region_notran(screen_width - 16, screen_height / 2 - 32, screen_width, screen_height / 2 + 32)) {
			show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS_DOWN].dat, screen, 16, 0, screen_width - 16, screen_height / 2 - 32, 16, 64); show_mouse(screen);
			while (mouse_b & 1) {
				if (mouse_in_region_notran(screen_width - 16, screen_height / 2 - 32, screen_width, screen_height / 2 + 32) != in_region) {
					if (in_region == TRUE) {
						show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS].dat, screen, 16, 0, screen_width - 16, screen_height / 2 - 32, 16, 64); show_mouse(screen);
						in_region = FALSE;
					} else {
						show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS_DOWN].dat, screen, 16, 0, screen_width - 16, screen_height / 2 - 32, 16, 64); show_mouse(screen);
						in_region = TRUE;
					}
				}
			}
			show_mouse(NULL); blit(worminator_data_file[ED_TILE_BUTTONS].dat, screen, 16, 0, screen_width - 16, screen_height / 2 - 32, 16, 64); show_mouse(screen);

			// Make sure the mouse is still over the button
			if (in_region) done_looping = TRUE;
		}
	} while (!done_looping);

	// If these are both 1, then it has the same effect as both of them being 0
	if (multi_select_width < 2 && multi_select_height < 2) multi_select_width = 0, multi_select_height = 0;

	// Tell the editor to re-draw the scrolling display
	return TRUE;
}

/************************\
|** GRID ON/OFF BUTTON **|
\************************/
char grid_on_off_button()
{
	// Swap the grid on/off flag
	if (show_grid == 0) {
		show_grid = 1;
		ed_render_map();
	} else {
		show_grid = 0;
		ed_render_map();
	}

	// Tell the editor to re-draw the scrolling display
	return TRUE;
}

/***********************\
|** TOOLS MENU BUTTON **|
\***********************/
char tools_menu_button()
{
	char done_looping = FALSE;
	char mouse_button_1_register = 0;
	int x, y;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 48, ed_control_x + 4, ed_control_y + 52, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 52, ed_control_x + 55, ed_control_y + 67) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 52, ed_control_x + 4, ed_control_y + 52, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 48, ed_control_x + 4, ed_control_y + 52, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 52, ed_control_x + 4, ed_control_y + 52, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Scroll out the popup select layer window
	x = popout_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);
	y = ed_control_y + 52;

	do {
		// Update mouse button status
		if (mouse_b & 1) {
			if (mouse_button_1_register == 0) mouse_button_1_register = 1;
			else if (mouse_button_1_register == 1) mouse_button_1_register = 2;
		} else {
			mouse_button_1_register = 0;
		}

		// Fill area button /////////////////////////////////////////////////////////
		if (mouse_button_1_register == 1 && mouse_clicked_region_notran(x, y, x + 52, y + 16)) {
			done_looping = fill_area_button(x, y);
		}
		// Patterned fill button ////////////////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_clicked_region_notran(x, y + 16, x + 52, y + 32)) {
			done_looping = patterned_fill_button(x, y);
		}
		// Copy / paste button //////////////////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_clicked_region_notran(x, y + 32, x + 52, y + 48)) {
			done_looping = copy_paste_button(x, y);
		}
		// Background to foreground button //////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_clicked_region_notran(x + 52, y, x + 104, y + 16)) {
			done_looping = background_to_foreground_button(x, y);
		}
		// Foreground to background button //////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_clicked_region_notran(x + 52, y + 16, x + 104, y + 32)) {
			done_looping = foreground_to_background_button(x, y);
		}
		// Cancel button ////////////////////////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_clicked_region_notran(x + 52, y + 32, x + 104, y + 48)) {
			show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 52, 32, x + 52, y + 32, 52, 16); show_mouse(screen);
			while (mouse_b & 1) {
			}
			show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 52, 32, x + 52, y + 32, 52, 16); show_mouse(screen);
			if (mouse_in_region_notran(x + 52, y + 32, x + 104, y + 48)) {
				done_looping = TRUE;
				popin_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);
			}
			mouse_button_1_register = 0;
		}
	} while (done_looping == FALSE);
	return TRUE;
}

/**********************\
|** FILL AREA BUTTON **|
\**********************/
char fill_area_button(int x, int y)
{
	char done_looping = FALSE;
	char keep_looping;
	int tile_to_fill_over = 0;
	int x_fill_loop, y_fill_loop;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 0, 0, x, y, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(x, y, x + 52, y + 16) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 0, 0, x, y, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 0, 0, x, y, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 0, 0, x, y, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Make sure that there is only one tile selected
	if (multi_select_width > 0 || multi_select_height > 0) {
		alert("You can not perform a patterned", "fill with this button.  Select", "patterned fill from the tools menu.", "OK", 0, 0, 0);
		return FALSE;
	}

	// Say what we are doing
	if (current_editing_layer == 0) current_action = "Background Layer Fill.";
	if (current_editing_layer == 1) current_action = "Foreground Layer Fill.";
	if (current_editing_layer == 2) current_action = "Info Layer Fill.";
	if (current_editing_layer == 3) current_action = "Graphical Properties Layer Fill.";

	// Render the map so the new current action shows up
	ed_render_map();

	// Put away the tools menu bar
	popin_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);

	// Wait for the user to click on the scrolling display so we know where to fill
	do {
		// Check to see if the user scrolled the display
		if (check_scroll() == TRUE) ed_render_map();

		if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200)) {
			done_looping = TRUE;
			if (current_editing_layer == 0) {
				tile_to_fill_over = editor_map.background_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16];
				if (tile_to_fill_over != tile_grid_attribute) editor_map.background_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = -1;
			} else if (current_editing_layer == 1) {
				tile_to_fill_over = editor_map.foreground_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16];
				if (tile_to_fill_over != tile_grid_attribute) editor_map.foreground_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = -1;
			} else if (current_editing_layer == 2) {
				tile_to_fill_over = editor_map.info_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16];
				if (tile_to_fill_over != tile_grid_attribute) editor_map.info_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = -1;
			} else if (current_editing_layer == 3) {
				tile_to_fill_over = editor_map.graphical_properties_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16];
				if (tile_to_fill_over != tile_grid_attribute) editor_map.graphical_properties_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = -1;
			}
		}
	} while (done_looping != TRUE);

	if (tile_to_fill_over == tile_grid_attribute) {
		alert("You have tried to fill a", "tile with itself.  This is not", "allowed by the command of Dave.", "OK", 0, 0, 0);

		// The current action has to be editing now that the fill was cancled
		if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
		if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
		if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
		if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";

		// Show the new current action
		ed_render_map();

		// Slide out the tools menu bar
		popout_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);
		return FALSE;
	}

	// Loop through the map and fill until we are done filling
	do {
		keep_looping = 0;
		for (x_fill_loop = 0; x_fill_loop < editor_map.map_width; x_fill_loop++) {
			for (y_fill_loop = 0; y_fill_loop < editor_map.map_height; y_fill_loop++) {
				if (current_editing_layer == 0) {
					if (editor_map.background_tile_grid[x_fill_loop][y_fill_loop] == -1) {
						editor_map.background_tile_grid[x_fill_loop][y_fill_loop] = tile_grid_attribute;
						if (editor_map.background_tile_grid[x_fill_loop - 1][y_fill_loop] == tile_to_fill_over && x_fill_loop > 0) editor_map.background_tile_grid[x_fill_loop - 1][y_fill_loop] = -1;
						if (editor_map.background_tile_grid[x_fill_loop + 1][y_fill_loop] == tile_to_fill_over && x_fill_loop < editor_map.map_width - 1) editor_map.background_tile_grid[x_fill_loop + 1][y_fill_loop] = -1;
						if (editor_map.background_tile_grid[x_fill_loop][y_fill_loop - 1] == tile_to_fill_over && y_fill_loop > 0) editor_map.background_tile_grid[x_fill_loop][y_fill_loop - 1] = -1;
						if (editor_map.background_tile_grid[x_fill_loop][y_fill_loop + 1] == tile_to_fill_over && y_fill_loop < editor_map.map_height - 1) editor_map.background_tile_grid[x_fill_loop][y_fill_loop + 1] = -1;
						keep_looping = 1;
					}
				} else if (current_editing_layer == 1) {
					if (editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop] == -1) {
						editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop] = tile_grid_attribute;
						if (editor_map.foreground_tile_grid[x_fill_loop - 1][y_fill_loop] == tile_to_fill_over && x_fill_loop > 0) editor_map.foreground_tile_grid[x_fill_loop - 1][y_fill_loop] = -1;
						if (editor_map.foreground_tile_grid[x_fill_loop + 1][y_fill_loop] == tile_to_fill_over && x_fill_loop < editor_map.map_width - 1) editor_map.foreground_tile_grid[x_fill_loop + 1][y_fill_loop] = -1;
						if (editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop - 1] == tile_to_fill_over && y_fill_loop > 0) editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop - 1] = -1;
						if (editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop + 1] == tile_to_fill_over && y_fill_loop < editor_map.map_height - 1) editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop + 1] = -1;
						keep_looping = 1;
					}
				} else if (current_editing_layer == 2) {
					if (editor_map.info_tile_grid[x_fill_loop][y_fill_loop] == -1) {
						editor_map.info_tile_grid[x_fill_loop][y_fill_loop] = tile_grid_attribute;
						if (editor_map.info_tile_grid[x_fill_loop - 1][y_fill_loop] == tile_to_fill_over && x_fill_loop > 0) editor_map.info_tile_grid[x_fill_loop - 1][y_fill_loop] = -1;
						if (editor_map.info_tile_grid[x_fill_loop + 1][y_fill_loop] == tile_to_fill_over && x_fill_loop < editor_map.map_width - 1) editor_map.info_tile_grid[x_fill_loop + 1][y_fill_loop] = -1;
						if (editor_map.info_tile_grid[x_fill_loop][y_fill_loop - 1] == tile_to_fill_over && y_fill_loop > 0) editor_map.info_tile_grid[x_fill_loop][y_fill_loop - 1] = -1;
						if (editor_map.info_tile_grid[x_fill_loop][y_fill_loop + 1] == tile_to_fill_over && y_fill_loop < editor_map.map_height - 1) editor_map.info_tile_grid[x_fill_loop][y_fill_loop + 1] = -1;
						keep_looping = 1;
					}
				} else if (current_editing_layer == 3) {
					if (editor_map.graphical_properties_tile_grid[x_fill_loop][y_fill_loop] == -1) {
						editor_map.graphical_properties_tile_grid[x_fill_loop][y_fill_loop] = tile_grid_attribute;
						if (editor_map.graphical_properties_tile_grid[x_fill_loop - 1][y_fill_loop] == tile_to_fill_over && x_fill_loop > 0) editor_map.graphical_properties_tile_grid[x_fill_loop - 1][y_fill_loop] = -1;
						if (editor_map.graphical_properties_tile_grid[x_fill_loop + 1][y_fill_loop] == tile_to_fill_over && x_fill_loop < editor_map.map_width - 1) editor_map.graphical_properties_tile_grid[x_fill_loop + 1][y_fill_loop] = -1;
						if (editor_map.graphical_properties_tile_grid[x_fill_loop][y_fill_loop - 1] == tile_to_fill_over && y_fill_loop > 0) editor_map.graphical_properties_tile_grid[x_fill_loop][y_fill_loop - 1] = -1;
						if (editor_map.graphical_properties_tile_grid[x_fill_loop][y_fill_loop + 1] == tile_to_fill_over && y_fill_loop < editor_map.map_height - 1) editor_map.graphical_properties_tile_grid[x_fill_loop][y_fill_loop + 1] = -1;
						keep_looping = 1;
					}
				}
			}
		}
	} while (keep_looping == 1);

	while (mouse_b & 1) {
	}

	// The current action has to be editing now that the fill is done
	if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
	if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
	if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
	if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";

	// Tell the tools menu to close itself
	return TRUE;
}

/***************************\
|** PATTERNED FILL BUTTON **|
\***************************/
char patterned_fill_button(int x, int y)
{
	char done_looping = FALSE;
	char keep_looping;
	int tile_to_fill_over = 0;
	int x_fill_loop, y_fill_loop;
	int x_anchor = 0, y_anchor = 0, anchored_tile;
	int x_tile, y_tile;
	int old_mouse_x_position = -1, old_mouse_y_position = -1;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 0, 16, x, y + 16, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(x, y + 16, x + 52, y + 32) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 0, 16, x, y + 16, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 0, 16, x, y + 16, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 0, 16, x, y + 16, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Make sure that the info layer is not selected
	if (current_editing_layer == 2) {
		alert("You can not perform", "a patterned fill in", "the info layer.", "OK", 0, 0, 0);
		return FALSE;
	}

	// Make sure that the graphical properties layer is not selected
	if (current_editing_layer == 3) {
		alert("You can not perform", "a patterned fill in", "the graphical properties layer.", "OK", 0, 0, 0);
		return FALSE;
	}

	// Make sure that there is more than one tile selected
	if (!(multi_select_width > 1 || multi_select_height > 1)) {
		alert("You can not perform a non patterned", "fill with this button.  Select", "fill area from the tools menu.", "OK", 0, 0, 0);
		return FALSE;
	}

	// Say what we are doing
	if (current_editing_layer == 0) current_action = "Patterned Background Fill.";
	if (current_editing_layer == 1) current_action = "Patterned Foreground Fill.";

	// Render the map so the new current action shows up
	ed_render_map();

	// Put away the tools menu bar
	popin_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);

	// Referesh the display to show the multi select box.
	ed_render_map();

	// Wait for the user to click on the scrolling display so we know where to fill
	do {
		// Check to see if the user scrolled the display
		if (check_scroll() == TRUE) ed_render_map();

		// This updates the multi select box like the code in main() would
		if (!mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200)) {
			mouse_x_position = (mouse_x + screen_x_position) / 16;
			mouse_y_position = (mouse_y + screen_y_position) / 16;
			if (mouse_x_position != old_mouse_x_position || mouse_y_position != old_mouse_y_position) {
				ed_render_map();
				old_mouse_x_position = mouse_x_position;
				old_mouse_y_position = mouse_y_position;
			}
		}

		if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200)) {
			done_looping = TRUE;
			if (current_editing_layer == 0) {
				tile_to_fill_over = editor_map.background_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16];
				editor_map.background_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = -1;
				x_anchor = (mouse_x + screen_x_position) / 16;
				y_anchor = (mouse_y + screen_y_position) / 16;
			} else if (current_editing_layer == 1) {
				tile_to_fill_over = editor_map.foreground_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16];
				editor_map.foreground_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = -1;
				x_anchor = (mouse_x + screen_x_position) / 16;
				y_anchor = (mouse_y + screen_y_position) / 16;
			}
		}
	} while (done_looping != TRUE);

	// Loop through the map and fill until we are done filling
	do {
		keep_looping = 0;
		for (x_fill_loop = 0; x_fill_loop < editor_map.map_width; x_fill_loop++) {
			for (y_fill_loop = 0; y_fill_loop < editor_map.map_height; y_fill_loop++) {
				if (current_editing_layer == 0) {
					if (editor_map.background_tile_grid[x_fill_loop][y_fill_loop] == -1) {
						editor_map.background_tile_grid[x_fill_loop][y_fill_loop] = -666;
						if (editor_map.background_tile_grid[x_fill_loop - 1][y_fill_loop] == tile_to_fill_over && x_fill_loop > 0) editor_map.background_tile_grid[x_fill_loop - 1][y_fill_loop] = -1;
						if (editor_map.background_tile_grid[x_fill_loop + 1][y_fill_loop] == tile_to_fill_over && x_fill_loop < editor_map.map_width - 1) editor_map.background_tile_grid[x_fill_loop + 1][y_fill_loop] = -1;
						if (editor_map.background_tile_grid[x_fill_loop][y_fill_loop - 1] == tile_to_fill_over && y_fill_loop > 0) editor_map.background_tile_grid[x_fill_loop][y_fill_loop - 1] = -1;
						if (editor_map.background_tile_grid[x_fill_loop][y_fill_loop + 1] == tile_to_fill_over && y_fill_loop < editor_map.map_height - 1) editor_map.background_tile_grid[x_fill_loop][y_fill_loop + 1] = -1;
						keep_looping = 1;
					}
				} else if (current_editing_layer == 1) {
					if (editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop] == -1) {
						editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop] = -666;
						if (editor_map.foreground_tile_grid[x_fill_loop - 1][y_fill_loop] == tile_to_fill_over && x_fill_loop > 0) editor_map.foreground_tile_grid[x_fill_loop - 1][y_fill_loop] = -1;
						if (editor_map.foreground_tile_grid[x_fill_loop + 1][y_fill_loop] == tile_to_fill_over && x_fill_loop < editor_map.map_width - 1) editor_map.foreground_tile_grid[x_fill_loop + 1][y_fill_loop] = -1;
						if (editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop - 1] == tile_to_fill_over && y_fill_loop > 0) editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop - 1] = -1;
						if (editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop + 1] == tile_to_fill_over && y_fill_loop < editor_map.map_height - 1) editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop + 1] = -1;
						keep_looping = 1;
					}
				}
			}
		}
	} while (keep_looping == 1);

	x_anchor -= multi_select_width * 212;
	y_anchor -= multi_select_height * 212;

	for (x_fill_loop = 0; x_fill_loop < editor_map.map_width; x_fill_loop++) {
		x_tile = abs(x_anchor - x_fill_loop);
		while (x_tile >= multi_select_width)
			x_tile -= multi_select_width;
		;
		for (y_fill_loop = 0; y_fill_loop < editor_map.map_height; y_fill_loop++) {
			y_tile = abs(y_anchor - y_fill_loop);

			while (y_tile >= multi_select_height)
				y_tile -= multi_select_height;
			;

			anchored_tile = (x_tile + (y_tile * 16) + tile_grid_attribute);

			if (current_editing_layer == 0)
				if (editor_map.background_tile_grid[x_fill_loop][y_fill_loop] == -666) editor_map.background_tile_grid[x_fill_loop][y_fill_loop] = anchored_tile;
			if (current_editing_layer == 1)
				if (editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop] == -666) editor_map.foreground_tile_grid[x_fill_loop][y_fill_loop] = anchored_tile;
		}
	}

	while (mouse_b & 1) {
	}

	// The current action has to be editing now that the fill is done
	if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
	if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";

	// Tell the tools menu to close itself
	return TRUE;
}

/*************************\
|** COPY / PASTE BUTTON **|
\*************************/
char copy_paste_button(int x, int y)
{
	int x_copy_loop, y_copy_loop;
	int x_anchor = 0, y_anchor = 0, x_width = 1, y_height = 1;
	int old_mouse_x_position = -1, old_mouse_y_position = -1;
	int multi_height_swap, multi_width_swap;
	int copyed_region[32][32];
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 0, 32, x, y + 32, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(x, y + 32, x + 52, y + 48) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 0, 32, x, y + 32, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 0, 32, x, y + 32, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 0, 32, x, y + 32, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Make sure that the info layer is not selected
	if (current_editing_layer == 2) {
		alert("You can not perform", "a copy / paste in", "the info layer.", "OK", 0, 0, 0);
		return FALSE;
	}

	// Make sure that the graphical properties layer is not selected
	if (current_editing_layer == 3) {
		alert("You can not perform", "a copy / paste in", "the graphical properties layer.", "OK", 0, 0, 0);
		return FALSE;
	}

	// Set the bounding box width and height to 1, so you can see what you are doing
	multi_width_swap = multi_select_width;
	multi_select_width = 0;
	multi_height_swap = multi_select_height;
	multi_select_height = 0;

	// Say what we are doing
	if (current_editing_layer == 0) current_action = "Copying Background.";
	else current_action = "Copying Foreground.";

	// Render the map so the new current action shows up
	ed_render_map();

	// Put away the tools menu bar and show the done button
	popin_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);
	done_button(0, ed_control_y + 52);

	// Wait for the user to click on the scrolling display so we know what to copy
	do {
		// Determine x coordinate of the done button
		if (ed_control_x <= screen_width - 60 - 52) x = ed_control_x + 60;
		else x = ed_control_x - 52;
		// Check to see if the user scrolled the display
		if (check_scroll()) {
			ed_render_map(); draw_tile_box(x_anchor, y_anchor, x_width, y_height); done_button(0, ed_control_y + 52);
		}

		// Set the upper left corner of the bounding box
		if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			x_width = 1, y_height = 1;
			x_anchor = (mouse_x + screen_x_position) / 16;
			y_anchor = (mouse_y + screen_y_position) / 16;
			ed_render_map(); draw_tile_box(x_anchor, y_anchor, x_width, y_height); done_button(0, ed_control_y + 52);
		}

		// Set the lower right corner of the bounding box
		if (mouse_b & 2 && mouse_in_region_notran(0, 0, screen_width - 2, screen_height - 2) && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			x_width = 1, y_height = 1;
			if ((mouse_x + screen_x_position) / 16 > x_anchor) x_width = (mouse_x + screen_x_position) / 16 - x_anchor + 1;
			if ((mouse_y + screen_y_position) / 16 > y_anchor) y_height = (mouse_y + screen_y_position) / 16 - y_anchor + 1;
			if (x_width > 32) x_width = 32;
			if (y_height > 32) y_height = 32;
			ed_render_map(); draw_tile_box(x_anchor, y_anchor, x_width, y_height); done_button(0, ed_control_y + 52);
		}
	} while (!done_button(1, ed_control_y + 52));

	// Now, we copy the region itself into our temporary cache
	for (x_copy_loop = x_anchor; x_copy_loop < x_anchor + x_width; x_copy_loop++) {
		for (y_copy_loop = y_anchor; y_copy_loop < y_anchor + y_height; y_copy_loop++) {
			if (current_editing_layer == 0) copyed_region[x_copy_loop - x_anchor][y_copy_loop - y_anchor] = editor_map.background_tile_grid[x_copy_loop][y_copy_loop];
			else copyed_region[x_copy_loop - x_anchor][y_copy_loop - y_anchor] = editor_map.foreground_tile_grid[x_copy_loop][y_copy_loop];
		}
	}

	// Set the bounding box to the correct size
	multi_select_width = x_width;
	multi_select_height = y_height;

	// Say what we are doing
	if (current_editing_layer == 0) current_action = "Pasteing Background.";
	else current_action = "Pasteing Foreground.";

	// Wait for both mouse buttons and the space bar to be released before we start the next part
	while (mouse_b & 1 || mouse_b & 2 || key[KEY_SPACE]) {
	}

	// Render the map so the new current action shows up
	ed_render_map();

	// Wait for the user to click on the scrolling display so we know where to paste
	do {
		// Determine x coordinate of the done button
		if (ed_control_x <= screen_width - 60 - 52) x = ed_control_x + 60;
		else x = ed_control_x - 52;
		// Check to see if the user scrolled the display
		if (check_scroll() == TRUE) {
			ed_render_map(); done_button(0, ed_control_y + 52);
		}

		// This updates the multi select box like the code in main() would
		if (!mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			mouse_x_position = (mouse_x + screen_x_position) / 16;
			mouse_y_position = (mouse_y + screen_y_position) / 16;
			if (mouse_x_position != old_mouse_x_position || mouse_y_position != old_mouse_y_position) {
				ed_render_map(); done_button(0, ed_control_y + 52);
				old_mouse_x_position = mouse_x_position;
				old_mouse_y_position = mouse_y_position;
			}
		}

		// Paste the region
		if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			for (x_copy_loop = 0; x_copy_loop < multi_select_width; x_copy_loop++)
				for (y_copy_loop = 0; y_copy_loop < multi_select_height; y_copy_loop++)
					set_map_layer_value((x_copy_loop * 16) + mouse_x, (y_copy_loop * 16) + mouse_y, current_editing_layer, copyed_region[x_copy_loop][y_copy_loop]);
			ed_render_map(); done_button(0, ed_control_y + 52);
		}
	} while (!done_button(1, ed_control_y + 52));

	// Restore the multi select box to its original dimensions
	multi_select_width = multi_width_swap;
	multi_select_height = multi_height_swap;

	// The current action has to be editing now that the conversion is done
	if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
	else current_action = "Editing Foreground Layer. . .";

	// Tell the tools menu to close itself
	return TRUE;
}

/*************************************\
|** BACKGROUND TO FOREGROUND BUTTON **|
\*************************************/
char background_to_foreground_button(int x, int y)
{
	char info_layer_setting = show_info_layer;
	short int info_layer_x, info_layer_y;
	int old_mouse_x_position = -1, old_mouse_y_position = -1;
	int multi_height_swap, multi_width_swap;
	int info_layer_backup[256][192];
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 52, 0, x + 52, y, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(x + 52, y, x + 104, y + 16) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 52, 0, x + 52, y, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 52, 0, x + 52, y, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 52, 0, x + 52, y, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Set the bounding box width and height to 1, so you can see what you are doing
	multi_width_swap = multi_select_width;
	multi_select_width = 0;
	multi_height_swap = multi_select_height;
	multi_select_height = 0;

	// Turn on info layer display, and backup and 0ify the info layer.
	show_info_layer = 1;
	for (info_layer_x = 0; info_layer_x < 256; info_layer_x++) {
		for (info_layer_y = 0; info_layer_y < 192; info_layer_y++) {
			info_layer_backup[info_layer_x][info_layer_y] = editor_map.info_tile_grid[info_layer_x][info_layer_y];
			editor_map.info_tile_grid[info_layer_x][info_layer_y] = 0;
		}
	}

	// Say what we are doing
	current_action = "Converting Background To Foreground.";

	// Render the map so the new current action shows up
	ed_render_map();

	// Put away the tools menu bar and show the done button
	popin_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);
	done_button(0, ed_control_y + 52);

	// Wait for the user to click on the scrolling display so we know where to convert
	do {
		// Determine x coordinate of the done button
		if (ed_control_x <= screen_width - 60 - 52) x = ed_control_x + 60;
		else x = ed_control_x - 52;
		// Check to see if the user scrolled the display
		if (check_scroll() == TRUE) {
			ed_render_map(); done_button(0, ed_control_y + 52);
		}

		// This updates the multi select box like the code in main() would
		if (!mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			mouse_x_position = (mouse_x + screen_x_position) / 16;
			mouse_y_position = (mouse_y + screen_y_position) / 16;
			if (mouse_x_position != old_mouse_x_position || mouse_y_position != old_mouse_y_position) {
				ed_render_map(); done_button(0, ed_control_y + 52);
				old_mouse_x_position = mouse_x_position;
				old_mouse_y_position = mouse_y_position;
			}
		}

		if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			editor_map.info_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = 191;
			ed_render_map(); done_button(0, ed_control_y + 52);
		}

		if (mouse_b & 2 && mouse_in_region_notran(0, 0, screen_width - 2, screen_height - 2) && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			editor_map.info_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = 0;
			ed_render_map(); done_button(0, ed_control_y + 52);
		}
	} while (!done_button(1, ed_control_y + 52));

	while (mouse_b & 1 || mouse_b & 2) {
	}

	// Restore the multi select box to its original dimensions
	multi_select_width = multi_width_swap;
	multi_select_height = multi_height_swap;

	// Restore the info layer settings and the info layer itself, and do the actual conversion
	show_info_layer = info_layer_setting;
	for (info_layer_x = 0; info_layer_x < 256; info_layer_x++) {
		for (info_layer_y = 0; info_layer_y < 192; info_layer_y++) {
			if (editor_map.info_tile_grid[info_layer_x][info_layer_y] == 191) editor_map.foreground_tile_grid[info_layer_x][info_layer_y] = editor_map.background_tile_grid[info_layer_x][info_layer_y];
			editor_map.info_tile_grid[info_layer_x][info_layer_y] = info_layer_backup[info_layer_x][info_layer_y];
		}
	}

	// The current action has to be editing now that the conversion is done
	if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
	if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
	if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
	if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";

	// Tell the tools menu to close itself
	return TRUE;
}

/*************************************\
|** FOREGROUND TO BACKGROUND BUTTON **|
\*************************************/
char foreground_to_background_button(int x, int y)
{
	char info_layer_setting = show_info_layer;
	short int info_layer_x, info_layer_y;
	int old_mouse_x_position = -1, old_mouse_y_position = -1;
	int multi_height_swap, multi_width_swap;
	int info_layer_backup[256][192];
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 52, 16, x + 52, y + 16, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(x + 52, y + 16, x + 104, y + 32) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 52, 16, x + 52, y + 16, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW_DOWN].dat, screen, 52, 16, x + 52, y + 16, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, screen, 52, 16, x + 52, y + 16, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Set the bounding box width and height to 1, so you can see what you are doing
	multi_width_swap = multi_select_width;
	multi_select_width = 0;
	multi_height_swap = multi_select_height;
	multi_select_height = 0;

	// Turn on info layer display, and backup and 0ify the info layer.
	show_info_layer = 1;
	for (info_layer_x = 0; info_layer_x < 256; info_layer_x++) {
		for (info_layer_y = 0; info_layer_y < 192; info_layer_y++) {
			info_layer_backup[info_layer_x][info_layer_y] = editor_map.info_tile_grid[info_layer_x][info_layer_y];
			editor_map.info_tile_grid[info_layer_x][info_layer_y] = 0;
		}
	}

	// Say what we are doing
	current_action = "Converting Foreground To Background.";

	// Render the map so the new current action shows up
	ed_render_map();

	// Put away the tools menu bar and show the done button
	popin_menu(worminator_data_file[ED_POPUP_TOOLS_MENU_WINDOW].dat, 52, 2);
	done_button(0, ed_control_y + 52);

	// Wait for the user to click on the scrolling display so we know where to convert
	do {
		// Determine x coordinate of the done button
		if (ed_control_x <= screen_width - 60 - 52) x = ed_control_x + 60;
		else x = ed_control_x - 52;
		// Check to see if the user scrolled the display
		if (check_scroll() == TRUE) {
			ed_render_map(); done_button(0, ed_control_y + 52);
		}

		// This updates the multi select box like the code in main() would
		if (!mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			mouse_x_position = (mouse_x + screen_x_position) / 16;
			mouse_y_position = (mouse_y + screen_y_position) / 16;
			if (mouse_x_position != old_mouse_x_position || mouse_y_position != old_mouse_y_position) {
				ed_render_map(); done_button(0, ed_control_y + 52);
				old_mouse_x_position = mouse_x_position;
				old_mouse_y_position = mouse_y_position;
			}
		}

		if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			editor_map.info_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = 191;
			ed_render_map(); done_button(0, ed_control_y + 52);
		}

		if (mouse_b & 2 && mouse_in_region_notran(0, 0, screen_width - 2, screen_height - 2) && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 52, x + 52, ed_control_y + 68)) {
			editor_map.info_tile_grid[(mouse_x + screen_x_position) / 16][(mouse_y + screen_y_position) / 16] = 0;
			ed_render_map(); done_button(0, ed_control_y + 52);
		}
	} while (!done_button(1, ed_control_y + 52));

	while (mouse_b & 1 || mouse_b & 2) {
	}

	// Restore the multi select box to its original dimensions
	multi_select_width = multi_width_swap;
	multi_select_height = multi_height_swap;

	// Restore the info layer settings and the info layer itself, and do the actual conversion
	show_info_layer = info_layer_setting;
	for (info_layer_x = 0; info_layer_x < 256; info_layer_x++) {
		for (info_layer_y = 0; info_layer_y < 192; info_layer_y++) {
			if (editor_map.info_tile_grid[info_layer_x][info_layer_y] == 191) editor_map.background_tile_grid[info_layer_x][info_layer_y] = editor_map.foreground_tile_grid[info_layer_x][info_layer_y];
			editor_map.info_tile_grid[info_layer_x][info_layer_y] = info_layer_backup[info_layer_x][info_layer_y];
		}
	}

	// The current action has to be editing now that the conversion is done
	if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
	if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
	if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
	if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";

	// Tell the tools menu to close itself
	return TRUE;
}

/*************************\
|** RESIZE LEVEL BUTTON **|
\*************************/
char resize_level_button()
{
	char done_looping = FALSE;
	int old_mouse_x = 5, old_mouse_y = 5;
	int mouse_x_position, mouse_y_position;
	int x = screen_width / 2 - 128, y = screen_height / 2 - 98;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 64, ed_control_x + 4, ed_control_y + 68, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 68, ed_control_x + 55, ed_control_y + 83) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 64, ed_control_x + 4, ed_control_y + 68, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 64, ed_control_x + 4, ed_control_y + 4, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 64, ed_control_x + 4, ed_control_y + 68, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Wait for the user to release the mouse button
	while (mouse_b & 1) {
	}

	// Clear the area where the scrolling display normally is
	show_mouse(NULL); rectfill(screen, 0, 0, screen_width, screen_height, 0); show_mouse(screen);

	// Set the mouse x and y coordinates to the current size of the level
	position_mouse(editor_map.map_width + x, editor_map.map_height + y);

	set_mouse_range(x + 16, y + 12, x + 256, y + 192);

	// Keep showing the current level size until the user clicks the left mouse button
	do {
		if (mouse_x != old_mouse_x || mouse_y != old_mouse_y) {
			show_mouse(NULL);
			mouse_x_position = mouse_x, mouse_y_position = mouse_y;
			rect(screen, x, y, old_mouse_x, old_mouse_y, 0);
			rect(screen, x, y, mouse_x_position, mouse_y_position, 255);
			rectfill(screen, screen_width - 64, screen_height - 32, screen_width, screen_height, 0);
			sprintf(message, "X: %-5d", mouse_x_position - x);
			textout(screen, font, message, screen_width - 64, screen_height - 32, 255);
			sprintf(message, "Y: %-5d", mouse_y_position - y);
			textout(screen, font, message, screen_width - 64, screen_height - 16, 255);
			old_mouse_x = mouse_x_position;
			old_mouse_y = mouse_y_position;
			show_mouse(screen);
		}
		if (mouse_b & 1) {
			mouse_x_position = mouse_x, mouse_y_position = mouse_y;
			if (mouse_x_position < x + 16) mouse_x_position = x + 16;
			else if (mouse_x_position > x + 256) mouse_x_position = x + 256;
			if (mouse_y_position < y + 12) mouse_y_position = y + 12;
			else if (mouse_y_position > y + 192) mouse_y_position = y + 192;
			editor_map.map_width = mouse_x_position - x;
			editor_map.map_height = mouse_y_position - y;
			done_looping = TRUE;
			while (mouse_b & 1) {
			}
		}
	} while (done_looping != TRUE);

	// Make sure that the screen position is not outside of the level
	check_scroll();

	set_mouse_range(0, 0, screen_width - 1, screen_height - 1);

	// Tell the editor to re-draw the scrolling display
	return TRUE;
}

/***********************\
|** ADD SPRITE BUTTON **|
\***********************/
char add_sprite_button()
{
	char done_looping = FALSE;
	unsigned char sprite_to_add;
	int sprite_search_loop;
	int x = screen_width / 2 - 128, y = screen_height / 2 - 98;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 80, ed_control_x + 4, ed_control_y + 84, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 84, ed_control_x + 55, ed_control_y + 99) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 84, ed_control_x + 4, ed_control_y + 84, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 80, ed_control_x + 4, ed_control_y + 84, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 84, ed_control_x + 4, ed_control_y + 84, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Show the sprite chart
	show_mouse(NULL);
	rectfill(screen, 0, 0, screen_width, screen_height, 0);
	blit(worminator_data_file[ED_EDITOR_SPRITE_CHART].dat, screen, 0, 0, x, y, 256, 192);
	show_mouse(screen);

	// Wait for the user to click on a tile
	do {
		if (mouse_clicked_region_notran(x, y, x + 255, y + 255)) {
			sprite_to_add = (mouse_x - x) / 16 + (mouse_y - y) / 16 * 16;
			done_looping = TRUE;
			while (mouse_b & 1) {
			}
		}
	} while (done_looping == FALSE);

	// Set the current action
	current_action = "Adding Sprites. . .";

	ed_render_map();
	done_looping = FALSE;

	done_button(0, ed_control_y + 84);

	do {
		if (check_scroll()) {
			ed_render_map(); done_button(0, ed_control_y + 84);
		}
		// Determine x coordinate of the done button
		if (ed_control_x <= screen_width - 60 - 52) x = ed_control_x + 60;
		else x = ed_control_x - 52;
		if (mouse_b & 1 && !mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(x, ed_control_y + 84, x + 52, ed_control_y + 100)) {
			// Parallaxing background icons
			if (sprite_to_add >= 160 && sprite_to_add < 170) {
				parallax_background = worminator_data_file[PARALLAXING_BACKGROUND_01 + sprite_to_add - 160].dat;

				for (sprite_search_loop = 1; sprite_search_loop < 255; sprite_search_loop++) {
					if (editor_map.starting_sprites[sprite_search_loop].sprite_type == 0) {
						;
						editor_map.starting_sprites[sprite_search_loop].sprite_type = sprite_to_add;
						editor_map.starting_sprites[sprite_search_loop].x_position_in_pixels = ((mouse_x + screen_x_position) / 16 * 16);
						editor_map.starting_sprites[sprite_search_loop].y_position_in_pixels = ((mouse_y + screen_y_position) / 16 * 16);
						sprite_search_loop = 255;
					}
				}
				while (mouse_b & 1) {
				}
				;
				return FALSE;
			}

			// Dark spawn icon
			/*if (sprite_to_add == 188 && editor_map.weather == 0) editor_map.weather = 3;
			 * else if (sprite_to_add == 188 && editor_map.weather != 0) {
			 *      // The current action has to be editing if this code is running
			 *      if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
			 *      if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
			 *      if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
			 *      if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";
			 *      ed_render_map();
			 *      done_button(0, ed_control_y + 84);
			 *
			 *      while (mouse_b & 1) { };
			 *      return FALSE;
			 *      }
			 *
			 * // Fog spawn icon
			 * if (sprite_to_add == 189 && editor_map.weather == 0) editor_map.weather = 4;
			 * else if (sprite_to_add == 189 && editor_map.weather != 0) {
			 *      // The current action has to be editing if this code is running
			 *      if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
			 *      if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
			 *      if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
			 *      if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";
			 *      ed_render_map();
			 *      done_button(0, ed_control_y + 84);
			 *
			 *      while (mouse_b & 1) { };
			 *      return FALSE;
			 *      }*/

			// Snow spawn icon
			if (sprite_to_add == 190 && editor_map.weather == 0) {
				editor_map.weather = 1;
			} else if (sprite_to_add == 190 && editor_map.weather != 0) {
				// The current action has to be editing if this code is running
				if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
				if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
				if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
				if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";
				ed_render_map();
				done_button(0, ed_control_y + 84);

				while (mouse_b & 1) {
				}
				;
				return FALSE;
			}

			// Rain spawn icon
			if (sprite_to_add == 191 && editor_map.weather == 0) {
				editor_map.weather = 2;
			} else if (sprite_to_add == 191 && editor_map.weather != 0) {
				// The current action has to be editing if this code is running
				if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
				if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
				if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
				if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";
				ed_render_map();
				done_button(0, ed_control_y + 84);

				while (mouse_b & 1) {
				}
				;
				return FALSE;
			}

			// Worminator starting position
			if (sprite_to_add == 0) {
				editor_map.starting_sprites[0].x_position_in_pixels = ((mouse_x + screen_x_position) / 16 * 16);
				editor_map.starting_sprites[0].y_position_in_pixels = ((mouse_y + screen_y_position) / 16 * 16);
			}
			// Everything but the starting position
			else {
				for (sprite_search_loop = 1; sprite_search_loop < 255; sprite_search_loop++) {
					if (editor_map.starting_sprites[sprite_search_loop].sprite_type == 0) {
						;
						editor_map.starting_sprites[sprite_search_loop].sprite_type = sprite_to_add;
						editor_map.starting_sprites[sprite_search_loop].x_position_in_pixels = ((mouse_x + screen_x_position) / 16 * 16);
						editor_map.starting_sprites[sprite_search_loop].y_position_in_pixels = ((mouse_y + screen_y_position) / 16 * 16);
						sprite_search_loop = 255;
					}
				}
			}

			// Show the new sprite
			ed_render_map();
			done_button(0, ed_control_y + 84);

			while (mouse_b & 1) {
			}
		}
		// Move the controls dialog box
		else if (mouse_b & 1 && mouse_in_region_notran(ed_control_x, ed_control_y, ed_control_x + 60, ed_control_y + 200) && !mouse_in_region_notran(ed_control_x + 4, ed_control_y + 4, ed_control_x + 55, ed_control_y + 179)) {
			int mx_offset = mouse_x - ed_control_x;
			int my_offset = mouse_y - ed_control_y;

			while (mouse_b & 1) {
				ed_control_x = mouse_x - mx_offset;
				ed_control_y = mouse_y - my_offset;

				ed_render_map();
				done_button(0, ed_control_y + 84);
			}
		}
	} while (!done_button(1, ed_control_y + 84) && !key[KEY_ESC]);

	// The current action has to be editing if this code is running
	if (current_editing_layer == 0) current_action = "Editing Background Layer. . .";
	if (current_editing_layer == 1) current_action = "Editing Foreground Layer. . .";
	if (current_editing_layer == 2) current_action = "Editing Info Layer. . .";
	if (current_editing_layer == 3) current_action = "Editing Graphical Properties Layer. . .";

	// Tell the editor to re-draw the scrolling display
	return TRUE;
}

/********************\
|** MEW MAP BUTTON **|
\********************/
char new_map_button()
{
	int erase_grid_x_loop, erase_grid_y_loop;
	int erase_sprites_loop;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 96, ed_control_x + 4, ed_control_y + 100, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 100, ed_control_x + 55, ed_control_y + 115) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 100, ed_control_x + 4, ed_control_y + 100, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 96, ed_control_x + 4, ed_control_y + 100, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 100, ed_control_x + 4, ed_control_y + 100, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	if (alert("Are you sure you", "want to start", "a new map?", "OK", "Cancel", 13, 0) != 1) return 0;

	// Erase the map
	editor_map.map_width = 32;
	editor_map.map_height = 32;
	editor_map.map_name[0] = 0;
	for (erase_grid_x_loop = 0; erase_grid_x_loop < 256; erase_grid_x_loop++) {
		for (erase_grid_y_loop = 0; erase_grid_y_loop < 192; erase_grid_y_loop++) {
			editor_map.background_tile_grid[erase_grid_x_loop][erase_grid_y_loop] = 0;
			editor_map.foreground_tile_grid[erase_grid_x_loop][erase_grid_y_loop] = 0;
			editor_map.info_tile_grid[erase_grid_x_loop][erase_grid_y_loop] = 0;
			editor_map.graphical_properties_tile_grid[erase_grid_x_loop][erase_grid_y_loop] = 0;
		}
	}
	for (erase_sprites_loop = 0; erase_sprites_loop < 256; erase_sprites_loop++) {
		editor_map.starting_sprites[erase_sprites_loop].sprite_type = 0;
		editor_map.starting_sprites[erase_sprites_loop].x_position_in_pixels = 0;
		editor_map.starting_sprites[erase_sprites_loop].y_position_in_pixels = 0;
	}
	editor_map.weather = 0;

	if (screen_x_position > editor_map.map_width * 16 - 256) screen_x_position = editor_map.map_width * 16 - 256;
	if (screen_y_position > editor_map.map_height * 16 - 192) screen_y_position = editor_map.map_height * 16 - 192;

	// Tell the editor that we need to re-draw the screen
	return TRUE;
}

/*******************************\
|** DISPLAY PROPERTIES BUTTON **|
\*******************************/
char display_properties_button()
{
	char done_looping = FALSE;
	char mouse_button_1_register = 0;
	BITMAP *ed_temp;
	int x, y;
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 112, ed_control_x + 4, ed_control_y + 116, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 116, ed_control_x + 55, ed_control_y + 131) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 116, ed_control_x + 4, ed_control_y + 116, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 112, ed_control_x + 4, ed_control_y + 116, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 116, ed_control_x + 4, ed_control_y + 116, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Scroll out the popup display properties window
	ed_temp = create_bitmap_ex(8, 52, 80);
	if (show_background_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 0, 0, 0, 52, 16);
	else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 0, 0, 0, 52, 16);
	if (show_foreground_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 16, 0, 16, 52, 16);
	else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 16, 0, 16, 52, 16);
	if (show_info_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 32, 0, 32, 52, 16);
	else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 32, 0, 32, 52, 16);
	if (show_graphical_properties_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 48, 0, 48, 52, 16);
	else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 48, 0, 48, 52, 16);
	blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 64, 0, 64, 52, 16);
	x = popout_menu(ed_temp, 116, 5);
	y = ed_control_y + 116;
	destroy_bitmap(ed_temp);

	do {
		// Update mouse button status
		if (mouse_b & 1) {
			if (mouse_button_1_register == 0) mouse_button_1_register = 1;
			else if (mouse_button_1_register == 1) mouse_button_1_register = 2;
		} else {
			mouse_button_1_register = 0;
		}
		// Display backround button /////////////////////////////////////////////////////
		if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y, x + 52, y + 16)) {
			if (show_background_layer == 1) show_background_layer = 0;
			else show_background_layer = 1;
			ed_render_map();
			show_mouse(NULL);
			if (show_background_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 0, x, y, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 0, x, y, 52, 16);
			if (show_foreground_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 16, x, y + 16, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 16, x, y + 16, 52, 16);
			if (show_info_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 32, x, y + 32, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 32, x, y + 32, 52, 16);
			if (show_graphical_properties_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 48, x, y + 48, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 48, x, y + 48, 52, 16);
			blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 64, x, y + 64, 52, 16);
			show_mouse(screen);
		}
		// Display foreground button ////////////////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y + 16, x + 52, y + 32)) {
			if (show_foreground_layer == 1) show_foreground_layer = 0;
			else show_foreground_layer = 1;
			ed_render_map();
			show_mouse(NULL);
			if (show_background_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 0, x, y, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 0, x, y, 52, 16);
			if (show_foreground_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 16, x, y + 16, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 16, x, y + 16, 52, 16);
			if (show_info_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 32, x, y + 32, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 32, x, y + 32, 52, 16);
			if (show_graphical_properties_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 48, x, y + 48, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 48, x, y + 48, 52, 16);
			blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 64, x, y + 64, 52, 16);
			show_mouse(screen);
		}
		// Display info button //////////////////////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y + 32, x + 52, y + 48)) {
			if (show_info_layer == 1) show_info_layer = 0;
			else show_info_layer = 1;
			show_graphical_properties_layer = 0;
			ed_render_map();
			show_mouse(NULL);
			if (show_background_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 0, x, y, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 0, x, y, 52, 16);
			if (show_foreground_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 16, x, y + 16, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 16, x, y + 16, 52, 16);
			if (show_info_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 32, x, y + 32, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 32, x, y + 32, 52, 16);
			if (show_graphical_properties_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 48, x, y + 48, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 48, x, y + 48, 52, 16);
			blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 64, x, y + 64, 52, 16);
			show_mouse(screen);
		}
		// Display graphical properties button //////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y + 48, x + 52, y + 64)) {
			if (show_graphical_properties_layer == 1) show_graphical_properties_layer = 0;
			else show_graphical_properties_layer = 1;
			show_info_layer = 0;
			ed_render_map();
			show_mouse(NULL);
			if (show_background_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 0, x, y, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 0, x, y, 52, 16);
			if (show_foreground_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 16, x, y + 16, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 16, x, y + 16, 52, 16);
			if (show_info_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 32, x, y + 32, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 32, x, y + 32, 52, 16);
			if (show_graphical_properties_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 48, x, y + 48, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 48, x, y + 48, 52, 16);
			blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, screen, 0, 64, x, y + 64, 52, 16);
			show_mouse(screen);
		}
		// Done button //////////////////////////////////////////////////////////////////
		else if (mouse_button_1_register == 1 && mouse_in_region_notran(x, y + 64, x + 52, y + 80)) {
			blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, screen, 0, 64, x, y + 64, 52, 16);
			while (mouse_b & 1) {
			}
			// Scroll in the popup display properties window
			ed_temp = create_bitmap_ex(8, 52, 80);
			if (show_background_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 0, 0, 0, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 0, 0, 0, 52, 16);
			if (show_foreground_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 16, 0, 16, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 16, 0, 16, 52, 16);
			if (show_info_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 32, 0, 32, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 32, 0, 32, 52, 16);
			if (show_graphical_properties_layer == 0) blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 48, 0, 48, 52, 16);
			else blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW_DOWN].dat, ed_temp, 0, 48, 0, 48, 52, 16);
			blit(worminator_data_file[ED_POPUP_DISPLAY_PROPERTIES_WINDOW].dat, ed_temp, 0, 64, 0, 64, 52, 16);
			popin_menu(ed_temp, 116, 5);
			destroy_bitmap(ed_temp);
			done_looping = TRUE;
			mouse_button_1_register = 0;
		}
	} while (done_looping == FALSE);
	return TRUE;
}

/*********************\
|** LOAD MAP BUTTON **|
\*********************/
char load_map_button()
{
	char file_path[256] = "";
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 128, ed_control_x + 4, ed_control_y + 132, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 132, ed_control_x + 55, ed_control_y + 147) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 132, ed_control_x + 4, ed_control_y + 132, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 128, ed_control_x + 4, ed_control_y + 132, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 132, ed_control_x + 4, ed_control_y + 132, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Display a file box and load if a valid map file selected
	if (file_select_ex("Select a map data file (.map files)", file_path, "map",
			   sizeof(file_path), 0, 0) != 0) ed_load_map(file_path);
	ed_render_map();

	// Re-draw the screen
	return TRUE;
}

/*********************\
|** SAVE MAP BUTTON **|
\*********************/
char save_map_button()
{
	char file_path[256] = "";
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 144, ed_control_x + 4, ed_control_y + 148, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 148, ed_control_x + 55, ed_control_y + 163) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 148, ed_control_x + 4, ed_control_y + 148, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 144, ed_control_x + 4, ed_control_y + 148, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 148, ed_control_x + 4, ed_control_y + 148, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Display a file box and save if a valid map file selected
	if (file_select_ex("Select a map data file (.map files)", file_path, "map", sizeof(file_path), 0, 0) != 0) ed_save_map(file_path);
	ed_render_map();

	// Re-draw the screen
	return TRUE;
}

/*************************\
|** EXIT PROGRAM BUTTON **|
\*************************/
char exit_program_button()
{
	int in_region = TRUE;	// Used to process buttion status

	// Display the button pushed down
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 160, ed_control_x + 4, ed_control_y + 164, 52, 16); show_mouse(screen);
	while (mouse_b & 1) {
		if (mouse_in_region_notran(ed_control_x + 4, ed_control_y + 164, ed_control_x + 55, ed_control_y + 179) != in_region) {
			if (in_region == TRUE) {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 164, ed_control_x + 4, ed_control_y + 164, 52, 16); show_mouse(screen);
				in_region = FALSE;
			} else {
				show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DOWN].dat, screen, 0, 160, ed_control_x + 4, ed_control_y + 164, 52, 16); show_mouse(screen);
				in_region = TRUE;
			}
		}
	}
	show_mouse(NULL); blit(worminator_data_file[ED_MAIN_EDITOR_CONTROLS_DISPLAY].dat, screen, 4, 164, ed_control_x + 4, ed_control_y + 164, 52, 16); show_mouse(screen);

	// Make sure the mouse is still over the button
	if (!in_region) return 0;

	// Tell the editor to quit
	if (alert("Are you sure", "you want to", "quit the editor?", "OK", "Cancel", 13, 0) == 1) return 1;
	return 0;
}

/******************\
|** CHECK SCROLL **|
\******************/
char check_scroll()
{
	char render_it = FALSE;

	// Scroll to the left ///////////////////////////////////////////////////////////////////////////
	if (mouse_x == 0) {
		if (mouse_b & 2) screen_x_position -= 16;
		else screen_x_position -= 2;
		render_it = TRUE;
	}

	if (key[KEY_LEFT]) {
		screen_x_position -= 16;
		render_it = TRUE;
	}

	// Scroll up ////////////////////////////////////////////////////////////////////////////////////
	if (mouse_y == 0) {
		if (mouse_b & 2) screen_y_position -= 16;
		else screen_y_position -= 2;
		render_it = TRUE;
	}

	if (key[KEY_UP]) {
		screen_y_position -= 16;
		render_it = TRUE;
	}

	// Scroll to the right //////////////////////////////////////////////////////////////////////////
	if (mouse_x == screen_width - 1) {
		if (mouse_b & 2) screen_x_position += 16;
		else screen_x_position += 2;
		render_it = TRUE;
	}

	if (key[KEY_RIGHT]) {
		screen_x_position += 16;
		render_it = TRUE;
	}

	// Scroll down //////////////////////////////////////////////////////////////////////////////////
	if (mouse_y == screen_height - 1) {
		if (mouse_b & 2) screen_y_position += 16;
		else screen_y_position += 2;
		render_it = TRUE;
	}

	if (key[KEY_DOWN]) {
		screen_y_position += 16;
		render_it = TRUE;
	}

	// Fix the screen coordinates
	if (screen_x_position > editor_map.map_width * 16 - screen_width) screen_x_position = editor_map.map_width * 16 - screen_width;
	if (screen_y_position > editor_map.map_height * 16 - screen_height) screen_y_position = editor_map.map_height * 16 - screen_height;

	if (screen_x_position < 0) screen_x_position = 0;
	if (screen_y_position < 0) screen_y_position = 0;

	return render_it;
}

/**************************\
|** REFRESH TILE DISPLAY **|
\**************************/
void refresh_tile_display()
{
	int tile_counter = top_tile_offset;
	int x1 = tile_grid_attribute, y1 = 0, x2, y2;
	int x_tile_loop, y_tile_loop;
	int x = screen_width / 2 - 128;

	tile_grid_attribute -= top_tile_offset;

	if (current_editing_layer == 2 || current_editing_layer == 3) tile_counter = 0;

	// Loop through and draw all the tiles in the current layer tileset
	rectfill(ed_double_buffer, 0, 0, screen_width, screen_height, 0);
	for (y_tile_loop = 0; y_tile_loop < (screen_height / 16) + 2; y_tile_loop++) {
		for (x_tile_loop = 0; x_tile_loop < 16; x_tile_loop++) {
			if (current_editing_layer < 2) blit(worminator_tiles, ed_double_buffer, tile_counter * 16, 0, x_tile_loop * 16 + x, y_tile_loop * 16, 16, 16);
			else if (current_editing_layer == 2 && y_tile_loop < 12) blit(worminator_info_tiles, ed_double_buffer, tile_counter * 16, 0, x_tile_loop * 16 + x, y_tile_loop * 16, 16, 16);
			else if (y_tile_loop < 12) blit(worminator_graphical_properties_tiles, ed_double_buffer, tile_counter * 16, 0, x_tile_loop * 16 + x, y_tile_loop * 16, 16, 16);
			tile_counter++;
		}
	}

	while (x1 >= 16) {
		x1 -= 16;
		y1++;
	}

	x1 = x1 * 16;
	y1 = (y1 * 16) - top_tile_offset;

	if (multi_select_width < 1) x2 = x1 + 15;
	else x2 = x1 + (multi_select_width * 16) - 1;

	if (multi_select_height < 1) y2 = y1 + 15;
	else y2 = y1 + (multi_select_height * 16) - 1;

	set_clip(ed_double_buffer, x, 0, x + 256 - 1, screen_height);
	rect(ed_double_buffer, x1 + x, y1, x2 + x, y2, 255);
	set_clip(ed_double_buffer, 0, 0, screen_width, screen_height);

	// Show the buttons
	blit(worminator_data_file[ED_TILE_BUTTONS].dat, ed_double_buffer, 0, 0, screen_width - 16, 0, 16, 32);
	blit(worminator_data_file[ED_TILE_BUTTONS].dat, ed_double_buffer, 0, 32, screen_width - 16, screen_height - 32, 16, 32);
	blit(worminator_data_file[ED_TILE_BUTTONS].dat, ed_double_buffer, 16, 0, screen_width - 16, screen_height / 2 - 32, 16, 64);

	// Show the tiles
	show_mouse(NULL); blit(ed_double_buffer, screen, 0, 0, 0, 0, screen_width, screen_height); show_mouse(screen);

	tile_grid_attribute += top_tile_offset;
}

/*******************\
|** DRAW TILE BOX **|
\*******************/
void draw_tile_box(unsigned char x_tile, unsigned char y_tile, unsigned char width, unsigned char height)
{
	int x1 = (x_tile * 16) - screen_x_position, y1 = (y_tile * 16) - screen_y_position, x2 = x1 + (width * 16) - 1, y2 = y1 + (height * 16) - 1;

	// This draws a white box from (x, y) to (x + width, y + height)
	//set_clip(screen, tranx(4), trany(4), tranx(259), trany(195));
	show_mouse(NULL);
	rect(screen, x1, y1, x2, y2, 255);
	show_mouse(screen);
	//set_clip(screen, 0, 0, screen_width - 1, screen_height - 1);
}

/*****************\
|** DONE BUTTON **|
\*****************/
char done_button(char draw_flags, int y)
{
	int x;

	// Draw flags notes:
	// draw_flags = 1 does not draw button
	// draw_flags = 2 does not erase button
	// draw_flags = 3 does not draw or erase button
	// draw_flags = 4 just erases and quits

	// Determine x coordinate of the done button
	if (ed_control_x <= screen_width - 60 - 52) x = ed_control_x + 60;
	else x = ed_control_x - 52;

	// See if we should just erase and quit
	if (draw_flags == 4) {
		ed_render_map();
		return TRUE;
	}

	// Draw the done button if it is not already drawn
	if ((draw_flags == 0 || draw_flags == 2)) {
		show_mouse(NULL); blit(worminator_data_file[ED_DONE_BUTTON].dat, screen, 0, 0, x, y, 52, 16); show_mouse(screen);
	}

	// Check the input
	if (mouse_clicked_region_notran(x, y, x + 52, y + 16) == TRUE) {
		show_mouse(NULL); blit(worminator_data_file[ED_DONE_BUTTON_DOWN].dat, screen, 0, 0, x, y, 52, 16); show_mouse(screen);
		while (mouse_b & 1) {
		}
		;
		show_mouse(NULL); blit(worminator_data_file[ED_DONE_BUTTON].dat, screen, 0, 0, x, y, 52, 16); show_mouse(screen);

		// If the button was actually clicked, then clear the done button and return true
		if (mouse_in_region_notran(x, y, x + 52, y + 16)) {
			if (draw_flags == 0 || draw_flags == 1) ed_render_map();
			return TRUE;
		}
	}

	// The button was not clicked, so return false
	return FALSE;
}
