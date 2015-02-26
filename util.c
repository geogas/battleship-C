#include <stdlib.h>

#include "util.h"

/*
 * Transforming letters to numbers (used for coordinates tranformations).
 */
int find_x_coordinate(char tempx) {
	switch(tempx) {
		case 'A':	return 0;
		case 'B':	return 1;
		case 'C':	return 2;
		case 'D':	return 3;
		case 'E':	return 4;
		case 'F':	return 5;
		case 'G':	return 6;
		case 'H':	return 7;
		case 'I':	return 8;
		case 'J':	return 9;
		default:	return -1;
	}
}

/*
 * Checks the validity of the coordinates user and rival chose
 * for their ship deployments. Messages are only printed for
 * the user's selections (row!=-1).
 */
int check_coordinates(char** table, point start, point end, int slength, int row) {
	int k, result = 0;

    //out of bounds coordinates
	if(start.x == -1 || start.y < 0 || start.y > 9 || end.x == -1 || end.y < 0 || end.y > 9 ) {
		if( row != -1 )
			mvprintw(row,  70,  "-  Ship coordinates out of bounds!");
		result = COOR_OUT_OF_BOUNDS;
	}

    //ship is vertically deployed
	else if(start.x != end.x && start.y != end.y) {
		if(row != -1)
			mvprintw(row,  70,  "-  Ship must be deployed only orizontally or vertically!");
		result = ORIZONTALLY_OR_VERTICALLY;
	}

    //ship length inaccurate - ship alligned horizontally
	else if(start.x == end.x && (end.y - start.y + 1 != slength)) {
		if(row != -1)
			mvprintw(row,  70,  "-  Ship must have length %d!", slength);
		result = WRONG_LENGTH;
	}

    //ship length inaccurate - ship deployed vertically
	else if(start.y == end.y && (end.x - start.x + 1 != slength)) {
		if(row != -1)
			mvprintw(row,  70,  "-  Ship must have length %d!", slength);
		result = WRONG_LENGTH;
	}

    //in case of an error code we return,  otherwise we continue with more sanity tests
	if(result) {
		if(row != -1) { refresh(); sleep(1); }
		return result;
	}

    //check if position is occupied by another ship (horizontally)
	if(start.x == end.x ) {
		for(k=start.y; k<=end.y; k++)
			if( table[start.x][k] != '^' ) {
				if(row != -1)
					mvprintw(row, 70, "-  There is already a ship in these coordinates!");
				result = USED_POSITION;
				break;
			}
	}

    //same check as the one above (vertically this time)
	else if(start.y == end.y) {
		for(k=start.x; k<=end.x; k++) {
			if(table[k][start.y] != '^') {
				if(row != -1)
					mvprintw(row, 70, "-  There is already a ship in these coordinates!");
				result = USED_POSITION;
				break;
			}
        }
    }

    //return in case no problem arose
	if(!result || (result && row == -1)) {
		return result;
    }

    //in case user specified coordinates already in use, a message is printed
	else {
		refresh();
		sleep(1);
		return result;
	}
}

/*
 * Dynamically allocates space for a 10x10 grid. Grid is
 * initialized using the ^ character.
 */
int initialize_table(char*** tablept) {
	int i, j;
	
	(*tablept) = malloc(10 * sizeof(char*));
	if( (*tablept) == NULL ) {
		printw("Cannot allocate memory.\n");
		exit(MALLOC_ERROR);
	} 

	for(i=0; i<10; i++) {
		(*tablept)[i] = malloc(10 * sizeof(char));		
		if((*tablept)[i] == NULL) {
			printw("Cannot allocate memory.\n");
			exit(MALLOC_ERROR);
		} 
	}

	for(i=0; i<10; i++)
		for(j=0; j<10; j++)
			(*tablept)[i][j] = '^';

	return 0;
}

/*
 * This function is responsible for freeing the
 * dynamically allocated grid space.
 */
int destroy_table(char*** tablept) {
	int i;
	
	for(i=0; i<10; i++) {
		free((*tablept)[i]);	
	}
	free(*tablept);

	return 0;
}

/*
 * Sanity check for a user's choice. Check whether selected position
 * lies within the allowed area.
 */
int bad_hit(point target) {
    //in case of a bad hit a message informs the user and user
    //can try again
	if(target.x > 9 || target.x < 0 || target.y > 9 || target.y < 0) {
		mvprintw( 43, 16, "-  You hit is out of bounds! Try again!" );		
		refresh();
		sleep(2);
		move(43, 6);
		clrtoeol();
		refresh();
		return BAD_HIT;
	}

	return 0;
}

/*
 * Function that checks if user chose an already selected position. If that's
 * the case, user is informed and he/she can try once more.
 *
 */
int already_hit(char** table , point target) {
	if(table[target.x][target.y] == 'x' || table[target.x][target.y] == '+') {	
		mvprintw( 43, 16, "-  You have already hit there! Try again!" );		
		refresh();
		sleep(2);
		move(43, 6);
		clrtoeol();
		refresh();
		return ALREADY_HIT;
	}

	return 0;
}

/*
 * Given a direction returns the opposite direction. Each direction
 * is assigned to a number: right->1, left->2, down->3 and up->4.
 * This function is used by the rival.
 *
 */
int reverse_dir(int dir) {
	switch(dir) {
		case 1: return 2;
		case 2: return 1;
		case 3: return 4;
		case 4: return 3;
	}

    return -1;
}

