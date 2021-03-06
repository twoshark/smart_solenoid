#include "solenoid_server.h"

WiFiServer server(80);

void SolenoidServer::begin()
{
  Serial.println("Begin Server...");
  server.begin(80);
}

/**
 * Server - Listen
 * 
 */
void SolenoidServer::Listen(Configuration config)
{
  String header;
  WiFiClient client = server.available(); // Listen for incoming clients

  if (client)
  {                                          // If a new client connects,
    Serial.println("New Client Connected."); // print a message out in the serial port
    String currentLine = "";                 // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        Serial.write(c);        // print it out the serial monitor
        header += c;
        if (c == '\n')
        { // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /output/on") >= 0)
            {
              Serial.println("Command: Output on");
              if (!this->output_state)
              {
                this->output_state = true;
                digitalWrite(config.output_pin, HIGH);
              }
            }
            else if (header.indexOf("GET /output/off") >= 0)
            {
              Serial.println("Output off");
              this->output_state = false;
              digitalWrite(config.output_pin, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Solenoid Controller</h1>");

            // Display current state, and ON/OFF buttons for the defined GPIO
            if (this->output_state)
            {
              client.println("<p>Output - State ON </p>");
            }
            else
            {
              client.println("<p>Output - State OFF </p>");
            }

            // If the this->output_state is off, it displays the ON button
            if (!this->output_state)
            {
              client.println("<p><a href=\"/output/on\"><button class=\"button\">ON</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/output/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          }
          else
          { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}