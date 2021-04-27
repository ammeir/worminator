/* -- Text.h --
 * Part of the Worminator 2D game engine
 * Copyright 2002 by Electroencephlogram Software
 *
 * For function descriptions and explanations, see proto.h
 * For a detailed explanation of how the engine works, see
 * the documents that came with this source code package. */

/**********************\
|** ADD TEXT MESSAGE **|
\**********************/
void add_text_message(char *message)
{
	char empty_finder;

	// Echo the message to the console
	add_console_line(message);

	for (empty_finder = 0; empty_finder < MAX_TEXT_MESSAGES; empty_finder++) {
		if (text_messages[empty_finder].timer == -1) {
			text_messages[empty_finder].timer = TEXT_MESSAGE_TIME;
			text_messages[empty_finder].message = message;
			return;
		}
	}

	// No slots were free, so kill the weakest message and add the new one at the end
	slide_messages(0);
	text_messages[MAX_TEXT_MESSAGES - 1].timer = TEXT_MESSAGE_TIME;
	text_messages[MAX_TEXT_MESSAGES - 1].message = message;
}

/*****************************\
|** ADD TEXT MESSAGE IF NEW **|
\*****************************/
void add_text_message_if_new(char *message)
{
	char empty_finder;

	// Bail out if the message already exists
	for (empty_finder = 0; empty_finder < MAX_TEXT_MESSAGES; empty_finder++)
		if (text_messages[empty_finder].message == message) return;

	// Echo the message to the console
	add_console_line(message);

	for (empty_finder = 0; empty_finder < MAX_TEXT_MESSAGES; empty_finder++) {
		if (text_messages[empty_finder].timer == -1) {
			text_messages[empty_finder].timer = TEXT_MESSAGE_TIME;
			text_messages[empty_finder].message = message;
			return;
		}
	}

	// No slots were free, so kill the weakest message and add the new one at the end
	slide_messages(0);
	text_messages[MAX_TEXT_MESSAGES - 1].timer = TEXT_MESSAGE_TIME;
	text_messages[MAX_TEXT_MESSAGES - 1].message = message;
}

/**************************\
|** UPDATE TEXT MESSAGES **|
\**************************/
void update_text_messages()
{
	char message_loop;

	for (message_loop = 0; message_loop < MAX_TEXT_MESSAGES; message_loop++) {
		if (text_messages[message_loop].timer != -1) {
			if (text_messages[message_loop].timer == 0) slide_messages(message_loop);
			else text_messages[message_loop].timer--;
		}
	}
}

/********************\
|** SLIDE MESSAGES **|
\********************/
void slide_messages(char message_number)
{
	char message_loop;

	if (message_number == MAX_TEXT_MESSAGES - 1) {
		text_messages[MAX_TEXT_MESSAGES - 1].timer = -1;
		text_messages[MAX_TEXT_MESSAGES - 1].message = 0;
		return;
	}

	for (message_loop = message_number; message_loop < MAX_TEXT_MESSAGES - 1; message_loop++) {
		text_messages[message_loop].timer = text_messages[message_loop + 1].timer;
		text_messages[message_loop].message = text_messages[message_loop + 1].message;
	}

	text_messages[MAX_TEXT_MESSAGES - 1].timer = -1;
	text_messages[MAX_TEXT_MESSAGES - 1].message = 0;
}


// Worm console code below

/*
 * Official list of worminator console commands:
 *
 * clear(); -> clear console
 * exit(); -> quit game
 * map(int n); -> load map #n
 * map("mapname"); -> load map mapname
 *
 * Ingame functions:
 * spawnSprite(int type, int x, int y, float x_vel, float y_vel, char direction);
 *
 * Toggle commands:
 * god -> toggle godmode on/off
 * wealth -> toggle wealth mode on/off
 *
 *
 *
 * stuff to add:
 * play demo
 * record demo
 * clear scores
 *
 *
 *
 */

// This function returns the first character of a string
char stringPeek(char *string)
{
	//if (strlen(string) == 0) return 0;
	return string[0];
}

