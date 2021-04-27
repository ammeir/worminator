/* -- Proto.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * This file includes all the function prototypes
 * for nearly every unique function in worminator. */

/*************************\
|** FUNCTION PROTOTYPES **|
\*************************/

//////////////////////////////////////////////////////////////////////////
// worm.c ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void fade_in_pal(void *palette, int delay);
void fade_out_pal(int delay);
void blit_to_screen(BITMAP *bmp);
void savedisplay();
void greenify();
void redify();
void little_tinge();
void darken(char always);
void restore_palette(char always);

/* This function is what accepst all keyboard input from the player during
 * the game.  The function will move the player in addition to polling for
 * input, so the core game loop only needs to call this and render_map()
 * to control the game for the most part.                               */
void handle_input();

/* This function will render the background layer, the sprites, and the
*  foreground layer on to the double buffer, and then copy the buffer to
*  the screen.  It takes no parameters and returns nothing.             */
void render_map();
void show_stats();
void show_map();
void update_weather();

/* This function is what allows the worminator to fire his shotgun.  It
*  takes no parameters, because it looks in the player data structure for
*  all the information it needs.                                        */
void fire_shotgun();
int find_laser_sight();

/* This function is what allows the worminator to fire his laser gun.  It
 * takes no parameters, because it looks in the player data structure for
 * all the information it needs.                                        */
void fire_laser_gun();

void fire_chainsaw();
void fire_railgun();

/* This function will load any map file into memory.  It will take care of
 * clearing the player and sprite arrays, so just call it and resume the
 * core game loop and you should be fine.                               */
char load_map(char new_level, char *_file_name, char _clear_player);

/* This function will load a saved game off the hard drive              */
char load_saved_game(char *saved_game_file_name);

/* This function will save a game to the hard drive                     */
void save_game(char *saved_game_file_name);

void load_config();
void save_config();

/* This function will load the background and foreground tilesets, as well
 * as the parallaxing background corresponding to the background tileset
 * into the local cache bitmaps.  The load map function should
 * automatically call this, so you wont normally need to use it at all. */
void load_tiles();

void change_game_speed(int speed);
void change_resolution(int x, int y);
void reset_sound();

/* This function will install the allegro game library, the keyboard, the
 * mouse, the sound, and create the cache bitmaps and anything else that
 * needs to be done only once at startup.  Call it only once at the start
 * of the game.                                                         */
void initialize();

/* This function is used for timeing purposes                           */
void increment_speed_counter();

//////////////////////////////////////////////////////////////////////////
// text.h ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void add_text_message(char *message);
void add_text_message_if_new(char *message);
void update_text_messages();
void slide_messages(char message_number);

// Console routines
char stringPeek(char *string);
void stringPop(char *string);
void stringPush(char *string, const char character);
int getToken(char *source, char *token);
void consoleExec(char *input);
int getConsoleInput();
void clear_console();
void add_console_line(char *string);
void draw_console(BITMAP *bmp, int position);

//////////////////////////////////////////////////////////////////////////
// anim.h ////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/* This function builds a look up table of all animated tiles on the
 * current level.  It is called automatically by the load map function,
 * so you don't normally need to call it.  It will return 0 on success
 * or 1 if there were too many animated tiles in the level.             */
void build_animation_table();

/* This function checks to see if a specified animation is at a give tile
 * or not.                                                              */
char check_tile_for_animation(short unsigned int tile_x, short unsigned int tile_y, int animation_start, int animation_length, char animation_speed, int number_of_animations);

/* This function is set up as an interrupt by the initialize function.
 * It is called 100 times a second, and will increment all the animation
 * frames for the animated tiles in the look up table build by the build
 * andimation table function.                                           */
void update_animations();

//////////////////////////////////////////////////////////////////////////
// beams.h ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/* This function will create a beam at x1, y1 heading in a direction or
*  going to x2, y2.  It can be any two colors and any color mode, but the
*  lifespan of any beam is always 16 updates                            */
void spawn_beam(int x1, int y, int x2, unsigned char direction, unsigned char color, unsigned char color_mode);

/* This function will return TRUE if there is an empty beam slot avalible,
 * or FALSE if all beam slots are full.                                 */
char beam_is_avalible();

