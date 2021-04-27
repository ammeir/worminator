/* -- Beams.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by David Layne
 *
 * For function descriptions and explanations, see proto.h
 * For a detailed explanation of how the engine works, see
 * the documents that came with this source code package. */

#define SOLID_COLOR_BEAM 0
#define FADEING_COLOR_BEAM 1

/****************\
|** SPAWN BEAM **|
\****************/
void spawn_beam(int x1, int y, int x2, unsigned char direction, unsigned char color, unsigned char color_mode)
{
	char beam_loop;

	for (beam_loop = 0; beam_loop < 16; beam_loop++) {
		if (particle_beams[beam_loop].beam_active == FALSE) {
			particle_beams[beam_loop].beam_active = TRUE;
			particle_beams[beam_loop].x1 = x1;
			particle_beams[beam_loop].y = y;
			if (direction != 0) {
			} else {
				particle_beams[beam_loop].x2 = x2;
			}
			particle_beams[beam_loop].color = color;
			particle_beams[beam_loop].color_mode = color_mode;
			particle_beams[beam_loop].timer = 8;
			return;
		}
	}
}

/**********************\
|** BEAM IS AVALIBLE **|
\**********************/
char beam_is_avalible()
{
	char beam_loop;

	// Scan all the beam slots and return true if one is found empty
	for (beam_loop = 0; beam_loop < 16; beam_loop++) if (particle_beams[beam_loop].beam_active == FALSE) return TRUE;

	// No beams slots were found empty, so return false
	return FALSE;
}

void clear_beams()
{
	char beam_loop;

	for (beam_loop = 0; beam_loop < 16; beam_loop++) {
		particle_beams[beam_loop].beam_active = FALSE;
		particle_beams[beam_loop].x1 = 0, particle_beams[beam_loop].y = 0, particle_beams[beam_loop].x2 = 0;
		particle_beams[beam_loop].color = 0;
		particle_beams[beam_loop].timer = 0;
		particle_beams[beam_loop].color_mode = 0;
	}
}

/******************\
|** UPDATE BEAMS **|
\******************/
void update_beams()
{
	char beam_loop;

	for (beam_loop = 0; beam_loop < 16; beam_loop++) {
		if (particle_beams[beam_loop].timer == 0) {
			particle_beams[beam_loop].beam_active = FALSE;
		} else {
			particle_beams[beam_loop].timer--;
			if (particle_beams[beam_loop].color_mode == FADEING_COLOR_BEAM) particle_beams[beam_loop].color--;
		}
	}
}

/******************\
|** UPDATE BEAMS **|
\******************/
void render_beams()
{
	char beam_loop;

	for (beam_loop = 0; beam_loop < 16; beam_loop++)
		if (particle_beams[beam_loop].beam_active == TRUE) hline(double_buffer, particle_beams[beam_loop].x1 - screen_x_position, particle_beams[beam_loop].y - screen_y_position, particle_beams[beam_loop].x2 - screen_x_position, particle_beams[beam_loop].color);

}
