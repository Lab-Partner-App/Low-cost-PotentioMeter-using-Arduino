#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // Uncomment if using an I2C LCD
// RX from HM-10 -> Pin 10, TX from HM-10 -> Pin 11
SoftwareSerial BTSerial(10, 11); // RX, TX

int aRead;
float avgVoltage, voltage, sumVoltage;

void setup() {

  Serial.begin(9600);      // Start Serial Monitor communication
  BTSerial.begin(9600);    // Initialize HM-10 communication

  delay(1000);  // Wait for the HM-10 to initialize

  // Optional: Set a new name for the HM-10 (if not already configured)
  sendATCommand("AT+NAMEBT05");

  // Wait for any setup or connection delays
  delay(3000);  

  Serial.println("Bluetooth ready. Sending data...");

  // Uncomment if using an LCD display
   lcd.init();
   lcd.backlight();

   // Set A0 as input (for reading analog values)
  pinMode(A2, INPUT);
}

void loop() {

  // Calculate voltage based on analog read value
  for (int i = 1; i <= 5; i++) {
    //  Read the analog input
      aRead = analogRead(A2);
      Serial.print("Analog Read: ");
      Serial.println(aRead);
      Serial.println("..............");

      voltage = (aRead * 0.02348) - 0.34855;
      voltage = (voltage - 0.0144) / 22.5410;
      voltage += 0.0228+0.0780;
     
      sumVoltage += voltage;
      voltage =0;
     delay(10);
  }

    // Average the calculated voltagevv
      avgVoltage = sumVoltage / 5;
      //avgVoltage += 0.0228+0.0690;
    

    sendBluetoothMessage(avgVoltage); 


  // Uncomment if using an LCD display
    lcd.setCursor(0, 0);
    lcd.print("Voltage:");
    lcd.print(avgVoltage,4);
    
   sumVoltage = 0;  // Reset sum for the next loop
    
    avgVoltage=0;
    
    delay(200);  // Delay between readings
    
  

  //delay(5000);  // Send every 5 seconds (adjust as needed)
}

// Function to send AT commands (optional configuration)
void sendATCommand(const char *command) {
  Serial.print("Sending AT Command: ");
  Serial.println(command);
  BTSerial.println(command);
  delay(1000);  // Wait for the response
}

// Function to send a string message over Bluetooth
void sendBluetoothMessage(float message) {
  BTSerial.println(message,4);  // Send the message to the connected device
}



  