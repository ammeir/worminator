/* -- Infodefs.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * This file is a list of all the different info tile layer attributes.  This
 * is merely to allow you to add and remove new attributes without re-writing
 * the entire engine. */

#define blank_info_tile                             0		// does not effect the player
#define solid_info_tile                             1		// blocks everything
#define solid_from_top_no_drop_info_tile            2		// solid from the top only, and the player can't drop through it
#define solid_from_top_allow_ammo_info_tile         3		// solid from the top only, but weapons can shoot through
#define solid_from_bottom_allow_ammo_info_tile      4		// solid from the bottom only, but weapons can shoot through
#define solid_from_right_allow_ammo_info_tile       5		// solid from the right only, but weapons can shoot through
#define solid_from_left_allow_ammo_info_tile        6		// solid from the left only, but weapons can shoot through
#define solid_from_top_no_ammo_info_tile            7		// solid from the top only, and weapons can't shoot through
#define solid_from_bottom_no_ammo_info_tile         8		// solid from the bottom only, and weapons can't shoot through
#define solid_from_right_no_ammo_info_tile          9		// solid from the right only, and weapons can't shoot through
#define solid_from_left_no_ammo_info_tile           10		// solid from the left only, and weapons can't shoot through
#define lethal_info_tile                            11		// kills anything that touches it
#define very_painful_info_tile                      12		// does lots of damage to anything that touches it
#define painful_info_tile                           13		// hurts anything that touches it
#define painful_from_top_info_tile                  14		// tile hurts worm if he hits them from above
#define painful_from_bottom_info_tile               15		// tile hurts worm if he hits them from below

#define up_turning_point_info_tile                  16		// path followers turn up here
#define down_turning_point_info_tile                17		// path followers turn down here
#define right_turning_point_info_tile               18		// path followers turn right here
#define left_turning_point_info_tile                19		// path followers turn left here
#define up_right_turning_point_info_tile            20		// path followers turn up and right here
#define down_left_turning_point_info_tile           21		// path followers turn down and left here
#define up_left_turning_point_info_tile             22		// path followers turn up and left here
#define down_right_turning_point_info_tile          23		// path followers turn down and right here
#define wait_16_info_tile                           24		// path followers wait 16 updates here
#define wait_32_info_tile                           25		// path followers wait 32 updates here
#define wait_64_info_tile                           26		// path followers wait 64 updates here
#define wait_128_info_tile                          27		// path followers wait 128 updates here
#define wait_256_info_tile                          28		// path followers wait 256 updates here
#define wait_512_info_tile                          29		// path followers wait 512 updates here
#define wait_1024_info_tile                         30		// path followers wait 1024 updates here
#define wait_2048_info_tile                         31		// path followers wait 2048 updates here

#define teleporter_1_info_tile                      32		// this will take you to the other teleporter 1
#define teleporter_2_info_tile                      33		// this will take you to the other teleporter 2
#define teleporter_3_info_tile                      34		// this will take you to the other teleporter 3
#define teleporter_4_info_tile                      35		// this will take you to the other teleporter 4
#define teleporter_5_info_tile                      36		// this will take you to the other teleporter 5
#define teleporter_6_info_tile                      37		// this will take you to the other teleporter 6
#define teleporter_7_info_tile                      38		// this will take you to the other teleporter 7
#define teleporter_8_info_tile                      39		// this will take you to the other teleporter 8
#define teleporter_9_info_tile                      40		// this will take you to the other teleporter 9
#define teleporter_10_info_tile                     41		// this will take you to the other teleporter 10
#define teleporter_11_info_tile                     42		// this will take you to the other teleporter 11
#define teleporter_12_info_tile                     43		// this will take you to the other teleporter 12
#define teleporter_13_info_tile                     44		// this will take you to the other teleporter 13
#define teleporter_14_info_tile                     45		// this will take you to the other teleporter 14
#define teleporter_15_info_tile                     46		// this will take you to the other teleporter 15
#define teleporter_16_info_tile                     47		// this will take you to the other teleporter 16

