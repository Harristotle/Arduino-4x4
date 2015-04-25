/* L E D  C U B E  V I D E O    P L A Y ER
 Version 0.1
 Status:ALPHA
 TODO: fix random characters in serial output
       fix overflow with long commands
       
 Leon Harris, 2014
 
 This program is the sequence player for the 4x4x4 LED cube 
 http://www.instructables.com/id/Arduino-4x4x4-LED-Cube/
 
 It reads a series of strings from a static array stored in 
 flash and replays them on the LED cube. The code is mostly gutted 
 from the sequencer, with a simple loop to cycle through the array.
 If I were a better man, I would rewrite it as a library. Maybe I will
 if time permits.
 
 

 Concepts and Language mapping
 Name         Description                    language structure
 Bracket      A group of leds that are       [ 
 acted upon simultaneously                   ]
 
 Wait         Pause for a certain number     w 
 of milliseconds. Wait maps onto the 
 arduino languge structure delay() 
 
 pause        Set the default time to        p
 pause between lighting LEDs to the 
 given number. Number can be 0-9999 msec
 
 Separator    The symbol used to separate      ,
 individual leds in a sequence
 
 
 LED          The 2 dimensional position         A number from 0-15
 of the LED. This is a number on
 the following grid:
 3   7    11  15
 2   6    10  14
 1   5    9   13
 0   4    8   12
 
 Layer        The layer (height) of the led     A letter from a-d
 1-4, corresponding to pins
 A2-A5
 
 
 Note: all letter commands are lowercase. You may supply a maximum of 64 leds in a command or block. After this **Baaaaa..ddd* things *may* happen.
 If bad things happen to your ledcube, remember you can always press the reset button.
 
 A good way of using this is to copy and paste test sequences into a text editor, modify them, and then paste them onto the command line.
 
 
 A sample sequence to chase around the outside of layer 2  then flash all layer 2 would be 
 
 0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0a,1a,2a,3A,4a,5a,6a,7a,8a,9a,10a,11a,12a,13a,14a,15a] 
 
 */

#include <avr/pgmspace.h>

//Fill in your sequence here. You must put in the lines you have done
// by hand, without a loop. You have 20KB of sequence or more, which
//is easily 80 "frames" each element of an array is a frame. 
// (I have put 20 below).

int number_of_frames=10;   // put the number of frames below

prog_char frame_0[] PROGMEM = "0a,1a,2a,3a,0c,1c,2c,3c"; //put first sequence in quotes
prog_char frame_1[] PROGMEM = "p10,0d,1d2d,3d,4d,5d,6d,7d,8d,9d,10d,11d,12d,13d,14d,15d"; 
prog_char frame_2[] PROGMEM = "0a,1a,2a,3a,0c,1c,2c,3c"; 
prog_char frame_3[] PROGMEM = "p50,0d,1d2d,3d,4d,5d,6d,7d,8d,9d,10d,11d,12d,13d,14d,15d"; 
prog_char frame_4[] PROGMEM = "1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]"; 
prog_char frame_5[] PROGMEM = "0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0b,1b,2b,3b,4b,5b,6b,7b,8b,9b,10b,11b,12b,13b,14b,15b]"; 
prog_char frame_6[] PROGMEM = "p50,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p100,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p200,0a,1a,2a,3a,3b,3c,3d,2d"; 
prog_char frame_7[] PROGMEM = "1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]"; 
prog_char frame_8[] PROGMEM = "0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0c,1c,2c,3c,4c,5c,6c,7c,8c,9c,10c,11c,12c,13c,14c,15c]"; 
prog_char frame_9[] PROGMEM = "p50,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p100,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p200,0a,1a,2a,3a,3b,3c,3d,2d"; 
prog_char frame_10[] PROGMEM = "1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]"; 
prog_char frame_11[] PROGMEM = "0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0a,1a,2a,3A,4a,5a,6a,7a,8a,9a,10a,11a,12a,13a,14a,15a]"; 
prog_char frame_12[] PROGMEM = "p50,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p100,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p200,0a,1a,2a,3a,3b,3c,3d,2d"; 
prog_char frame_13[] PROGMEM = "1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]"; 
prog_char frame_14[] PROGMEM = "0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0a,1a,2a,3A,4a,5a,6a,7a,8a,9a,10a,11a,12a,13a,14a,15a]"; 
prog_char frame_15[] PROGMEM = "p50,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p100,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p200,0a,1a,2a,3a,3b,3c,3d,2d"; 
prog_char frame_16[] PROGMEM = "1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]"; 
prog_char frame_17[] PROGMEM = "0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0a,1a,2a,3A,4a,5a,6a,7a,8a,9a,10a,11a,12a,13a,14a,15a]"; 
prog_char frame_18[] PROGMEM = "p50,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p100,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p200,0a,1a,2a,3a,3b,3c,3d,2d"; 
prog_char frame_19[] PROGMEM = "1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]"; 
prog_char frame_20[] PROGMEM = "0a,1a,2a,3a,7a,11a,15a,14a,13a,12a,8b,4b,[0a,1a,2a,3A,4a,5a,6a,7a,8a,9a,10a,11a,12a,13a,14a,15a]"; 


