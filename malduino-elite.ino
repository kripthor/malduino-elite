/* 
 * Malduino Elite rewrite by kripthor
 * 
 * original work by Seytonic and Spacehuhn,  see malduino.com
 *
 * Check Keyboard.cpp for correct keyboard map and go to https://github.com/Nurrl/LocaleKeyboard.js for other maps 
 *
 */
 

#include <SPI.h>
#include <SD.h>
#include "Keyboard.h"
#include "DipSwitch.h"

//#define DEBUG true // <-- uncomment to turn serial output on
#define CSPIN 4 //Chip-Select of the SD-Card reader
#define ledPin 3
#define blinkInterval 50

File payload;
File fromSD;
DipSwitch dip;
String lastLine;


int defaultDelay = 5;
int defaultCharDelay = 5;
int lastDip = -1;
bool ledOn = true;

void keyboardWrite(uint8_t c){  
  Keyboard.press(c);
  delay(defaultCharDelay);
  Keyboard.release(c);
}

void keyboardWriteString(String s){  
 for (int i = 0; i < s.length(); i++) keyboardWrite(s.charAt(i));
}

//Simple function to output the content of a file in the sdcard as escape chars 
//For example, in linux you can use ' echo -n -e "\xAA\xBB\XCC" > /tmp/file '
void echoFileHex(String sdFileName) {
  #ifdef DEBUG 
   Serial.println("echoFileHex: Opening file '"+sdFileName+"'");
  #endif 
  fromSD = SD.open(sdFileName);
  if(!fromSD){
    #ifdef DEBUG 
     Serial.println("echoFileHex: Couldn't find file: '"+sdFileName+"'");
    #endif
    return;
  }else{
    while(fromSD.available()){
      int c = fromSD.read();
      String hex = String(c, HEX);
      if (hex.length() > 1) {
        keyboardWriteString("\\x"+hex);
      } else {
        keyboardWriteString("\\x0"+hex);
      }
    }
    fromSD.close();
  }   
}


void repeat(int times) {
  //Won't repeat a REPEAT
  if (lastLine.startsWith("REPEAT ")) return;
  for (int i=0;i<times;i++) {
    parseLine(lastLine);
  }
}


void parseLine(String line){

  if (line.startsWith("REM ")) return;
  else if (line.startsWith("DEFAULTDELAY ")) defaultDelay = line.substring(12).toInt();
  else if (line.startsWith("DEFAULT_DELAY ")) defaultDelay = line.substring(13).toInt();
  else if (line.startsWith("DEFAULTCHARDELAY ")) defaultCharDelay = line.substring(16).toInt();
  else if (line.startsWith("DEFAULT_CHAR_DELAY ")) defaultCharDelay = line.substring(18).toInt();
  else if (line.startsWith("DELAY ")) delay(line.substring(6).toInt());
  else if (line.startsWith("ECHOFILEHEX ")) echoFileHex(line.substring(12));
  else if (line.startsWith("STRING ")) keyboardWriteString(line.substring(7));
  else if (line.startsWith("REPEAT ")) repeat(line.substring(7).toInt());
  else parseKeys(line);
  Keyboard.releaseAll();
  delay(defaultDelay);
  
}


void parseKeys(String keys) {
  String key;
  String nextKeys = keys;
  int p;
  
  #ifdef DEBUG 
    Serial.println("parseKeys: '"+nextKeys+"'");
  #endif
  
  nextKeys.trim();
  p = nextKeys.indexOf(" ");
  while (p > 0) {
    key = nextKeys.substring(0,p);
    nextKeys = nextKeys.substring(p+1);
    key.trim();
    #ifdef DEBUG 
    Serial.println("Key: '"+key+"' Next:'"+nextKeys+"' p:"+String(p));
    #endif
    parseKey(key);
    p = nextKeys.indexOf(" ");
  }
  nextKeys.trim();
  parseKey(nextKeys);
  
}

