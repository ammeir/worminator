/* -- Globals.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * This file includes all of the global variables used in the Worminator
 * game engine.  It also has all of the structure typedefs. */

/*************\
|** DEFINES **|
\*************/
#define WORMINATOR_SKIN 0
#define DAMAGED_WORM_1_SKIN 1
#define DAMAGED_WORM_2_SKIN 2
#define DAMAGED_WORM_3_SKIN 3
#define THE_BRAIN_SKIN 4
#define THE_YIPNIK_MIKIMBO_SKIN 5
#define ORIGINAL_WORMINATOR_SKIN 6
#define HOTDOG_HERO_SKIN 7
#define VOODOO_BADIE_SKIN 8
#define YUANTI_SKIN 9
#define MELON_O_DEATH_SKIN 10
#define BAKED_TURKEY_SKIN 11
#define WATERMELON_WONDER_SKIN 12
#define THE_INVERTEBRATOR_SKIN 13
#define BROWNIE_SKIN 14
#define EVIL_CUBE_O_DEATH_SKIN 15
#define ENDO_SKELETON_SKIN 16
#define MUMMY_WORM_SKIN 17
#define THE_ANGEL_SKIN 18
#define THE_DEVIL_SKIN 19
#define TANK_SKIN 20

#define GODLY_MODE        0
#define FREEZE_MODE       1
#define RAPID_FIRE_MODE   2
#define FLY_MODE          3
#define WEALTH_MODE       4
#define SUPER_TURKEY_MODE 5
#define EKG_MODE          6
#define COOL_MODE         7

#define HELP_GENERAL      0
#define HELP_ENEMIES      1
#define HELP_WEAPONS	  2
#define HELP_STORY        3
#define HELP_CREDITS      4

#define FULL_VERSION 0
#define STRIPPED_VERSION 1
#define DEVELOPMENT_VERSION 1

#define GAME_SPEED_BULLET_TIME 64
#define GAME_SPEED_VERY_SLOW 96
#define GAME_SPEED_SLOW 128
#define GAME_SPEED_NORMAL 160
#define GAME_SPEED_FAST 212
#define GAME_SPEED_VERY_FAST 256

#define DIFFICULTY_WIMP 0
#define DIFFICULTY_EASY 1
#define DIFFICULTY_NORMAL 2
#define DIFFICULTY_HARD 3
#define DIFFICULTY_NIGHTMARE 4

#define INPUT_UP 0
#define INPUT_DOWN 1
#define INPUT_LEFT 2
#define INPUT_RIGHT 3
#define INPUT_JUMP 4
#define INPUT_FIRE 5
#define INPUT_DUCK 6
#define INPUT_MAP 7
#define INPUT_BULLIT_TIME 8
#define INPUT_STATS 9
#define INPUT_LOOK 10
#define INPUT_WEAPON_0 11
#define INPUT_WEAPON_1 12
#define INPUT_WEAPON_2 13
#define INPUT_WEAPON_3 14
#define INPUT_WEAPON_4 15
#define INPUT_WEAPON_5 16
#define INPUT_WEAPON_6 17
#define INPUT_WEAPON_7 18
#define INPUT_WEAPON_8 19
#define INPUT_WEAPON_9 20
#define INPUT_WEAPON_10 21
#define INPUT_WEAPON_11 22
#define INPUT_WEAPON_12 23
#define INPUT_WEAPON_BEST 24
#define INPUT_WEAPON_LAST 25
#define INPUT_WEAPON_NEXT 26
#define INPUT_CHANGE_AMMO_TYPE 27

#define MAX_TEXT_MESSAGES 4
#define TEXT_MESSAGE_TIME 1024

#define MAX_CONSOLE_LINES 4096
#define MAX_CONSOLE_LINE_LENGTH 512

#ifndef DATADIR
#define DATADIR ""
#endif

int screen_width = 640, screen_height = 480;

