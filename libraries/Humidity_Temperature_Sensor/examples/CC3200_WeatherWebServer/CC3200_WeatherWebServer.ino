///
/// @mainpage	CC3200 Weather WebServer
///
/// @details	CC3200 Weather Web Server with responsive design
///
/// @n	Based on WiFi Web Server
///
/// A simple web server that shows the value of the analog input pins.
/// using a WiFi shield.
///
/// This example is written for a network using WPA encryption. For
/// WEP or WPA, change the Wifi.begin() call accordingly.
///
/// @n	Circuit:
/// * CC3200 LaunchPad
/// * Grove BoosterPack
/// * Grove Temperature and humidy Sensor Pro
///
/// @n	Release history
/// * created 13 July 2010 by dlf (Metodo2 srl)
/// * modified 31 May 2012 by Tom Igoe
/// * Ported to HTML5 by Rei Vilo, May 20, 2015, CC = BY SA NC
/// @n
/// @n
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
///
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
/// @date		18/05/2015 19:30
/// @version	101
///
/// @copyright	(c) Rei Vilo, 2015
/// @copyright	All rights reserved
///
/// @see		ReadMe.txt for references
///

///
/// @file	CC3200_WebServer.ino
///
/// @details	Weather Web Server
///

// Core library for code-sense - IDE-based
#if defined(MBED) // mbed specific
#include "mbed.h"
#elif defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(ROBOTIS) // Robotis specific
#include "libpandora_types.h"
#include "pandora.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(RFDUINO) // RFduino specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined or not supported
#endif // end IDE


// Include application, user and local libraries
#include <SPI.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include "DHT.h"
#include "stdarg.h"

// Prototypes
void printWifiStatus();
void printTemperatureHumidity();
boolean endsWith(char* inString, char* compString);
String formatString(const char * format, ... );


// Define variables and constants
///
/// @brief      Name of the LED
/// @details	Each board has a LED but connected to a different pin
///
uint8_t myLED;

///
/// @brief	DHT sensor by Grove
/// @param	24 pin
/// @param	DHT22 sensor tupe
///
DHT myDHT(24, DHT22);            /* temperature&humidity sensor object */

///
/// @brief	SSID = network name
///
char ssid[] = "SSID";

///
/// @brief	Network password
///
char password[] = "password";

///
/// @brief	Finite State Machine for requests
///
uint8_t fsm = 0;

///
/// @brief	<#Description#>
///
IPAddress remoteIP;

///
/// @brief	Values from sensor
///
int temperatureX10, ///< temperature in C x10
	humidityX10; ///< relative humidity in % x10


///
/// @brief	WiFiServer
/// @param	80 port
///
WiFiServer server(80);


