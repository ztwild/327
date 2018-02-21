To compile code run $ make, and to execute the binary file enter $ ./rlg327.
The user can use the switch '--nummon' followed by a positive number, and it 
will set the number of monsters in the dungeon, otherwise it will default to 
10 monsters.  The pc now gets a random position in the first room made and the 
monsters that cannot tunnel get random positions in the other rooms.  The monsters 
with the tunnel ability get a random position anywhere.  

Once it starts the game it pushes the pc and all the monsters into a priority queue
and first sorts by the smallest turn count, else if equal then it sorts by the 
soonest event time (PC always has the event time of 0), then moves the character.
Once the character is dequeued, it moves if possible, increments the turn count, 
and sorts it back into the queue. If it dequeues the pc, then it prints the updated
grid before it does anything to the pc. 

For the monster's move, it first checks if its erratic and filps the coin to see
if it will randomly move into a valid cell.  Else if it is intelligent, the monster
moves to the cheapest position depending on the tunneling ability.  Otherwise if 
its unintelligent, it will move straight to the pcs current location if possible.

If the monster is going to move into the cell of the pc, it kills the player and 
ends the game.
