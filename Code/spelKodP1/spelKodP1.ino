#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <stdlib.h>
#include "pitches.h"
#include <avr/wdt.h>
#include <SPI.h>
#include <Ethernet.h>

//Spelkoden Författare: Ömer Kolsuz, Fatima Kadum, Bashar Hassan, Oscar Svantesson
//Kod för anslutningen för Server Författare : Bilal, Firoz, Oscar
//Endast kommenterat en av filerna då koderna är exakt samma. 


String clientUsername;
EthernetClient client;
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 101); 
IPAddress serverIp(192, 168, 1, 177); 
int serverPort = 3002;
bool isMoving;
const int buzzer = 8;
int hits = 0;
const float songSpeed = 1.0;

//Ömer, Fatima
#define LED_PIN     9
#define BUTTON_PIN1 A5 // HÖGER
#define BUTTON_PIN2 A3 // VÄNSTER
#define BUTTON_PIN3  A4// up 
#define BUTTON_PIN4  A1// down 
#define BUTTON_PIN5  A2//fire //fire
#define NUM_LEDS    64
#define COLOR_DELAY 0
#define BUZZER_PIN 8


//Bilal, Ömer
void restartProgram() {
  wdt_enable(WDTO_15MS);
  while (true) {
    Ethernet.begin(mac, ip);
    Serial.println("Connecting to server...");
    client.connect(serverIp, serverPort);
    Serial.println("Connected to server");

    while (client.connected() && client.available()) {
      String messageServer = client.readStringUntil('\n');
      if (messageServer.startsWith("Username:")) {
        clientUsername = messageServer.substring(9);
        Serial.println("Received clientUsername from server: " + clientUsername);
      }
    }

    if (client.connected()) {
      break;
    }
  }
}

//Ömer, Firoz, Bashar
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
int i = 0;
int tries = 0;
int board[8][8] = {{0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0},
                   {0,0,0,0,0,0,0,0}};
int hit[NUM_LEDS] = {0};
int miss[NUM_LEDS] = {0};
String username;


//Bilal, Ömer, Oscar
void setup() {
    Ethernet.begin(mac, ip);
  Serial.begin(9600);
    Serial.println("Connecting to server...");
    client.connect(serverIp, serverPort);
    Serial.println("Connected to server");
      

  //Bashar, Ömer
  lcd.begin(16, 2);
  strip.begin();
  strip.show();
  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);
  pinMode(BUTTON_PIN3, INPUT);
  pinMode(BUTTON_PIN4, INPUT);
  pinMode(BUTTON_PIN5, INPUT);
  randomSeed(analogRead(A0));
  initializeBoard();
  lcd.print("Lycka till!");
  lcd.setCursor(0, 1);
  lcd.print("Tryck for start");
  for(int i=0; i<NUM_LEDS; i++){
    strip.setPixelColor(i, 0, 0, 255); 
  }
  strip.setPixelColor(0, 0, 0, 255); 
  strip.show();

  
}

//Fatima, Bilal, Firoz, Oscar, Ömer
void loop() {
   if (client.connected()) {
    if (client.available()) {
      String messageServer = client.readStringUntil('\n');
    //  Serial.println(messageServer);
      if (messageServer.startsWith("Username:")) {
        clientUsername = messageServer.substring(9);
        Serial.println("Received clientUsername from server: " + clientUsername);
        lcd.clear();
        username = clientUsername;
        lcd.setCursor(0,0);
        lcd.println("Welcome!             ");
        lcd.setCursor(0,1);
        lcd.println(username + "            ");
    noTone(buzzer);
      }
    }
   }
    if (!isMoving) {

    strip.setPixelColor(i, 0, 0, 0); 
    strip.show();
    delay(75);
    if(hit[i]){
       strip.setPixelColor(i, 0, 255, 0);
      strip.show();
    }
  
  else if(miss[i]){
    strip.setPixelColor(i, 255, 0, 0);
    strip.show();
  }
  else{
    strip.setPixelColor(i, 0, 0, 255);
    strip.show();
  }
    
    delay(100);

  }
  switch (digitalRead(BUTTON_PIN1)) { 
    case HIGH:
      if (i % 8 != 0) {
        moveLeft();
        tone(8,NOTE_G4,35);
                      delay(35);
                        tone(8,NOTE_G5,35);
                        delay(35);
                        tone(8,NOTE_G6,35);
                        delay(35);
                        noTone(8);
                       delay(0); 
      }
      break;
    case LOW:
      switch (digitalRead(BUTTON_PIN2)) { 
        case HIGH:
          if (i % 8 != 7) {
            moveRight();
            tone(8,NOTE_G4,35);
                      delay(35);
                        tone(8,NOTE_G5,35);
                        delay(35);
                        tone(8,NOTE_G6,35);
                        delay(35);
                        noTone(8);
                       delay(0); 
          }
          break;
        case LOW:
          switch (digitalRead(BUTTON_PIN3)) { 
            case HIGH:
              if (i >= 8) {
                moveUp();
                tone(8,NOTE_G4,35);
                      delay(35);
                        tone(8,NOTE_G5,35);
                        delay(35);
                        tone(8,NOTE_G6,35);
                        delay(35);
                        noTone(8);
                       delay(0); 
              }
              break;
            case LOW:
              switch (digitalRead(BUTTON_PIN4)) { 
                case HIGH:
                  if (i < 56) {
                    moveDown();
                    tone(8,NOTE_G4,35);
                      delay(35);
                        tone(8,NOTE_G5,35);
                        delay(35);
                        tone(8,NOTE_G6,35);
                        delay(35);
                        noTone(8);
                       delay(0); 
                  }
                  break;
                case LOW:
                  switch (digitalRead(BUTTON_PIN5)){ 
                    case HIGH:
                      fire();
                      break;
                    case LOW:
                      break;
                  }
                  break;
              }
              break;
          }
          break;
      }
      break;
  }
}