PROGMEM const char *frame[] = 	   
{   
  frame_0,
  frame_1,
  frame_2,
  frame_3,
  frame_4,
  frame_5,
  frame_6,
  frame_7,
  frame_8,
  frame_9,
  frame_10,
  frame_11,
  frame_12,
  frame_13,
  frame_14,
  frame_15,
  frame_16,
  frame_17,
  frame_18,
  frame_19,
  frame_20};


//globals
const int buffersize = 256;
char sequenceBuffer[buffersize];
int sprite[64][2];
int Delay=250; // the default time in ms that a LED is on
int Size=0;
int leftBracket=0;
bool valid=true;
bool inbrace=false;
bool donebrace=false;
int idxBrace=0;
int *cmdBlock;


void downSerial() {
  Serial.flush();
  Serial.end();
  pinMode(0,OUTPUT);
  pinMode(1,OUTPUT);
}

void upSerial() {
  for (int i=14; i < 20; i++) {
    pinMode(i,OUTPUT);
    digitalWrite(i, HIGH);
  }
  Serial.begin(9600);


}


void lightLED(int led, int layer) {
  digitalWrite(led, HIGH);
  digitalWrite(layer + 16, LOW);
}


void darkenLED(int led, int layer) {
  digitalWrite(led, LOW );
  digitalWrite(layer + 16, HIGH);
} 


void displaysprite(int idx) {
  for (int ltime=0; ltime < (2*Delay/idx); ltime++) { // ugly timing loop, psedoproportional to number of LEDS
    for (int ptr=0; ptr < idx ; ptr++) { 
      lightLED(sprite[ptr][0], sprite[ptr][1]);
      delay(1); 
      darkenLED(sprite[ptr][0], sprite[ptr][1]);
    }
  }
}





void resetLED() {
  for (int layer =0; layer < 4; layer++) {
    for (int led = 0; led < 16; led++) {
      darkenLED(led,layer);
    }
  }
}




int * parseCommand(int cstart, char *SB) {
  static int rowcol[3];
  int j = 0;
  int k =j + cstart;
  char fragment[4]={    
    '\0'          };

  while (SB[k] != ',' &&  SB[k] !=']' && j < 4) {

    if (SB[k] >  47 && SB[k] < 58) { //0-9
      fragment[k-cstart]=SB[k];
      valid=true;
    }
    else if (SB[k] >  96 && SB[k] < 101) { //a-d
      rowcol[1]=SB[k]-97;
      valid=true;
    }

    else { 
      break;

    }


    k++;
  }
  //test for ']' may not be necessary -> part of debugging
  if (SB[k] == ']') {
    inbrace=false;
    donebrace=true;
  }
  rowcol[0]=atoi(fragment);
  rowcol[2]=k-cstart;
  return rowcol;

} 


void setup() {

  memset(sequenceBuffer, 0, buffersize); 
  for (int i=0;i<19;i++){
    pinMode(i,OUTPUT);
  }


}




