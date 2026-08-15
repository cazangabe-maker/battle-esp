
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
int frameindex = 0;
String wifi_message = "WiFi";
bool interact_pressed = false;  
int framedelayer = 500;
void rightbuttonhandler();
unsigned long rightpee = 0;
volatile int screenindex = 2;
unsigned long lastpee;
unsigned long lastrightbuttonclick = 0;
void interact_handler() {
  rightpee = millis();
  if (rightpee - lastpee >=200) {
    lastpee = rightpee;
    interact_pressed = true;
  }
  
}
unsigned long lastdelay = 0;
const unsigned char wifi_wifi [] PROGMEM = {
  0x7e, 0x81, 0x3c, 0x42, 0x18, 0x24, 0x00, 0x18
};
// 'wifi4', 8x8px
const unsigned char wifi_wifi4 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18
};
// 'wifi3', 8x8px
const unsigned char wifi_wifi3 [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x18
};
// 'wifi2', 8x8px
const unsigned char wifi_wifi2 [] PROGMEM = {
  0x00, 0x00, 0x3c, 0x42, 0x18, 0x24, 0x00, 0x18
};const unsigned char* frames[] = { wifi_wifi, wifi_wifi4, wifi_wifi3, wifi_wifi2};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
void setup() {
  Wire.begin(2,15);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // put your setup code here, to run once:
    display.clearDisplay();
    pinMode(12, INPUT_PULLUP);  
    attachInterrupt(digitalPinToInterrupt(12),rightbuttonhandler, FALLING);
    Serial.begin(9600);
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

void loop() { 
  if (screenindex > 1) {
    screenindex = 0;
  }
  
  switch(screenindex) {
    case 0:
      wifi_animation();
      draw_right_arrow();
      print_centered_message(SCREEN_HEIGHT/2 + 6, wifi_message);
      display.display();
      break;
    case 1:
      display.clearDisplay();
      draw_left_arrow();
      display.display();
      break;
    default:
      Serial.println("ERR: SCEEN INDEX GOT OUT OF RANGE");
      screenindex = 0;
  }
}
void wifi_animation() {
 if (millis() - lastdelay >= framedelayer) {
  lastdelay = millis();
  frameindex++;
  if (frameindex >= 4) {
    frameindex = 0;
  }
  show_wifi_frame(frameindex);
  if(frameindex == 0) {
    framedelayer = 1000;
  } else {
    framedelayer = 500;
  }
 }
}
unsigned long lasttime = 0;
void show_wifi_frame(int frame) {
  // this causes it to make the triangle have to be drawn again. probably better to just add display.clearDisplay() in the running code
  display.clearDisplay();
  display.drawBitmap(SCREEN_WIDTH/2 - 4 ,SCREEN_HEIGHT/2 - 4 ,frames[frame],8,8,1,0);
  //display.display();
}
void draw_left_arrow() {
  display.fillTriangle(8,20,8,12,4,16,WHITE);
}
void draw_right_arrow() {
  display.fillTriangle(120,20,120,12,124,16,WHITE);
}
void rightbuttonhandler() {
  unsigned long rightnow = millis();
  if (rightnow - lastrightbuttonclick >= 300) {
    lastrightbuttonclick = rightnow;
    screenindex += 1;
  }   
}
void print_centered_message(int y, String message) {
  int centeredx = (SCREEN_WIDTH - message.length() * 6) /2;
  display.setCursor(centeredx, y);
  display.print(message);
}
