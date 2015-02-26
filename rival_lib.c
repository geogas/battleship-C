#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "util.h"
#include "rival_lib.h"

/*
 * Manages the deployments of rival's ships. Rand is being employed for finding
 * the initial positions for every ship.
 */
int rival_find_coordinates(char*** tablept, int child_pid, int pid, int slength, int time) {
	int k, temp;
	point start, end;

	do {
		start.x = rand() / child_pid % 10;
		start.y = rand() / child_pid % 10;
		end.x = rand() / pid % 10;
		end.y = rand() / pid % 10;

		if(start.x == end.x && start.y > end.y) {
			temp = end.y; end.y = start.y ; start.y = temp;  
		}

		else if(start.y == end.y && start.x > end.x) {				
			temp = end.x; end.x = start.x ; start.x = temp;
		}

    //-1 -> no messages are printed for the rival in case of errors
	} while(check_coordinates(*tablept, start, end, slength, -1));

    //horizontal deployment
	if(start.x == end.x) {
		for(k=start.y; k<=end.y; k++) {
			(*tablept)[start.x][k] = 'o';
        }
    }

    //vertical deployment
	else if(start.y == end.y) {
		for(k=start.x; k<=end.x; k++) {
			(*tablept)[k][start.y] = 'o';	
        }
    }

	return 0;
}

/*
 * Manages rival's grid creation and ships deployments.
 */
int rival_ship_deployment(char*** tablept, int child_pid, int pid) {
	initialize_table(tablept);
	rival_find_coordinates(tablept, child_pid, pid, 5, 0);
	rival_find_coordinates(tablept, child_pid, pid, 4, 1);
	rival_find_coordinates(tablept, child_pid, pid, 3, 2);
	rival_find_coordinates(tablept, child_pid, pid, 3, 3);
	rival_find_coordinates(tablept, child_pid, pid, 2, 4);

	return 0;
}

/*
 * Function implementing the logic behind the rival's attack. flag_pt checks if we should
 * move towards one or both directions (e.g. left and right). dir_pt reflects the
 * direction we should move in.
 */
