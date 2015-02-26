Project Description
===================
This is an implementation of the popular game called battleship written
in C. The fork system call for creating child processes as well as pipes
for exchanging messages between processes are being employed. The game
heavily depends on the ncurses programming library allowing the developer
to develop a GUI-like application software that runs under a terminal
emulator. A detailed game description can be found on [wikipedia](http://en.wikipedia.org/wiki/Battleship_%28game%29).
A good source of information regarding ncurses can be found [here](http://en.wikipedia.org/wiki/Ncurses) and [here](http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Curses.pdf).

Screenshots
----------
![Screenshot](https://cloud.githubusercontent.com/assets/4787612/6402534/99f303cc-be08-11e4-9016-854ba91a5166.png?raw=true)

![Screenshot](https://cloud.githubusercontent.com/assets/4787612/6402566/d8ba5a74-be08-11e4-915c-c2dff2bbee08.png?raw=true)

![Screenshot](https://cloud.githubusercontent.com/assets/4787612/6402569/e60c3878-be08-11e4-9d74-84ed9be40c03.png?raw=true)

Installation
-------------
To manually install the prerequisites on a *ubuntu/debian* system type the following:
```bash
sudo apt-get install libncurses5-dev
```

Components
-------------
The game is split up into a number of files (source and headers).

###grid.{c,h}
Functions responsible for building and updating the grid. The logic
behind the various logos being depicted is also implemented here.

###util.{c,h}
Functions being used by both user and rival.

###user.{c,h}
User needed functions.

###rival.{c,h}
Rival's attack logic.

###battleship.c
Contains the main function.

Compiling the program
---------------------
A Makefile is provided so that the user can build the game easily.
```bash
cd battleship 
make
```

Play the game
-------------
For launching the game simply execute:
```bash
cd battleship
./battleship
```

Cleanup
-------
Execute the following command in order to get rid of the executable
and the object files
```bash
make clean
```