#define quick_teleporter_1_info_tile                48		// this will take you to the quick teleporter 1 destination
#define quick_teleporter_2_info_tile                49		// this will take you to the quick teleporter 2 destination
#define quick_teleporter_3_info_tile                50		// this will take you to the quick teleporter 3 destination
#define quick_teleporter_4_info_tile                51		// this will take you to the quick teleporter 4 destination
#define quick_teleporter_5_info_tile                52		// this will take you to the quick teleporter 5 destination
#define quick_teleporter_6_info_tile                53		// this will take you to the quick teleporter 6 destination
#define quick_teleporter_7_info_tile                54		// this will take you to the quick teleporter 7 destination
#define quick_teleporter_8_info_tile                55		// this will take you to the quick teleporter 8 destination
#define quick_teleporter_9_info_tile                56		// this will take you to the quick teleporter 9 destination
#define quick_teleporter_10_info_tile               57		// this will take you to the quick teleporter 10 destination
#define quick_teleporter_11_info_tile               58		// this will take you to the quick teleporter 11 destination
#define quick_teleporter_12_info_tile               59		// this will take you to the quick teleporter 12 destination
#define quick_teleporter_13_info_tile               60		// this will take you to the quick teleporter 13 destination
#define quick_teleporter_14_info_tile               61		// this will take you to the quick teleporter 14 destination
#define quick_teleporter_15_info_tile               62		// this will take you to the quick teleporter 15 destination
#define quick_teleporter_16_info_tile               63		// this will take you to the quick teleporter 16 destination

#define slope_up_left_to_right_45_info_tile         64		// 45 degree slope from left to right
#define slope_up_right_to_left_45_info_tile         65		// 45 degree slope from right to left
#define slope_up_left_to_right_225_left_info_tile   66		// 22.5 degree slope from left to right
#define slope_up_left_to_right_225_right_info_tile  67		// 22.5 degree slope from left to right
#define slope_up_right_to_left_225_left_info_tile   68		// 22.5 degree slope from right to left
#define slope_up_right_to_left_225_right_info_tile  69		// 22.5 degree slope from right to left
#define slope_filler_info_tile                      70		// Used to stop ammo and stuff
#define lose_control_info_tile                      71		// The player will lose control when this tile is hit
#define regain_control_info_tile                    72		// The player will regain control when this tile is hit
#define tank_solid_from_top_no_drop_info_tile            73	// Solid from top only when in a tank
#define tank_slope_up_left_to_right_45_info_tile         74	// 45 degree slope from left to right, only for tanks
#define tank_slope_up_right_to_left_45_info_tile         75	// 45 degree slope from right to left, only for tanks
#define tank_slope_up_left_to_right_225_left_info_tile   76	// 22.5 degree slope from left to right, only for tanks
#define tank_slope_up_left_to_right_225_right_info_tile  77	// 22.5 degree slope from left to right, only for tanks
#define tank_slope_up_right_to_left_225_left_info_tile   78	// 22.5 degree slope from right to left, only for tanks
#define tank_slope_up_right_to_left_225_right_info_tile  79	// 22.5 degree slope from right to left, only for tanks

#define breakable_wall_info_tile                    80		// this wall can be destroyed
#define block_enemy_info_tile                       81		// this will block all enemies
#define water_info_tile                             82		// this area is under water
#define conveyor_belt_right_info_tile               83		// this pushes the Worminator right
#define conveyor_belt_left_info_tile                84		// this pushes the Worminator left
#define enemy_helper_info_tile                      85		// this tile is just used as a space filler
#define half_slippery_info_tile                     86		// the Worminator will slide around a bit on this tile
#define mostly_slippery_info_tile                   87		// the Worminator will slide around a lot on this tile
#define turbo_launch_1_info_tile                    88		// this launches the Worminator upward lightly
#define turbo_launch_2_info_tile                    89		// this launches the Worminator upward moderately
#define turbo_launch_3_info_tile                    90		// this launches the Worminator upward strongly
#define turbo_launch_4_info_tile                    91		// this launches the Worminator upward with great force
#define tank_solid_info_tile                        95		// Solid only when in a tank