int rival_attack(char** table, point* f_pt, point* s_pt, point* t_pt, int* flag_pt, int* dir_pt, int* rev_pt) {
	int pos;

    //first attack is drawn in random
	if(((*f_pt).x == -1 && (*f_pt).y == -1) || table[(*f_pt).x][(*f_pt).y] == '+') {
		do {
			(*f_pt).x = rand() % 10;
			(*f_pt).y = rand() % 10;
		} while(table[(*f_pt).x][(*f_pt).y] != '^');

		return 0;
	}

    //in case rival hit a ship, we don't rely on rand, but we start moving towards to the adjacent positions
	else if(table[(*f_pt).x][(*f_pt).y] == 'x' && (((*s_pt).x == -1 && (*s_pt).y == -1) || table[(*s_pt).x][(*s_pt).y] == '+')) {		
		do {

            //choose adjacent position in random (right, left, up, down)
			pos = rand() % 4 + 1;

            //right
			if(pos == 1) {
				if((*f_pt).y + 1 < 10) { (*s_pt).x = (*f_pt).x; (*s_pt).y = (*f_pt).y + 1; }
			}
            //left
			else if(pos == 2) {
				if( (*f_pt).y - 1 >= 0) { (*s_pt).x = (*f_pt).x; (*s_pt).y = (*f_pt).y - 1; }
			}
            //down
			else if(pos == 3) {
				if((*f_pt).x + 1 < 10) { (*s_pt).x = (*f_pt).x + 1; (*s_pt).y = (*f_pt).y; }
			}
            //up
			else if(pos == 4) {
				if((*f_pt).x - 1 >= 0) { (*s_pt).x = (*f_pt).x - 1; (*s_pt).y = (*f_pt).y; }
			}

        //check if choses position is within the limits and not yet attacked
		} while(((*s_pt).x == -1 && (*s_pt).y == -1 ) || table[(*s_pt).x][(*s_pt).y] != '^');

		return 0;
	}

    //ship compartment was identified in a adjacent position
	else if(table[(*f_pt).x][(*f_pt).y] == 'x' && table[(*s_pt).x][(*s_pt).y] == 'x' && !(*flag_pt)) {	

        //first and second success lie on the same row
		if((*f_pt).x == (*s_pt).x) {
			(*t_pt).x = (*f_pt).x;
            //second success was in the right direction
			if((*s_pt).y > (*f_pt).y) {
				(*t_pt).y = (*s_pt).y + 1;							

                //out of bounds
				if((*t_pt).y > 9) {

                    //move one step to the left from the first attack
					(*t_pt).y = (*f_pt).y - 1;

                    //already chosen -> continue with a rand
					if(table[(*t_pt).x][(*t_pt).y] != '^')
						return 1;

                    //move to the left, since left is out of bounds
					*flag_pt = 1;
					*dir_pt = 2;

					return 0;											
				}
                //within the bounds, but already attacked
				else if(table[(*t_pt).x][(*t_pt).y] != '^') {
                    //move to the left
					(*t_pt).y = (*f_pt).y - 1;

                    //out of bounds or already chosen -> rand
					if((*t_pt).y < 0 || table[(*t_pt).x][(*t_pt).y] != '^')
						return 1;

                    //otherwise move to the left
					*flag_pt = 1;
					*dir_pt = 2; 

					return 0;		
				}

                //in case moving to the right is valid we choose that
				*flag_pt = 2;
				*dir_pt = 1;

				return 0;									
			}

            //second success was in the left
			else {
                //same checks as above
				(*t_pt).y = (*s_pt).y - 1;
				if((*t_pt).y < 0) {
					(*t_pt).y = (*f_pt).y + 1;						

					if(table[(*t_pt).x][(*t_pt).y] != '^')	
						return 1;

					*flag_pt = 1;
					*dir_pt = 1; 

					return 0;		
				}
				else if(table[(*t_pt).x][(*t_pt).y] != '^') {		
					(*t_pt).y = (*f_pt).y + 1;
					if((*t_pt).y > 9 || table[(*t_pt).x][(*t_pt).y] != '^')
						return 1;

					*flag_pt = 1;			
					*dir_pt = 1; 

					return 0;		
				}

                //move to the left
				*flag_pt = 2;
				*dir_pt = 2; 

				return 0;		
			}
		}
        //first and second success share same column -> apply same checks as above
		else if((*f_pt).y == (*s_pt).y) {
			(*t_pt).y = (*f_pt).y;

            //second attack was farther down in the grid
			if( (*s_pt).x > (*f_pt).x ) {
				(*t_pt).x = (*s_pt).x + 1;				

                //out of bounds
				if((*t_pt).x > 9) {
					(*t_pt).x = (*f_pt).x - 1;
					if(table[(*t_pt).x][(*t_pt).y] != '^')
						return 1;

					*flag_pt = 1;
					*dir_pt = 4;

					return 0;
				}

                //already attacked this spot
				else if(table[(*t_pt).x][(*t_pt).y] != '^') {
					(*t_pt).x = (*f_pt).x - 1;

					if((*t_pt).x < 0 || table[(*t_pt).x][(*t_pt).y] != '^')	
						return 1;

					*dir_pt = 4;
					*flag_pt = 1;

					return 0;
				}

                //valid hit to take place
				*flag_pt = 2;
				*dir_pt = 3;

				return 0;
			}
            //second attach was farther up
			else {
				(*t_pt).x = (*s_pt).x - 1;							

                //out of bounds
				if((*t_pt).x < 0) {									  
					(*t_pt).x = (*f_pt).x + 1;						

					if( table[(*t_pt).x][(*t_pt).y] != '^' )	
						return 1;										

					*flag_pt = 1;
					*dir_pt = 3;										

					return 0;											
				}

                //already hit
				else if(table[(*t_pt).x][(*t_pt).y] != '^') {	
					(*t_pt).x = (*f_pt).x + 1;
					if( (*t_pt).x > 9 || table[(*t_pt).x][(*t_pt).y] != '^' )		 
						return 1;

					(*flag_pt) = 1;				
					(*dir_pt) = 3; 			

					return 0;					
				}

                //valid hit
				*flag_pt = 2;
				*dir_pt = 4; 				

				return 0;						
			}
		}
	}
    //fourth attack onwards, move into two directions (flag_pt = 2)
	else if(table[(*f_pt).x][(*f_pt).y] == 'x' && table[(*s_pt).x][(*s_pt).y] == 'x' && (*flag_pt) == 2) {
        //right
		if(*dir_pt == 1) {
			(*t_pt).y++;
            //out of bounds in the right or already chosen 
			if((*t_pt).y > 9 || table[(*t_pt).x][(*t_pt).y] != '^') {

                //from now on move in one direction
				*flag_pt = 1;
                //move in left
				*dir_pt = 2;
                //new choice will be made based on the first success
				*rev_pt = 1;
                //recursive call
				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);
			}
			return 0;
		}
        //left
		else if(*dir_pt == 2) {
			(*t_pt).y--;
			if((*t_pt).y < 0 || table[(*t_pt).x][(*t_pt).y] != '^') {
				*flag_pt = 1;
				*dir_pt = 1;
				*rev_pt = 1;

				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);		
			}
			return 0;
		}
        //up
		else if(*dir_pt == 3) {
			(*t_pt).x++;
			if((*t_pt).x > 9 || table[(*t_pt).x][(*t_pt).y] != '^') {
				*flag_pt = 1;
				*dir_pt = 4;
				*rev_pt = 1;

				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);
			}
			return 0;
		}
        //down
		else if(*dir_pt == 4) {
			(*t_pt).x--;
			if((*t_pt).x < 0 || table[(*t_pt).x][(*t_pt).y] != '^') {
				*flag_pt = 1;
				*dir_pt = 3;
				*rev_pt = 1;

				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);		
			}
			return 0;
		}
	}
    //move only in one direction
	else if(table[(*f_pt).x][(*f_pt).y] == 'x' && table[(*s_pt).x][(*s_pt).y] == 'x' && (*flag_pt) == 1) {	
        //right
		if(*dir_pt == 1) {
			(*t_pt).y++;
			if(*rev_pt) { 
                (*t_pt).y = (*f_pt).y + 1;
                *rev_pt = 0;
            }

			if((*t_pt).y > 9 || table[(*t_pt).x][(*t_pt).y] != '^') {
				(*f_pt).x = (*f_pt).y = (*s_pt).x = (*s_pt).y = (*t_pt).x = (*t_pt).y = -1;
				*flag_pt = *dir_pt = *rev_pt = 0;

				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);
			}
			return 0;
		}
        //left
		else if(*dir_pt == 2) {
			(*t_pt).y--;
			if( *rev_pt ) {
                (*t_pt).y = (*f_pt).y - 1;
                *rev_pt = 0;
            }

			if( (*t_pt).y < 0 || table[(*t_pt).x][(*t_pt).y] != '^' ) {
				(*f_pt).x = (*f_pt).y = (*s_pt).x = (*s_pt).y = (*t_pt).x = (*t_pt).y = -1;
				*flag_pt = *dir_pt = *rev_pt = 0;

				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);		
			}
			return 0;
		}																		
        //down
		else if(*dir_pt == 3) {
			(*t_pt).x++;
			if(*rev_pt) {
                (*t_pt).x = (*f_pt).x + 1;
                *rev_pt = 0;
            }
			if((*t_pt).x > 9 || table[(*t_pt).x][(*t_pt).y] != '^') {
				(*f_pt).x = (*f_pt).y = (*s_pt).x = (*s_pt).y = (*t_pt).x = (*t_pt).y = -1;
				*flag_pt = *dir_pt = *rev_pt = 0;

				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);		
			}
			return 0;
		}
        //up
		else if(*dir_pt == 4) {
			(*t_pt).x--;
			if( *rev_pt ) {
                (*t_pt).x = (*f_pt).x - 1;
                *rev_pt = 0;
            }	
			if( (*t_pt).x < 0 || table[(*t_pt).x][(*t_pt).y] != '^' ) {
				(*f_pt).x = (*f_pt).y = (*s_pt).x = (*s_pt).y = (*t_pt).x = (*t_pt).y = -1;
				*flag_pt = *dir_pt = *rev_pt = 0;

				return rival_attack(table, f_pt, s_pt, t_pt, flag_pt, dir_pt, rev_pt);		
			}
			return 0;
		}
	}
}