/* This function adjusts the lifespan timers on all the beams and will
 * destroy the beam if a beam's timer has expired.  It will also adjust
 * the beam's color if necessecary.                                     */
void update_beams();

/* This function draws all the beams on the double buffer.  It is called
 * by the render_map() function, so there is no need to call it.        */
void render_beams();

//////////////////////////////////////////////////////////////////////////
// player.h //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int check_weapon_autoswitch();
int best_weapon_safe();
int best_weapon();
int last_weapon();
int next_weapon();

/* This function attempts to move the player in the direction specified by
 * x_amount and y_amount.  However, it will only consider either the x or
 * y direction at a time, so if you want to move the player diagonallly
 * up and left, call the function twice, using the parameters (0, -1) to
 * move the player up one pixel, and then (-1, 0) to move the player left
 * one pixel.  The values x_amount and y_amound must be equal to or less
 * than one, except for one special case.  If you call push_player with
 * the parameters (512, -512), then it will check to see if the player can
 * drop down one pixel, which can happen only when the player is standing
 * on at least one solid from top info tile.                            */
void push_player(float x_amount, float y_amount);

/* This function will check to see if the player has touched a special
 * info tile, such as painful info tiles or ammo info tiles.  If so, it
 * will take appropriate action.  This function is automatically called
 * by the push player function, so you probally won't need to call it
 * yourself.                                                            */
void player_touched(int x_tile_location, int y_tile_location);

void control_player();
void trigger_switch();

/* This function will assign the player.frame attribute to an appropriate
 * value, based on what the player is doing at the time.                */
void pick_player_frame();

/* This function will actually do the rendering of the worminator.  You
*  should make sure that pick_player_frame() has been called before you
*  call this function to make sure that the correct frame will be drawn */
void draw_player();

/* This function will check to see if the player is touching any one of
*  the teleporter info tiles, and if so, will move the player to the
*  other corresponding teleport info tile.  This function is called when
*  ever the up key is pressed, so you probally won't need to call it.   */
char check_player_teleport();

/* This function will reset the player data structure to the default
 * values.  This function is automatically called by the load level
 * function, so you won't normally need to call it.                     */
void clear_player();

/* This function refreshes the display panel to reflect new values for
 * ammo, score, keys, etc...  You should call it every time any of these
 * values change.  However, don't call it if only the Worminator's health
 * has changed, as calling the function hurt_player with a parameter of 0
 * will refresh the health bar display.                                 */
void update_player_stats();

/* This function will add or subtract health from the worminator.  If you
 * call it with a positive parameter, it will add health, which is useful
 * if you want to add health packs or other items that heal the
 * Worminator.  Calling the function with a negative value will hurt the
 * Worminator, which is used when enemies attack the Worminator, and when
 * The worminator hits painful or lethal info tiles.  Calling this
 * function with a parameter of 0 will simply refresh the health bar
 * display.  Call it with a parameter of -10 to kill the worminator or
 * call it with a parameter of 10 to fully heal the Worminator.         */
void hurt_player(char amount_to_hurt);

/* This function will display the death animation, re-load the level, and
 * then return.  You should not normally call this.  Instead, call hurt
 * player with a parameter of -10 to kill the Worminator.               */
void kill_player();

//////////////////////////////////////////////////////////////////////////
// sprites.h /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void spawn_sprite(int sprite_type, int x_position_in_pixels, int y_position_in_pixels, float x_acceleration, float y_acceleration, unsigned char direction);
void destroy_sprite(int sprite_number);
void clear_sprites();
void update_sprites();
int sprite_is_touching_player(int sprite_number);
void do_damage(int x, int y, int w, int h, float damage_amount);
int sprite_is_touching_sprite(int sprite_number, char flag);
void check_sprite_turning_arrows(int sprite_number, char flag, float x_speed_to_set, float y_speed_to_set);
int check_point_collisions_grid(int sprite_number, int x, int y, char type_flag);
void spawn_shrapgibs(int x, int y, unsigned char number_of_shraps, unsigned char number_of_gibs);
void spawn_stardust(int x, int y, float x_speed, float y_speed, unsigned char starting_color, unsigned char final_color, unsigned char path_type, unsigned char number_of_stardust_particles);
char out_of_player_range(int sprite_number);
int compute_distance(int thing1, int thing2);
void poof_out(int x, int y, int frame);

