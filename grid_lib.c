#include <string.h>
#include <curses.h> 	

#include "util.h"

/*
 * Function responsible for building and displaying the grid.
 */
void make_grid(char* username) {
	int i, j, length, prev, after;
	char letter = 'A';

	clear();
	move(1, 15);
	attron(A_STANDOUT);
	attron(COLOR_PAIR(1));
	printw("Ships Deployment\n");	
	attroff(COLOR_PAIR(1));
	attroff(A_STANDOUT);
	refresh();	

	attron(A_BOLD);
	mvprintw(9, 10, "--------RIVAL-------");
	move(11, 10);
	for(i=1;i<=10;i++)
		printw("%d ", i);

	for(i=0;i<10;i++) {
		move(13+i, 6);
		printw("%c",  letter++);
	}	
	move(13, 10);
	for(i=0; i<10; i++) {
		for(j=0; j<10; j++)
			printw("%c ", '^');	
		printw("\n");
		move(14+i, 10);
	}
	attroff(A_BOLD);	
	refresh();	

	attron(A_BOLD);	
	length = strlen(username);
	if( !(length % 2) ) {
		prev = after = (20 - length) / 2;
	}
	else {	
		prev = (20 - length) / 2;
		after = prev + 1;
	}
	for(i=0; i<prev; i++)		
		mvprintw(25, 10+i, "-");
	printw("%s",  username);
	for(i=0; i<after; i++)		
		printw("-");

	move(27, 10);
	for(i=1;i<=10;i++)
		printw("%d ", i);

	letter = 'A';
	for(i=0;i<10;i++) {
		move(29+i, 6);
		printw("%c",  letter++);
	}
		
	move(29, 10);
	for(i=0; i<10; i++) {
		for(j=0; j<10; j++)
			printw("%c ", '^');	
		printw("\n");
		move(30+i, 10);
	}
	attroff(A_BOLD);	
	refresh();	
}

/*
 * When the game is over this function is invoked and the
 * logo "end of game" is depicted.
 */
void end_of_game_logo(void) {
	int rows, cols;

	getmaxyx(stdscr,  rows,  cols);
	clear();

	attron(A_BOLD);
	attron(COLOR_PAIR(4));
	mvprintw(rows/2 -10, cols/2 -51,  "                        #                      ####                                               ");
	refresh();
	usleep(200000);
	mvprintw(rows/2 -9,  cols/2 -51,  "#######                 #             ###     #                  ####                             ");
	refresh();
	usleep(200000);
	mvprintw(rows/2 -8,  cols/2 -51,  "#                       #            #   #    #                 #    #                            ");
	refresh();
	usleep(200000);
	mvprintw(rows/2 -7,  cols/2 -51,  "#        # ####    ######           #     #  ####              #         ######  ### ##    #####  ");
	refresh();
	usleep(200000);
	mvprintw(rows/2 -6,  cols/2 -51,  "#####    ##    #  #     #           #     #   #                #   ###  #     #  #  #  #  #     # ");
	refresh();
	usleep(200000);
	mvprintw(rows/2 -5,  cols/2 -51,  "#        #     #  #     #           #     #   #                #     #  #     #  #  #  #  ####### ");
	refresh();
	usleep(200000);
	mvprintw(rows/2 -4,  cols/2 -51,  "#        #     #  #     #            #   #    #                 #    #  #    ##  #  #  #  #       ");
	refresh();
	usleep(200000);
	mvprintw(rows/2 -3,  cols/2 -51,  "#######  #     #   ######             ###     #                  #####   #### #  #     #   #####  ");
	refresh();
	attroff(COLOR_PAIR(4));
	attroff(A_BOLD);
}

/*
 * Function displaying "you won / you lost". It firstly calls end_of_game and then
 * based on the outcome of the game it prints the corresponding message.
 */
void end_of_game(int result) {
	int rows, cols;
	
	getmaxyx(stdscr,  rows,  cols);
	end_of_game_logo();

	if( result ) {							//you lost colored in red
		attron(COLOR_PAIR(1));
		mvprintw(rows/2+3, cols/2 -42, " __  __     ______     __  __        __         ______     ______     ______  ");
		mvprintw(rows/2+4, cols/2 -42, "/\\ \\_\\ \\   /\\  __ \\   /\\ \\/\\ \\      /\\ \\       /\\  __ \\   /\\  ___\\   /\\__  _\\ ");
		mvprintw(rows/2+5, cols/2 -42, "\\ \\____ \\  \\ \\ \\/\\ \\  \\ \\ \\_\\ \\     \\ \\ \\____  \\ \\ \\/\\ \\  \\ \\___  \\  \\/_/\\ \\/ ");
		mvprintw(rows/2+6, cols/2 -42, " \\/ _____\\  \\ \\_____\\  \\ \\_____\\     \\ \\_____\\  \\ \\_____\\  \\/ _____\\    \\ \\_\\ ");
		mvprintw(rows/2+7, cols/2 -42, "   /_____/   \\/_____/   \\/_____/      \\/_____/   \\/_____/    /_____/     \\/_/ ");		
		attroff(COLOR_PAIR(1));
	}
	else {									//you won colored in green
		attron(COLOR_PAIR(2));
        mvprintw(rows/2+3,  cols/2 -40, " __  __     ______     __  __        __     __     ______     __   __");
        mvprintw(rows/2+4,  cols/2 -40, "/\\ \\_\\ \\   /\\  __ \\   /\\ \\/\\ \\      /\\ \\  _ \\ \\   /\\  __ \\   /\\ \"-.\\ \\");
        mvprintw(rows/2+5,  cols/2 -40, "\\ \\____ \\  \\ \\ \\/\\ \\  \\ \\ \\_\\ \\     \\ \\ \\/ \".\\ \\  \\ \\ \\/\\ \\  \\ \\ \\-.  \\");
        mvprintw(rows/2+6,  cols/2 -40, " \\/ _____\\  \\ \\_____\\  \\ \\_____\\     \\ \\__/\".~\\_\\  \\ \\_____\\  \\ \\_\\\\\"\\_\\");
        mvprintw(rows/2+7,  cols/2 -40, "   /_____/   \\/_____/   \\/_____/      \\/_/   \\/_/   \\/_____/   \\/_/ \\/_/");
		attroff(COLOR_PAIR(2));
	}
}

