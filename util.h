#ifndef UTIL_H
#define UTIL_H

/*
 * Definitions of error codes to be returned.
 */
#define PIPE_ERROR -1				  //error in pipe() invokation
#define FORK_ERROR -2				  //error in fork() invokation
#define COOR_OUT_OF_BOUNDS -3		  //coordinates out of bounds
#define ORIZONTALLY_OR_VERTICALLY -4  //ship was deployed diagonally
#define WRONG_LENGTH -5				  //ship lenght is wrong
#define USED_POSITION -6			  //coordinates already in use
#define MALLOC_ERROR -7				  //malloc failure
#define BAD_HIT -8					  //user attempted to attack an out of grid position
#define ALREADY_HIT -9  		 	  //user has already attacked this position

/*
 * Definitions of constants being used. 
 */
#define SIZE 10						  //size of pipes employed for reading and writting messages
#define SHIP_PARTS 17				  //number of ship compartments in total (for all ships)
#define MAX_NAME 15					  //maximum length of user name

/*
 * Struct point reflecting a position in the grid.
 */
typedef struct point{
	int x;
	int y;
}point;

/*
 * User and rival functions.
 */
int find_x_coordinate(char);
int check_coordinates(char**, point, point, int, int); 
int initialize_table(char***);
int destroy_table(char***);
int bad_hit(point);
int already_hit(char**, point);
int reverse_dir(int);

#endif
