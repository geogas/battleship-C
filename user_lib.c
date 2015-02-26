#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h> 	
#include <ctype.h>

#include "util.h"
#include "user_lib.h"

/*
 * Function responsible for the ship deployment of the user. User specifies the desired
 * coordinates in stdin. The function validates them. In case no problem exists grid
 * is updated. Otherwise, user is again prompted to specify the coordinates for this ship. 
 */
int user_find_coordinates(char*** tablept, char* sname, int slength, int row, int colour) {
	int num=1, k, tempx, tempy, temp;
	char inbuf1[SIZE];
	char inbuf2[SIZE];
	point start, end;
	
    //do while loop entitled to check ship coordinates as given by the user
	do {
		memset(inbuf1, 0, SIZE);
		memset(inbuf2, 0, SIZE);
		if(num == 1) {
			attron(A_BOLD);
			mvprintw(row, 0, "Choose coordinates for");
            //every ship is reflected by its first letter in the
            //grid; every ship is assigned to a unique color 
			attron(COLOR_PAIR(colour));
			mvprintw(row, 23, "%s", sname);	
			attroff(COLOR_PAIR(colour));
			attroff(A_BOLD);
			mvprintw(row, 23 + 16, " [%d] -> Start: ", slength);
			getstr(inbuf1);
			mvprintw(row, 57,  "- End: ");
			getstr(inbuf2);
		}
        //illegal coordinates -> user is prompted to enter coordinates again
		else {
			move(row, 54);
			getyx(stdscr, tempx, tempy);
			clrtoeol();
			refresh();
			move(tempx, tempy);
			getstr(inbuf1);
			mvprintw(row, 57, "- End: ");
			getstr(inbuf2);
		}

        //coordinates are case insensitive and start - end order does not matter
		start.x = find_x_coordinate(toupper(inbuf1[0]));
		start.y = atoi(&inbuf1[1]) - 1;
		end.x = find_x_coordinate(toupper(inbuf2[0]));
		end.y = atoi(&inbuf2[1]) - 1;

		if(start.x == end.x && start.y > end.y) {
			temp = end.y; end.y = start.y ; start.y = temp;  
		}
		else if(start.y == end.y && start.x > end.x) {
			temp = end.x; end.x = start.x ; start.x = temp;  
		}
		num++;

	} while(check_coordinates(*tablept, start, end, slength, row));

    //ship is horizontally deployed -> update grid
	if(start.x == end.x) {
		for(k=start.y; k<=end.y; k++) {		
			(*tablept)[start.x][k] = 'o';
			move(29 + start.x, 10 + 2*k);
			attron(COLOR_PAIR(colour));
			attron(A_BOLD);
			printw("%c", sname[0]);
			attroff(A_BOLD);		
			attroff(COLOR_PAIR(colour));
			refresh();
		}
    }
    //ship is vertically alligned -> update grid
	else if(start.y == end.y) {
		for(k=start.x; k<=end.x; k++) {		
			(*tablept)[k][start.y] = 'o';	
			move(29 + k, 10 + 2 * start.y);
			attron(COLOR_PAIR(colour));
			attron(A_BOLD);
			printw("%c", sname[0]);
			attroff(A_BOLD);		
			attroff(COLOR_PAIR(colour));
			refresh();
		}
    }

	return 0;
}

/*
 * Manages grid creation. Moreover, takes care of ships deployment.
 */
int user_ship_deployment(char*** tablept) {
    //grid initialization
	initialize_table(tablept);

    //parameters: user's grid, ship type, ship length, row where
    //message will be depicted and corresponding color
	user_find_coordinates(tablept, "Aircraft carrier", 5, 3, 2);
	user_find_coordinates(tablept, "Battleship", 4, 4, 5);
	user_find_coordinates(tablept, "Frigate", 3, 5, 3);
	user_find_coordinates(tablept, "Submarine", 3, 6, 4);
	user_find_coordinates(tablept, "Minesweeper", 2, 7, 6);

	return 0;
}

/*
 * This function implements the logic behind the user's game. It
 * invokes the function responsible for starting the game and
 * displaying the initial ship/submarine logos. In addition, it
 * reads user's name, creates the grid, deploys the user's
 * ships and initializes the table the user maintains for the
 * already performed attacks (table is known as rival's table).
 * Parameters: read and write file descriptors where the process
 * will read and write messages.
 *
 */
