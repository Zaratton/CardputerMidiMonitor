#include "M5Cardputer.h"
#include <MIDI.h>


#define MIDI_RX_PIN 1
#define MIDI_TX_PIN 2
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, MIDI);

byte msgHist[6] = {0};  //I save a history of the last 6 midi messages to print on the screen.
int param1Hist[6] = {0};  //Param1 is int instead of byte because it is also used for pitch bend messages.
byte param2Hist[6] = {0};
byte param3Hist[6] = {0};

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  fillHistory(1, pitch, velocity, channel);                                 
}
void handleNoteOff(byte channel, byte pitch, byte velocity)
{
   fillHistory(2, pitch, velocity, channel);
}
void handleControlChange(byte channel, byte control, byte cvalue)
{
   fillHistory(3, control, cvalue, channel);
}
void handleAfterTouchPoly(byte channel, byte note, byte pressure){
  fillHistory(4, note, pressure, channel);
}
void handleProgramChange(byte channel, byte number){
  fillHistory(5, number, 0, channel);
}
void handleAfterTouchChannel(byte channel, byte pressure){
  fillHistory(6, pressure, 0, channel);
}
void handlePitchBend(byte channel, int bend){
  fillHistory(7, bend, 0, channel);
}

void printScreen (byte screenpos, byte type, int param1, byte param2, byte param3){
  String msgType;
  String valueType;
  switch (type){
    case 1: //note on
      msgType = "NtOn: ";
      valueType = "Vel: ";
      break;
    case 2: //note off
      msgType = "NtOff: ";
      valueType = "Vel: ";
      break;
    case 3: //Control Change
      msgType = "CC: ";
      valueType = "Val: ";
      break;
    case 4: //AfterTouchPoly
      msgType = "ATP: ";
      valueType = "Pres: ";
      break;
    case 5: //Program Change
      msgType = "PC: ";
      valueType = "--- ";
      break;
    case 6: //AfterTouchChannel
      msgType = "ATC: ";
      valueType = "--- ";
      break;
    case 7: //PitchBend
      msgType = "PB: ";
      valueType = "--- ";
      break;
    default:
      break;
  }
  int line = 120 - (screenpos*20);
  
  M5Cardputer.Display.setCursor(0,line);
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.print(msgType);
    M5Cardputer.Display.setTextColor(0xF81F);
  if (type == 7){  //Pitch Bend positive values Green, negative values Red.
    if(param1 < 0){
      M5Cardputer.Display.setTextColor(RED);  
    }else{
      M5Cardputer.Display.setTextColor(GREEN);  
    } 
  }
  M5Cardputer.Display.setCursor(40,line);
  M5Cardputer.Display.print(param1);
  M5Cardputer.Display.setCursor(90,line);
  if (type != 7){
    M5Cardputer.Display.setTextColor(YELLOW);
    M5Cardputer.Display.print(valueType);
    M5Cardputer.Display.setCursor(130,line);
    M5Cardputer.Display.setTextColor(0xF81F);
    M5Cardputer.Display.print(param2);
  }
  M5Cardputer.Display.setCursor(170,line);
  M5Cardputer.Display.setTextColor(YELLOW);
  M5Cardputer.Display.print("Chan: ");
  M5Cardputer.Display.setCursor(220,line);
  M5Cardputer.Display.setTextColor(0xF81F);
  M5Cardputer.Display.print(param3);
}

void fillHistory (byte type, int param1, byte param2, byte param3){
  M5Cardputer.Display.clear();
  for (int i= 5; i >= 0; i--){
    if (i == 0){
      byte j = i+1;
        msgHist[j] = msgHist[i];
        param1Hist[j] = param1Hist[i];
        param2Hist[j] = param2Hist[i];
        param3Hist[j] = param3Hist[i];      
        printScreen(j, msgHist[j], param1Hist[j], param2Hist[j], param3Hist[j]);

        msgHist[i] = type;
        param1Hist[i] = param1;
        param2Hist[i] = param2;
        param3Hist[i] = param3;
        printScreen(i, msgHist[i], param1Hist[i], param2Hist[i], param3Hist[i]);
    }else{
      if (msgHist[i] != 0){
        if (i < 5){
        byte j = i+1;
        msgHist[j] = msgHist[i];
        param1Hist[j] = param1Hist[i];
        param2Hist[j] = param2Hist[i];
        param3Hist[j] = param3Hist[i];      
        printScreen(j, msgHist[j], param1Hist[j], param2Hist[j], param3Hist[j]);
        }
      }
    }
  }
}


void setup() {
  
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleAfterTouchPoly(handleAfterTouchPoly);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.setHandleAfterTouchChannel(handleAfterTouchChannel);
  MIDI.setHandlePitchBend(handlePitchBend);
  

  // Initiate MIDI communications, listen to all channels
  
  auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.setTextColor(YELLOW);
    M5Cardputer.Display.setTextDatum(bottom_left);
    M5Cardputer.Display.setTextFont(&fonts::Orbitron_Light_32);
    M5Cardputer.Display.setTextSize(0.7);
    M5Cardputer.Display.drawString("Midi Monitor 1.0", 0, 60);
    M5Cardputer.Display.setTextColor(0xF81F);
    M5Cardputer.Display.drawString("by Cosas Ratas", 0,120);
    M5Cardputer.Display.setTextSize(0.4);
                                   
    Serial2.begin(31250, SERIAL_8N1, MIDI_RX_PIN, MIDI_TX_PIN);
    MIDI.begin(MIDI_CHANNEL_OMNI);
}

void loop() {
  MIDI.read();
}
