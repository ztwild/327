To compile code run $ make, and to execute the binary file enter $ ./rlg327.

Implemented two functionalities: fog and teleportation. To activate the 
teleportation the user presses 't' and the cursor will be displayed.  The user
can use the same keys to move the pc as they can with the cursor.  the user can
press 't' again to place the pc where the cursors current position is. The user 
can also press 'r' to randomly place the pc in an open area of the dungeon.

The Fog is automatically toggled on and displays a visibility radius of three cells.
An extra array was added to dungeon to store the memory of the map. Depending on 
the placement of the pc, it will update the memory array with the chars within the
visibility radius.  The dungeon can be toggled on by pressing 'f', as well as 
turning off.

And changed all c files into cpp files, and fixed error messages.

  