int user(int read_fd, int write_fd) {
	char outbuf[SIZE];
	char inbuf[SIZE];
	int user_ship_parts=0, rival_ship_parts=0, length;
	char** user_table, ** rival_table, * name;
	point user_target, rival_target;

	name = malloc(MAX_NAME * sizeof(char*));
	if(name == NULL) {
		printw("Cannot allocate memory.\n");
		exit(MALLOC_ERROR);
	} 

	start_game(&name);
	length = strlen(name);

    //grid creation
	make_grid(name);

    //user sets the coordinates for his/hers ships
	user_ship_deployment(&user_table);

    //creates the table for the rival (empty)
	initialize_table(&rival_table);

	while(1) {
		attron(A_BOLD);	
		mvprintw(41, 6, "Rival > ");
		mvprintw(43, 6, "%s > ",  name);
		attroff(A_BOLD);	
		refresh();
	
		memset(inbuf, 0, SIZE);
		memset(outbuf, 0, SIZE);
		
		move(43,  9 + length);

        //read user's choice
		getstr(outbuf);

        //transform letters to numbers
		user_target.x = find_x_coordinate(toupper(outbuf[0]));
		user_target.y = atoi(&outbuf[1]) - 1;
	
        //check bad hit case
		if(bad_hit(user_target))
			continue;

        //check if user attacks an already hit position
		if(already_hit(rival_table, user_target))
			continue;

        //inform rival about user's attack
		write(write_fd, outbuf, strlen(outbuf));

        //rival informs whether attack was succesfull or not
		read(read_fd, inbuf, sizeof(inbuf));

        //if attack was succesfull
		if(!strcmp(inbuf, "yes")) {
			mvprintw(41, 14, "%s", inbuf);					
			refresh();
			sleep(2);

            //update the table the user maintains for rival's positions
			rival_table[user_target.x][user_target.y] = 'x';

            //update rival's grid (human visible)
			update_grid(0, user_target, 0);

            //if user won the game
			if(++rival_ship_parts == SHIP_PARTS) {
				sleep(1);

                //game is over -> corresponsing messages are printed
				end_of_game(0);
				refresh();
				sleep(10);
				memset(outbuf, 0, SIZE);
				strcpy(outbuf, "end");

                //inform rival process about the end of the game
				write(write_fd, outbuf, strlen(outbuf));
				break;
			}
		}

        //user hit nothing, rival sent its choice
		else {
			mvprintw(41, 14, "%s", "no");
			refresh();
			sleep(2);

            //update user's rival table 
			rival_table[user_target.x][user_target.y] = '+';

            //update the rival's grid with a + meaining no damage took place
			update_grid(0, user_target, 1);
			move(43,  9 + length);
			clrtoeol();
			refresh();
			mvprintw(41, 14, "%s", inbuf);
			refresh();
			sleep(2);

			rival_target.x = find_x_coordinate(toupper(inbuf[0]));
			rival_target.y = atoi(&inbuf[1]) - 1;
	
            //while rival hits user's ships we continue reading
            //its preferences
			while(user_table[rival_target.x][rival_target.y] == 'o') {
				user_table[rival_target.x][rival_target.y] = 'x'; 		

                //marks user's grid with an x
				update_grid(1, rival_target, 0);

                //if rival has drowned all user's fleet
				if(++user_ship_parts == SHIP_PARTS) {
					sleep(1);

                    //end of game logo, user lost
					end_of_game(1);
					refresh();
					sleep(10);
					memset(outbuf, 0, SIZE);
					strcpy(outbuf,  "end");
					write(write_fd, outbuf, strlen(outbuf));

                    //free space
					destroy_table(&user_table);
					destroy_table(&rival_table);	

					return 0;
				}

                //inform rival that it successed and it can continue
				memset(outbuf, 0, SIZE);
				strcpy(outbuf, "yes");
				write(write_fd, outbuf, strlen(outbuf));
				memset(inbuf, 0, SIZE);

                //read new choice
				read(read_fd, inbuf, sizeof(inbuf));
				move(41, 14);		
				clrtoeol();	
				mvprintw(41, 14, "%s", inbuf);
				refresh();
				sleep(2);
				rival_target.x = find_x_coordinate(toupper(inbuf[0]));
				rival_target.y = atoi( &inbuf[1] ) - 1;
			}
        
            //rival hit nothing
			if(user_table[rival_target.x][rival_target.y] == '^') {
				user_table[rival_target.x][rival_target.y] = '+';				
				memset(outbuf, 0, SIZE);
				strcpy(outbuf, "no");

                //inform rival that its attach was unsuccessful
				write(write_fd, outbuf, strlen(outbuf));
				update_grid(1, rival_target, 1);
			}
		}

		move(41, 6);		
		clrtoeol();
		move(43, 6);
		clrtoeol();
		refresh();
	}

    //free memory
	destroy_table(&user_table);
	destroy_table(&rival_table);

	return 0;
}

