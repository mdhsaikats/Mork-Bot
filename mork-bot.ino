#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "eyes.h"

// --- Configuration ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 6
#define OLED_SCL 7
#define TOUCH_PIN 4 
#define LDR_PIN 1   

#define DARK_THRESHOLD 500 
#define SHAKE_THRESHOLD 15.0 
const unsigned long INTERACTION_LOCK_TIME = 3000; 
const unsigned long NEGLECT_TIMEOUT = 60000;      
const unsigned long SAD_TIMEOUT = 120000;        
const unsigned long EYE_MOVE_INTERVAL = 3000;
const unsigned long MOOD_CYCLE_INTERVAL = 6000;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_MPU6050 mpu;

enum State { IDLE, LOVE, ANGRY, SLEEPING };
State currentState = IDLE;

int currentMood = 0;      
int eyeOffsetX = 0, eyeOffsetY = 0;

unsigned long lastInteraction = 0; 
unsigned long stateStartTime = 0;  
unsigned long lastEyeMove = 0;
unsigned long lastMoodChange = 0;
bool wasTouched = false;

void setup() {
  Serial.begin(115200);
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) for(;;); 

  pinMode(TOUCH_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  randomSeed(analogRead(0));
  lastInteraction = millis();
  forceMood(0); 
}

void loop() {
  unsigned long now = millis();

  // 1. SENSOR READS
  int lightLevel = analogRead(LDR_PIN);
  bool isTouched = digitalRead(TOUCH_PIN);
  
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  float accelMag = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));
  float movement = abs(accelMag - 9.81);

  // 2. STATE MACHINE LOGIC

  // SLEEP LOGIC (Optimized to prevent flicker)
  if (lightLevel < DARK_THRESHOLD) {
    if (currentState != SLEEPING) {
      currentState = SLEEPING;
      drawEyes(2); // Draw once and STOP
      Serial.println(F("Mork is sleeping..."));
    }
    return; // Exit loop so no other drawing happens
  } 
  else if (currentState == SLEEPING) {
    // This runs ONLY the moment light returns
    currentState = IDLE;
    lastInteraction = now; 
    forceMood(0); 
    Serial.println(F("Mork woke up!"));
  }

  // TOUCH LOGIC
  if (isTouched && !wasTouched) {
    currentState = LOVE;
    stateStartTime = now;
    lastInteraction = now; 
    blink();
    drawEyes(6); 
  }
  wasTouched = isTouched;

  // SHAKE LOGIC
  if (movement > SHAKE_THRESHOLD && currentState == IDLE) {
    currentState = ANGRY;
    stateStartTime = now;
    lastInteraction = now;
    blink();
    drawEyes(3); 
  }

  // 3. EXIT LOCKED STATES
  if ((currentState == LOVE || currentState == ANGRY) && (now - stateStartTime > INTERACTION_LOCK_TIME)) {
    currentState = IDLE;
    forceMood(currentMood); // Return to current background mood
  }

  // 4. IDLE BEHAVIOR
  if (currentState == IDLE) {
    // Eye movement logic
    if (now - lastEyeMove > EYE_MOVE_INTERVAL) {
      updateEyeOffset();
      drawEyes(currentMood);
      lastEyeMove = now;
    }

    // Mood decay logic
    unsigned long timeSinceLastPet = now - lastInteraction;
    if (timeSinceLastPet > SAD_TIMEOUT) {
      if (currentMood != 5) forceMood(5);
    } 
    else if (timeSinceLastPet > NEGLECT_TIMEOUT) {
      if (currentMood != 4) forceMood(4);
    } 
    else {
      if (now - lastMoodChange > MOOD_CYCLE_INTERVAL) {
        int randomMood = random(0, 2); 
        if(currentMood != randomMood) forceMood(randomMood);
        lastMoodChange = now;
      }
    }
  }
}

// --- Display Helpers ---

void drawEyes(int moodID) {
  display.clearDisplay();
  int lx = 20 + eyeOffsetX;
  int rx = 74 + eyeOffsetX;
  int ly = 8 + eyeOffsetY;
  int ry = 8 + eyeOffsetY;
  display.drawBitmap(lx, ly, peyes[moodID][0][0], 32, 32, WHITE);
  display.drawBitmap(rx, ry, peyes[moodID][0][1], 32, 32, WHITE);
  display.display();
}

void forceMood(int moodID) {
  currentMood = moodID;
  blink();
  drawEyes(currentMood);
}

void blink() {
  display.clearDisplay();
  display.drawBitmap(20, 8, eye0, 32, 32, WHITE);
  display.drawBitmap(74, 8, eye0, 32, 32, WHITE);
  display.display();
  delay(60); 
}

void updateEyeOffset() {
  int choice = random(0, 5);
  eyeOffsetX = (choice == 1) ? -5 : (choice == 2) ? 5 : 0;
  eyeOffsetY = (choice == 3) ? -4 : (choice == 4) ? 4 : 0;
}