const float PI = 3.14159;
const float RADIAN_CONVERTER = 3.14159 / 180;

/****************\
|** STRUCTURES **|
\****************/

typedef struct PLAYER {
	int	timer;
	int	width, height;
	int	frame;
	char	animation_timer;
	char	direction;
	char	current_weapon, ammo_type;
	char	on_ground;
	char	in_water;
	char	is_ducking;
	char	letter_order;
	int	score;
	char	health_bars_left;
	char	has_red_keycard, has_green_keycard, has_blue_keycard;
	int	bullets_left, shells_left, rockets_left, cells_left, gasoline_left, mortars_left, plasma_left, mines_left;
	int	chainsaw_duration_left, plasma_bullets_left, explosive_shells_left, micro_nukes_left, walking_mines_left, spikestars_left, rails_left;
	float	x_position_in_pixels, y_position_in_pixels;
	float	x_acceleration, y_acceleration;
	char	skin;
	char	turbo_launch, no_control;
} PLAYER;

typedef struct SPRITE {
	int		sprite_active;
	int		sprite_type;
	char		in_range;
	int		timer;
	char		animation_state;
	int		width, height;
	int		action_state;
	int		frame;
	float		hit_points_left;
	float		x_position_in_pixels, y_position_in_pixels;
	float		x_acceleration, y_acceleration;
	char		direction;
	unsigned char	rotate_factor;
} SPRITE;

typedef struct PATRTICLE_BEAM {
	char		beam_active;
	int		x1, y, x2;
	unsigned char	color;
	unsigned char	timer;
	unsigned char	color_mode;
} PARTICLE_BEAM;

typedef struct EDITOR_SPRITE {
	short int	sprite_type;
	int		x_position_in_pixels, y_position_in_pixels;
} EDITOR_SPRITE;

typedef struct WORMINATOR_MAP_FORMAT {
	unsigned short int	map_width;
	unsigned short int	map_height;
	char			map_name[80];
	short int		background_tile_grid[256][192];
	short int		foreground_tile_grid[256][192];
	short int		info_tile_grid[256][192];
	char			graphical_properties_tile_grid[256][192];
	EDITOR_SPRITE		starting_sprites[256];
	char			weather;
} WORMINATOR_MAP_FORMAT;

typedef struct TEXT_MESSAGE {
	short int	timer;
	char *		message;
} TEXT_MESSAGE;

typedef struct WORMINATOR_CONFIGURATION {
	int		screen_width, screen_height;
	int		controls[28];
	int		secondary_controls[28];
	int		high_scores[10];
	char		high_score_names[10][16];
	int		game_speed;
	char		difficulty;
	int		skin;

	char		particle_detail;
#ifdef ALLEGRO_WINDOWS
	char		safe_mode;
#endif
	char		stretch_levels;

	char		remember_skin;
	char		may_tricks_mode;
	char		autoswitch_weapons;
	char		switch_weapons_on_pickup;

	char		godly_mode;
	char		freeze_mode;
	char		rapid_fire_mode;
	char		fly_mode;
	char		wealth_mode;
	char		super_turkey_mode;
	char		ekg_mode;
	char		cool_mode;

	unsigned char	sound_volume;
	unsigned char	music_volume;
	char		disable_sound;
	char		disable_music;

	char		skip_intro;
	char		display_particles;
	char		show_laser_sight;
	char		run_windowed;
	char		enable_FSAA;
} WORMINATOR_CONFIGURATION;

/*****************\
|** GLOBAL DATA **|
\*****************/

COLOR_MAP trans_table;

PLAYER player;
SPRITE sprites[1024];
PARTICLE_BEAM particle_beams[16];
WORMINATOR_MAP_FORMAT worminator_map;
WORMINATOR_CONFIGURATION wormy_config;

WORMINATOR_MAP_FORMAT buffer_map;

DATAFILE *worminator_data_file = NULL;
FILE *demofile;

