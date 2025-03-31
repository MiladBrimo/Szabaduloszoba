#define SOFTWARE_SETUP_COMMON_INCLUDED

#define DEBUG_OFF     0
#define DEBUG_STATES  1<<0
#define DEBUG_DATA    1<<1
#define DEBUG_RFID    1<<2

#define MAIN_LOOP_INTERVAL_MS 50
#define RFID_READER_INIT_INTERVAL_MS  50
#define MAGNET_OPEN_INTERVAL_MS 3000

#define FLAG_BOOK   1<<0
#define FLAG_VIAL   1<<1
#define FLAG_SNITCH 1<<2
#define FLAG_TICKET 1<<3

#define LED_BLINK_ON_MS 150

#define DEBUG_MESSAGE_DELAY_INTERVAL_MS 100


void inline print_rfid_debug_text_initial(byte reader_index);
void init_rfid_reader(byte reader_index, bool with_version);
void init_rfid_reader(byte reader_index);
String get_rfid_tag_id_from_buffer(byte *buffer, byte bufferSize);
bool is_correct_tag_present(byte reader_index, String correct_tag_id);
bool is_ticket_placed();
void open_magnet();
void close_magnet();
void feedback_led_blink(byte times);