// DEBUG_OFF
// DEBUG_STATES
// DEBUG_DATA
// DEBUG_RFID
#define DEBUG_LEVEL DEBUG_RFID

#include <SPI.h>
#include <MFRC522.h>

#ifndef HARDWARE_SETUP_COMMON_INCLUDED
#include "hardware_setup.h"
#endif

#ifndef SOFTWARE_SETUP_COMMON_INCLUDED
#include "software_setup.h"
#endif


MFRC522 mfrc522[NUM_OF_RFID_READERS];

byte component_flags;
byte component_flags_target = FLAG_BOOK | FLAG_VIAL | FLAG_SNITCH | FLAG_TICKET;

void setup() {
#if DEBUG_LEVEL >= 1
  Serial.begin(115200);
  Serial.println();
  Serial.println("RESTARTED");
  Serial.println();
#endif
  SPI.begin();

  for (byte i = 0; i < NUM_OF_RFID_READERS; i++) {
#if DEBUG_LEVEL >= 1
    print_rfid_debug_text_initial(i);
#endif
    init_rfid_reader(i, true);
  }

  pinMode(INDUCTION_SENSOR_PIN, INPUT);

  pinMode(MAGNET_PIN, OUTPUT);
  pinMode(FEEDBACK_LED_PIN, OUTPUT);

  close_magnet();
}

void loop() {
  delay(MAIN_LOOP_INTERVAL_MS);

  component_flags = 0;

  if (is_correct_tag_present(RFID_READER_INDEX_BOOK, RFID_TAG_ID_OF_BOOK)) {
    component_flags |= FLAG_BOOK;
  }

  if (is_correct_tag_present(RFID_READER_INDEX_VIAL, RFID_TAG_ID_OF_VIAL)) {
    component_flags |= FLAG_VIAL;
  }

  if (is_correct_tag_present(RFID_READER_INDEX_SNITCH, RFID_TAG_ID_OF_SNITCH)) {
    component_flags |= FLAG_SNITCH;
  }

  if (is_ticket_placed()) {
    component_flags |= FLAG_TICKET;
  }

  feedback_led_blink(component_flags & (FLAG_BOOK | FLAG_VIAL | FLAG_SNITCH));

#if DEBUG_LEVEL >= 1
  Serial.println();

  if (component_flags & FLAG_BOOK) {
    Serial.println("Book is placed.");
  }

  if (component_flags & FLAG_VIAL) {
    Serial.println("Vial is placed.");
  }

  if (component_flags & FLAG_SNITCH) {
    Serial.println("Snitch is placed.");
  }

  if (component_flags & FLAG_TICKET) {
    Serial.println("Ticket is placed.");
  }

  Serial.println();
#endif

  if (component_flags == component_flags_target) {
    open_magnet();
#if DEBUG_LEVEL >= 1
    Serial.println("Magnet is opened.");
    Serial.println();
#endif

    delay(MAGNET_OPEN_INTERVAL_MS);

    close_magnet();
#if DEBUG_LEVEL >= 1
    Serial.println("Magnet is closed.");
    Serial.println();
#endif
  }
}

void inline print_rfid_debug_text_initial(byte reader_index) {
  Serial.print("Reader ");
  Serial.print(reader_index + 1);
  Serial.print(": ");
}

void init_rfid_reader(byte reader_index, bool with_version) {
  mfrc522[reader_index].PCD_Init(RFID_READER_PINS[reader_index], RFID_READER_RESET_PIN);
  delay(RFID_READER_INIT_INTERVAL_MS);

#if DEBUG_LEVEL >= 1
  if (with_version) {
    mfrc522[reader_index].PCD_DumpVersionToSerial();
    delay(DEBUG_MESSAGE_DELAY_INTERVAL_MS);
  }
#endif
}

void init_rfid_reader(byte reader_index) {
  init_rfid_reader(reader_index, false);
}

String get_rfid_tag_id_from_buffer(byte *buffer, byte bufferSize) {
  String tag_id = "";

  for (byte i = 0; i < bufferSize; i++) {
    tag_id += String(buffer[i], DEC);
  }

#if DEBUG_LEVEL >= 3
  Serial.print("Raw tag id: \"");
  Serial.println(tag_id + "\"");
#endif

  return tag_id;
}

bool is_correct_tag_present(byte reader_index, String correct_tag_id) {
  String tag_id;

#if DEBUG_LEVEL >= 1
  print_rfid_debug_text_initial(reader_index);
#endif
  init_rfid_reader(reader_index);

  if (mfrc522[reader_index].PICC_IsNewCardPresent() && mfrc522[reader_index].PICC_ReadCardSerial()) {
    tag_id = get_rfid_tag_id_from_buffer(mfrc522[reader_index].uid.uidByte, mfrc522[reader_index].uid.size);

    mfrc522[reader_index].PICC_HaltA();       // Halt PICC
    mfrc522[reader_index].PCD_StopCrypto1();  // Stop encryption on PCD

    return (tag_id == correct_tag_id);
  } else {
#if DEBUG_LEVEL >= 2
    Serial.println("no tag found.");
    delay(DEBUG_MESSAGE_DELAY_INTERVAL_MS);
#endif
  }

  return false;
}

bool is_ticket_placed() {
  return (METAL_FOUND == digitalRead(INDUCTION_SENSOR_PIN));
}

void open_magnet() {
  digitalWrite(MAGNET_PIN, MAGNET_OPENED);
}

void close_magnet() {
  digitalWrite(MAGNET_PIN, MAGNET_CLOSED);
}

void feedback_led_blink(byte times) {
  for (byte i = 0; i < times; i++) {
    digitalWrite(FEEDBACK_LED_PIN, HIGH);
    delay(LED_BLINK_ON_MS);
    digitalWrite(FEEDBACK_LED_PIN, LOW);
    delay(LED_BLINK_ON_MS);
  }
}