/*
 * Function implementing the logic of the rival's game.
 *
 */
int rival(int read_fd, int write_fd, int child_pid, int pid) {
	int code, flag=0, dir, rev=0;
	char inbuf[SIZE];
	char outbuf[SIZE];
	char** rival_table, ** user_table, letter = 'A';	
	point user_target, first, second, try;

	first.x = first.y = second.x = second.y = try.x = try.y = -1;

    //we firstly deploy rival's fleet
	rival_ship_deployment(&rival_table, child_pid, pid);

    //and we initialize rival's information about the user's deployments 
	initialize_table(&user_table);

	memset(inbuf, 0, SIZE);

    //while this function reads from the pipe
	while(read(read_fd, inbuf, sizeof(inbuf)) > 0) {
		memset(outbuf, 0, SIZE);

        //end of the game
		if(!strcmp(inbuf, "end"))
			break;

        //rival's attack was succesfull
		else if(!strcmp(inbuf, "yes")) {
            //first attack on target
			if(first.x != -1 && second.x == -1 && try.x == -1)
				user_table[first.x][first.y] = 'x';
            //second attack on target
			else if(first.x != -1 && second.x != -1 && try.x == -1)
				user_table[second.x][second.y] = 'x';
            //third attack on target
			else if(first.x != -1 && second.x != -1 && try.x != -1 && try.y != -1)
				user_table[try.x][try.y] = 'x';

            //calculate rival's next choice
			code = rival_attack(user_table, &first, &second, &try, &flag, &dir, &rev);
            //in case no available option for the third attack -> reset the variables
			if(code) {
				first.x = first.y = second.x = second.y = try.x = try.y = -1;
				flag = dir = rev = 0;
				rival_attack(user_table, &first, &second, &try, &flag, &dir, &rev);
			}
		
            //first attack -> struct first
			if(user_table[first.x][first.y] == '^' && second.x == -1 && second.y == -1)
				sprintf(outbuf, "%c%d", letter + first.x, first.y + 1);

            //second attack -> struct second
			else if(user_table[first.x][first.y] == 'x' && user_table[second.x][second.y] == '^' && try.x == -1 && try.y == -1)
				sprintf(outbuf, "%c%d", letter + second.x, second.y + 1);

            //third attack onwards -> struct try
			else if(user_table[first.x][first.y] == 'x' && user_table[second.x][second.y] == 'x' && user_table[try.x][try.y] == '^')
					sprintf(outbuf, "%c%d", letter + try.x, try.y + 1);
		}
        //no ship was hit by rival
		else if(!strcmp(inbuf, "no")) {
            //update internal rival's user table with the misplaced attacks
			if(first.x != -1 && second.x == -1 && try.x == -1)
				user_table[first.x][first.y] = '+';
			else if(first.x != -1 && second.x != -1 && try.x == -1)
				user_table[second.x][second.y] = '+';
			else if(first.x != -1 && second.x != -1 && try.x != -1 && try.y != -1)
				user_table[try.x][try.y] = '+';

            //if that was the third (or further) attach update the variables
            //checking directions
			if(user_table[first.x][first.y]=='x' && user_table[second.x][second.y]=='x' && user_table[try.x][try.y]!='x' && flag==2) {
                //now I should move in the other direction
				dir = reverse_dir(dir);
				flag = 1;
				rev = 1;
                //if first was A7, second A8 and third A9 (bad hit) I should pay a visit to A6
				continue;
			}
            //if I was already moving in only one direction and I had a bad
            //hit reset the variables so we use rand
			if(user_table[first.x][first.y]=='x' && user_table[second.x][second.y]=='x' && user_table[try.x][try.y]!='x' && flag!=2) {
				first.x = first.y = second.x = second.y = try.x = try.y = -1;
				flag = dir = rev = 0;
			}
			continue;
		}
        //check user's attack
		else {
			user_target.x = find_x_coordinate(toupper(inbuf[0]));
			user_target.y = atoi(&inbuf[1]) - 1;

            //user had a succesfull attack
			if(rival_table[user_target.x][user_target.y] == 'o') {
                //update rival's table with the hit
				rival_table[user_target.x][user_target.y] = 'x';
                //inform the user
				strcpy(outbuf,  "yes");
			}
            //bad user hit
			else {
				rival_table[user_target.x][user_target.y] = '+';

				code = rival_attack(user_table, &first, &second, &try, &flag, &dir, &rev);
				if(code) {
					first.x = first.y = second.x = second.y = try.x = try.y = -1;
					flag = dir = rev = 0;
					rival_attack(user_table, &first, &second, &try, &flag, &dir, &rev);		
				}			

                //inform user about rival's attack plan
				if(user_table[first.x][first.y] == '^' && second.x == -1 && try.x == -1)
					sprintf(outbuf, "%c%d",  letter + first.x,  first.y + 1);
				else if(user_table[first.x][first.y] == 'x' && user_table[second.x][second.y] == '^' && try.x == -1 && try.y == -1)
					sprintf(outbuf, "%c%d",  letter + second.x,  second.y + 1);
				else if(user_table[first.x][first.y] == 'x' && user_table[second.x][second.y] == 'x' && user_table[try.x][try.y] == '^')
					sprintf(outbuf, "%c%d",  letter + try.x,  try.y + 1);	
			}
		}
        //send the message to the user via the pipe
		write(write_fd, outbuf, strlen(outbuf));
		memset(inbuf, 0, SIZE);  
	}

    //end of game -> free dynamically allocated space
	destroy_table(&user_table);
	destroy_table(&rival_table);

	return 0;
}

