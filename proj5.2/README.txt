To compile code run $ make, and to execute the binary file enter $ ./rlg327.

Implemented curses into the project, and in the main method added the initialization
of the screen and ending of the window.  Also added a method that retrieves the
players move once its the pc's turn with getch().  It goes through a series of switch
statements.  If the player makes an illegal move then it does not count as a move
as well as bringing up the monster list.  Added staircases to the dungeon, and it
redraws another dungeon layout, frees the monsters and makes a new list, and
repositions the pc and resets its turn to 0.  Game can quit with the key character 'Q'.