/*
 * Prints a ship, a submarine and the battleship logo (Ascii art). User is prompted
 * to enter his name.
 */
void start_game(char** name) {
	int rows, cols;

	getmaxyx(stdscr,  rows,  cols);
	clear();

	attron(COLOR_PAIR(2));
	mvprintw(rows/2 -20, cols/2 -37,  "                                     # #  ( )");
	mvprintw(rows/2 -19, cols/2 -37,  "                                  ___#_#___|__");
	mvprintw(rows/2 -18, cols/2 -37,  "                              _  |____________|  _");
	mvprintw(rows/2 -17, cols/2 -37,  "                       _=====| | |            | | |==== _");
	mvprintw(rows/2 -16, cols/2 -37,  "                 =====| |.---------------------------. | |====");
	mvprintw(rows/2 -15, cols/2 -37,  "   <--------------------'   .  .  .  .  .  .  .  .   '--------------/");
	mvprintw(rows/2 -14, cols/2 -37,  "     \\                                                            /");
	mvprintw(rows/2 -13, cols/2 -37,  "      \\___________________________________________________________/");
	mvprintw(rows/2 -12, cols/2 -37,  "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
	mvprintw(rows/2 -11, cols/2 -37,  "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww");
	mvprintw(rows/2 -10, cols/2 -37,  "   wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww ");
	attroff(COLOR_PAIR(2));

	attron(A_BOLD);
	attron(COLOR_PAIR(1));
	mvprintw(rows/2 -5, cols/2 -28,  " ____    __   ____  ____  __    ____  ___  _   _  ____  ____");
	mvprintw(rows/2 -4,  cols/2 -28,  "(  _ \\  /__\\ (_  _)(_  _)(  )  ( ___)/ __)( )_( )(_  _)(  _ \\");
	mvprintw(rows/2 -3,  cols/2 -28,  " ) _ < /(__)\\  )(    )(   )(__  )__) \\__ \\ ) _ (  _)(_  )___/");
	mvprintw(rows/2 -2,  cols/2 -28,  "(____/(__)(__)(__)  (__) (____)(____)(___/(_) (_)(____)(__)  ");
	attroff(COLOR_PAIR(1));

	attron(COLOR_PAIR(7));
	mvprintw(rows/2 +2, cols/2 -10,  "Enter your name: ");
	attroff(COLOR_PAIR(7));
	attroff(A_BOLD);

	attron(COLOR_PAIR(6));
	mvprintw(rows/2 +8, cols/2 -28,  "                           |`-:_");
	mvprintw(rows/2 +9, cols/2 -28,  "  , ----....____            |    `+.");
	mvprintw(rows/2 +10, cols/2 -28,  " (             ````----....|___   |");
	mvprintw(rows/2 +11, cols/2 -28,  "  \\     _                      ````----....____");
	mvprintw(rows/2 +12, cols/2 -28,  "   \\    _)                                     ```---.._");
	mvprintw(rows/2 +13, cols/2 -28,  "    \\                                                   \\ ");
	mvprintw(rows/2 +14, cols/2 -28,  "  )`.\\  )`.   )`.   )`.   )`.   )`.   )`.   )`.   )`.   )`.   )");
	mvprintw(rows/2 +15, cols/2 -28,  "-'   `-'   `-'   `-'   `-'   `-'   `-'   `-'   `-'   `-'   `-' ");
	attroff(COLOR_PAIR(6));
	refresh();
	move(rows/2 +2, cols/2 + 8);
	getstr(*name);
}

/*
 * Function taking care of grid updates based on
 * user's and rival's attempts.
 */
int update_grid(int table, point target, int result) {
    //turn on bold font weight
	attron(A_BOLD);

    //rival's table
	if(!table) {

        //user made a succesfull choice
		if( !result ) {
			attron(COLOR_PAIR(1));
			mvprintw(13 + target.x, 10 + 2 * target.y, "%c ", 'x');
			attroff(COLOR_PAIR(1));
		}

        //user hit nothing
		else {
			attron(COLOR_PAIR(2));
			mvprintw( 13 + target.x, 10 + 2 * target.y, "%c ", '+');
			attroff(COLOR_PAIR(2));
		}
	}

    //user's table
	else {

        //rival hit a user's ship
		if(!result) {
			attron(COLOR_PAIR(1));																						
			mvprintw(29 + target.x, 10 + 2 * target.y, "%c ", 'x');
			attroff(COLOR_PAIR(1));																						
		}

        //rival damaged no user's ship
		else {
			mvprintw(29 + target.x, 10 + 2 * target.y, "%c ", '+');
        }
	}

    //turn bold off
	attroff(A_BOLD);

    //refresh grid so that modifications can be spotted
	refresh();
	
	return 0;
}

