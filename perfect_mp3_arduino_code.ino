#include "SoftwareSerial.h"
SoftwareSerial mySerial(10, 11);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

# define ACTIVATED LOW

int buttonNext = 2;
int buttonPause = 3;
int buttonPrevious = 4;
int volumeUP = 5;
int volumeDOWN = 6;
int volume = 40;
boolean isPlaying = false;



void setup () {

  pinMode(buttonPause, INPUT_PULLUP);
  pinMode(buttonNext, INPUT_PULLUP);
  pinMode(buttonPrevious, INPUT_PULLUP);
  pinMode(volumeUP, INPUT_PULLUP);
  pinMode(volumeDOWN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);

  mySerial.begin (9600);
  delay(1000);
  playFirst();
  isPlaying = true;


}


void loop () {
  // digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // delay(1000);                      // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  // delay(1000);                      // wait for a second
  if (isPlaying) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  

  if (digitalRead(buttonPause) == ACTIVATED)
  {
    if (isPlaying)
    {
      pause1();
      isPlaying = false;
    } else
    {
      isPlaying = true;
      play();
    }
  }


  if (digitalRead(buttonNext) == ACTIVATED)
  {
    if (isPlaying)
    {
      playNext();
    }
  }

  if (digitalRead(buttonPrevious) == ACTIVATED)
  {
    if (isPlaying)
    {
      playPrevious();
    }
  }

  if (digitalRead(volumeUP) == ACTIVATED)
  {
    volumeINC();
  }
  if (digitalRead(volumeDOWN) == ACTIVATED)
  {
    volumeDEC();
  }
//  delay(10);
//  change_state();
}

void playFirst()
{
  execute_CMD(0x3F, 0, 0);
  delay(500);
  execute_CMD(0x06, 0, volume);
  delay(500);
  execute_CMD(0x11, 0, 1);
  delay(500);
}


void pause1()
{
  execute_CMD(0x0E, 0, 0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D, 0, 1);
  delay(500);
}

void playNext()
{
  execute_CMD(0x01, 0, 1);
  delay(500);
}

void playPrevious()
{
  execute_CMD(0x02, 0, 1);
  delay(500);
}

void volumeINC()
{
  volume = volume + 1;
  if (volume == 31)
  {
    volume = 30;
  }
  execute_CMD(0x06, 0, volume);
  delay(500);
}
void volumeDEC()
{
  volume = volume - 1;
  if (volume == -1)
  {
    volume = 0;
  }
  execute_CMD(0x06, 0, volume);
  delay(500);
}

void change_state()
{
  if (isPlaying)
    {
      pause1();
      isPlaying = false;
    } else
    {
      isPlaying = true;
      play();
    }
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
  // Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
                            Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte
                          };
  //Send the command line to the module
  for (byte k = 0; k < 10; k++)
  {
    mySerial.write( Command_line[k]);
  }
}