//Ömer, Kolsuz
void moveLeft() {
i--;
isMoving = true;
//strip.setPixelColor(i, 0, 255, 255); // white
//strip.show();
//delay(50);


isMoving = false;
}
//ömer, oscar
void moveRight() {
//strip.setPixelColor(i, 0, 0, 255); // blue
i++;
isMoving = true;
//strip.setPixelColor(i, 0, 255, 255); // white
//strip.show();
//delay(50);

isMoving = false;
}
//ömer, oscar
void moveUp() {
//strip.setPixelColor(i, 0, 0, 255); // blue
i -= 8;
isMoving = true;
//strip.setPixelColor(i, 0, 255, 255); // white
//strip.show();
//delay(50);

isMoving = false;
}
//ömer, oscar
void moveDown() {
//strip.setPixelColor(i, 0, 0, 255); // blue
isMoving = true;
i += 8;
//strip.setPixelColor(i, 0, 255, 255); // white
//strip.show();
//delay(50);

isMoving = false;
}

//Fatima, Oscar, Ömer, Bashar
void fire() {
  if (hit[i] || miss[i]) { 
    return;
  } else if (board[i / 8][i % 8] == 1) { 

    hit[i] = 1; 
    
    strip.setPixelColor(i, 0, 255, 0); 
                      tone(8,NOTE_E6,125);
                      delay(35);
                      tone(8,NOTE_G6,125);
                      delay(35);
                      tone(8,NOTE_E7,125);
                      delay(35);
                      tone(8,NOTE_C7,125);
                      delay(35);
  
    lcd.clear();
    for (int j = 0; j < NUM_LEDS; j++) { 
      if (hit[j] == 1) {
        strip.setPixelColor(j, 0, 255, 0);
        
      }
    }
    hits++;
  } else if (board[i / 8][i % 8] == 0) { 
  tone(8,NOTE_A4,35);
                      delay(35);
                        tone(8,NOTE_C5,35);
                        delay(35);
                        tone(8,NOTE_A4,35);
                        delay(35);
                        noTone(8);
                       delay(0); 
    miss[i] = 1; 
    strip.setPixelColor(i, 255, 0, 0); 
    lcd.clear();
  }
  strip.show();
  delay(200);
  tries++;
  
  lcd.print("Hits:");
  lcd.print(hits);
   lcd.setCursor(0, 1);
  lcd.print("Tries: ");
  lcd.print(tries);
  checkGameOver();
}

//Ömer Oscar
void initializeBoard() {
  // clear board
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      board[row][col] = 0;
    }
  }

  placeShipsRandomly();

  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, 0, 0, 255); // blue
  }
  strip.setPixelColor(0, 0, 255, 255); // purple
  strip.show();

}