// Ai routines in particle ai.h //////////////////////////////////////////////
void flyup_sprite_ai(int sprite_number);
void small_spark_ai(int sprite_number);
void water_bubble_ai(int sprite_number);
void water_splash_ai(int sprite_number);
void shrapgib_ai(int sprite_number);
void basic_spinning_sprite_ai(int sprite_number);
void stardust_particle_ai(int sprite_number);

// Ai routines in projectile ai.h //////////////////////////////////////////////
void worminator_bullet_ai(int sprite_number);
void worminator_rocket_ai(int sprite_number);
void worminator_bfg_plasma_ball_ai(int sprite_number);
void worminator_mine_ai(int sprite_number);
void enemy_plasma_bullet_ai(int sprite_number);
void enemy_laser_bolt_ai(int sprite_number);
void small_enemy_rocket_ai(int sprite_number);
void large_enemy_rocket_ai(int sprite_number);
void enemy_mortar_ai(int sprite_number);
void enemy_toxin_ai(int sprite_number);
void worminator_plasma_bullet_ai(int sprite_number);
void worminator_spikestar_ai(int sprite_number);
void worminator_micro_nuke_ai(int sprite_number);
void enemy_missile_ai(int sprite_number);
void firework_ai(int sprite_number);
void giant_rocket_ai(int sprite_number);

// Ai routines in enemy ai.h //////////////////////////////////////////////
void check_enemy_ranges();
char check_enemy_slopes(int sprite_number);
char check_enemy_turns(int sprite_number);
char check_enemy_slope_exclusions(int sprite_number);
void robotic_dog_enemy_ai(int sprite_number);
void small_gun_turret_ai(int sprite_number);
void mortar_gun_turret_ai(int sprite_number);
void big_gun_turret_ai(int sprite_number);
void flying_probe_enemy_ai(int sprite_number);
void giant_robot_enemy_ai(int sprite_number);
void mechanical_crusher_enemy_ai(int sprite_number);
void nuclear_worm_enemy_ai(int sprite_number);
void laser_rebel_worm_enemy_ai(int sprite_number);
void rocket_rebel_worm_enemy_ai(int sprite_number);
void rifle_rebel_worm_enemy_ai(int sprite_number);
void big_green_slime_blob_enemy_ai(int sprite_number);
void gumdrop_enemy_ai(int sprite_number);
void nuclear_bomb_enemy_ai(int sprite_number);
void w_1000_enemy_ai(int sprite_number);
void fireworm_segment_enemy_ai(int sprite_number);
void flame_jet_enemy_ai(int sprite_number);
void worm_cannon_enemy_ai(int sprite_number);
void wermaid_enemy_ai(int sprite_number);
void blue_slime_enemy_ai(int sprite_number);
void heavy_weapon_rebel_worm_enemy_ai(int sprite_number);
void thug_worm_enemy_ai(int sprite_number);
void spear_tapeworm_enemy_ai(int sprite_number);
void egg_tapeworm_enemy_ai(int sprite_number);
void boulder_enemy_ai(int sprite_number);
void stomping_foot_enemy_ai(int sprite_number);
void cigarette_butt_enemy_ai(int sprite_number);
void boss_enemy_ai(int sprite_number);
void punk_worm_enemy_ai(int sprite_number);

// Ai routines in civilian ai.h //////////////////////////////////////////////
void worm_girl_ai(int sprite_number);
void worm_boy_ai(int sprite_number);
void worm_stripper_one_ai(int sprite_number);
void worm_blonde_nude_one_ai(int sprite_number);

