To compile code run $ make, and to execute the binary file enter $ ./rlg327.

PC can now pick up items and will automatically store them into the carry array.  Once
all the carry slots are occupied, the pc will no longer pick up items.  

Additional cammands are added:
'w' - player is prompted the items that are carried and slot number, user can choose
      the slot they would like to choose by pressing the key, and that item will be      
      placed in the equiped array coressponding to its type, if already occupied, then 
      it will swap places with the equiped (Rings take up two equiped slots)
't' - Take of an item from the equiped array.  User will be prompt items equipped and 
      slot numbers to choose.  if the carry array is filled, then it will drop it on
      the floor where it stands
'd' - Player is prompt with carry array and choose which item to drop by pressing the 
      key of the slot number. Multiple items per dungeon cell is replaced by the 
      last dropped item
'X' - Player is prompt with carry array, key pressed of the slot number will remove 
      item from the entire game.  From the equip array, any others from the carray 
      array, all from the map, and the coressponding description from the dungeon
      vector
'i' - Will display the PCs items that are in the carry array
'e' - Will display the PCs items that are in the equip array
'I' - Player is prompt with carry array, key pressed to the corresponding slot number
      will display the items description
'L' - Game will go into display similar to teleport with '*' as the currsor showing 
      all visible monsters, pressing 'g' over a monster will display the monsters 
      description. Press any key to exit description.

to exit any of these commands, user can press escape

Npc's no longer attack each other and pc's speed updates everytime one of the commands 
are called 
