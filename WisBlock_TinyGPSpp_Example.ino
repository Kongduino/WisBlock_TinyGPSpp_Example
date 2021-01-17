#undef min
#undef max
#include <string>
#include <TinyGPS++.h>
#include <SPI.h>
#include <U8g2lib.h> // https://github.com/olikraus/u8g2

using namespace std;
template class basic_string<char>; // https://github.com/esp8266/Arduino/issues/1136
// Required or the code won't compile!
/*
  Add the following in:
  ~/Library/Arduino15/packages/raknrf/tools/arm-none-eabi-gcc/9-2019q4/arm-none-eabi/include/c++/9.2.1/bits/basic_string.h
  namespace std _GLIBCXX_VISIBILITY(default) {
  _GLIBCXX_BEGIN_NAMESPACE_VERSION
  void __throw_bad_alloc() {}
  void __throw_length_error(char const*) {}
  }
*/

TinyGPSPlus gps;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
uint8_t gLed = 0, bLed = 255;
#define ss Serial1
bool validLoc = false;

void setup() {
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(BLUE_LED, LOW);
  Serial.begin(115200);
  delay(2000);
  Serial.flush();
  Serial.println("\nWisBlock TinyGPS++ test.");
  u8g2.begin();
  u8g2.clearBuffer(); // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font
  u8g2.drawStr(3, 15, "TinyGPS++");
  u8g2.sendBuffer();
  // GPS init
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);
  pinMode(34, OUTPUT);
  digitalWrite(34, 0);
  delay(1000);
  digitalWrite(34, 1);
  delay(1000);
  Serial1.begin(9600);
  while (!Serial1);
  Serial.println("GPS UART init ok!");
  u8g2.drawStr(90, 15, "[ok]");
  u8g2.drawStr(103, 61, "[x]");
  u8g2.sendBuffer();
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
}

void loop() {
  digitalWrite(GREEN_LED, gLed);
  digitalWrite(BLUE_LED, bLed);
  gLed = 255 - gLed;
  bLed = 255 - bLed;
  delay(100);
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo() {
  Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    u8g2.setDrawColor(0x00);
    for (uint8_t ix = 18; ix < 50; ix++) u8g2.drawHLine(0, ix, 128);
    u8g2.setDrawColor(0xFF);
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(" :: "));
    Serial.print(gps.location.lng(), 6);
    u8g2.drawStr(3, 32, to_string(gps.location.lat()).c_str());
    u8g2.drawStr(3, 48, to_string(gps.location.lng()).c_str());
    u8g2.sendBuffer();
    if (!validLoc) {
      validLoc = true;
      u8g2.drawStr(103, 61, "[o]");
      u8g2.sendBuffer();
    }
  } else {
    if (validLoc) {
      validLoc = false;
      u8g2.drawStr(103, 61, "[x]");
      u8g2.sendBuffer();
    }
    Serial.print(F("INVALID"));
  }
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    std::string s = to_string(gps.date.year());
    s.append("/");
    s.append(to_string(gps.date.month()));
    s.append("/");
    s.append(to_string(gps.date.day()));
    Serial.print(s.c_str());
    // u8g2.setDrawColor(0x00);
    // for (uint8_t ix = 41; ix < 63; ix++) u8g2.drawHLine(0, ix, 96);
    // u8g2.setDrawColor(0xFF);
    // u8g2.drawStr(3, 50, s.c_str());
    // u8g2.sendBuffer();
  } else {
    Serial.print(F("INVALID"));
  }
  Serial.print(F(" "));
  if (gps.time.isValid()) {
    Serial.print(F(" @ "));
     u8g2.setDrawColor(0x00);
    for (uint8_t ix = 50; ix < 63; ix++) u8g2.drawHLine(0, ix, 96);
     u8g2.setDrawColor(0xFF);
    std::string s;
    if (gps.time.hour() < 10) s = "0";
    s.append(to_string(gps.time.hour()));
    s.append(":");
    if (gps.time.minute() < 10) s.append("0");
    s.append(to_string(gps.time.minute()));
    s.append(":");
    if (gps.time.second() < 10) s.append("0");
    s.append(to_string(gps.time.second()));
    s.append(":");
    if (gps.time.centisecond() < 10) s.append("0");
    s.append(to_string(gps.time.centisecond()));
    Serial.println(s.c_str());
    u8g2.drawStr(3, 63, s.c_str());
    u8g2.sendBuffer();
  } else {
    Serial.println(F("INVALID"));
  }
}
