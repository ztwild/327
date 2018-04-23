to compile cod, enter $ make.  to run program, enter $ ./ssg

This is a side scroller game where '@' is the PC. The environment has a size of 250 x 82, and the 
terminal prints with the view size of 80 x 21 while it follows the PC.  The PC can move with the 
arroy keys and can jump with 'space bar'.  



The environment is made of the following types: 
  '?' - Box, pc and npcs can walk on this environment type, cannot move into, but pc can jump into 
        from below ad break it and a random object will replace it.
  '#' - Ground, pc and npcs can walk on this environment type, cannot move into. 
  
  'H' - Ladder, pc can climb up and down
  
  '&' - Cloud, pc and npcs can walk on, pc can move into and drop down.
  
  
The environment contains the following opbjects:
  '*' - Fire if yellow, or invincibility if changing colors.
        - Fire allows the pc to shoot fire balls with 'a' 's' 'd' or 'w' to kill npcs
        - invincibility will automatically kill any npc, the pc walks into.  pc will change colors
          util its run out time. pc has 25 fire balls
  
  '$' - Coin, once 100 are accumulated, pc will gain an extra life
  
  '^' - Life, pc will gain an extra life
  
  'v' - Checkpoint, will update checkpoint if further than pervious picked up


The player will reach the end of the level once it sees the vertical text "End of level".
The next level will be randomly redrawn, and the higher levels lead to higher chance of more 
npcs, more pits, bigger pits, and higher frequency of ladders being built

once the pc jumps it cannot double jump until it lands, but can wall jump if next to environment
types. Npcs can be killed by jumping on them or breaking a box they are stnding on.  npcs also 
move and all terrains, util it comes to an edge, then it will turn around. It kills the pc if 
either walk into the other, but will turn around if running into another pc.
