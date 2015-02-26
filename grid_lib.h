#ifndef GRID_LIB_H
#define GRID_LIB_H

/*
 * Grid functions.
 */
void make_grid(char*);
void end_of_game_logo(void);
void end_of_game(int);		
void start_game(char**);
int update_grid(int, point, int); 

#endif
