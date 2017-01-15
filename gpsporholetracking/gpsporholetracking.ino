//Libraries needed

#include <NMEAGPS.h> // GPS parser library
#include "GPSport.h"// GPS parser library
#include <GPSfix_cfg.h>// GPS parser library
#include <Adafruit_NeoPixel.h>// neopixel library
#include "botones.h"// library to use with buttons
#include "gps_report.h"// data structure
#include "fram.h"// library to the FRAM 

//Some "constansts"
#define NeoPixelPin 8 //this is the pin of the Neopixel Led
#define totalButtons 3 // total of buttons that attached to the Flora
#define NOTE_G5  784 // frecency to use with the buzzer

BUTTON buttons[totalButtons] = { BUTTON(12), BUTTON(6), BUTTON(9) };//initialize the buttons in the array
FRAM fram(0x50);// send the i2c addres to initialize it
GPS_REPORT report;// create a data strucutre to save the GPS info lat, long, timestamp, HDOP and the event or button pushed
/*GPS_REPORT data structure
   int32_t lat;
  int32_t lon;
  clock_t timestamp;
  uint16_t hdop;
  uint8_t numbutton;
  uint8_t reserved;
*/

DEVICE_INFO deviceinfo;//create a data structur to save the device info ID and number of registeres in the FRAM
/*DEVICE_INFO data structure
  int16_t ID;
  int16_t numreg;
  int32_t res1;
  int32_t res2;
  int32_t res3;

*/
Adafruit_NeoPixel led = Adafruit_NeoPixel(1, NeoPixelPin, NEO_GRB + NEO_KHZ800);//initizalize the Neopixel Led
static NMEAGPS  gps; // This parses the GPS characters
boolean serialdata = false; // turns true when serial communication is available
gps_fix fix; //gps object that store and parse the data from the gps module


void setup() {
  led.begin();//initialize the neopixel led
  led.show();// turn off the led if is on.
  Serial.begin(9600);// begin the serial communication on 9600 bauds to transmit
  gps_port.begin(9600);// begin the serial communication on 9600 bauds to receive from the GPS
  // initialize the buttons.
  buttons[0].begin();
  buttons[1].begin();
  buttons[2].begin();
  fram.begin();  //initialize the Fram memory
  fram.read(0x0, &deviceinfo, sizeof(deviceinfo));// read the device info data from FRAM beginning in 0x0 ant ends in 0x0f
  Serial.println(deviceinfo.ID);
  Serial.println(deviceinfo.numreg);
}

void loop()
{

  
  if (!Serial) {
    static int32_t t_lect = 0;
    int32_t t_act = millis();
    if (t_act - t_lect >= 10) {//each 10 miliseconds read the buttons states
      for (int i = 0; i < totalButtons; i++) {
        buttons[i].read();
      }
      t_lect += 10;
    }//end if t_act - t_lect >= 10
    for (int i = 0; i < totalButtons; i++) {// read the state of  the buttons
      if (buttons[i].pressed()) {
        if (fix.valid.location) {//if there's a valid location save the data on FRAM else do nothing
          saveData(i);
        }//end if fix valid location
     //   printData();
        on_off_Neopixel(true, 127, 0, 127, NOTE_G5);// turn on the led and play a note in the buzzer
      }//end if buttons.pressed
    }
    GPSLoop();
  } else {
    if (!serialdata) {
      Serial.println("Hola Bienvenido");
      Serial.println("1. leer Datos");
      Serial.println("2. borrar Datos");
      serialdata = true;
    }
    if (Serial.available() > 0) {
      char Data = Serial.read();
      if (Data == '1') {
        printData();
      } else if (Data == '2') {
        Serial.println("borrado");
        
        printData();
      } else {
        Serial.println("incorrecto Vuelva a intentar");
        Serial.println("Hola Bienvenido");
        Serial.println("1. leer Datos");
        Serial.println("2. borrar Datos");
      }
    }
  }
}

//-------- saveData --------//
/*
 * This function receive an uint8_t that represents the button was pressed
 * save the data on the FRAM and update the numreg byte by 1
 */

void saveData(uint8_t buttonpressed) {
  report.lat = fix.latitudeL();
  report.lon = fix.longitudeL();
  report.timestamp = fix.dateTime;
  report.hdop = fix.hdop;
  report.numbutton = buttonpressed;
  report.reserved = 0;
  //LLenar el reporte primero, luego se escribe asi:
  fram.write((deviceinfo.numreg + 1) * sizeof(report), &report, sizeof(report));
  fram.read((deviceinfo.numreg + 1) * sizeof(report), &report, sizeof(report));
  deviceinfo.numreg += 1;
  fram.write(2, &deviceinfo.numreg, sizeof(deviceinfo.numreg));
  fram.read(2, &deviceinfo.numreg, sizeof(deviceinfo.numreg));

}

//-------- GPSLoop --------//
/*
this function capture the Serial communication from GPS module parse the data and save it in fix
 */
void GPSLoop() {

  while (gps.available( gps_port )) {
    fix = gps.read();
    if (fix.valid.location) {
      on_off_Neopixel(true, 0, 127, 0, 0);
    } else {
      on_off_Neopixel(false, 127, 0, 0, 0);
      Serial.println("No encontrado");
    }
  }
}

//-------- on_off_Neopixel --------//
/*
this function receives a boleean to turn on the neopixel send a true value else a false to turn off
the data in RGB to show a color and a play tone for the buzzer

 */

void on_off_Neopixel(uint8_t state, uint8_t red, uint8_t green, uint8_t blue, uint16_t playTone) {

  if (state) {

    tone(10, playTone, 500);
    led.setPixelColor(0, led.Color(red, green, blue));
    led.show();
  } else {

    noTone(10);
    led.setPixelColor(0, led.Color(0, 0, 0));
    led.show();
  }
}
//-------- printData --------//
/*
this function print the data in the serial communication with the pc 
in json format


 */

void printData() {


  Serial.print("{\"device\": {\"id\":");
  Serial.print(deviceinfo.ID);
  Serial.print(", ");
  Serial.print("\"numreg\":");
  Serial.print(deviceinfo.numreg);
  Serial.print(", ");
  Serial.print("\"data\":[");
  for (int i = 0; i < deviceinfo.numreg; i++) {
    fram.read((i + 1) * sizeof(report), &report, sizeof(report));
    Serial.print('{');
    Serial.print("\"lat\":");
    Serial.print(report.lat);
    Serial.print(", ");
    Serial.print("\"long\":");
    Serial.print(report.lon);
    Serial.print(", ");
    Serial.print("\"timestamp\":");
    Serial.print(report.timestamp);
    Serial.print(", ");
    Serial.print("\"hdop\":");
    Serial.print(report.hdop);
    Serial.print(", ");
    Serial.print("\"button\":");
    Serial.print(report.numbutton);
    Serial.print('}');
    if (i < deviceinfo.numreg - 1) {
      Serial.print(", ");
    } else {

    }
  }
  Serial.print("] } } \n");
}