BITMAP *swap_buffer; // for color conversion
BITMAP *screen_buffer;
BITMAP *double_buffer;
BITMAP *stretch_buffer;
BITMAP *FSAA_buffer;
BITMAP *worminator_tiles;
BITMAP *parallax_background;
BITMAP *spawned_sprite;
BITMAP *screenbackup;

// This is used for the text messages in the game
TEXT_MESSAGE text_messages[MAX_TEXT_MESSAGES];

// These are used for special key functions
unsigned char jump_key_state = 0, up_key_state = 0, ammo_type_key_state = 0, next_weapon_key_state = 0, last_weapon_key_state = 0;
unsigned int fire_key_state = 0;

// These are the coordinates of the current view portal
int screen_x_position = 0, screen_y_position = 0;

// This is used so that we dont need to scan the entire sprite array, just what is used.
int number_of_sprites = 0;

// Built in tile animation:  Layer, X, Y, Add Until, Length, Speed, Speed clock
short int tile_animations[1024][7];

// This is just to let certain functions display text.
char message[512];

// Array for weather effects
float weather[64][2];

// This is used to allow worminator to run at a decent speed on almost any computer
volatile int speed_counter = 0;
volatile int idle_counter = 0;
volatile int idle_speed_counter = 0;

// This is used to tell the game timer when to update and when not to
char game_is_running = FALSE;

// This is used to indicate what level we are on
char current_level = 1;

// This is used to decide when to kill the game
char time_to_quit = FALSE;

// Signal if special parts of the screen need a redraw
char borders_dirty = FALSE;
char stats_dirty = FALSE;

// Global timer - loops from 0 to 15, 0 to 31, and 0 to 63 on regular intervals
char global_timer = 0;
char global_timer_2 = 0;
char global_timer_3 = 0;
int global_timer_4 = 0;
int hp_regen_timer = 0;
int fps_timer, frame_counter = 0, fps = 0;
char show_fps = FALSE;

// Global variable to allow for screen shakes and earthquakes and cool stuff
int quake_timer = 0;

// Global variable to allow weapon switching
char desired_weapon = 0;

// Timer to allow player to look up/down and a look shifter variable
int look_timer = 0;
int look_shift = 0;

// Variable to help with part of the loading function
char player_just_died = FALSE;

// This is the path of the current level
char current_level_mapfile_path[1024] = "";

// Map and stats toggles and togglers
char display_map = FALSE;
char display_stats = FALSE;
char toggle_map = FALSE;
char toggle_stats = FALSE;

// May tricks mode bullet time timer
short int bullet_time = 0;
char bullet_time_engaged = FALSE;
char bullet_time_key_state = FALSE;

// Mouse cursor animation variable
int mouse_frame = 0;

// Demo recording/playback stuff
char recording_demo = FALSE;
char playing_demo = FALSE;
char lockkey[KEY_MAX];
int demox = 0, demoy = 0;
int demoxdir = 1, demoydir = 1;
int demospin = 0;

// Idle timer
int idle_timer = 0;

// Customizeable control crap
char input_buffer[28];

#ifdef ALLEGRO_WINDOWS
// Special modes
char safe_mode = FALSE;
#endif

// Stores the location of every key
int keylocations[KEY_MAX];

// Worminatrix code
float wormcode[64][3];

// Override to stop weapon autoswitch from fucking things up
char override_autoswitch = FALSE;

// Used for screen fading
PALETTE srcpal, destpal, temppal;
int pal_time = 0;

// Console data and variables
int console_position = -1;				// -1 indicates console inactive
int console_key_state = 0;				// Used for input
char console_state = 0;					// 0 = inactive, 1 = lowering, 2 = lowered, 3 = raising
char consoleText[MAX_CONSOLE_LINES][MAX_CONSOLE_LINE_LENGTH];
char inputString[MAX_CONSOLE_LINE_LENGTH] = { 0 };	// The input string we are reading currently