void parseKey(String key){

  #ifdef DEBUG 
    Serial.println("parseKey: '"+key+"'");
  #endif
  
  if(key.length() == 1) Keyboard.press(key.charAt(0));
  else if(key.equals("ENTER")) Keyboard.press(KEY_RETURN);
  else if(key.equals("GUI") || key.equals("WINDOWS")) Keyboard.press(KEY_LEFT_GUI);
  else if(key.equals("SHIFT")) Keyboard.press(KEY_LEFT_SHIFT);
  else if(key.equals("ALT")) Keyboard.press(KEY_LEFT_ALT);
  else if(key.equals("CTRL") || key.equals("CONTROL")) Keyboard.press(KEY_LEFT_CTRL);
  else if(key.equals("CAPSLOCK")) Keyboard.press(KEY_CAPS_LOCK);
  else if(key.equals("DELETE")) Keyboard.press(KEY_DELETE);
  else if(key.equals("END")) Keyboard.press(KEY_END);
  else if(key.equals("ESC") || key.equals("ESCAPE")) Keyboard.press(KEY_ESC);
  else if(key.equals("HOME")) Keyboard.press(KEY_HOME);
  else if(key.equals("INSERT")) Keyboard.press(KEY_INSERT);
  else if(key.equals("PAGEUP")) Keyboard.press(KEY_PAGE_UP);
  else if(key.equals("PAGEDOWN")) Keyboard.press(KEY_PAGE_DOWN);
  else if(key.equals("SPACE")) Keyboard.press(' ');
  else if(key.equals("TAB")) Keyboard.press(KEY_TAB);
  else if(key.equals("UP") || key.equals("UPARROW")) Keyboard.press(KEY_UP_ARROW);
  else if(key.equals("DOWN") || key.equals("DOWNARROW")) Keyboard.press(KEY_DOWN_ARROW);
  else if(key.equals("LEFT") || key.equals("LEFTARROW")) Keyboard.press(KEY_LEFT_ARROW);
  else if(key.equals("RIGHT") || key.equals("RIGHTARROW")) Keyboard.press(KEY_RIGHT_ARROW);
  else if(key.equals("PRINTSCREEN")) Keyboard.press(PRINTSCREEN);
  else if(key.equals("F1")) Keyboard.press(KEY_F1);
  else if(key.equals("F2")) Keyboard.press(KEY_F2);
  else if(key.equals("F3")) Keyboard.press(KEY_F3);
  else if(key.equals("F4")) Keyboard.press(KEY_F4);
  else if(key.equals("F5")) Keyboard.press(KEY_F5);
  else if(key.equals("F6")) Keyboard.press(KEY_F6);
  else if(key.equals("F7")) Keyboard.press(KEY_F7);
  else if(key.equals("F8")) Keyboard.press(KEY_F8);
  else if(key.equals("F9")) Keyboard.press(KEY_F9);
  else if(key.equals("F10")) Keyboard.press(KEY_F10);
  else if(key.equals("F11")) Keyboard.press(KEY_F11);
  else if(key.equals("F12")) Keyboard.press(KEY_F12);
  else if(key.equals("NUM_0")) keyboardWrite(KEYPAD_0);
  else if(key.equals("NUM_1")) keyboardWrite(KEYPAD_1);
  else if(key.equals("NUM_2")) keyboardWrite(KEYPAD_2);
  else if(key.equals("NUM_3")) keyboardWrite(KEYPAD_3);
  else if(key.equals("NUM_4")) keyboardWrite(KEYPAD_4);
  else if(key.equals("NUM_5")) keyboardWrite(KEYPAD_5);
  else if(key.equals("NUM_6")) keyboardWrite(KEYPAD_6);
  else if(key.equals("NUM_7")) keyboardWrite(KEYPAD_7);
  else if(key.equals("NUM_8")) keyboardWrite(KEYPAD_8);
  else if(key.equals("NUM_9")) keyboardWrite(KEYPAD_9);
  else if(key.equals("NUM_ASTERIX")) keyboardWrite(KEYPAD_ASTERIX);
  else if(key.equals("NUM_ENTER")) keyboardWrite(KEYPAD_ENTER);
  else if(key.equals("NUM_Minus")) keyboardWrite(KEYPAD_MINUS);
  else if(key.equals("NUM_PERIOD")) keyboardWrite(KEYPAD_PERIOD);
  else if(key.equals("NUM_PLUS")) keyboardWrite(KEYPAD_PLUS);

  #ifdef DEBUG 
      else Serial.println("parseKey: failed");
  #endif

}



void searchAndRunScript() {
    
  String scriptName = dip.getDipsString()+".txt";
  
  payload = SD.open(scriptName);
  if (!payload) {
    #ifdef DEBUG 
      Serial.println("setup: not found '"+scriptName+"'");
    #endif
    return;
  } 
  else {   
    #ifdef DEBUG 
      Serial.println("setup: found script '"+scriptName+"'");
    #endif  
    
    String line;
    Keyboard.begin();
    while(payload.available()){     
      line = payload.readStringUntil('\n');
      line.trim(); 
      parseLine(line);
      lastLine = line;   
    }   
    payload.close();
    Keyboard.end();
  }
}


void setup() {
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  #ifdef DEBUG
    Serial.begin(115200);
    delay(3000);
    Serial.println("DEBUG started, press any key to init.");
    while (!(Serial.available() > 0)) delay(100);
  #endif

  
  if(!SD.begin(CSPIN)) {
    #ifdef DEBUG 
      Serial.println("setup: couldn't access sd-card");
    #endif
    return;
  }  
  
}


void loop() {

  //In the event that you can switch dips while connected to usb, 
  //the malduino will execute the script of the new dip configuration
  if (lastDip != dip.getDips()) {
    lastDip = dip.getDips();
    searchAndRunScript();
  } 

  //Flash fast when script ends
  else {
    ledOn = !ledOn;
    digitalWrite(ledPin, ledOn);
    delay(blinkInterval);
  }
}
