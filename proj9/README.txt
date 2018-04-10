To compile code run $ make, and to execute the binary file enter $ ./rlg327.

Similar to the last assignment, the program will parse through the text files of 
monster and object descriptions and create vecotrs for each on the dungeon class.
The function that generates monsters will randomly choose a monster description 
from the dungeon, and randomly generate a number between 0 and 99, compare it 
to the monsters rarity and add it to the heap if less than the rarity, otherwise 
it will loop until it gets a monster that satisfies the requirements. Then it 
will do the same for the 10 objects.

for each function that prints the dungeon, it will get the value of the color from
the monsters and objects, and print the character in its respective color.

Once the pc navigates through a stairwell, it reallocates the object map and 
repopulates the map. 