// This function removes the first character of a string
void stringPop(char *string)
{
	int length = strlen(string);

	strncpy(string, string + 1, length - 1);
	string[length - 1] = 0;
}

// This function inserts a character onto the back of a string
void stringPush(char *string, const char character)
{
	unsigned int offset = strlen(string);

	string[offset] = character;
	string[offset + 1] = 0;
}

int getToken(char *source, char *token)
{
	// Prepare the token
	token[0] = 0;	// Null terminator to indicate empty string

	// Scan out whitespace
	while (stringPeek(source) == ' ' || stringPeek(source) == '\n' || stringPeek(source) == '\t') stringPop(source);

	// Examine the first character
	switch (stringPeek(source)) {
	case 0:	// End of input
		return 0;
		break;
	case '(':	// Open parenthesis token
		stringPush(token, '(');
		stringPop(source);
		return 1;
		break;
	case ')':	// Close parenthesis token
		stringPush(token, ')');
		stringPop(source);
		return 1;
		break;
	case ';':	// Semicolon token
		stringPush(token, ';');
		stringPop(source);
		return 1;
		break;
	case '=':	// Equals sign token
		stringPush(token, '=');
		stringPop(source);
		return 1;
		break;
	case ',':	// Comma token
		stringPush(token, ',');
		stringPop(source);
		return 1;
		break;
	case '"':	// Quotation mark, start of literal string
		stringPop(source);
		while (stringPeek(source) != '"' && stringPeek(source) != 0) {
			stringPush(token, stringPeek(source));
			stringPop(source);
		}
		if (stringPeek(source) == '"') stringPop(source);
		else return 0;
		return 1;
		break;
	}

	// See if the input can be classified as a name
	if ((stringPeek(source) >= 'a' && stringPeek(source) <= 'z') || (stringPeek(source) >= 'A' && stringPeek(source) <= 'Z') || stringPeek(source) == '_') {
		while ((stringPeek(source) >= 'a' && stringPeek(source) <= 'z') || (stringPeek(source) >= 'A' && stringPeek(source) <= 'Z') || (stringPeek(source) >= '0' && stringPeek(source) <= '9') || stringPeek(source) == '_') {
			stringPush(token, stringPeek(source));
			stringPop(source);
		}
		return 1;
	}

	// See if the input can be classified as a number
	if (stringPeek(source) >= '0' && stringPeek(source) <= '9') {
		while (stringPeek(source) >= '0' && stringPeek(source) <= '9') {
			stringPush(token, stringPeek(source));
			stringPop(source);
		}
		if (stringPeek(source) == '.') {
			stringPush(token, stringPeek(source));
			stringPop(source);
		}
		while (stringPeek(source) >= '0' && stringPeek(source) <= '9') {
			stringPush(token, stringPeek(source));
			stringPop(source);
		}
		return 1;
	}

	return 0;
}

void parseError(char *msg)
{
	add_console_line("ERROR:  Parse error.");
	if (msg != NULL) add_console_line(msg);
}

