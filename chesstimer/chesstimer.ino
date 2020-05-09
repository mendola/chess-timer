// Pin definitions
#define VBATPIN       A9  // Measure battery voltage
#define DIGIT_EN_0    10  // Enable pin for Digit 0
#define DIGIT_EN_1    5  // Enable pin for Digit 0
#define DIGIT_EN_2    6  // Enable pin for Digit 0
#define DIGIT_EN_3    23  // Enable pin for Digit 0
#define LED_SEG_A     11  // Top LED Segment (see http://acoptex.com/project/95/basics-project-018b-4-digit-7-segment-display-modules-5641ah-or-3461bs-timer-at-acoptexcom/#sthash.WCXanRfB.iTY2iznD.dpbs)
#define LED_SEG_B     12  // Top Right LED
#define LED_SEG_C     19  // Bottom right LED (A1)
#define LED_SEG_D     21  // Bottom LED (A3)
#define LED_SEG_E     22  // Bottom Left LED (A4)
#define LED_SEG_F     13  // Top left LED
#define LED_SEG_G     18  // Center LED (A0)
#define LED_SEG_DP    20  // Dot/Period LED (A2)

#define PRINT_DEBUG 1

const uint8_t g_digit_mux[4] = {
DIGIT_EN_0,
DIGIT_EN_1,
DIGIT_EN_2,
DIGIT_EN_3};

const uint8_t g_led_seg_bitmask[8] = {
  LED_SEG_A, // bit index 0
  LED_SEG_B,
  LED_SEG_C,
  LED_SEG_D,
  LED_SEG_E,
  LED_SEG_F,
  LED_SEG_G,
};

typedef enum digit_ {
  NONE = 0b00000000,
  ZERO = 0b00111111,
  ONE =  0b00000110,
  TWO =  0b01011011,
  THREE =0b01001111,
  FOUR = 0b01100110,
  FIVE = 0b01101101,
  SIX =  0b01111101,
  SEVEN =0b00000111,
  EIGHT =0b01111111,
  NINE = 0b01101111,
} digit_t;

float GetBatteryVoltage(void) {
  float vbat = analogRead(VBATPIN);
  vbat *= 2; // The VBATPIN is at the center of a 1:1 voltage divider
  vbat /= 1024; // Max reading is 1024
  vbat *= 3.3; // Max voltage is 3.3V
#if PRINT_DEBUG
  Serial.print("VBat: ");
  Serial.println(vbat);
#endif
  return vbat; 
}

void ClearAll() {
  for (uint8_t muxpin_id = 0; muxpin_id < 4; ++muxpin_id){
    digitalWrite(g_digit_mux[muxpin_id], HIGH);
  }
  // Set the individual LEDs
  for (uint8_t i=0; i<7; ++i) {
    digitalWrite(g_led_seg_bitmask[i], LOW); // Let current flow into pin (to drive current thru LED)
  }

  // Set period/dot LED
  digitalWrite(LED_SEG_DP, LOW);
}

void ShowDigit(const uint8_t digit_id, const digit_t digit, const bool point) {
  uint8_t muxpin = g_digit_mux[digit_id];
  ClearAll();
  for (uint8_t muxpin_id = 0; muxpin_id < 4; ++muxpin_id){
    // Set the digit_enable pins
    if (digit_id == muxpin_id) {
      digitalWrite(g_digit_mux[muxpin_id], LOW);
    } else {
      digitalWrite(g_digit_mux[muxpin_id], HIGH);   
    }
  }
  // Set the individual LEDs
  for (uint8_t i=0; i<7; ++i) {
    if (digit & (1 << i)) {
      digitalWrite(g_led_seg_bitmask[i], HIGH); // Let current flow into pin (to drive current thru LED)
    } else {
      digitalWrite(g_led_seg_bitmask[i], LOW); // Don't
    }
  }

  // Set period/dot LED
  if (point) {
    digitalWrite(LED_SEG_DP, HIGH);
  } else {
    digitalWrite(LED_SEG_DP, LOW);
  }
}



void setup() {
  // put your setup code here, to run once:
  // Initialize LED segment pins
  for (uint8_t i=0; i<7; ++i) {
    pinMode(g_led_seg_bitmask[i], OUTPUT);
  }
  // Don't forget dot/period LED
  pinMode(LED_SEG_DP, OUTPUT);

  // Set the Digit enable /mux pin settings
  for (uint8_t i=0; i<4; ++i) {
    pinMode(g_digit_mux[i], OUTPUT);
  } 
}

void loop() {
  // put your main code here, to run repeatedly:
  digit_t test_digits[4] = {EIGHT, ZERO, ZERO, EIGHT};
  for (uint8_t digit_id=0; digit_id<4; ++digit_id) {
    //ClearAll();
    ShowDigit(digit_id, test_digits[digit_id], false);
    delay(1);
  }
}
