
#define ETH_CLK_MODE ETH_CLOCK_GPIO17_OUT
#define ETH_PHY_POWER 12

#include <ETH.h>

// Static IP
IPAddress ip( 192, 168, 0, 123 );    
IPAddress gateway( 192, 168, 0, 1 );
IPAddress subnet( 255, 255, 255, 0 );

// Web Server on port 80
WiFiServer server(80);

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  Serial.println("Serial port initialized");
  
  // Initializing ETH and webserver
  Serial.println("Starting ETH");

  ETH.begin();
  delay(1000);

  ETH.config(ip, gateway, subnet, gateway);
  delay(1000);
  ETH.setHostname("ESP32PoE");

  // Starting the web server
  Serial.println("Starting webserver");
  server.begin();
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(ETH.localIP());
  Serial.println(ETH.macAddress());
  Serial.println(ETH.getHostname());
}

// runs over and over again
void loop() {

  WiFiClient client = server.available();

  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        if (c == '\n' && blank_line) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();

            client.println("<!DOCTYPE HTML>");
            client.println("<!--HTML comment hello world-->");
            client.println("<html>");
            client.println("<head><title>OLIMEX ESP32-POE</title></head>");
            client.println("<body>");
            client.println("    <h1>OLIMEX ESP32-POE</h1>");
            client.println("    <h3>hello world!</h3>");
            client.println("</body>");
            client.println("</html>");
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
}   
