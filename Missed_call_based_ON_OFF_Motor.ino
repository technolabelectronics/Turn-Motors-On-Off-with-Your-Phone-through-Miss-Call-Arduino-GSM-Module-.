#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(9, 10); // RX, TX for the GSM module
int MotorPin = 13; // Motor connected to digital pin 13

void setup() {
    pinMode(MotorPin, OUTPUT); // Initialize the digital pin as an output
    gsmSerial.begin(9600);   // Set baud rate for the GSM module
    Serial.begin(9600);      // Set baud rate for serial communication with PC

    Serial.println("Ready...");
    delay(1000);

    // Turn off echo
    gsmSerial.println("ATE0");
    delay(1000);

    // Set SMS text mode
    gsmSerial.println("AT+CMGF=1");
    delay(1000);
}

void loop() {
    if (gsmSerial.available()) {
        String inputString = gsmSerial.readString();
        Serial.print(inputString); // You can see the GSM module output in the Serial Monitor

        // Check if the string contains "RING"
        if (inputString.indexOf("RING") != -1) {
            toggleMOTOR(); // Toggle the Motor state on receiving a call
            // Optionally, you can hang up the call
            gsmSerial.println("ATH"); // Send hang-up command to the GSM module
        }
    }
}

void toggleMOTOR() {
    static bool motorState = false;
    motorState = !motorState;
    digitalWrite(MotorPin, motorState ? HIGH : LOW);

    // Send SMS notification
    String smsMessage = motorState ? "Motor is ON" : "Motor is OFF";
    sendSMS("+918543053029", smsMessage); // Replace with your phone number
}

void sendSMS(String number, String text) {
    gsmSerial.println("AT+CMGS=\"" + number + "\"");
    delay(1000);
    gsmSerial.println(text);
    delay(1000);
    gsmSerial.write(26); // ASCII code for CTRL+Z
}
