# Arduino-4x4
Here are a collection of code to design movies and displays on a 4x4x4 LED cube, including command interpreter and playback engine.

They are written to work on the LED cube described in the Instructables article  http://www.instructables.com/id/Arduino-4x4x4-LED-Cube/.

They form the basis of a term-long electronics project for student: build the 4x4x4 LED Cube,
use this software to design sequences of lights (including 3d sprites)
and then replay those sequences through a sequencer/player to make 3d animation.

For example, I challenged my students to find a dance movie, and animate it, frame by frame using
the sequencer tools and Notepad to design the frames, and then the player to display them.



## Concepts and Language mapping
# Name         Description                    language structure
 #Bracket      A group of leds that are       [ 
 acted upon simultaneously                   ]
 
 #Wait         Pause for a certain number     w 
 of milliseconds. Wait maps onto the 
 arduino languge structure delay() 
 
 #pause        Set the default time to        p
 pause between lighting LEDs to the 
 given number. Number can be 0-9999 msec
 
 #Separator    The symbol used to separate      ,
 individual leds in a sequence
 
 
 #LED          The 2 dimensional position         A number from 0-15
 of the LED. This is a number on
 the following grid:
 3   7    11  15
 2   6    10  14
 1   5    9   13
 0   4    8   12
 
 #Layer        The layer (height) of the led     A letter from a-d
 1-4, corresponding to pins
 A2-A5
 
 
 Note: all letter commands are lowercase. You may supply a maximum of 64 leds in a command or block. After this **Baaaaa..ddd* things *may* happen.
 If bad things happen to your ledcube, remember you can always press the reset button.
 
 A good way of using this is to copy and paste test sequences into a text editor, modify them, and then paste them onto the command line.
 
 
 A sample sequence to chase around the outside of layer 2  then flash all layer 2 would be 
 
 0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0a,1a,2a,3A,4a,5a,6a,7a,8a,9a,10a,11a,12a,13a,14a,15a] 
 
 */
