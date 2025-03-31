#include <RH_ASK.h> // Include RadioHead Amplitude Shift Keying Library
#include <SPI.h> // Include dependant SPI Library, Not in use, only for compile

RH_ASK rf_driver; // Create Amplitude Shift Keying Object

const char *FixedKey = "palclada"; //Fixed key

bool palcak[8] = {0, 0, 0, 0, 0, 0, 0, 0};


void setup()
{
  rf_driver.init(); // Initialize ASK Object
  Serial.begin(9600); // Setup Serial Monitor
}


void loop()
{
  palcak_allapota();

  delay(10);
}



void palcak_allapota()
{
  uint8_t buf[17]; // Buffer for 8 (key) + 8 (data) + null terminator
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen))
  {
    // Message received with valid checksum
    buf[buflen] = '\0'; // Ensure null terminator

    // Separate key and data from the received message
    char key[9]; // 8-character key + null terminator
    char data[9]; // 8-character data + null terminator

    strncpy(key, (char *)buf, 8);
    key[8] = '\0';

    strncpy(data, (char *)buf + 8, 8);
    data[8] = '\0';

    // Print key and data
    Serial.print("Key: ");
    Serial.println(key);
    Serial.print("Data: ");
    Serial.println(data);


    // Check if the received key matches the fixed key
    if(strcmp(key, FixedKey) == 0)
    {
      for(int i = 0; i < 8; i++)
      {
        if(data[i] == '1') palcak[i] = 1;
        if(data[i] == '0') palcak[i] = 0;
      }
    }
    else
    {
      // Incorrect key, ignore the message
      Serial.println(F("Incorrect Key. Ignoring the message."));
    }

    for(int i=0; i<sizeof(palcak); i++)
    {
      Serial.print(i+1);
      Serial.print(". palca: ");
      Serial.println(palcak[i]);
    }
    Serial.println("");
  }

}