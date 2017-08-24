#include <Arduino.h>

/*
  Passthrough keypresses from serial to Keyboard

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 Protocol:
 2 bytes total:
   1 byte - press flag
     0x0 - next key released
     0x1 - next key pressed
   1 byte - character pressed
     for special chars look at arduino keyboard mapping

 */
#include <SoftwareSerial.h>
#include <Keyboard.h>
#include <SoftPWM.h>
#include <TimerOne.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(9, 10); // RX, TX

// flag if the next key is pressed or released
bool press=true;
const int led = LED_BUILTIN_RX;  // the pin with a LED
void blinkLED(void);
LiquidCrystal_I2C ui(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
char text[6];

void setup(){

  TXLED0;
  RXLED0;
  mySerial.begin(9600);
  ui.begin(20,4);         // initialize the lcd for 20 chars 4 lines, turn on backlight
  ui.clear(); // display
  ui.print("Press a key to begin");
  while(mySerial.available() <= 0) {
		delay(200);
  }
  ui.clear(); // display
  ui.print("Pressed");
	ui.clear(); // display
  while(mySerial.available()) {
    mySerial.read();
    delay(200);
  }

//	delay(8000);
//  ui.lcd_contrast(45);  // 3V3 contrast
//  ui.lcd_contrast(25);  // 5V contrast
  ui.clear(); // display
//  ui.lcd_mode(1); // dual ht
  ui.print("Serial data display");
  delay(1500);


  Timer1.initialize(1500000);
  Timer1.attachInterrupt(blinkLED); // blinkLED to run every 0.15 seconds

  Keyboard.begin();
  //Serial.begin(9600);
  // Initialize
  SoftPWMBegin();

  // Create and set pin 13 to 0 (off)
  SoftPWMSet(led, 0);

  // Set fade time for pin 13 to 100 ms fade-up time, and 500 ms fade-down time
  SoftPWMSetFadeTime(17, 300, 600);
}

int ledState = LOW;
int remember1 = 0;
int remember2 = 0;
int remember3 = 0;
int sendit = 0;

void blinkLED(void)
{
  if (ledState == LOW) {
    ledState = HIGH;
    // Turn on - set to 100%
    SoftPWMSetPercent(17, 100);
  } else {
    ledState = LOW;
      // Turn off - set to 0%
    SoftPWMSetPercent(17, 0);

    // Wait for LED to turn off
  }
}

void loop(){

  if (mySerial.available())
  {
    byte k = mySerial.read();
    //Serial.write(k);
    //Serial.println(" received");
    ui.clear(); // display
    ui.print("Key: ");

    if (k == 0){
      press = 0;
    }else if (k == 1){
      press = 1;
    }else {
      //mySerial.write(k);
      if (press){
        if (k == 128){
            remember1 = 1;
        }
        if (k == 130){
            remember2 = 1;
        }
        if (k == 50){
            remember3 = 1;
        }
        itoa(k, text, 10);
        ui.print(String(text) + String(' '));
        if (remember1 == 1 && remember2 == 1 && remember3 == 1){
            sendit = 1;
            ui.write(sendit + ' ');
        }
        Keyboard.press(k);
        //mySerial.println(" pressed");
        TXLED1;
        delay(40);
        TXLED0;

      } else {
        if (k == 128){
            remember1 = 0;
        }
        if (k == 130){
            remember2 = 0;
        }
        if (k == 50){
            remember3 = 0;
        }
        Keyboard.release(k);
        itoa(k, text, 10);
        ui.print(String(text) + String(' '));
        if (remember1 == 0 && remember2 == 0 && remember3 == 0 && sendit == 1){
            sendit = 0;
            Keyboard.press(KEY_LEFT_CTRL);
            delay(100);
            Keyboard.press(KEY_LEFT_ALT);
            delay(100);
            Keyboard.press(KEY_DELETE);
            delay(100);
            Keyboard.releaseAll();
            ui.clear(); // display
//						ui.lcd_mode(0); // dual ht
            ui.print("Sent");
            ui.setCursor(0, 1);
            ui.print("CTRL+ALT+DELETE");
            Timer1.detachInterrupt();
            SoftPWMSet(led, 0);
            RXLED0;
            TXLED1;
            delay(400);
            TXLED0;
            delay(400);
            TXLED1;
            delay(400);
            TXLED0;
            delay(400);
            TXLED1;
            delay(400);
            TXLED0;
            delay(400);
            TXLED1;
            delay(400);
            TXLED0;
            delay(400);
        }
        //mySerial.println(" released");
        TXLED1;
        delay(40);
        TXLED0;
      }
    }
  }
  // Wait for LED to turn on - you could do other tasks here
}