void loop() {
// delete  upSerial(); // fire up the serial connection so that we can get commands from it

//  while (Size==0) {
//    Size =getSequence();  
//  }
// delete  downSerial();

for (int i = 0; i < number_of_frames; i++)
  {
    strcpy_P(sequenceBuffer, (char*)pgm_read_word(&(frame[i]))); // Necessary casts and dereferencing, just copy. 
    Size=strlen(sequenceBuffer);
    upSerial();
    Serial.println("size = ");
    Serial.println(Size);
    downSerial();
  

  resetLED(); // start from fresh slate

  int i=0;
  while ((i < Size) && (valid==true)) { 
    valid=false; //that which is not allowed is forbidden !


    switch  (sequenceBuffer[i]) {

    case ' ':
      valid=true;
      break; //do nothing if a space

    case ',':
      valid=true;
      break; //do nothing if a comma

    case '[':
      leftBracket++;
      if (leftBracket > 1) { // we don't allow nested brackets
        valid=false;
        break;
      }
      valid=true;
      inbrace=true;
      break;

    case ']':
      leftBracket--;
      inbrace=false;
      donebrace=true;
      if (leftBracket < 0 ) { // unbalanced right bracket
        valid=false;
        break;
      }
      valid=true;
      break;

    case 'w':
      i++; // jump over w command and get pause
      cmdBlock = parseCommand(i,sequenceBuffer);  
      delay(cmdBlock[0]);
      i+=cmdBlock[2];
      if (valid== false) break;
      valid=true;
      break;

    case 'p':
      // pause function
      // all delays from here are set to this value
      i++; // jump over p command and get pause
      cmdBlock = parseCommand(i,sequenceBuffer); 
      Delay=cmdBlock[0];
      i+=cmdBlock[2];
      if (valid== false) break;
      valid=true;
      break;


    default:
      // if 0-9, it is the start of a LED address
      // decode address
      if (sequenceBuffer[i] < 47 && sequenceBuffer[i] > 58) {
        valid=false;
        break;
      }

      //      if (sequenceBuffer[i] > 47 && sequenceBuffer[i] < 58) {

      cmdBlock = parseCommand(i,sequenceBuffer);
      i+=cmdBlock[2];

      if (valid== false) {
        upSerial();
        Serial.print("killed by:");
        for (int y=0; y < 3; y++ ) {
          Serial.print(cmdBlock[y]);
          Serial.print(" ");
        }
        downSerial();
        break;
      }


      valid=true;
      //        Serial.print(" ");
      //        for (int y=0; y < 3; y++ ) {
      //          Serial.print(cmdBlock[y]);
      //          Serial.print(" ");
      //        }

      // test if in []s - ie processing a sprite
      if (inbrace == true || donebrace == true) { //build sprite if in between []s
        sprite[idxBrace][0]=cmdBlock[0];
        sprite[idxBrace][1]=cmdBlock[1];
        idxBrace++;

      }
      if (inbrace == false && donebrace == true) { // display sprite
        displaysprite(idxBrace);
        donebrace=false;
        idxBrace=0;
        leftBracket=0; // reset once displayed
        break; // try this
      }

      else if (inbrace == false && donebrace == false) { // just a LED
        //just display a LED ordinarily
        lightLED(cmdBlock[0], cmdBlock[1]);
        delay(Delay);
        darkenLED(cmdBlock[0], cmdBlock[1]);
      }



    } //end of case


    if (valid==false) { // show error and bust out to get next sequence
      upSerial();
      for (int idx=0; idx < Size; idx++) {
        if (idx==i) Serial.print(" err----->");
        Serial.print(sequenceBuffer[idx]);
        downSerial();
      }

      Size=0;
      break; //kill parse loop

    }



    i++;





  }
  Size=0;

  valid=true; //reset so we are good next time
  Size=0; //clutching at debug straws here
  idxBrace=0;
  inbrace=false;
  donebrace=false;
}
  }

// test sequence p50,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p100,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p200,,0a,1a,2a,3a,3b,3c,3d,2d
//1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]





