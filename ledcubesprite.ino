/* L E D  C U B E  S E Q U E N C E R
 Version 0.3
 Status: functional, but not completely bug free
 TODO: fix random characters in serial output
       fix overflow with long commands
       
 Leon Harris, 2013
 
 This program was written to help students design sequences for their
 led cubes, for the Arduino LED cube pd 9th Nov 2013
 It reads a sequence from the serial console to be displayed on the 4x4x4
 led cube of http://www.instructables.com/id/Arduino-4x4x4-LED-Cube/
 
 As all i/o lines are used to drive the LED cube, this code must take
 down the serial port and reestablish it after each sequence.
 
 
 
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


int getSequence(){
  int Size=0;
  if (Serial.available() > 0) {

    Size =  Serial.readBytesUntil('\r', sequenceBuffer, 255);

    return Size;

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
  upSerial(); // fire up the serial connection so that we can get commands from it

  while (Size==0) {
    Size =getSequence();  
  }
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

// test sequence p50,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p100,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,p200,,0a,1a,2a,3a,3b,3c,3d,2d
//1d,0d,0c,0b,0a,p300,0a,1a,2a,3a,3b,3c,3d,2d,1d,0d,0c,0b,0a,[0a,1a,2a,3a],[3a,3b,3c,3d],[3d,2d,1d,0d],[0d,0c,0b,0a]