///
/// @brief	Setup
///
void setup()
{
    // 1. Serial
    Serial.begin(115200);
    
    // 2. LEDs
    pinMode(RED_LED, OUTPUT);      // set the LED pin mode
    pinMode(GREEN_LED, OUTPUT);      // set the LED pin mode
    pinMode(YELLOW_LED, OUTPUT);      // set the LED pin mode
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    
    // 3. Connect to WiFi network
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while ( WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    
    Serial.println("\nYou're connected to the network");

    // 3. Get IP address
    Serial.println("Waiting for an ip address");
    while (WiFi.localIP() == INADDR_NONE)
    {
        Serial.print(".");
        delay(300);
    }
    
    // 4. Console output
    printWifiStatus();
    
    Serial.println("Starting webserver on port 80");
    server.begin();                           // start the web server on port 80
    Serial.println("Webserver started!");
    
    Serial.println("GREEN  webserver available");
    Serial.println("YELLOW connection");
    Serial.println("RED    managed by user");
    
    
    digitalWrite(GREEN_LED, HIGH);
    myDHT.begin();
}


void loop()
{
    int i = 0;
    WiFiClient client = server.available();   // listen for incoming clients
    
    if (client)
    {                             // if you get a client,
        Serial.println("new client");           // print a message out the serial port
        char buffer[150] = {0};                 // make a buffer to hold incoming data
        while (client.connected())
        {            // loop while the client's connected
            if (client.available())
            {             // if there's bytes to read from the client,
                digitalWrite(YELLOW_LED, HIGH);
                char c = client.read();             // read a byte, then
                Serial.write(c);                    // print it out the serial monitor
                if (c == '\n')
                {                    // if the byte is a newline character
                    
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (strlen(buffer) == 0) {
                        // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                        // and a content-type so the client knows what's coming, then a blank line:
                        
                        // HTML5
                        client.println("<!DOCTYPE html>");

                        client.println("<html>");
                        client.println("<head>");
                        client.println("<title>Weather WiFi Web Server</title>");
                        
                        // Responsive design
                        client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
                        
                        // Styles, CCS in HTML5
                        client.println("<style>");
                        client.println("h1 { color: red; font-family: Arial, Helvetica, sans-serif; text-align: center; }");
                        client.println("p { font-family: Arial, Helvetica, sans-serif; text-align: center; }");
                        client.println("table { margin: 0px auto; }");
                        client.println("td { font-family: Arial, Helvetica, sans-serif; text-align: left; }");
                        client.println("</style>");
                        client.println("</head>");

                        // Body
                        client.println("<body>");
                        client.println("<h1 >Weather WiFi Web Server</font></h1>");
                        client.println("<p>LED <button onclick=\"location.href='/H'\">HIGH</button>");
                        client.println("<button onclick=\"location.href='/L'\">LOW</button></p>");
                        client.println("<p><button onclick=\"location.href='/A'\">About</button>");
                        client.println("<button onclick=\"location.href='/W'\">Weather</button></p><br>");
                        
                        client.println();
                        
                        // Execute FSM
                        switch (fsm)
                        {
                            case 1:
                                client.println("<p>LED is on</p>");
                                break;
                                
                            case 2:
                                client.println("<p>LED is off</p>");
                                break;
                                
                            case 3:
                                client.print("<table><tr>");
                                client.print("<td>Local IP</td><td>");
                                client.print(WiFi.localIP());
                                client.print("</td></tr><tr>");
                                client.print("<td>Remote IP</td><td>");
//                                client.print(WiFi.remoteIP());
                                client.print("#NA");
                                client.print("</td></tr><tr>");
                                client.print("<td>Gateway IP</td><td>");
                                client.print(WiFi.gatewayIP());
                                client.print("</td></tr></table>");
                                
                                break;
                            
                            case 4:
                                client.print("<table><tr>");
                                client.print("<td>Temperature</td>");
                                client.print(formatString("<td>%i.%i &deg;C</td>", temperatureX10 /10, temperatureX10 %10));
                                client.print("</tr><tr>");
                                client.print("<td>Humidity</td>");
                                client.print(formatString("<td>%i.%i %%</td>", humidityX10 /10, humidityX10 %10));
                                client.print("</tr></table>");
                                break;
                                
                            default:
                                break;
                        }
                        client.println("</body>");
                        client.println("</html>");
                        fsm = 0;
                        break;
                    }
                    else {      // if you got a newline, then clear the buffer:
                        memset(buffer, 0, 150);
                        i = 0;
                    }
                }
                else if (c != '\r') {    // if you got anything else but a carriage return character,
                    buffer[i++] = c;      // add it to the end of the currentLine
                }
                
                // Check request and set fsm
                if (endsWith(buffer, "GET /H"))
                {
                    digitalWrite(RED_LED, HIGH);
                    fsm = 1;
                }
                else if (endsWith(buffer, "GET /L"))
                {
                    digitalWrite(RED_LED, LOW);
                    fsm = 2;
                }
                else if (endsWith(buffer, "GET /W"))
                {
                    temperatureX10 = myDHT.readTemperature() *10;
                    humidityX10 = myDHT.readHumidity() *10;
                    
                    fsm = 4;
                }
                else if (endsWith(buffer, "GET /A"))
                {
                    fsm = 3;
                    remoteIP = WiFi.getLatestDevice();
                    Serial.print(">>>");
                    Serial.println(remoteIP);
                    
                    for (uint8_t i=0; i<MAX_SOCK_NUM; i++)
                    {
                        Serial.print(i, DEC);
                        Serial.print("\t");
                        Serial.println("\t");
                    }
                    
                    /*
                     client.print("<b>Remote IP: </b>");
                     
                     
                     
                     IPAddress myAddress = client.  .remoteIP();
                     //        client.print(myAddress[0]);
                     //        client.print(".");
                     //        client.print(myAddress[1]);
                     //        client.print(".");
                     //        client.print(myAddress[2]);
                     //        client.print(".");
                     //        client.print(myAddress[3]);
                     //        client.println();
                     client.print(myAddress);
                     client.print("</p>");
                     */
                }
            
            } // end client.available
        } // end client.connected
        digitalWrite(YELLOW_LED, LOW);
        // close the connection:
        client.stop();
        Serial.println("client disconnected");
        Serial.println("----");
    } // end client
}

void printWifiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("Network Name: ");
    Serial.println(WiFi.SSID());
    
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    
    uint8_t macTemp[6];
    
    if (WiFi.macAddress(macTemp) > 0)
    {
        Serial.println(formatString("MAC address: %x:%x:%x:%x:%x:%x", macTemp[0], macTemp[1], macTemp[2], macTemp[3], macTemp[4], macTemp[5]));
    }
    
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}

//
//a way to check if one array ends with another array
//
boolean endsWith(char* inString, char* compString)
{
    int compLength = strlen(compString);
    int strLength = strlen(inString);
    
    //compare the last "compLength" values of the inString
    int i;
    for (i = 0; i < compLength; i++) {
        char a = inString[(strLength - 1) - i];
        char b = compString[(compLength - 1) - i];
        if (a != b) {
            return false;
        }
    }
    return true;
}

///
/// @brief      Format string
/// @details
/// @param      format format with standard codes
/// @param      ... list of values
/// @return     string with values formatted
/// @see        http://www.cplusplus.com/reference/cstdio/printf/?kw=printf for codes
///
String formatString(const char * format, ... )
{
    char bufferOut[128];

    va_list args;
    va_start(args, format);
    vsprintf(bufferOut, format, args);
    va_end(args);
    return String(bufferOut);
}