#define exit_to_level_1_info_tile                   96		// this changes the level to level 1 when the Worminator touches this
#define exit_to_level_2_info_tile                   97		// this changes the level to level 2 when the Worminator touches this
#define exit_to_level_3_info_tile                   98		// this changes the level to level 3 when the Worminator touches this
#define exit_to_level_4_info_tile                   99		// this changes the level to level 4 when the Worminator touches this
#define exit_to_level_5_info_tile                   100		// this changes the level to level 5 when the Worminator touches this
#define exit_to_level_6_info_tile                   101		// this changes the level to level 6 when the Worminator touches this
#define exit_to_level_7_info_tile                   102		// this changes the level to level 7 when the Worminator touches this
#define exit_to_level_8_info_tile                   103		// this changes the level to level 8 when the Worminator touches this
#define exit_to_level_9_info_tile                   104		// this changes the level to level 9 when the Worminator touches this
#define exit_to_level_10_info_tile                  105		// this changes the level to level 10 when the Worminator touches this
#define exit_to_level_11_info_tile                  106		// this changes the level to level 11 when the Worminator touches this
#define exit_to_level_12_info_tile                  107		// this changes the level to level 12 when the Worminator touches this
#define exit_to_level_13_info_tile                  108		// this changes the level to level 13 when the Worminator touches this
#define exit_to_level_14_info_tile                  109		// this changes the level to level 14 when the Worminator touches this
#define exit_to_level_15_info_tile                  110		// this changes the level to level 15 when the Worminator touches this
#define exit_to_level_16_info_tile                  111		// this changes the level to level 16 when the Worminator touches this

#define quick_exit_to_level_1_info_tile             112		// this changes the level to level 1 when the Worminator touches this
#define quick_exit_to_level_2_info_tile             113		// this changes the level to level 2 when the Worminator touches this
#define quick_exit_to_level_3_info_tile             114		// this changes the level to level 3 when the Worminator touches this
#define quick_exit_to_level_4_info_tile             115		// this changes the level to level 4 when the Worminator touches this
#define quick_exit_to_level_5_info_tile             116		// this changes the level to level 5 when the Worminator touches this
#define quick_exit_to_level_6_info_tile             117		// this changes the level to level 6 when the Worminator touches this
#define quick_exit_to_level_7_info_tile             118		// this changes the level to level 7 when the Worminator touches this
#define quick_exit_to_level_8_info_tile             119		// this changes the level to level 8 when the Worminator touches this
#define quick_exit_to_level_9_info_tile             120		// this changes the level to level 9 when the Worminator touches this
#define quick_exit_to_level_10_info_tile            121		// this changes the level to level 10 when the Worminator touches this
#define quick_exit_to_level_11_info_tile            122		// this changes the level to level 11 when the Worminator touches this
#define quick_exit_to_level_12_info_tile            123		// this changes the level to level 12 when the Worminator touches this
#define quick_exit_to_level_13_info_tile            124		// this changes the level to level 13 when the Worminator touches this
#define quick_exit_to_level_14_info_tile            125		// this changes the level to level 14 when the Worminator touches this
#define quick_exit_to_level_15_info_tile            126		// this changes the level to level 15 when the Worminator touches this
#define quick_exit_to_level_16_info_tile            127		// this changes the level to level 16 when the Worminator touches this

#define quick_teleporter_exit_1_info_tile           128		// this is where the Worminator will come out of quick teleporter 1
#define quick_teleporter_exit_2_info_tile           129		// this is where the Worminator will come out of quick teleporter 2
#define quick_teleporter_exit_3_info_tile           130		// this is where the Worminator will come out of quick teleporter 3
#define quick_teleporter_exit_4_info_tile           131		// this is where the Worminator will come out of quick teleporter 4
#define quick_teleporter_exit_5_info_tile           132		// this is where the Worminator will come out of quick teleporter 5
#define quick_teleporter_exit_6_info_tile           133		// this is where the Worminator will come out of quick teleporter 6
#define quick_teleporter_exit_7_info_tile           134		// this is where the Worminator will come out of quick teleporter 7
#define quick_teleporter_exit_8_info_tile           135		// this is where the Worminator will come out of quick teleporter 8
#define quick_teleporter_exit_9_info_tile           136		// this is where the Worminator will come out of quick teleporter 9
#define quick_teleporter_exit_10_info_tile          137		// this is where the Worminator will come out of quick teleporter 10
#define quick_teleporter_exit_11_info_tile          138		// this is where the Worminator will come out of quick teleporter 11
#define quick_teleporter_exit_12_info_tile          139		// this is where the Worminator will come out of quick teleporter 12
#define quick_teleporter_exit_13_info_tile          140		// this is where the Worminator will come out of quick teleporter 13
#define quick_teleporter_exit_14_info_tile          141		// this is where the Worminator will come out of quick teleporter 14
#define quick_teleporter_exit_15_info_tile          142		// this is where the Worminator will come out of quick teleporter 15
#define quick_teleporter_exit_16_info_tile          143		// this is where the Worminator will come out of quick teleporter 16

