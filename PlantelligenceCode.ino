#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//Include all the libraries for this project
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int pumpPin = 7;
const int moistureSensorPin = A1;
//Initialize pin variables for hardware components

double checkInterval = 1800;
//This is the time between each check in miliseconds

int amountToPump = 300;
//This is the amount of time the pump will turn on for in miliseconds

int moistureSensorValue = 0;
//Initialize the moisture sensor val as 0

bool enoughwater = true;

int const highmoisture = 400;
int const lowmoisture = 470;
int const drylevel = 450;

void setup() {
  Serial.begin(9600);
  //Start the serial monitor

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  pinMode(pumpPin, OUTPUT);
  pinMode(moistureSensorPin, INPUT);
  //Setup the function of each pin 

  digitalWrite(pumpPin, HIGH);
  //Set the initial state of the pump pin HIGH
  //(For our project, we are using a relay with the pump connected to the common pin and the normally open pin therefore we must reverse the HIGH and LOW output. For example if we want the pump to turn on, we must write LOW instead of HIGH.)


}
void screen() { //used to display the text onto the screen
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Water Level:");
  if (enoughwater) { // check if there is enough water in tank
    display.setCursor(110 + 12 - (String("Good").length() - 1) * 6, 0);
    //Finding the length of the word and making sure it aligns to the left side
    display.print("Good");
  }
  else {
    display.setCursor(110 + 12 - (String("Low").length() - 1) * 6, 0);
    //Finding the length of the word and making sure it aligns to the left side
    display.print("Low");
  }
  display.setCursor(0, 40);
  display.print("Moisture Level:");
  display.setCursor(110 + 12 - (String(moistureSensorValue).length() - 1) * 6, 40);
  //Finding the length of the word and making sure it aligns to the left side
  if (moistureSensorValue > lowmoisture) {
    moistureSensorValue = lowmoisture;
  }
  else if (moistureSensorValue < highmoisture) {
    moistureSensorValue = highmoisture;
  }
  display.print(String( (int) (((double)lowmoisture - (double)moistureSensorValue) / ((double)lowmoisture - (double)highmoisture) * 100.0)) + "%");
  //used to find the percentage value of moisture using the max and min values

}
void loop() {
  moistureSensorValue = analogRead(moistureSensorPin);
  //Read a new moisture level value at the start of each loop iteration
  Serial.print("Soil moisture sensor is currently: ");
  Serial.print(moistureSensorValue);
  delay(1000);
  screen();
  display.display();
  // update the display
  if (moistureSensorValue > drylevel) {
    digitalWrite(pumpPin, LOW);
    //Tell the pump to turn on
    Serial.println("pump on");
    delay(amountToPump);
    digitalWrite(pumpPin, HIGH);
    //Tell the pump to turn off
    Serial.println("pump off");
    delay(800);
    // turn the pump on and off

  }
  moistureSensorValue = analogRead(moistureSensorPin);
  if (moistureSensorValue > drylevel) {
    enoughwater = false;
  }
  else {
    enoughwater = true;
  }
  //check if pump works and if not, display on screen water level is low
  delay(checkInterval);
  //Delay for the set interval
}
