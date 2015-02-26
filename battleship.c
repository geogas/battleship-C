#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curses.h> 	
#include <time.h>								
#include <sys/types.h>
#include <sys/wait.h>

#include "util.h"	
#include "user_lib.h"
#include "rival_lib.h"

/*
 * That is the main function of the program. Here, the actual child process
 * generation takes place (fork). Moreover, which process is responsible for
 * which function is defined (child will call user function, etc).
 */
int main(){
	int fd1[2], fd2[2];
	pid_t pid;

	srand(time(NULL));
	initscr();
	cbreak();
	echo();

    //enable colors in case they are available
	if(has_colors() == TRUE)
		start_color();	

    //pairs of colors will be used during the game
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);

    //creation of the first pipe
	if(pipe(fd1) == -1){
		printw("Error in creating pipe");
		exit(PIPE_ERROR);
	}
    //second pipe creation
	if(pipe(fd2) == -1){
		printw("Error in creating pipe");
		exit(PIPE_ERROR);
	}

    //forking for creating a new process
	pid=fork();
	if(pid == -1){
		printw("Error in function fork");
		exit(FORK_ERROR);
	}
    //child process will write to fd2[1]
    //and read from fd1[0] and execute user function
	else if(!pid){
        //disable remaining read and write
		close(fd1[1]);
		close(fd2[0]);
		user(fd1[0], fd2[1]);
	}
    //father process will execute rival's code
	else{
		close(fd1[0]);
		close(fd2[1]);
		rival(fd2[0], fd1[1], pid, getpid());
		wait(NULL);
	}

	endwin();

	return 0;
}