#define switch_1_info_tile                          144		// this switch activates all trigger 1s
#define switch_2_info_tile                          145		// this switch activates all trigger 2s
#define switch_3_info_tile                          146		// this switch activates all trigger 3s
#define switch_4_info_tile                          147		// this switch activates all trigger 4s
#define switch_5_info_tile                          148		// this switch activates all trigger 5s
#define switch_6_info_tile                          149		// this switch activates all trigger 6s
#define switch_7_info_tile                          150		// this switch activates all trigger 7s
#define switch_8_info_tile                          151		// this switch activates all trigger 8s
#define switch_9_info_tile                          152		// this switch activates all trigger 9s
#define switch_10_info_tile                         153		// this switch activates all trigger 10s
#define switch_11_info_tile                         154		// this switch activates all trigger 11s
#define switch_12_info_tile                         155		// this switch activates all trigger 12s
#define switch_13_info_tile                         156		// this switch activates all trigger 13s
#define switch_14_info_tile                         157		// this switch activates all trigger 14s
#define switch_15_info_tile                         158		// this switch activates all trigger 15s
#define switch_16_info_tile                         159		// this switch activates all trigger 16s

#define trigger_1_info_tile                         160		// this will be activated by switch 1
#define trigger_2_info_tile                         161		// this will be activated by switch 2
#define trigger_3_info_tile                         162		// this will be activated by switch 3
#define trigger_4_info_tile                         163		// this will be activated by switch 4
#define trigger_5_info_tile                         164		// this will be activated by switch 5
#define trigger_6_info_tile                         165		// this will be activated by switch 6
#define trigger_7_info_tile                         166		// this will be activated by switch 7
#define trigger_8_info_tile                         167		// this will be activated by switch 8
#define trigger_9_info_tile                         168		// this will be activated by switch 9
#define trigger_10_info_tile                        169		// this will be activated by switch 10
#define trigger_11_info_tile                        170		// this will be activated by switch 11
#define trigger_12_info_tile                        171		// this will be activated by switch 12
#define trigger_13_info_tile                        172		// this will be activated by switch 13
#define trigger_14_info_tile                        173		// this will be activated by switch 14
#define trigger_15_info_tile                        174		// this will be activated by switch 15
#define trigger_16_info_tile                        175		// this will be activated by switch 16

#define exit_to_level_17_info_tile                  176		// this changes the level to level 17 when the Worminator touches this
#define exit_to_level_18_info_tile                  177		// this changes the level to level 18 when the Worminator touches this
#define exit_to_level_19_info_tile                  178		// this changes the level to level 19 when the Worminator touches this
#define exit_to_level_20_info_tile                  179		// this changes the level to level 20 when the Worminator touches this
#define exit_to_level_21_info_tile                  180		// this changes the level to level 21 when the Worminator touches this
#define exit_to_level_22_info_tile                  181		// this changes the level to level 22 when the Worminator touches this
#define exit_to_level_23_info_tile                  182		// this changes the level to level 23 when the Worminator touches this
#define exit_to_level_24_info_tile                  183		// this changes the level to level 24 when the Worminator touches this
#define quick_exit_to_level_17_info_tile            184		// this changes the level to level 17 when the Worminator touches this
#define quick_exit_to_level_18_info_tile            185		// this changes the level to level 18 when the Worminator touches this
#define quick_exit_to_level_19_info_tile            186		// this changes the level to level 19 when the Worminator touches this
#define quick_exit_to_level_20_info_tile            187		// this changes the level to level 20 when the Worminator touches this
#define quick_exit_to_level_21_info_tile            188		// this changes the level to level 21 when the Worminator touches this
#define quick_exit_to_level_22_info_tile            189		// this changes the level to level 22 when the Worminator touches this
#define quick_exit_to_level_23_info_tile            190		// this changes the level to level 23 when the Worminator touches this
#define quick_exit_to_level_24_info_tile            191		// this changes the level to level 24 when the Worminator touches this


#define marker_info_tile                            255	// this is used for script purposes only