void consoleExec(char *input)
{
	char token[MAX_CONSOLE_LINE_LENGTH];		// The current token
	char paramBuf[MAX_CONSOLE_LINE_LENGTH];		// Buffer to hold temporary text
	char buffers[64][MAX_CONSOLE_LINE_LENGTH];	// Buffers to hold function parameters and other data

	if (getToken(input, token) == 0) return;

	if (strcmp(token, "clear") == 0) {	// Clear function
		getToken(input, token);
		if (strcmp(token, "(") == 0) {
			getToken(input, token);
			if (strcmp(token, ")") == 0) {
				getToken(input, token);
				if (strcmp(token, ";") == 0) clear_console();
				else add_console_line("ERROR: Parse error.  Correct usage: clear();");
			} else {
				add_console_line("ERROR: Parse error.  Correct usage: clear();");
			}
		} else {
			add_console_line("ERROR: Parse error.  Correct usage: clear();");
		}
	}
	// Finish building the checks into this function
	else if (strcmp(token, "spawnSprite") == 0) {
		getToken(input, token);
		if (strcmp(token, "(") == 0) {
			getToken(input, &buffers[0][0]);			// Read sprite type
			if (atoi(&buffers[0][0]) >= 0 && atoi(&buffers[0][0]) <= 1024) {
				getToken(input, token);				// Skip comma
				if (strcmp(token, ",") == 0) {
					getToken(input, &buffers[1][0]);	// Read sprite x position
					getToken(input, token);			// Skip comma
					getToken(input, &buffers[2][0]);	// Read sprite y position
					getToken(input, token);			// Skip comma
					getToken(input, &buffers[3][0]);	// Read sprite x velocity
					getToken(input, token);			// Skip comma
					getToken(input, &buffers[4][0]);	// Read sprite y velocity
					getToken(input, token);			// Skip comma
					getToken(input, &buffers[5][0]);	// Read sprite direction
					getToken(input, token);			// Read parenthesis
					getToken(input, token);			// Read semicolon
					spawn_sprite(atoi(&buffers[0][0]), atoi(&buffers[1][0]), atoi(&buffers[2][0]), atof(&buffers[3][0]), atof(&buffers[4][0]), atoi(&buffers[5][0]));
				} else {
					add_console_line("ERROR: Parse error.  Correct usage: spawnSprite(int sprite_type, int x, int y, float x_vel, float y_vel, unsigned char direction);");
				}
			} else {
				add_console_line("ERROR: Parse error.  Correct usage: spawnSprite(int sprite_type, int x, int y, float x_vel, float y_vel, unsigned char direction);");
			}
		} else {
			add_console_line("ERROR: Parse error.  Correct usage: spawnSprite(int sprite_type, int x, int y, float x_vel, float y_vel, unsigned char direction);");
		}
	} else if (strcmp(token, "exit") == 0) {// Exit function
		getToken(input, token);
		if (strcmp(token, "(") == 0) {
			getToken(input, token);
			if (strcmp(token, ")") == 0) {
				getToken(input, token);
				if (strcmp(token, ";") == 0) time_to_quit = TRUE;
				else add_console_line("ERROR: Parse error.  Correct usage: exit();");
			} else {
				add_console_line("ERROR: Parse error.  Correct usage: exit();");
			}
		} else {
			add_console_line("ERROR: Parse error.  Correct usage: exit();");
		}
	} else if (strcmp(token, "map") == 0) {	// Map function
		getToken(input, token);
		if (strcmp(token, "(") == 0) {
			getToken(input, paramBuf);
			getToken(input, token);
			if (strcmp(token, ")") == 0) {
				getToken(input, token);
				if (strcmp(token, ";") == 0) {
					// Input was syntactically correct
					if (exists(paramBuf) != 0)
						load_map(-1, paramBuf, 0);
					else if (atoi(paramBuf) >= 0 && atoi(paramBuf) <= 24)
						load_map(atoi(paramBuf), NULL, 0);
					else add_console_line("ERROR: Invalid map name.  Correct usage: map(filename or #1-24);");
				} else {
					add_console_line("ERROR: Parse error.  Correct usage: map(filename or #1-24);");
				}
			} else {
				add_console_line("ERROR: Parse error.  Correct usage: map(filename or #1-24);");
			}
		} else {
			add_console_line("ERROR: Parse error.  Correct usage: map(filename or #1-24);");
		}
	} else if (strcmp(token, "god") == 0) {	// God mode toggle
		if (wormy_config.godly_mode) {
			wormy_config.godly_mode = FALSE;
			add_console_line("--> God mode off.");
		} else {
			wormy_config.godly_mode = TRUE;
			add_console_line("--> God mode on.");
		}
	} else if (strcmp(token, "wealth") == 0) {	// Wealth mode toggle
		if (wormy_config.wealth_mode) {
			wormy_config.wealth_mode = FALSE;
			add_console_line("--> Wealth mode off.");
		} else {
			wormy_config.wealth_mode = TRUE;
			add_console_line("--> Wealth mode on.");
		}
	}
}

