#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h> // Include dependant SPI Library, Not in use, only for compile
RH_ASK rf_driver; // Create Amplitude Shift Keying Object

#include <Wire.h> //Fot the I2C communication
byte I2C_data; // data received from I2C bus

const char *FixedKey = "palclada"; //Fixed key


void setup()
{
  rf_driver.init(); // Initialize ASK Object
  Serial.begin(9600); // Setup Serial Monitor
  Wire.begin(0x08); // Initialize I2C communication
  Wire.onReceive(dataRcv); // register an event handler for received data
}

void loop()
{
  char data[9];  // 8-character data + null terminator
  data[8] = '\0';

  // Read data from the I2C bus
  Wire.requestFrom(0x08, 8);
  if(I2C_data)
  {
    for (int i = 0; i < 8; i++)
    {
     data[i] = bitRead(I2C_data, 7-i) + '0'; // Convert numeric value to character string
    }

    // Combine fixed key and data into a single message
    char message[17];  // 8 (key) + 8 (data) + null terminator
    strcpy(message, FixedKey);
    strcat(message, data);

    // Send the message
    rf_driver.send((uint8_t *)message, strlen(message));
    rf_driver.waitPacketSent();

    Serial.println("Message Sent: ");
    Serial.println(message);
  }
  I2C_data = 0;

  delay(10);
}


//received data handler function
void dataRcv(int numBytes)
{
	while(Wire.available()) // read all bytes received
  {
  I2C_data = Wire.read();
	}
}