#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

// Listen on default port 5555, the webserver on the Arduino
// will forward there all the HTTP requests for us.
BridgeServer server;
String startString;
long hits = 0;

void setup() {
  Serial.begin(9600);

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  server.listenOnLocalhost();
  server.begin();

  // get the time that this sketch started:
  Process startTime;
  startTime.runShellCommand("date");
  while (startTime.available()) {
    char c = startTime.read();
    startString += c;
  }
}

void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // read the command
    String command = client.readString();
    command.trim();        
    Serial.println(command);
    // is "value" command?
    if (command == "value") {

      // get the time from the server:
      Process time;
      time.runShellCommand("date");
      String timeString = "";
      while (time.available()) {
        char c = time.read();
        timeString += c;
      }
      Serial.println(timeString);
      int sensorValue = analogRead(A1);
      // print the value:
      client.print(sensorValue);
    }
    // Close connection and free resources.
    client.stop();
  }

  delay(30); // Poll every 50ms
}