int getConsoleInput()
{
	char keyval;
	int keyscan;

	if (keypressed()) {
		keyscan = readkey();
		keyval = (char)(keyscan & 0xff);						// Grab input character

		if (keyscan >> 8 == KEY_ENTER) {						// Call exec here
			add_console_line(inputString);						// Add input to console
			consoleExec(inputString);						// Execute the input string
			inputString[0] = 0;							// Reset the input
		} else if (keyscan >> 8 == KEY_BACKSPACE) {
			if (inputString[0] != 0) inputString[strlen(inputString) - 1] = 0;	// Backspace
		} else if (keyval >= 32 && keyval < 127) {
			stringPush(inputString, keyval);					// Add the new character to the input string
		}
	}

	//	add_text_message(inputString);

	return 0;
}

void clear_console()
{
	int i, j;

	for (i = 0; i < MAX_CONSOLE_LINES; i++) for (j = 0; j < MAX_CONSOLE_LINE_LENGTH; j++) consoleText[i][j] = 0;

	add_console_line("Console cleared.");
}

#ifndef ALLEGRO_WINDOWS
//char log_txt[] = "/tmp/log.txt";
char log_txt[] = "ux0:data/trace.log";
#else
char log_txt[] = "./log.txt";
#endif

// logs the text to the text file
void log2file(char *format, ...) {
	va_list ptr; /* get an arg pointer */
	FILE *fp;
	static int flag = 0;

	fp = fopen(log_txt, (!flag ? "wt" : "at"));
	if (fp) {
		/* initialize ptr to point to the first argument after the format string */
		va_start(ptr, format);

		/* Write to logfile. */
		vfprintf(fp, format, ptr); // Write passed text.
		fprintf(fp, "\n"); // New line..

		va_end(ptr);

		fclose(fp);
		flag = 1;
	}
}

void add_console_line(char *string)
{
	int i;

#if defined (PSVITA) && !defined (PSV_DEBUG_CODE) 
	// Log only in Debug version.
	return;
#endif

	// We might get passed an empty string
	if (!string)
		return;

	// Copy to log file
	log2file(string);

	// Search for empty space to add
	for (i = 0; i < MAX_CONSOLE_LINES; i++) if (consoleText[i][0] == 0) {	// Find the first free line
			strcpy(&consoleText[i][0], string);
			return;
		}

	// Add failed, we gotta shift everything down
	for (i = 0; i < MAX_CONSOLE_LINES - 1; i++) strcpy(&consoleText[i][0], &consoleText[i + 1][0]);
	strcpy(&consoleText[MAX_CONSOLE_LINES - 1][0], string);
}

void draw_console(BITMAP *bmp, int position)
{
	BITMAP *consoleBmp;
	int i, j;

	if (console_position == -1) return;	// Console is hidden

	// Create and clear the console bitmap
	consoleBmp = create_bitmap_ex(8, screen_width, screen_height / 2);
	rectfill(consoleBmp, 0, 0, screen_width, screen_height / 2, makecol(0, 0, 0));

	// Find the first free line and draw up from there
	for (i = 0; i < MAX_CONSOLE_LINES; i++) if (consoleText[i][0] == 0) break;

	j = (screen_height / 2) - text_height(font);
	textout(consoleBmp, font, ">", 8, j, makecol(255, 255, 255));
	textout(consoleBmp, font, &inputString[0], 8 + text_length(font, ">"), j, makecol(0, 255, 0));
	j -= 6;
	rectfill(consoleBmp, 10, j + 2, screen_width - 10, j + 4, makecol(255, 255, 255));
	j -= text_height(font);
	while (j > 0 && i >= 0) {
		textout(consoleBmp, font, &consoleText[i][0], 8, j, makecol(255, 0, 0));
		j -= text_height(font);
		i--;
	}

	blit(consoleBmp, bmp, 0, 0, 0, position - (screen_height / 2), screen_width, screen_height);

	destroy_bitmap(consoleBmp);
}
