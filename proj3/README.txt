To compile the program, run 'make' and run '$./rlg327' to execute the binary file.
Files I created for this portion of the assignment are path_find.c and queue.c.
Once the program constructs the dungeon, it creates a struct path, which holds
two 2d uint8_t arrays for the paths of the monsters that can only travel along 
the open areas and the monsters that can tunnel through walls. Then it randomly 
chooses a starting position for the pc by randomly choosing a room, then a random 
spot in the room, then saves the coordinates in a struct pair.  init_path then 
initializes the path object by setting all the values of the 2d array to the 
highest value. 

For breadth first search, it recursively calls a function and 
looks at the neighboring cells in the dungeon and adds its value+1 if it hasnt 
been visited or has a higher value until all cells have been set to their lowest 
value.  

For dijkstras, it starts with the pc's position and checks if the neighboring 
cells have been visited if they havent then it pushed that cell position onto 
a queue. And then updates the value of the cell distance if needed.  Then dequeues
the next cell position until all the cells have been visited.  It calculates the 
weight by adding the adjacent cells value + (hardness / 85) or 1 if cell is an 
open area. 



