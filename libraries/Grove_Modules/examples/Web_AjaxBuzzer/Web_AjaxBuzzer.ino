/* Web_Buzzer.pde - example sketch for Webduino library */

#include "SPI.h"
#include "WiFi.h"
#include "WebServer.h"

//your network name and password
char ssid[] = "COAP-STATION";
char password[] = "depot05101";

// CHANGE THIS TO YOUR OWN UNIQUE VALUE
static uint8_t mac[6] = { 0x02, 0xAA, 0xBB, 0xCC, 0x00, 0x22 };

// CHANGE THIS TO MATCH YOUR HOST NETWORK
static uint8_t ip[4] = { 192, 168, 1, 210 }; // area 51!

/* all URLs on this server will start with /buzz because of how we
 * define the PREFIX value.  We also will listen on port 80, the
 * standard HTTP service port */
#define PREFIX "/cc3200"
WebServer webserver(PREFIX, 80);

/* the piezo speaker on the Danger Shield is on PWM output pin #3 */
#define BUZZER_PIN 24

/* this is the number of microseconds to wait after turning the
 * speaker on before turning it off. */
int buzzDelay = 0;

/* toggle is used to only turn on the speaker every other loop
iteration. */
char toggle = 0;

/* This command is set as the default command for the server.  It
 * handles both GET and POST requests.  For a GET, it returns a simple
 * page with some buttons.  For a POST, it saves the value posted to
 * the buzzDelay variable, affecting the output of the speaker */
void buzzCmd(WebServer &server, WebServer::ConnectionType type, char *, bool)
{
  if (type == WebServer::POST)
  {
    bool repeat;
    char name[16], value[16];
    do
    {
      /* readPOSTparam returns false when there are no more parameters
       * to read from the input.  We pass in buffers for it to store
       * the name and value strings along with the length of those
       * buffers. */
      repeat = server.readPOSTparam(name, 16, value, 16);

      /* this is a standard string comparison function.  It returns 0
       * when there's an exact match.  We're looking for a parameter
       * named "buzz" here. */
      if (strcmp(name, "buzz") == 0)
      {
	/* use the STRing TO Unsigned Long function to turn the string
	 * version of the delay number into our integer buzzDelay
	 * variable */
        buzzDelay = strtoul(value, NULL, 10);
      }
      /* handle led */
      else if(strcmp(name, "red_led") == 0)
      {
          int16_t state = strtoul(value, NULL, 10);
          boolean pin_status = (state == 1) ? HIGH : LOW; 
          digitalWrite(RED_LED, pin_status);
          Serial.println("red button\n");
          Serial.println(value);
      }
    } while (repeat);
    
    // after procesing the POST data, tell the web browser to reload
    // the page using a GET method. 
    server.httpSeeOther(PREFIX);
    return;
  }

  /* for a GET or HEAD, send the standard "it's all OK headers" */
  server.httpSuccess();

  /* we don't output the body for a HEAD request */
  if (type == WebServer::GET)
  {
    /* store the HTML in program memory using the P macro */
    P(message) = 
"<!DOCTYPE html><html><head>"
  "<title>Grove Starter Kit For CC3200 LaunchPad</title>"
  "<link href='https://code.jquery.com/ui/1.11.1/themes/smoothness/jquery-ui.css' rel=stylesheet />"
  "<script src='https://code.jquery.com/jquery-1.11.1.min.js'></script>"
  "<script src='https://code.jquery.com/ui/1.11.1/jquery-ui.min.js'></script>"
  "<style> #slider { margin: 10px; } </style>"
  "<style type='text/css'>"
	"a {"  
        "background-color: #dcdcdc;"		
		"cursor: pointer;"
		"border-top: solid 2px #eaeaea;"
		"border-left: solid 2px #eaeaea;"
		"border-bottom: solid 2px #777;"
		"border-right: solid 2px #777;"
		"padding: 5px 5px;   	"
		"}"
	"a.down {"
        "background-color: #dc143c;"		
		"border-top: solid 2px #777;"
		"border-left: solid 2px #777;"
		"border-bottom:solid 2px  #eaeaea;"
		"border-right: solid 2px #eaeaea;"
		"}"
  "</style>"

  "<script>"
    "var state = 1;"
    "function changeBuzz(event, ui) { $('#indicator').text(ui.value); $.post('/cc3200', { buzz: ui.value } ); }"    
    "$(document).ready(function(){"
	  "$('a#button').click(function(){" 
	      "$(this).toggleClass('down');" 		  		   
	      "$.post('/cc3200', { red_led: state } );"		  
	      "if(state == 0)" 
	          "{state = 1;}"
              "else if(state == 1)"
                  "{state = 0;}"
	  "});"
	  "$('#slider').slider({min: 0, max:8000, change:changeBuzz});"
	"});"
  "</script>"
"</head>"
"<body style='font-size:62.5%;'>"
  "<h1>Control the Device on LaunchPad:</h1>"
  "<div id=slider></div>"
  "<p id=indicator>0</p>"
  "<a id=button title='button'>Red Led</a>"
"</body>"
"</html>";
    /* send the html back to the browser */
    server.printP(message);
  }
}

void setup()
{
  Serial.begin(9600);
  // set the PWM output for the buzzer to out
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // setup the Ehternet library to talk to the Wiznet board
  Serial.print("Attempting to connect to Network:");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(300);
  }
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  // you're connected now, so print out the status  
  printWifiStatus();
  
  /* register our default command (activated with the request of
   * http://x.x.x.x/cc3200 */
  webserver.setDefaultCommand(&buzzCmd);

  /* start the server to wait for connections */
  webserver.begin();
}

void loop()
{
  // process incoming connections one at a time forever
  webserver.processConnection();

  /* every other time through the loop, turn on and off the speaker if
   * our delay isn't set to 0. */
  if ((++toggle & 1) && (buzzDelay > 0))
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delayMicroseconds(buzzDelay);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("Network Name: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