// Ai routines for item ai.h ////////////////////////////////////////////////
void getable_health_ai(int sprite_number);
void minigun_ai(int sprite_number);
void getable_bullets_ai(int sprite_number);
void shotgun_ai(int sprite_number);
void getable_shels_ai(int sprite_number);
void rocket_launcher_ai(int sprite_number);
void getable_rockets_ai(int sprite_number);
void laser_gun_ai(int sprite_number);
void getable_cells_ai(int sprite_number);
void flamethrower_ai(int sprite_number);
void getable_gasoline_ai(int sprite_number);
void mortar_cannon_ai(int sprite_number);
void getable_mortars_ai(int sprite_number);
void bfg_ai(int sprite_number);
void getable_plasma_ai(int sprite_number);
void minelayer_ai(int sprite_number);
void getable_mines_ai(int sprite_number);
void red_keycard_ai(int sprite_number);
void green_keycard_ai(int sprite_number);
void blue_keycard_ai(int sprite_number);
void letter_ai(int sprite_number);
void soda_sixpack_ai(int sprite_number);
void doughnuts_ai(int sprite_number);
void box_of_doughnuts_ai(int sprite_number);
void mud_beer_ai(int sprite_number);
void dirt_chips_ai(int sprite_number);
void mud_ice_cream_ai(int sprite_number);
void mini_mount_n_do_ai(int sprite_number);
void mud_pie_ai(int sprite_number);
void big_tasty_ball_o_dirt_ai(int sprite_number);

void getable_plasma_bullets_ai(int sprite_number);
void getable_explosive_shells_ai(int sprite_number);
void getable_micro_nuke_ai(int sprite_number);
void getable_walking_mines_ai(int sprite_number);
void chainsaw_ai(int sprite_number);
void railgun_ai(int sprite_number);
void getable_rails_ai(int sprite_number);
void starduster_ai(int sprite_number);
void getable_spikestars_ai(int sprite_number);
void tank_ai(int sprite_number);

// Ai routines in misc ai.h /////////////////////////////////////////////////
void floating_platform_ai(int sprite_number);
void small_explosion_ai(int sprite_number);
void medium_explosion_ai(int sprite_number);
void bfg_plasma_ball_explosion_ai(int sprite_number);
void small_explosive_barrel_ai(int sprite_number);
void small_explosive_nuclear_crate_ai(int sprite_number);
void mushroom_cloud_ai(int sprite_number);
void pushable_crate_ai(int sprite_number);
void pervert_message_ai(int sprite_number);
void grey_door_ai(int sprite_number);
void red_door_ai(int sprite_number);
void red_door_top_ai(int sprite_number);
void red_door_bottom_ai(int sprite_number);
void green_door_ai(int sprite_number);
void green_door_top_ai(int sprite_number);
void green_door_bottom_ai(int sprite_number);
void blue_door_ai(int sprite_number);
void water_drop_ai(int sprite_number);
void bored_sprite_ai(int sprite_number);
void exploding_car_ai(int sprite_number);
void palette_effect_ai(int sprite_number);

// Ai routines in spawn ai.h ////////////////////////////////////////////////
void spark_spawner_ai(int sprite_number);
void water_drop_spawner_ai(int sprite_number);
void nuclear_bomb_spawner_ai(int sprite_number);
void arcspark_ai(int sprite_number);
void cigarette_butt_spawner_ai(int sprite_number);
void firework_spawner_ai(int sprite_number);

// Ai routines in sound.h //////////////////////////////////////////////
void play_relative_sound(int sound_number, int sprite_number);

// Routines in menus.h //////////////////////////////////////////////
int tranx(int value);
int trany(int value);
char mouse_clicked_region(int x1, int y1, int x2, int y2);
char mouse_in_region(int x1, int y1, int x2, int y2);
char region_clicked(int x1, int y1, int x2, int y2);
char mouse_clicked_region_notran(int x1, int y1, int x2, int y2);
char mouse_in_region_notran(int x1, int y1, int x2, int y2);
char region_clicked_notran(int x1, int y1, int x2, int y2);
void wormy_menu();
void wormy_options_menu();
char start_new_custom_game();
char play_a_demo();
void record_a_demo();
char wormy_dummy_box(char dummy_question_patch_number, char show_mouse_pointer);
void display_snapshot(char snapshot_number, char show_mouse_pointer);
void display_story(char show_mouse_pointer);
void check_high_score();
void display_recap(char show_mouse_pointer);
void get_highscore_name(int position);
void dofinalescroll();
void draw_key(BITMAP *bmp, int x, int y, int key);
int get_new_key();
int wormy_controls_menu_1();
int wormy_controls_menu_2();
int wormy_controls_menu_3();
void wormy_controls_menu();