//Oscar, Bashar
void placeShipsRandomly() {
  int shipSizes[] = {5, 4, 3, 2, 1};
  int shipCount = sizeof(shipSizes) / sizeof(shipSizes[0]);

  for (int shipIndex = 0; shipIndex < shipCount; shipIndex++) {
    bool shipPlaced = false;

    while (!shipPlaced) {
      int row = random(8);
      int col = random(8);
      int orientation = random(2); 

      bool canPlace = true;

      if (orientation == 0) { 
        if (col + shipSizes[shipIndex] > 8) {
          canPlace = false;
        } else {
          
          for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= shipSizes[shipIndex]; j++) {
              int rowIdx = row + i;
              int colIdx = col + j;
              if (rowIdx >= 0 && rowIdx < 8 && colIdx >= 0 && colIdx < 8) {
                if (board[rowIdx][colIdx] == 1) {
                  canPlace = false;
                  break;
                }
              }
            }
          }
        }
      } else { 
        if (row + shipSizes[shipIndex] > 8) {
          canPlace = false;
        } else {
        
          for (int i = -1; i <= shipSizes[shipIndex]; i++) {
            for (int j = -1; j <= 1; j++) {
              int rowIdx = row + i;
              int colIdx = col + j;
              if (rowIdx >= 0 && rowIdx < 8 && colIdx >= 0 && colIdx < 8) {
                if (board[rowIdx][colIdx] == 1) {
                  canPlace = false;
                  break;
                }
              }
            }
          }
        }
      }

      if (canPlace) {
        if (orientation == 0) { 
          for (int i = 0; i < shipSizes[shipIndex]; i++) {
            board[row][col + i] = 1;
          }
        } else {
          for (int i = 0; i < shipSizes[shipIndex]; i++) {
            board[row + i][col] = 1;
          }
        }

        shipPlaced = true;
      }
    }
  }
}

//Fatima, Ömer
void checkGameOver() {
      if(tries > 44){

      resetGame();
      }
      for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] == 1 && hit[row*8+col] == 0) {
                return;
            } 
              
            
        }
    }
        resetGameWin();
      
}
//Fatima, Ömer
void resetGameWin() {    
  String message = String(tries);
  client.println(clientUsername + " Tries: " + message); // Bilal, Oscar
  Serial.println("Sent message to server: " + message); // Bilal, Oscar
  lcd.clear();
  lcd.print("Congrats Win!");
  lcd.setCursor(0, 1);
  lcd.print("Total Tries:");
  lcd.print(tries);

  int letters[8][8] = {

  {1, 1, 1, 1, 1, 1, 1, 1}, 
  {1, 0, 0, 0, 0, 0, 0, 1}, 
  {1, 0, 0, 1, 1, 0, 0, 1}, 
  {1, 0, 1, 0, 0, 1, 0, 1}, 
  {1, 0, 0, 0, 0, 0, 0, 1}, 
  {1, 0, 1, 0, 0, 1, 0, 1}, 
  {1, 0, 0, 0, 0, 0, 0, 1},  
  {1, 1, 1, 1, 1, 1, 1, 1}  
};

    for (int i = 0; i < 8; i++) { 
      for (int j = 0; j < NUM_LEDS; j++) {
        if (letters[i][j] == 1) { 
          strip.setPixelColor(j, 255, 255, 0); 
        } else {
          strip.setPixelColor(j, 0, 0, 0); 
        }
        strip.show();
        delay(25);
      }
    }
    

  bool redButtonPressed = false;
  lcd.clear();
  lcd.print("Play again?");
  lcd.setCursor(0, 1);
  lcd.print("Click red button");
  delay(3000);

  while (!redButtonPressed) { 
    if (digitalRead(BUTTON_PIN5) == HIGH) {
      redButtonPressed = true;
      lcd.clear();
      strip.clear();
    }

  
  }

  restartProgram();
}


void resetGame() { // Ömer & Fatima 
    
  lcd.clear();
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
          lcd.print("Total Tries:");
          lcd.print(tries);
          delay(100);
          
  int letters[8][8] = {

  {1, 0, 0, 0, 0, 0, 0,1}, 
  {0, 1, 0, 0, 0, 0, 1,0}, 
  {0, 0, 1, 0, 0, 1, 0,0}, 
  {0, 0, 0, 1, 1, 0, 0,0}, 
  {0, 0, 0, 1, 1, 0, 0,0}, 
  {0, 0, 1, 0, 0, 1, 0,0}, 
  {0, 1, 0, 0, 0, 0, 1,0},  
  {1, 0, 0, 0, 0, 0, 0,1}  
};


    for (int i = 0; i < 8; i++) { 
      for (int j = 0; j < NUM_LEDS; j++) {
        if (letters[i][j] == 1) {
          strip.setPixelColor(j, 255, 0, 0);
        } else {
          strip.setPixelColor(j, 0, 0, 0); 
        }
      strip.show();
      delay(25);
      }
    }
    
      
     bool redButtonPressed = false;
 lcd.clear();
    lcd.print("Play again?");
    lcd.setCursor(0, 1);
          lcd.print("Click red button");
          delay(3000);
            
while (!redButtonPressed) {
    if (digitalRead(BUTTON_PIN5) == HIGH) {
      redButtonPressed = true;
      lcd.clear();
      strip.clear();
    }

  
  }

  restartProgram();

}

