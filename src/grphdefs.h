/* -- Grphdefs.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * This file is a list of all the different info graphical properties layer
 * attributes.  This is merely to allow you to add and remove new attributes
 * without re-writing the entire engine. */

#define blank_grph_tile                         0	// draw these tile(s) normally
#define masked_background_grph_tile             1	// mask this background tile against the parallaxing background
#define masked_foreground_grph_tile             2	// draw this foreground tile behind all sprites
#define masked_everything_grph_tile             3	// combine the effects of the above
#define translucent_foreground_grph_tile        4	// draw this foreground tile as translucent
#define parallaxing_foreground_grph_tile        5	// make the foreground have a parallaxing tile
#define water_mask_grph_tile                    6	// draw water here just under the foreground
#define scroll_stop_grph_tile                   7	// this is an isolated room that should be rendered alone
#define hide_grph_tile                          8	// this foreground tile will not be drawn
