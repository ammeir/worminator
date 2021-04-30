
#ifndef PSVITA_H
#define PSVITA_H

#include "debug.h"
#include "ime_dialog.h"
#include <psp2/types.h>

#define PSV_APP_DIR			          "ux0:data/worminator/"
#define PSV_DATA_DIR PSV_APP_DIR      "data/"
#define PSV_SAVE_DIR PSV_APP_DIR	  "save/"
#define DATADIR PSV_DATA_DIR


int     psvInit();
int		psvInputDialog(const char* title, const char* initial_text, char* ret_buf);
int		psvChangeClockSpeed(int speed);

#endif