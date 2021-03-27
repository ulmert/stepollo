/*
   Stepollo - A MIDI Step Sequencer sketch
   Copyright (C) 2021 Jacob Ulmert

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of the
   License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include <Midiboy.h>
#include <EEPROM.h>

#define VERSION 0x01

/*
#define N_TRACKS 3
#define SEQUENCE_LENGTH 64
#define TRACK_POLYPHONY 2
*/

#define N_TRACKS 6
#define SEQUENCE_LENGTH 32
#define TRACK_POLYPHONY 2

/*
#define N_TRACKS 12
#define SEQUENCE_LENGTH 16
#define TRACK_POLYPHONY 2
*/

//#define ACCENT_TRIGGERS_CC

#define MIDI_IN_ENABLED

//#define MIDI_FLUSH_ON_IRQ

#define VISIBLE_TRACKS 3
#define GUI_VISIBLE_ROWS 7

#define FONT MIDIBOY_FONT_5X7

/*
static const PROGMEM uint8_t VELOCITY[4] = {8, 42, 80, 127};
static const PROGMEM uint8_t DURATION[8] = {1, 2, 3, 4, 5, 6, 7, 8};
static const PROGMEM uint8_t LENGTH[8] = {1, 2, 3, 4, 5, 6, 7, 8};
*/

static const uint8_t VELOCITY[4] = {8, 42, 80, 127};
static const uint8_t DURATION[8] = {1, 2, 3, 4, 5, 6, 7, 8};
static const uint8_t LENGTH[8] = {1, 2, 3, 4, 5, 6, 7, 8};

#define MIN_TRIGGER_COUNT 6
#define MAX_TRIGGER_COUNT 48
/*
static const PROGMEM uint8_t TIMER_TRIGGER_COUNT[4] = {MIN_TRIGGER_COUNT, 12, 24, MAX_TRIGGER_COUNT};
*/
static const uint8_t TIMER_TRIGGER_COUNT[4] = {MIN_TRIGGER_COUNT, 12, 24, MAX_TRIGGER_COUNT};

static const PROGMEM uint8_t LOGO[8][128] = {
{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x0,0x0,0x0,0x0,0x82,0x54,0x38,0x10,0x10,0x10,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x4,0xa8,0x70,0x20,0x20,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc0,0x40,0x80,0x0,0xc0,0x0,0x0,0x0,0x80,0x40,0x80,0x0,0xc0,0x0,0xc0,0x0,0x80,0x40,0x80,0x0,0x80,0x40,0x80,0x0,0xc0,0x40,0x80,0x0,0x0,0x80,0x40,0x80,0x0,0xc0,0x40,0x0,0xc0,0x0,0xc0,0x0,0xc0,0x40,0x40,0x0,0xc0,0x0,0xc0,0x0,0x0,0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x15,0xe,0x4,0x4,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x41,0x2a,0x1c,0x8,0x8,0x9,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x50,0xe0,0x40,0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3,0x1,0x0,0x1,0x0,0x1,0x0,0x0,0x0,0x0,0x7,0x0,0x3,0x5,0x3,0x0,0x3,0x4,0x2,0x0,0x3,0x4,0x3,0x0,0x7,0x5,0x2,0x0,0x0,0x7,0x0,0x7,0x0,0x7,0x0,0x0,0x7,0x3,0x7,0x0,0x7,0x5,0x4,0x0,0x7,0x5,0x2,0x0,0x4,0x7,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
{0x0,0x0,0x0,0x80,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x40,0xc0,0xc0,0xc0,0xc0,0xc0,0x0,0x0,0x0,0x0,0x0,0x80,0x40,0xc0,0xc0,0xc0,0xc0,0xc2,0xc1,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x80,0x40,0xc0,0xc0,0xc0,0xc0,0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x80,0x80,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x80,0x80,0x0,0x0,0x0,0x80,0x40,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x0,0x40,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x0,0x0,0x0,0x0,0x0,0x80,0x80,0xc0,0xc0,0xc0,0xc0,0xc0,0xc0,0x80,0x80,0x0,0x0,0x0,0x0},
{0x3c,0x12,0xa9,0xc7,0xe7,0xf7,0xf7,0xf3,0xf3,0xfb,0xf3,0xff,0xff,0xff,0x7f,0x3f,0x0,0x0,0x0,0x0,0xff,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,0xff,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xf3,0xf3,0xf3,0xf3,0x3,0x3,0xb,0xf7,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xf0,0xf0,0xf0,0xf0,0xf0,0xe0,0xe0,0xc0,0x4,0x2,0xfd,0xfe,0xff,0xff,0xff,0x17,0x13,0x13,0xf3,0xb,0x7,0xff,0xff,0xff,0xfe,0xfd,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0x13,0x13,0x13,0x13,0x13,0xb,0xf7,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0x13,0x13,0x13,0x13,0x13,0xb,0xf7,0x0,0x2,0xfd,0xfe,0xff,0xff,0xff,0x17,0x13,0x13,0xf3,0xb,0x7,0xff,0xff,0xff,0xfe,0xfc,0x0},
{0x7e,0xa1,0x10,0x8f,0x5f,0x3f,0x3f,0x3f,0x3c,0x3c,0x3c,0x3c,0x3e,0xbf,0x5e,0xe,0x90,0x48,0x3c,0x3c,0x3f,0x3c,0x3c,0x3f,0x3f,0x3f,0x3f,0x3f,0x3c,0x3c,0x3c,0xbc,0x7d,0x0,0x0,0x3f,0x3f,0x3f,0x3f,0x3f,0x3c,0x3c,0x3e,0x3d,0x3c,0x3c,0xbc,0x7d,0x80,0x40,0x3f,0x3f,0x3f,0x3f,0x3f,0x3c,0x3c,0x3e,0x3d,0xbf,0x7f,0x1f,0xf,0x0,0x24,0x43,0xc7,0xaf,0x9f,0x5f,0x3e,0x3c,0x3c,0x3f,0xbc,0x7e,0x9f,0x5f,0x2f,0x7,0xfb,0x80,0x40,0x3f,0x3f,0x3f,0xbf,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0x80,0x40,0x3f,0x3f,0x3f,0xbf,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0xf,0x18,0x24,0x43,0xc7,0xaf,0x9f,0x5f,0x3e,0x3c,0x3c,0x3f,0xbc,0x7e,0x9f,0x5f,0x2f,0x7,0x3,0x0},
{0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x1,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}
};

static const PROGMEM uint8_t BITMAP_FLAG[4][5] = {
  {0b11111111, 0b00000000, 0b00011000, 0b00011000, 0b00000000},
  {0b11111111, 0b00000000, 0b00000110, 0b00000110, 0b00000000},
  {0b11111111, 0b00000000, 0b01100000, 0b01100000, 0b00000000},
  {0b11111111, 0b00000000, 0b01100110, 0b01100110, 0b00000000},
};

static const PROGMEM uint8_t BITMAP_FLAG_INV[4][5] = {
  {0b00000000, 0b00000000, 0b00011000, 0b00011000, 0b00000000},
  {0b00000000, 0b00000000, 0b00000110, 0b00000110, 0b00000000},
  {0b00000000, 0b00000000, 0b01100000, 0b01100000, 0b00000000},
  {0b00000000, 0b00000000, 0b01100110, 0b01100110, 0b00000000}
};

static const PROGMEM uint8_t BITMAP_TRACKLANE[15][5] = {
  {0b00000000, 0b00000000, 0b01010101, 0b00000000, 0b00000000},
  {0b00000000, 0b00111000, 0b00010000, 0b00000000, 0b00000000},
  {0b01111110, 0b01000010, 0b01111010, 0b01111110, 0b00000000},
  {0b00000000, 0b00010000, 0b00010000, 0b00010000, 0b00000000},
  {0b00000000, 0b00000000, 0b00011000, 0b00111100, 0b01111110},
  {0b11111111, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {0b01010101, 0b00000000, 0b00000000, 0b00000000, 0b00000000},  
  {0b00000001, 0b00010001, 0b00010001, 0b00000001, 0b00000000},
  {0b01111101, 0b01111101, 0b01111101, 0b01111101, 0b00000000},
  {0b00000000, 0b00010000, 0b00010000, 0b00000000, 0b00000000},
  {0b01111100, 0b01111100, 0b01111100, 0b01111100, 0b00000000},
  {0b01000000, 0b01111111, 0b00000000, 0b00000000, 0b00000000},
  {0b00000000, 0b11111111, 0b00000000, 0b00000000, 0b00000000},
  {0b00000010, 0b11111110, 0b00000000, 0b00000000, 0b00000000},
  {0b01000010, 0b01111110, 0b00000000, 0b00000000, 0b00000000},
};

uint8_t g_scratchBitmap[4];

static const PROGMEM uint8_t SCALES[3][12] = {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11}, 
  {0, 0 ,2 ,2 ,4 ,5 ,5 ,7 ,7 ,9 ,9,11},
  {0, 0 ,2 ,2 ,3 ,5 ,5 ,7 ,7 ,8 ,8,10}
};

static const PROGMEM char LABEL_SCALE[3][12] = {"CHROMATIC\0", "MAJOR\0", "MINOR\0"};
static const PROGMEM char LABEL_NOTES[12][3] = {"C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B "};
static const PROGMEM char LABEL_MASTER[1][7] = {"MASTER"};
static const PROGMEM char LABEL_MIDI[1][5] = {"MIDI"};
static const PROGMEM char LABEL_MUTE[1][5] = {"MUTE"};
static const PROGMEM char LABEL_SOLO[1][5] = {"SOLO"};
static const PROGMEM char LABEL_BOOL[2][4] = {{"OFF"}, {"ON\0"}};

typedef struct step_t {
  uint8_t note; 
  uint8_t propA;
  uint8_t propB;
};

// PropertiesA
// dddLLLvv
//   |  | |- Note velocity
//   |  |--- Step length
//   |------ Note length

// PropertiesB
// ffHsCCCC
// | ||   |- Loop count
// | ||----- Loop start
// | |------ Hold
// | 
// |-------- Flag

enum stepProperty {
  STEP_PROPERTY_NOTE,
  STEP_PROPERTY_LENGTH,
  STEP_PROPERTY_DURATION,
  STEP_PROPERTY_VELOCITY,
  STEP_PROPERTY_FLAG,
  STEP_PROPERTY_HOLD,
  STEP_PROPERTY_LOOP_START,
  STEP_PROPERTY_LOOP_END,
  STEP_PROPERTY_LAST
};

/*
#define STEP_GET_DURATION pgm_read_byte(&DURATION[0] + (step->propA >> 5 & 0x07))
#define STEP_GET_LENGTH pgm_read_byte(&LENGTH[0] + (step->propA >> 2 & 0x07))
#define STEP_GET_VELOCITY pgm_read_byte(&VELOCITY[0] + (step->propA & 0x03))
*/

#define STEP_GET_DURATION DURATION[(step->propA >> 5 & 0x07)]
#define STEP_GET_LENGTH LENGTH[(step->propA >> 2 & 0x07)]
#define STEP_GET_VELOCITY VELOCITY[(step->propA & 0x03)]

#define STEP_GET_VELOCITY_RAW (step->propA & 0x03)
#define STEP_SETDEFAULT_VELOCITY (step->propA = (step->propA & 0xFC) | 2)

#define STEP_INC_VELOCITY if ((step->propA & 0x03) < 3) (step->propA = (step->propA & 0xFC) | ((step->propA & 0x03) + 1))
#define STEP_DEC_VELOCITY if ((step->propA & 0x03) > 0) (step->propA = (step->propA & 0xFC) | ((step->propA & 0x03) - 1))

#define STEP_GET_LOOP_END (step->propB & 0x0f)
#define STEP_INC_LOOP_END if ((step->propB & 0x0F) < 0x0f) (step->propB = (step->propB & 0xf0) | ((step->propB & 0x0f) + 1))
#define STEP_DEC_LOOP_END if ((step->propB & 0x0F) > 0) (step->propB = (step->propB & 0xf0) | ((step->propB & 0x0f) - 1))

#define STEP_GET_LOOP_START (step->propB & 0b00010000)
#define STEP_TOGGLE_LOOP_START (step->propB ^= 0b00010000)

#define STEP_GET_DURATION_RAW (step->propA >> 5 & 0x07)
#define STEP_INC_DURATION_RAW if ((step->propA >> 5 & 0x07) < 0x07) (step->propA = (step->propA & 0x1F) | (((step->propA >> 5 & 0x07) + 1) << 5))
#define STEP_DEC_DURATION_RAW if ((step->propA >> 5 & 0x07) > 0) (step->propA = (step->propA & 0x1F) | (((step->propA >> 5 & 0x07) - 1) << 5))
#define STEP_SETDEFAULT_DURATION (step->propA = (step->propA & 0x1F) | ((0) << 5))

#define STEP_GET_LENGTH_RAW (step->propA >> 2 & 0x07))
#define STEP_INC_LENGTH_RAW if ((step->propA >> 2 & 0x07) < 0x07) (step->propA = (step->propA & 0xE3) | (((step->propA >> 2 & 0x07) + 1) << 2))
#define STEP_DEC_LENGTH_RAW if ((step->propA >> 2 & 0x07) > 0) (step->propA = (step->propA & 0xE3) | (((step->propA >> 2 & 0x07) - 1) << 2))
#define STEP_SETDEFAULT_LENGTH (step->propA = (step->propA & 0xE3) | (0 << 2))

#define STEP_GET_NOTE (step->note & 0x7f)
#define STEP_INC_NOTE if ((step->note & 0x7F) < 120) (step->note = (step->note & 0x80) | ((step->note & 0x7F) + 1))
#define STEP_DEC_NOTE if ((step->note & 0x7F) > 24) (step->note = (step->note & 0x80) | ((step->note & 0x7F) - 1))

#define STEP_GET_MUTE (step->note & 0x80)

#define STEP_TOGGLE_MUTE (step->note ^= 0x80)

#define STEP_GET_HOLD (step->propB >> 5 & 0x01)
#define STEP_TOGGLE_HOLD (step->propB ^=  0x20)

#define STEP_GET_FLAG (step->propB >> 6 & 0x03)
#define STEP_INC_FLAG if ((step->propB >> 6 & 0x03) < 0x03) (step->propB = (step->propB & 0x3F) | ((step->propB >> 6 & 0x03) + 1) << 6)
#define STEP_DEC_FLAG if ((step->propB >> 6 & 0x03) > 0) (step->propB = (step->propB & 0x3F) | ((step->propB >> 6 & 0x03) - 1) << 6)

typedef struct track_t {
  uint8_t propA;
  uint8_t propB;
  uint8_t propC;
  uint8_t propD;
  uint8_t ptrnA;
  uint8_t ptrnB;
  uint8_t ptrnC;
  uint8_t len;
  uint8_t preDelay;
  step_t steps[SEQUENCE_LENGTH];
};

// PropertiesA
// sTTmMMMM
// || |   |- MIDI channel
// || |----- Mute
// ||------- Time factor
// |-------- Solo

// PropertiesB
// CCpprRSS
// | | || |- Transpose source
// | | ||--- Rotate Note pattern
// | | |---- Rotate Track pattern
// | |------ Polyphony
// |-------- Scale

// PropertiesC
// Rttttttt
// |      |- Transpose
// |-------- Master reset

// PropertiesD
// lllLLLmm
//   |  | |- Accent modifier
//   |  |--- Track pattern length (unused)
//   |------ Note pattern length


enum transposeSource {
  TRANSPOSESOURCE_OFF,
  TRANSPOSESOURCE_MASTERTRACK,
  TRANSPOSESOURCE_IN,
  TRANSPOSESOURCE_LAST
};

enum trackProperty {
  TRKPROP_MUTE,
  TRKPROP_SOLO,
  TRKPROP_TIMEFACTOR,
  TRKPROP_MIDI,
  TRKPROP_TRANSPOSE,
  TRKPROP_TRANSPOSESOURCE,
  TRKPROP_ACCENTPATTERN,
  TRKPROP_ACCENTMODIFIER,
  TRKPROP_NOTEPATTERN,
  TRKPROP_NOTEPATTERNLEN,
  TRKPROP_NOTEPATTERNROTATION,
  TRKPROP_TRACKPATTERN,
  TRKPROP_TRACKPATTERNROTATION,
  TRKPROP_MASTERRESET,
  TRKPROP_POLYPHONY,
  TRKPROP_SCALE,
  TRKPROP_PREDELAY,
  TRKPROP_LAST
};

static const PROGMEM char LABEL_TRKPROP[TRKPROP_LAST][16] = {"MUTE\0", "SOLO\0", "TIME FACTOR\0", "MIDI\0", "SHIFT\0", "TRANSPOSE\0", 
                                                             "ACCENT FILT.\0", "ACCENT MOD.\0",
                                                             "STEP FILT.\0", "STEP LEN.\0", "STEP ROT.\0", 
                                                             "TRACK FILT.\0", "TRACK ROT.\0", 
                                                             "MASTER RESET\0", "POLYPHONY\0" , "SCALE\0", "PREDELAY\0"};

#define TRK_GET_MIDICHANNEL (track->propA & 0x0F)

#define TRK_GET_TIMEFACTOR_RAW (track->propA >> 5 & 0x03)
/*
#define TRK_GET_TIMEFACTOR pgm_read_byte(&TIMER_TRIGGER_COUNT[TRK_GET_TIMEFACTOR_RAW])
*/
#define TRK_GET_TIMEFACTOR TIMER_TRIGGER_COUNT[TRK_GET_TIMEFACTOR_RAW]

#define TRK_INC_TIMEFACTOR if ((track->propA >> 5 & 0x03) < 0x03) (track->propA = (track->propA & 0x9F) | ((track->propA >> 5 & 0x03) + 1) << 5)
#define TRK_DEC_TIMEFACTOR if ((track->propA >> 5 & 0x03) > 0) (track->propA = (track->propA & 0x9F) | ((track->propA >> 5 & 0x03) - 1) << 5)

#define TRK_GET_MUTE (track->propA >> 4 & 0x01)
#define TRK_TOGGLE_MUTE (track->propA ^= (0x01 << 4))

#define TRK_GET_SOLO (track->propA >> 7 & 0x01)
#define TRK_TOGGLE_SOLO (track->propA ^= (0x01 << 7))

#define TRK_INC_MIDICHANNEL if ((track->propA & 0x0F) < 0x0F) (track->propA = (track->propA & 0xF0) | ((track->propA & 0x0F) + 1))
#define TRK_DEC_MIDICHANNEL if ((track->propA & 0x0F) > 0) (track->propA = (track->propA & 0xF0) | ((track->propA & 0x0F) - 1))

#define TRK_GET_TRACKPATTERN (track->ptrnA)
#define TRK_INC_TRACKPATTERN if (track->ptrnA < 0xff) (track->ptrnA++)
#define TRK_DEC_TRACKPATTERN if (track->ptrnA > 0) (track->ptrnA--)

#define TRK_GET_NOTEPATTERN (track->ptrnB)
#define TRK_INC_NOTEPATTERN if (track->ptrnB < 0xff) (track->ptrnB++)
#define TRK_DEC_NOTEPATTERN if (track->ptrnB > 0) (track->ptrnB--)

#define TRK_GET_ACCENTPATTERN (track->ptrnC)
#define TRK_INC_ACCENTPATTERN if (track->ptrnC < 0xff) (track->ptrnC++)
#define TRK_DEC_ACCENTPATTERN if (track->ptrnC > 0) (track->ptrnC--)

#define TRK_GET_PREDELAY (track->preDelay)
#define TRK_INC_PREDELAY if (track->preDelay < 0xff) (track->preDelay++)
#define TRK_DEC_PREDELAY if (track->preDelay > 0) (track->preDelay--)

#define TRK_GET_LENGTH (track->len)

#define TRK_GET_TRANSPOSESOURCE (track->propB & 0x03)
#define TRK_INC_TRANSPOSESOURCE if ((track->propB & 0x03) < (TRANSPOSESOURCE_LAST - 1)) (track->propB = (track->propB & 0xFC) | (track->propB & 0x03) + 1)
#define TRK_DEC_TRANSPOSESOURCE if ((track->propB & 0x03) > 0) (track->propB = (track->propB & 0xFC) | (track->propB & 0x03) - 1)
static const PROGMEM char LABEL_TRANSPOSESOURCE[TRANSPOSESOURCE_LAST][9] = {"OFF\0", "MASTER\0", "MIDI\0"};

#define TRK_GET_NOTEPATTERNROTATION ((track->propB & 0x04) >> 2)
#define TRK_TOGGLE_NOTEPATTERNROTATION (track->propB ^= 0x04)

#define TRK_GET_TRACKPATTERNROTATION ((track->propB & 0x08) >> 3)
#define TRK_TOGGLE_TRACKPATTERNROTATION (track->propB ^= 0x08)

#define TRK_GET_TRANSPOSE (((track->propC & 0x7F) >= 63) ? (track->propC & 0x7F) - 63 : (track->propC & 0x7F) - 64)
#define TRK_INC_TRANSPOSE if ((track->propC & 0x7F) < 0x7F) (track->propC = (track->propC & 0x80) | (track->propC & 0x7F) + 1)
#define TRK_DEC_TRANSPOSE if ((track->propC & 0x7F) > 0) (track->propC = (track->propC & 0x80) | (track->propC & 0x7F) - 1)
#define TRK_SETDEFAULT_TRANSPOSE (track->propC = (track->propC & 0x80) | 0x3F)

/*
#define TRK_GET_TRANSPOSE (((track->propC & 0x7F) >= 75) ? (track->propC & 0x7F) - 75 : (track->propC & 0x7F) - 76)
#define TRK_INC_TRANSPOSE if ((track->propC & 0x7F) < 0x7F) (track->propC = (track->propC & 0x80) | (track->propC & 0x7F) + 1)
#define TRK_DEC_TRANSPOSE if ((track->propC & 0x7F) > 0) (track->propC = (track->propC & 0x80) | (track->propC & 0x7F) - 1)
#define TRK_SETDEFAULT_TRANSPOSE (track->propC = (track->propC & 0x80) | 0x4B)
*/

#define TRK_GET_MASTERRESET ((track->propC & 0x80) >> 7)
#define TRK_TOGGLE_MASTERRESET (track->propC ^= 0x80)

#define TRK_GET_POLYPHONY ((track->propB >> 4 & 0x03) + 1)

#define TRK_INC_POLYPHONY if ((track->propB >> 4 & 0x03) < (TRACK_POLYPHONY - 1)) (track->propB = (track->propB & 0xCF) | ((track->propB >> 4 & 0x03) + 1) << 4)
#define TRK_DEC_POLYPHONY if ((track->propB >> 4 & 0x03) > 0) (track->propB = (track->propB & 0xCF) | ((track->propB >> 4 & 0x03) - 1) << 4)

#define TRK_GET_SCALE (track->propB >> 6 & 0x03)
#define TRK_INC_SCALE if ((track->propB >> 6 & 0x03) < 2) (track->propB = (track->propB & 0x3F) | ((track->propB >> 6 & 0x03) + 1) << 6)
#define TRK_DEC_SCALE if ((track->propB >> 6 & 0x03) > 0) (track->propB = (track->propB & 0x3F) | ((track->propB >> 6 & 0x03) - 1) << 6)

#define TRK_GET_NOTEPATTERNLEN ((track->propD >> 5 & 0x07) + 1)
#define TRK_GET_NOTEPATTERNLEN_RAW (track->propD >> 5 & 0x07)
#define TRK_INC_NOTEPATTERNLEN if ((track->propD >> 5 & 0x07) < 0x07) (track->propD = (track->propD & 0x1F) | (((track->propD >> 5 & 0x07) + 1) << 5))
#define TRK_DEC_NOTEPATTERNLEN if ((track->propD >> 5 & 0x07) > 0) (track->propD = (track->propD & 0x1F) | (((track->propD >> 5 & 0x07) - 1) << 5))
#define TRK_SETDEFAULT_NOTEPATTERNLEN (track->propD = (track->propD & 0x1F) | (7 << 5))

#define TRK_GET_ACCENTMOD (track->propD & 0x03)
#define TRK_INC_ACCENTMOD if ((track->propD & 0x03) < (ACCENTMOD_LAST - 1)) (track->propD = (track->propD & 0xFC) | ((track->propD & 0x03) + 1))
#define TRK_DEC_ACCENTMOD if ((track->propD & 0x03) > 0) (track->propD = (track->propD & 0xFC) | ((track->propD & 0x03) - 1))

enum accentModifier {
  ACCENTMOD_NONE,
  ACCENTMOD_VOLUME,
  ACCENTMOD_STEPLEN,
  ACCENTMOD_VOLUME_STEPLEN,
  ACCENTMOD_LAST
};

typedef struct song_t {
  uint8_t bpm;
  uint8_t propA;
  uint8_t propB;
  uint8_t propC;
};

// PropertiesA
// tffSS000
// | | |  |- (Unused)
// | | |---- Sync mode
// | |------ Initial flags
// |-------- Transpose mode

// PropertiesB
// xnnnnnnn
//        |- Center note (transpose)

// PropertiesC
// mmmm0lll
//    |   |- Accent pattern length
//    |----- Master track


enum transposeMode {
  TRANSPOSEMODE_STEP,
  TRANSPOSEMODE_LOOP
};

#define SONG_GET_TEMPO (g_song.bpm)
#define SONG_INC_TEMPO if (g_song.bpm < 255) g_song.bpm++
#define SONG_DEC_TEMPO if (g_song.bpm > 40) g_song.bpm--
#define SONG_SETDEFAULT_TEMPO g_song.bpm = 120

#define SONG_GET_MASTERTRACK ((g_song.propC >> 4 & 0x0F) + 1)
#define SONG_GET_MASTERTRACK_RAW (g_song.propC >> 4 & 0x0F)
#define SONG_INC_MASTERTRACK if ((g_song.propC >> 4 & 0x0F) < (N_TRACKS - 1)) (g_song.propC = (g_song.propC & 0x0F) | ((g_song.propC >> 4 & 0x0F) + 1) << 4)
#define SONG_DEC_MASTERTRACK if ((g_song.propC >> 4 & 0x0F) > 0) (g_song.propC = (g_song.propC & 0x0F) | ((g_song.propC >> 4 & 0x0F) - 1) << 4)

enum syncMode {
  SYNCMODE_OFF,
  SYNCMODE_GENCLOCK,
  SYNCMODE_GENCLOCKANDSTARTSTOP,
  SYNCMODE_LAST
};

#define SONG_GET_SYNC (g_song.propA >> 3 & 0x03)
#define SONG_INC_SYNC if ((g_song.propA >> 3 & 0x03) < (SYNCMODE_LAST - 1)) (g_song.propA = (g_song.propA & 0xE7) | ((g_song.propA >> 3 & 0x03) + 1) << 3)
#define SONG_DEC_SYNC if ((g_song.propA >> 3 & 0x03) > 0) (g_song.propA = (g_song.propA & 0xE7) | ((g_song.propA >> 3 & 0x03) - 1) << 3)
#define SONG_SETDEFAULT_SYNC g_song.propA |= (SYNCMODE_GENCLOCK << 3)             
static const PROGMEM char LABEL_SYNCMODE[SYNCMODE_LAST][16] = {{"OFF\0"}, {"CLK\0"}, {"ALL\0"}};

#define SONG_GET_ROOTNOTE (g_song.propB & 0x7F)
#define SONG_INC_ROOTNOTE if ((g_song.propB & 0x7F) < 120) (g_song.propB = (g_song.propB & 0x80) | ((g_song.propB & 0x7F) + 1) & 0x7F)                
#define SONG_DEC_ROOTNOTE if ((g_song.propB & 0x7F) > 24) (g_song.propB = (g_song.propB & 0x80) | ((g_song.propB & 0x7F) - 1) & 0x7F)                

#define SONG_SETDEFAULT_ROOTNOTE g_song.propB = (g_song.propB & 0x80) | (0x3C)             

#define SONG_GET_TRANSPOSEMODE (g_song.propA >> 7 & 0x01)
#define SONG_TOGGLE_TRANSPOSEMODE (g_song.propA ^= 0x80)                
static const PROGMEM char LABEL_TRANSPOSEMODE[2][6] = {{"STEP\0"}, {"LOOP\0"}};

#define SONG_GET_FLAGS (g_song.propA >> 5 & 0x03)
#define SONG_INC_FLAGS if ((g_song.propA >> 5 & 0x03) < 3) (g_song.propA = (g_song.propA & 0x9F) | ((g_song.propA & 0x60) + 0x20))                
#define SONG_DEC_FLAGS if ((g_song.propA >> 5 & 0x03) > 0) (g_song.propA = (g_song.propA & 0x9F) | ((g_song.propA & 0x60) - 0x20))                

#define SONG_GET_ACCENTLEN_RAW (g_song.propC & 0x07)
#define SONG_GET_ACCENTLEN ((g_song.propC & 0x07) + 1)
#define SONG_INC_ACCENTLEN if ((g_song.propC & 0x07) < 7) (g_song.propC = (g_song.propC & 0xF8) | ((g_song.propC & 0x07) + 1))                
#define SONG_DEC_ACCENTLEN if ((g_song.propC & 0x07) > 0) (g_song.propC = (g_song.propC & 0xF8) | ((g_song.propC & 0x07) - 1))                
#define SONG_SETDEFAULT_ACCENTLEN (g_song.propC = (g_song.propC & 0xF8) | 3)                
static const PROGMEM char LABEL_ACCENTMOD[ACCENTMOD_LAST][12] = {"OFF\0", "VOL\0", "LEN\0", "V+L\0"};

enum songProperty {
  SONGOPTIONS_TEMPO,
  SONGOPTIONS_MASTERTRACK,
  SONGOPTIONS_TRANSPOSEMODE,
  SONGOPTIONS_ACCENTLEN,
  SONGOPTIONS_SYNC,
  SONGOPTIONS_ROOTNOTE,
  SONGOPTIONS_FLAGS,
  SONGOPTIONS_LAST
};
static const PROGMEM char LABEL_SONGOPTIONS[SONGOPTIONS_LAST][16] = {"TEMPO\0", "MASTER TRACK\0",  "TRANSP.MODE\0","ACCENT LEN.\0", "SEND SYNC\0", "CENTER NOTE\0", "INIT. FLAGS\0"};

enum songMenu {
  SONGMENU_LOAD,
  SONGMENU_SAVE,
  SONGMENU_OPTIONS,
  SONGMENU_LAST
};
static const PROGMEM char LABEL_SONGMENU[SONGMENU_LAST][5] = {"LOAD", "SAVE"};

enum editState {
  EDITSTATE_SEQUENCER = 0,
  EDITSTATE_TRACK = 1,
  EDITSTATE_SONG = 2
};
uint8_t g_editState = EDITSTATE_SEQUENCER;

song_t g_song;
track_t g_tracks[N_TRACKS];

typedef struct playing_t {
  uint8_t note;
  uint8_t duration;  
  uint8_t control;
};

#define STEPCONTROL_MODULATION 0x01
#define STEPCONTROL_HOLD 0x02

playing_t g_playing[N_TRACKS][TRACK_POLYPHONY];
uint8_t g_lastPlayingIndex[N_TRACKS];

uint8_t g_timerCount = MAX_TRIGGER_COUNT;
uint8_t g_songBeatCount = 0;
uint16_t g_soloTracks = 0;
uint8_t g_stepIndex[N_TRACKS];
uint8_t g_stepIndexPlaying[N_TRACKS];
uint8_t g_currentStepLength[N_TRACKS];
uint8_t g_trackOrder[N_TRACKS];
uint8_t g_flags = 0;

uint8_t g_trackPatternIndex[N_TRACKS];
uint8_t g_notePatternIndex[N_TRACKS];

int8_t g_transposeNote[N_TRACKS];
uint8_t g_accentPatternIndex = 0;

uint8_t g_currentLoopStep[N_TRACKS];
uint8_t g_currentLoopCount[N_TRACKS];
#define LOOPSTEP_NOTSET 0xff

#define SEQUENCERSTATE_IDLE 0x00
#define SEQUENCERSTATE_PENDING_STOP 0x01
#define SEQUENCERSTATE_PENDING_PLAY 0x02
#define SEQUENCERSTATE_PLAYING 0x10
volatile uint8_t g_SequencerState = SEQUENCERSTATE_IDLE;
uint8_t g_logoCount = 4;

#define REFRESHSONG_ALL 0xff
#define REFRESHSONG_CLEAR 0x00
uint8_t g_refreshSongMask = REFRESHSONG_ALL;
#define REFRESHSONG_TEMPO 0x01
#define REFRESHSONG_SOLOTRACKS 0x02
#define REFRESHSONG_TRACKORDER 0x04
#define REFRESHSONG_TRACKLENGTHS 0x08

uint8_t g_lastRowTrack = 0;

uint16_t g_guiRow = 0;
uint8_t g_guiTrackIndex = 0;
uint8_t g_guiSongMenuIndex = 0;
uint8_t g_guiPropertyIndex = 0;
uint8_t g_guiStepEditIndex = 0;

enum keyModifier
{
  KEYMODIFIER_NONE = 0,
  KEYMODIFIER_A = 1,
  KEYMODIFIER_B = 2,
  KEYMODIFIER_EDIT = 4,
  KEYMODIFIER_CANCEL = 8,
  KEYMODIFIER_HOLD = 64,
  KEYMODIFIER_BLINK = 128
};
int8_t g_keyModifier = KEYMODIFIER_HOLD;

unsigned long g_blinkTimestamp = 0;
unsigned long g_stepDetailsTimestamp = 0;

void setTrackOrder() {
  for (uint8_t i = 0; i < N_TRACKS; i++) {
    g_trackOrder[i] = i;  
  }
  g_trackOrder[SONG_GET_MASTERTRACK_RAW] = 0;
  g_trackOrder[0] = SONG_GET_MASTERTRACK_RAW; 
}

void setSoloTracks() {
  for (uint8_t i = 0; i < N_TRACKS; i++) {
    track_t *track = &g_tracks[i];
    uint16_t v = g_soloTracks & (0xffff ^ (0x0001 << i)) | (TRK_GET_SOLO << i); 
    g_soloTracks = v;  
  }  
}

void setStepDefaults(step_t *step) {
  memset(step, 0,  sizeof(step_t));
  step->note = 48;
  STEP_SETDEFAULT_LENGTH;
  STEP_SETDEFAULT_DURATION;
  STEP_SETDEFAULT_VELOCITY;
  STEP_TOGGLE_MUTE;
}

void deleteStep(track_t *track, uint8_t stepIndex) {
  if (stepIndex < (SEQUENCE_LENGTH - 1)) {
    for (uint8_t i = stepIndex; i < (SEQUENCE_LENGTH - 1); i++) {
      memcpy(&track->steps[i],  &track->steps[i + 1], sizeof(step_t));
    }
    setStepDefaults(&track->steps[(SEQUENCE_LENGTH - 1)]);
  }
}

void insertStep(track_t *track, uint8_t stepIndex) {
 if (stepIndex < (SEQUENCE_LENGTH - 1)) {
    for (uint8_t i = (SEQUENCE_LENGTH - 1); i > stepIndex; i--) {
      memcpy(&track->steps[i],  &track->steps[i - 1], sizeof(step_t));
    }
    setStepDefaults(&track->steps[stepIndex]);
  }
}

void initTrack(track_t *track, int8_t midiChannel) {
  memset(track, 0,  sizeof(track_t));
 
  track->propA = midiChannel;

  TRK_SETDEFAULT_NOTEPATTERNLEN;
  TRK_SETDEFAULT_TRANSPOSE;
  track->len = TRK_GET_NOTEPATTERNLEN;

  track->ptrnA = 0xff;
  track->ptrnB = 0xff;
  track->ptrnC = 0x80;

  for (uint8_t i = 0; i < SEQUENCE_LENGTH; i++) {
    setStepDefaults(&track->steps[i]);
  }
}

uint16_t getTrackLength(uint8_t i) {
  uint16_t l = 0;
  for (uint8_t stepIndex = 0; stepIndex <  SEQUENCE_LENGTH; stepIndex++) { 
    step_t *step = &g_tracks[i].steps[stepIndex];
    l += STEP_GET_LENGTH;
  }
  return l;
}

void initSong() {
  memset(&g_song, 0, sizeof(song_t));

  SONG_SETDEFAULT_ACCENTLEN;
  SONG_SETDEFAULT_ROOTNOTE;
  SONG_SETDEFAULT_SYNC;
  SONG_SETDEFAULT_TEMPO;
  
  for (uint8_t i = 0; i < N_TRACKS; i++) {
    initTrack(&g_tracks[i], i);
    for (uint8_t t = 0; t < TRACK_POLYPHONY; t++) {
      memset(&g_playing[i][t], 0, sizeof(playing_t));
    }
  }
}

void printString_P(const char *pText, bool inverted) {
  char v = pgm_read_byte(pText++);
  while (v != 0) {
    Midiboy.drawBitmap_P(&FONT::DATA_P[(v - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
    v = pgm_read_byte(pText++);
  }
}

void printString(const char *pText, bool inverted) {
  while (*pText != 0) {
    Midiboy.drawBitmap_P(&FONT::DATA_P[(*pText++ - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
  }
}

void printNotePropertyValue(uint8_t v, char n, bool inverted) {
  Midiboy.drawBitmap_P(&FONT::DATA_P[(n - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
  uint8_t t = (v / 10);
  if (t == 0) {
    Midiboy.drawSpace(FONT::WIDTH, inverted);
  } else {
    Midiboy.drawBitmap_P(&FONT::DATA_P[('0' + t - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
  }
  t = v - (t * 10);
  Midiboy.drawBitmap_P(&FONT::DATA_P[('0' + t - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
}

void drawLogo() {
  for (uint8_t i = 0; i < 8; i++) {
    Midiboy.drawBitmap_P(LOGO[i], 128, false);
  }
}

void printByte(uint8_t v, bool inverted) {
  uint8_t t = (v / 100);
  if (t > 0)
    Midiboy.drawBitmap_P(&FONT::DATA_P[('0' + t - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
  v -= (t * 100);
  if (t > 0 || v > 9) {
    t = (v / 10);
    Midiboy.drawBitmap_P(&FONT::DATA_P[('0' + t - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
  }
  t = (v - (v / 10 * 10));
  Midiboy.drawBitmap_P(&FONT::DATA_P[('0' + t - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
}

void printByteSigned(int8_t v, bool inverted) {
  if (v > 0) {
    Midiboy.drawBitmap_P(&FONT::DATA_P[('+' - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
  } else if (v < 0) {
    Midiboy.drawBitmap_P(&FONT::DATA_P[('-' - ' ') * FONT::WIDTH], FONT::WIDTH, inverted);
    v = -v;
  }
  printByte(v, inverted);
}

enum { EEPROM_SAVE_START_ADDRESS = 20 };

void loadFromEEPROM() {
  uint16_t offset = 0;

  if (EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++)) == VERSION && 
      EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++)) == N_TRACKS &&
      EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++)) == SEQUENCE_LENGTH) {

    g_song.bpm = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
    g_song.propA = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
    g_song.propB = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
    g_song.propC = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));

    for (uint8_t i = 0; i < N_TRACKS; i++) {
      g_tracks[i].propA = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      g_tracks[i].propB = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      g_tracks[i].propC = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      g_tracks[i].propD = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      g_tracks[i].ptrnA = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      g_tracks[i].ptrnB = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      g_tracks[i].len = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      g_tracks[i].preDelay = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));

      for (uint8_t j = 0; j < SEQUENCE_LENGTH; j++) {
        g_tracks[i].steps[j].note = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
        g_tracks[i].steps[j].propA = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
        g_tracks[i].steps[j].propB = EEPROM.read(EEPROM_SAVE_START_ADDRESS + (offset++));
      }
    }
  }
}

void saveToEEPROM() {
  uint16_t offset = 0;

  EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), VERSION);
  EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), N_TRACKS);
  EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), SEQUENCE_LENGTH);

  EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_song.bpm);
  EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_song.propA);
  EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_song.propB);
  EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_song.propC);

  for (uint8_t i = 0; i < N_TRACKS; i++) {
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].propA);
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].propB);
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].propC);
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].propD);
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].ptrnA);
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].ptrnB);
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].len);
    EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].preDelay);
    for (uint8_t j = 0; j < SEQUENCE_LENGTH; j++) {
      EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].steps[j].note);
      EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].steps[j].propA);
      EEPROM.update(EEPROM_SAVE_START_ADDRESS + (offset++), g_tracks[i].steps[j].propB);
    }
  }
}

void setTimer(uint8_t bpmByte) {
  float bpm = (float) bpmByte;
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = (uint16_t) ((16.0 * (10e6)) / ((1.0 / 60.0) * (bpm * (96.0 * 2.0)) * 64.0) + 0.5) - 3;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11) | (1 << CS10);

  TIMSK1 |= (1 << OCIE1A);
  sei();
}

void setup() {
  initSong();

  Midiboy.begin();
  Midiboy.setButtonRepeatMs(50);
  
  Midiboy.setDrawPosition(0, 0);
  drawLogo();
}

int8_t g_MIDIinNote = 0x40;
uint8_t g_MIDIinData = 0;

ISR(TIMER1_COMPA_vect) {

  if (SONG_GET_SYNC != SYNCMODE_OFF) {
    Midiboy.dinMidi().write(0xF8);
  }

  if (g_SequencerState & SEQUENCERSTATE_PENDING_STOP) {
    if (g_SequencerState & SEQUENCERSTATE_PLAYING) {
      for (uint8_t i = 0; i < N_TRACKS; i++) {
        track_t *track = &g_tracks[i];
        for (uint8_t t = 0; t < TRACK_POLYPHONY; t++) {
          Midiboy.dinMidi().write(0x80 | TRK_GET_MIDICHANNEL);
          Midiboy.dinMidi().write(g_playing[i][t].note & 0x7F);
          Midiboy.dinMidi().write((uint8_t)0x00);
          g_playing[i][t].note = 0;
        }
        Midiboy.dinMidi().write(0xB0 | TRK_GET_MIDICHANNEL);
        Midiboy.dinMidi().write(123);
        Midiboy.dinMidi().write((uint8_t)0x00);
        g_trackPatternIndex[i] = 0x00;
        g_notePatternIndex[i] = 0x77;
      }
      if (SONG_GET_SYNC == SYNCMODE_GENCLOCKANDSTARTSTOP) {
        Midiboy.dinMidi().write(0xFC);
      }
      g_SequencerState ^= SEQUENCERSTATE_PLAYING;
    }
    g_SequencerState ^= SEQUENCERSTATE_PENDING_STOP;
  }

  if (g_SequencerState & SEQUENCERSTATE_PENDING_PLAY) {
    if (!(g_SequencerState & SEQUENCERSTATE_PLAYING)) {
      for (uint8_t i = 0; i < N_TRACKS; i++) {
        track_t *track = &g_tracks[i];
        
        g_stepIndex[i] = 0;
        g_trackPatternIndex[i] = 0x00;
        g_notePatternIndex[i] = 0x77;
        g_currentStepLength[i] = 0;
        g_currentLoopStep[i] = LOOPSTEP_NOTSET;
        g_currentLoopCount[i] = 0;
        g_transposeNote[i] = 0;
        g_flags = SONG_GET_FLAGS;
      }
      if (SONG_GET_SYNC == SYNCMODE_GENCLOCKANDSTARTSTOP) {
        Midiboy.dinMidi().write(0xFA);
      }
      g_MIDIinData = 0;
      g_accentPatternIndex = 0;
      g_songBeatCount = 0;
      g_timerCount = MAX_TRIGGER_COUNT;
     
      g_SequencerState ^= SEQUENCERSTATE_PLAYING;
    }
    g_SequencerState ^= SEQUENCERSTATE_PENDING_PLAY;
  }

  if (g_SequencerState & SEQUENCERSTATE_PLAYING) {
    uint8_t count = 0;
    uint8_t masterReset = 0;
    while (count < N_TRACKS) {
      uint8_t i = g_trackOrder[count++];  
      track_t *track = &g_tracks[i];

      bool triggerMasterReset = (masterReset == 1 && TRK_GET_MASTERRESET == 1);
      bool accentModulation = (track->ptrnC & (0x80 >> g_accentPatternIndex));
      
      if (((g_timerCount % (TRK_GET_TIMEFACTOR)) == 0 || triggerMasterReset) && (g_songBeatCount >= TRK_GET_PREDELAY)) {
        for (uint8_t t = 0; t < TRACK_POLYPHONY; t++) {
          if (g_playing[i][t].note & 0x80) {
            if (!(g_playing[i][t].control & STEPCONTROL_HOLD)) {
              if (g_playing[i][t].duration == 0) {
                Midiboy.dinMidi().write(0x80 | TRK_GET_MIDICHANNEL);
                Midiboy.dinMidi().write(g_playing[i][t].note & 0x7F);
                Midiboy.dinMidi().write((uint8_t)0x00);
  
                if (g_playing[i][t].control & STEPCONTROL_MODULATION) {
                  Midiboy.dinMidi().write(0xB0 | TRK_GET_MIDICHANNEL);
                  Midiboy.dinMidi().write(0x01);
                  Midiboy.dinMidi().write((uint8_t)0x00);
                }
                g_playing[i][t].note ^= 0x80;
                g_playing[i][t].control = 0;
              } else {
                g_playing[i][t].duration--;
              }
            }
          }
        }
        
        if (triggerMasterReset || g_currentStepLength[i] == 0) {
          if (g_stepIndex[i] >= g_tracks[i].len || triggerMasterReset) {
            if (i == SONG_GET_MASTERTRACK_RAW) {
              masterReset = 1; 
            }
            if ((g_trackPatternIndex[i] & 0x07) == 0x07) {
              if (TRK_GET_TRACKPATTERNROTATION) {
                g_trackPatternIndex[i] = (g_trackPatternIndex[i] + 0x10) & 0x70;
              } else {
                g_trackPatternIndex[i] = 0x00;
              }
            } else {
              g_trackPatternIndex[i]++;
            }
            g_stepIndex[i] = 0;
            g_currentStepLength[i] = 0;
            g_currentLoopStep[i] = LOOPSTEP_NOTSET;
            g_currentLoopCount[i] = 0;
          }

          if (SONG_GET_TRANSPOSEMODE == TRANSPOSEMODE_STEP || g_stepIndex[i] == 0) {
            g_transposeNote[i] = (TRK_GET_TRANSPOSESOURCE == TRANSPOSESOURCE_MASTERTRACK) ? (int8_t)(g_playing[SONG_GET_MASTERTRACK_RAW][g_lastPlayingIndex[SONG_GET_MASTERTRACK_RAW]].note & 0x7F) : (g_MIDIinNote & 0x7F);
          }

          step_t *step = &g_tracks[i].steps[g_stepIndex[i]];
          if (!STEP_GET_FLAG || (g_flags & STEP_GET_FLAG) || (i == SONG_GET_MASTERTRACK_RAW)) {
            if (i == SONG_GET_MASTERTRACK_RAW) {
              g_flags ^= STEP_GET_FLAG;  
            }
            g_stepIndexPlaying[i] = g_stepIndex[i];

            uint8_t stepLengthFactor = 1;
            if (STEP_GET_MUTE == 0 && (g_soloTracks == 0 || ((g_soloTracks & (0x0001 << i)) != 0))) {
              uint8_t pattern = track->ptrnA;
              if (g_trackPatternIndex[i] >> 4 > 0) {
                pattern = (pattern << (g_trackPatternIndex[i] >> 4)) | (pattern >> (8 - (g_trackPatternIndex[i] >> 4)));
              }
              if (pattern & (0x80 >> (g_trackPatternIndex[i] & 0x07))) {
                if ((g_notePatternIndex[i] & 0x07) >= TRK_GET_NOTEPATTERNLEN_RAW) {
                  if (TRK_GET_NOTEPATTERNROTATION) {
                    g_notePatternIndex[i] = (g_notePatternIndex[i] + 0x10) & 0x70;
                  } else {
                    g_notePatternIndex[i] = 0x00;
                  }
                } else {
                  g_notePatternIndex[i]++;
                }
                
                bool accentModulation = (track->ptrnC & (0x80 >> g_accentPatternIndex));
                if (!accentModulation) {
                  pattern = track->ptrnB;
                  if (g_notePatternIndex[i] >> 4 > 0) {
                    pattern = (pattern << (g_notePatternIndex[i] >> 4)) | (pattern >> (8 - (g_notePatternIndex[i] >> 4)));
                  }
                } else if (TRK_GET_ACCENTMOD & ACCENTMOD_STEPLEN) {
                  stepLengthFactor += 1;
                }
                if (accentModulation || (pattern & (0x80 >> (g_notePatternIndex[i] & 0x07)))) {
                  int8_t note = ((STEP_GET_NOTE + TRK_GET_TRANSPOSE) & 0x7F);
                  if (TRK_GET_TRANSPOSESOURCE != 0) {
                    if (g_transposeNote[i] != 0) {
                      int16_t v = (int8_t) note + (g_transposeNote[i] - SONG_GET_ROOTNOTE);
                      if (v < 0) {
                        v = 0;
                      } else if (v > 127) {
                        v = 127;
                      }
                      note = (uint8_t)v;
                    }
                  }
                  note = ((note / 12)) * 12 + pgm_read_byte(&SCALES[TRK_GET_SCALE][note % 12]);
  
                  uint8_t playingIndex = (g_lastPlayingIndex[i] >= TRK_GET_POLYPHONY) ? 0 : g_lastPlayingIndex[i];
                  for (uint8_t t = 0; t < TRACK_POLYPHONY; t++) {
                    if ((!(g_playing[i][t].note & 0x80) || (g_playing[i][t].note & 0x7F) == note) && t < TRK_GET_POLYPHONY) {
                      playingIndex = t;
                      break;
                    } else if (STEP_GET_HOLD && (g_playing[i][t].control & STEPCONTROL_HOLD) && t < TRK_GET_POLYPHONY) {
                      playingIndex = t;
                      break;
                    }
                  }
                  g_lastPlayingIndex[i] = playingIndex;
                    
                  if (g_playing[i][playingIndex].note & 0x80) { 
                    Midiboy.dinMidi().write(0x80 | TRK_GET_MIDICHANNEL);
                    Midiboy.dinMidi().write(g_playing[i][playingIndex].note & 0x7F);
                    Midiboy.dinMidi().write((uint8_t)0x00);
                    if (g_playing[i][playingIndex].control & STEPCONTROL_MODULATION) {
                      Midiboy.dinMidi().write(0xB0 | TRK_GET_MIDICHANNEL);
                      Midiboy.dinMidi().write(0x01);
                      Midiboy.dinMidi().write((uint8_t)0x00);
                    }
                    g_playing[i][playingIndex].control = 0;
                  }

                  g_playing[i][playingIndex].note = note;
                  g_playing[i][playingIndex].duration = STEP_GET_DURATION - 1;

                  if (TRK_GET_MUTE == 0) {
                    uint8_t velocity = STEP_GET_VELOCITY_RAW;
                    if (accentModulation && (TRK_GET_ACCENTMOD & ACCENTMOD_VOLUME)) {
                      if (velocity < 3) {
                        velocity++;
                        accentModulation = false;
                      }
                    }
#ifdef ACCENT_TRIGGERS_CC                    
                    if (accentModulation) {
                      g_playing[i][playingIndex].control |= STEPCONTROL_MODULATION;
                      Midiboy.dinMidi().write(0xB0 | TRK_GET_MIDICHANNEL);
                      Midiboy.dinMidi().write(0x01);
                      Midiboy.dinMidi().write((uint8_t)0x40);
                    }
#endif                    
                    Midiboy.dinMidi().write(0x90 | TRK_GET_MIDICHANNEL);
                    Midiboy.dinMidi().write(g_playing[i][playingIndex].note);
/*
                    Midiboy.dinMidi().write(pgm_read_byte(&VELOCITY[0] + velocity));
*/                    
                    Midiboy.dinMidi().write(VELOCITY[velocity]);

                    g_playing[i][playingIndex].note |= 0x80;
                    if (STEP_GET_HOLD) {
                      g_playing[i][playingIndex].control |= STEPCONTROL_HOLD;  
                    } else {
                      g_playing[i][playingIndex].control &= (0xff ^ STEPCONTROL_HOLD);
                    }
                    
                  }
                }
              }
            }  
            g_currentStepLength[i] = STEP_GET_LENGTH * stepLengthFactor - 1; 
    
            if (STEP_GET_LOOP_START) {
              if (g_currentLoopCount[i] == 0) {
                g_currentLoopStep[i] = g_stepIndex[i];
              }
            }
            if (STEP_GET_LOOP_END > 0 && g_currentLoopStep[i] != LOOPSTEP_NOTSET) {
              if (g_currentLoopCount[i] > 0) {
                g_currentLoopCount[i]--;
                if (g_currentLoopCount[i] > 0) {
                  g_stepIndex[i] = g_currentLoopStep[i];
                } else {
                  g_currentLoopStep[i] = 0xff;
                  g_stepIndex[i]++;
                }
              } else {
                g_currentLoopCount[i] = STEP_GET_LOOP_END;
                g_stepIndex[i] = g_currentLoopStep[i];
              }
            } else {
              g_stepIndex[i]++;
            }            
          }
        } else {
          g_currentStepLength[i]--;
        }
      }
    }

    g_timerCount--;
    if (g_timerCount == 0) {
      g_timerCount = MAX_TRIGGER_COUNT;
    }
    g_accentPatternIndex += (g_timerCount % MIN_TRIGGER_COUNT == 1);
    if (g_accentPatternIndex > SONG_GET_ACCENTLEN_RAW) {
      g_accentPatternIndex = 0;
      if (g_songBeatCount != 0xff) {
        g_songBeatCount++;
      }
    }
  }

#ifdef MIDI_FLUSH_ON_IRQ
  Midiboy.dinMidi().flush();
#endif
}

void updateSongProperties(bool increase) {
  if (g_guiPropertyIndex == SONGOPTIONS_TEMPO) {
    if (increase) {SONG_INC_TEMPO;} else {SONG_DEC_TEMPO;}
    g_refreshSongMask |= REFRESHSONG_TEMPO;
  } else if (g_guiPropertyIndex == SONGOPTIONS_MASTERTRACK) {
    if (increase) {SONG_INC_MASTERTRACK;} else {SONG_DEC_MASTERTRACK;}
    g_refreshSongMask |= REFRESHSONG_TRACKORDER;
  } else if (g_guiPropertyIndex == SONGOPTIONS_SYNC) {
    if (increase) {SONG_INC_SYNC;} else {SONG_DEC_SYNC;}
  } else if (g_guiPropertyIndex == SONGOPTIONS_ROOTNOTE) {
    if (increase) {SONG_INC_ROOTNOTE;} else {SONG_DEC_ROOTNOTE;}
  } else if (g_guiPropertyIndex == SONGOPTIONS_FLAGS) {
    if (increase) {SONG_INC_FLAGS;} else {SONG_DEC_FLAGS;}
  } else if (g_guiPropertyIndex == SONGOPTIONS_ACCENTLEN) {
    if (increase) {SONG_INC_ACCENTLEN;} else {SONG_DEC_ACCENTLEN;}
  } else if (g_guiPropertyIndex == SONGOPTIONS_TRANSPOSEMODE) {
    SONG_TOGGLE_TRANSPOSEMODE;
  }
}

void updateTrackProperties(track_t *track, bool increase) {
  if (g_guiPropertyIndex == TRKPROP_MUTE) {
    TRK_TOGGLE_MUTE;
  } else if (g_guiPropertyIndex == TRKPROP_SOLO) {
    TRK_TOGGLE_SOLO;
    g_refreshSongMask |= REFRESHSONG_SOLOTRACKS;
  } else if (g_guiPropertyIndex == TRKPROP_MIDI) {
    if (increase) {TRK_INC_MIDICHANNEL;} else {TRK_DEC_MIDICHANNEL;}
  } else if (g_guiPropertyIndex == TRKPROP_TIMEFACTOR) {
    if (increase) {TRK_INC_TIMEFACTOR;} else {TRK_DEC_TIMEFACTOR;}
  } else if (g_guiPropertyIndex == TRKPROP_TRACKPATTERN) {
    if (increase) {TRK_INC_TRACKPATTERN;} else {TRK_DEC_TRACKPATTERN;}
  } else if (g_guiPropertyIndex == TRKPROP_TRACKPATTERNROTATION) {
    TRK_TOGGLE_TRACKPATTERNROTATION;
  } else if (g_guiPropertyIndex == TRKPROP_NOTEPATTERN) {
    if (increase) {TRK_INC_NOTEPATTERN;} else {TRK_DEC_NOTEPATTERN;}
  } else if (g_guiPropertyIndex == TRKPROP_NOTEPATTERNLEN) {
    if (increase) {TRK_INC_NOTEPATTERNLEN;} else {TRK_DEC_NOTEPATTERNLEN;}
  } else if (g_guiPropertyIndex == TRKPROP_NOTEPATTERNROTATION) {
    TRK_TOGGLE_NOTEPATTERNROTATION;
  } else if (g_guiPropertyIndex == TRKPROP_TRANSPOSESOURCE) {
    if (increase) {TRK_INC_TRANSPOSESOURCE;} else {TRK_DEC_TRANSPOSESOURCE;}
  } else if (g_guiPropertyIndex == TRKPROP_TRANSPOSE) {
    if (increase) {TRK_INC_TRANSPOSE;} else {TRK_DEC_TRANSPOSE;}
  } else if (g_guiPropertyIndex == TRKPROP_MASTERRESET) {
    TRK_TOGGLE_MASTERRESET;
  } else if (g_guiPropertyIndex == TRKPROP_POLYPHONY) {
    if (increase) {TRK_INC_POLYPHONY;} else {TRK_DEC_POLYPHONY;}
  } else if (g_guiPropertyIndex == TRKPROP_SCALE) {
    if (increase) {TRK_INC_SCALE;} else {TRK_DEC_SCALE;}
  } else if (g_guiPropertyIndex == TRKPROP_PREDELAY) {
    if (increase) {TRK_INC_PREDELAY;} else {TRK_DEC_PREDELAY;}
  } else if (g_guiPropertyIndex == TRKPROP_ACCENTPATTERN) {
    if (increase) {TRK_INC_ACCENTPATTERN;} else {TRK_DEC_ACCENTPATTERN;}
  } else if (g_guiPropertyIndex == TRKPROP_ACCENTMODIFIER) {
    if (increase) {TRK_INC_ACCENTMOD;} else {TRK_DEC_ACCENTMOD;}
  }             
}

void updateStepProperties(step_t *step, bool increase) {
  if (g_guiPropertyIndex == STEP_PROPERTY_NOTE) {
    if (increase) {STEP_INC_NOTE;} else {STEP_DEC_NOTE;}
  } else if (g_guiPropertyIndex == STEP_PROPERTY_DURATION) {
    if (increase) {STEP_INC_DURATION_RAW;} else {STEP_DEC_DURATION_RAW;}
  } else if (g_guiPropertyIndex == STEP_PROPERTY_LENGTH) {
    if (increase) {STEP_INC_LENGTH_RAW;} else {STEP_DEC_LENGTH_RAW;}
  } else if (g_guiPropertyIndex == STEP_PROPERTY_LOOP_START) {
    STEP_TOGGLE_LOOP_START;
  } else if (g_guiPropertyIndex == STEP_PROPERTY_LOOP_END) {
    if (increase) {STEP_INC_LOOP_END;} else {STEP_DEC_LOOP_END;}
  } else if (g_guiPropertyIndex == STEP_PROPERTY_VELOCITY) {
    if (increase) {STEP_INC_VELOCITY;} else {STEP_DEC_VELOCITY;}
  } else if (g_guiPropertyIndex == STEP_PROPERTY_HOLD) {
    STEP_TOGGLE_HOLD;
  } else if (g_guiPropertyIndex == STEP_PROPERTY_FLAG) {
    if (increase) {STEP_INC_FLAG;} else {STEP_DEC_FLAG;};
  }    
}

void renderStepTrackDetails() {
  track_t *track = &g_tracks[g_guiTrackIndex];

  printByte(g_guiStepEditIndex + 1, false);
  Midiboy.drawBitmap_P(&FONT::DATA_P[('/' - ' ') * FONT::WIDTH], FONT::WIDTH, false);
  printByte(TRK_GET_LENGTH, false);
  Midiboy.drawSpace(4, false);
  printString_P(LABEL_MIDI[0], false);
  Midiboy.drawSpace(2, false);
  printByte(TRK_GET_MIDICHANNEL + 1, false);
  Midiboy.drawSpace(2, false);
  if (TRK_GET_SOLO) {
    printString_P(LABEL_SOLO[0], false);
    Midiboy.drawSpace(4, false);
  } else if (TRK_GET_MUTE) {
    printString_P(LABEL_MUTE[0], false);
    Midiboy.drawSpace(4, false);
  }
  if (g_guiTrackIndex == SONG_GET_MASTERTRACK_RAW) {
    printString_P(LABEL_MASTER[0], false);
  }
  
}

void renderPlayingDetails() {
  track_t *track = &g_tracks[g_guiTrackIndex];
  
  if (g_SequencerState & SEQUENCERSTATE_PLAYING) {
    g_scratchBitmap[0] = 0b01111111;
    for (uint8_t t = 0; t < TRK_GET_POLYPHONY; t++) {
      if (g_playing[g_guiTrackIndex][t].note & 0x80) {
        uint8_t d = (g_playing[g_guiTrackIndex][t].duration & 0x07) + 1;
        d = d > 5 ? 5 : d;
        g_scratchBitmap[1] = (0b00111110 << d) & 0b00111110 | 0b01000001;
        g_scratchBitmap[2] = g_scratchBitmap[1];
      } else {
        g_scratchBitmap[1] = 0b01111111;
        g_scratchBitmap[2] = g_scratchBitmap[1];
      }
      Midiboy.drawBitmap(g_scratchBitmap, 3, false);
    }
    Midiboy.drawBitmap(g_scratchBitmap, 1, false);
    Midiboy.drawSpace(2, false);       
    printByte(g_songBeatCount, false);
    Midiboy.drawBitmap_P(&FONT::DATA_P[('.' - ' ') * FONT::WIDTH], FONT::WIDTH, false);
    printByte(g_accentPatternIndex, false);

    if (g_songBeatCount < TRK_GET_PREDELAY) {
      Midiboy.drawBitmap_P(BITMAP_TRACKLANE[1], 5, false);
      printByte(TRK_GET_PREDELAY, false);  
    }
  }
}

void renderTrackBar(bool invertedDraw) {
  track_t *track = &g_tracks[g_guiTrackIndex];
  Midiboy.drawSpace(1, invertedDraw);
  printString_P(LABEL_TRKPROP[g_guiPropertyIndex], invertedDraw);
  Midiboy.drawSpace(2, false);

  Midiboy.drawSpace(1, invertedDraw);
  if (g_guiPropertyIndex == TRKPROP_MUTE) {
    printString_P(LABEL_BOOL[TRK_GET_MUTE], invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_SOLO) {
    printString_P(LABEL_BOOL[TRK_GET_SOLO], invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_TIMEFACTOR) {
    printByte(TRK_GET_TIMEFACTOR_RAW + 1, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_MIDI) {
    printByte(TRK_GET_MIDICHANNEL + 1, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_TRACKPATTERN) {
    printByte(TRK_GET_TRACKPATTERN, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_TRACKPATTERNROTATION) {
    printString_P(LABEL_BOOL[TRK_GET_TRACKPATTERNROTATION], invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_NOTEPATTERN) {
    printByte(TRK_GET_NOTEPATTERN, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_NOTEPATTERNLEN) {
    printByte(TRK_GET_NOTEPATTERNLEN, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_NOTEPATTERNROTATION) {
    printString_P(LABEL_BOOL[TRK_GET_NOTEPATTERNROTATION], invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_TRANSPOSESOURCE) {
    printString_P(LABEL_TRANSPOSESOURCE[TRK_GET_TRANSPOSESOURCE], invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_TRANSPOSE) {
    printByteSigned(TRK_GET_TRANSPOSE, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_MASTERRESET) {
    printString_P(LABEL_BOOL[TRK_GET_MASTERRESET], invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_POLYPHONY) {
    printByte(TRK_GET_POLYPHONY, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_SCALE) {
    printString_P(LABEL_SCALE[TRK_GET_SCALE], invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_PREDELAY) {
    printByte(TRK_GET_PREDELAY, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_ACCENTPATTERN) {
    printByte(TRK_GET_ACCENTPATTERN, invertedDraw);
  } else if (g_guiPropertyIndex == TRKPROP_ACCENTMODIFIER) {
    printString_P(LABEL_ACCENTMOD[TRK_GET_ACCENTMOD], invertedDraw);
  }
}

void renderSongBar(bool invertedDraw) {
  for (uint8_t i = 0; i < SONGMENU_LAST; i++) {
    bool canEdit = ((g_guiSongMenuIndex == i) ? invertedDraw : false);
    if (i == SONGMENU_OPTIONS) {
      if (canEdit && g_keyModifier & KEYMODIFIER_EDIT) {
        canEdit = true;
      }
      Midiboy.drawSpace(1, canEdit);
      printString_P(LABEL_SONGOPTIONS[g_guiPropertyIndex], canEdit);
      Midiboy.drawSpace(2, false);
      Midiboy.drawSpace(1, canEdit);
      if (g_guiPropertyIndex == SONGOPTIONS_TEMPO) {
        printByte(SONG_GET_TEMPO, canEdit);
      } else if (g_guiPropertyIndex == SONGOPTIONS_MASTERTRACK) {
        printByte(SONG_GET_MASTERTRACK, canEdit);
      } else if (g_guiPropertyIndex == SONGOPTIONS_TRANSPOSEMODE) {
        printString_P(LABEL_TRANSPOSEMODE[SONG_GET_TRANSPOSEMODE], invertedDraw);
      } else if (g_guiPropertyIndex == SONGOPTIONS_ACCENTLEN) {
        printByte(SONG_GET_ACCENTLEN, canEdit);        
      } else if (g_guiPropertyIndex == SONGOPTIONS_SYNC) {
        printString_P(LABEL_SYNCMODE[SONG_GET_SYNC], canEdit);
      } else if (g_guiPropertyIndex == SONGOPTIONS_ROOTNOTE) {
        printString_P(LABEL_NOTES[(SONG_GET_ROOTNOTE % 12)], invertedDraw);
        Midiboy.drawBitmap_P(&FONT::DATA_P[('0' + (SONG_GET_ROOTNOTE / 12 - 2) - ' ') * FONT::WIDTH], FONT::WIDTH, invertedDraw);
      } else if (g_guiPropertyIndex == SONGOPTIONS_FLAGS) {
        Midiboy.drawBitmap_P(BITMAP_FLAG_INV[SONG_GET_FLAGS], FONT::WIDTH, true);
        Midiboy.drawSpace(2, true);
      }
    } else {
      Midiboy.drawSpace(1, canEdit);
      printString_P(LABEL_SONGMENU[i], canEdit);
    }
    Midiboy.drawSpace(2, false);
  }
}

void renderPattern(uint8_t pattern, uint8_t activeIndex, bool isLastPattern) {
  uint8_t i = 0;

  uint8_t bitmapSample = 0;
  for (uint8_t c = 0; c < 4; c++) {
    uint8_t i = c;
    if (activeIndex == i) {
      g_scratchBitmap[0] = (pattern & (0x80 >> i)) ? 0b01111000 : bitmapSample & 0b01000000 ? 0b01111000 : 0b00000000;
      g_scratchBitmap[1] = (pattern & (0x80 >> i)) ? 0b01111000 : 0b00000000;
      g_scratchBitmap[2] = g_scratchBitmap[1];
    } else {
      g_scratchBitmap[0] = (pattern & (0x80 >> i)) ? 0b01111000 : bitmapSample & 0b01000000 ? 0b01111000 : 0b00000000;
      g_scratchBitmap[1] = (pattern & (0x80 >> i)) ? 0b01001000 : 0b00000000;
      g_scratchBitmap[2] = g_scratchBitmap[1];
    }
    i += 4;
    if (activeIndex == i) {
      g_scratchBitmap[0] |= (pattern & (0x80 >> i)) ? 0b00001111 : bitmapSample & 0b00000001 ? 0b00001111 : 0b00000000;
      g_scratchBitmap[1] |= (pattern & (0x80 >> i)) ? 0b00001111 : 0b00000000;
      g_scratchBitmap[2] |= (pattern & (0x80 >> i)) ? 0b00001111 : 0b00000000;
    } else {
      g_scratchBitmap[0] |= (pattern & (0x80 >> i)) ? 0b00001111 : bitmapSample & 0b00000001 ? 0b00001111 : 0b00000000;
      g_scratchBitmap[1] |= (pattern & (0x80 >> i)) ? 0b00001001 : 0b00000000;  
      g_scratchBitmap[2] |= (pattern & (0x80 >> i)) ? 0b00001001 : 0b00000000;  
    }
    Midiboy.drawBitmap(g_scratchBitmap, 3, false);
    bitmapSample = g_scratchBitmap[1];
  }
  g_scratchBitmap[0] = bitmapSample & 0b01000000 ? 0b01111000 : 0b00000000;
  g_scratchBitmap[0] |= bitmapSample & 0b00000001 ? 0b00001111 : 0b00000000;
  g_scratchBitmap[1] = 0; 
  g_scratchBitmap[2] = 0; 
  Midiboy.drawBitmap(g_scratchBitmap, isLastPattern ? 1 : 3, false);
}

void renderPatterns() {
  Midiboy.drawSpace((127 - 3 * 14) - Midiboy.getDrawPositionX(), false);

  uint8_t pattern = g_tracks[g_guiTrackIndex].ptrnC;
  renderPattern(pattern, (g_SequencerState & SEQUENCERSTATE_PLAYING) ? (g_accentPatternIndex) : 0xff, false);

  pattern = g_tracks[g_guiTrackIndex].ptrnB;
  if (g_SequencerState & SEQUENCERSTATE_PLAYING) {
    if (g_notePatternIndex[g_guiTrackIndex] >> 4 > 0) {
      pattern = (pattern << (g_notePatternIndex[g_guiTrackIndex] >> 4)) | (pattern >> (8 - (g_notePatternIndex[g_guiTrackIndex] >> 4)));
    }
  }
  renderPattern(pattern, (g_SequencerState & SEQUENCERSTATE_PLAYING) ? (g_notePatternIndex[g_guiTrackIndex] & 0x07) : 0xff, false);

  pattern = g_tracks[g_guiTrackIndex].ptrnA;
  if (g_SequencerState & SEQUENCERSTATE_PLAYING) {
    if (g_trackPatternIndex[g_guiTrackIndex] >> 4 > 0) {
      pattern = (pattern << (g_trackPatternIndex[g_guiTrackIndex] >> 4)) | (pattern >> (8 - (g_trackPatternIndex[g_guiTrackIndex] >> 4)));
    }
  }
  renderPattern(pattern, (g_SequencerState & SEQUENCERSTATE_PLAYING) ? (g_trackPatternIndex[g_guiTrackIndex] & 0x07) : 0xff, true);
}

void renderTracks() {
  uint16_t guiRowOffset = g_guiRow;
  uint16_t editY = 4;

  if (guiRowOffset < 4) {
    editY = GUI_VISIBLE_ROWS - guiRowOffset;
    guiRowOffset = 0;
  } else {
    guiRowOffset -= 3;
  }

  uint8_t d = (SONG_GET_ACCENTLEN_RAW + 1);
  for (uint8_t i = 0; i < GUI_VISIBLE_ROWS; i++) {
    uint8_t v = GUI_VISIBLE_ROWS - i;
    Midiboy.setDrawPosition(0, v);
    Midiboy.drawBitmap_P(BITMAP_TRACKLANE[5 + (((i + guiRowOffset) / d) % 2)], FONT::WIDTH, false);
  }

  g_guiStepEditIndex = 0;
 
  uint8_t startIndex = (g_guiTrackIndex / VISIBLE_TRACKS) * VISIBLE_TRACKS;
  uint8_t endIndex = startIndex + VISIBLE_TRACKS;
  if (endIndex > N_TRACKS) {
    endIndex -= N_TRACKS;
  }
  for (uint8_t i = startIndex ; i < endIndex; i++) {
    uint16_t guiRow = 0;

    uint8_t x = (i - startIndex) * 8 * FONT::WIDTH + 8;
    Midiboy.setDrawPosition(x, GUI_VISIBLE_ROWS);
    if (i == g_guiTrackIndex) {
      if (i < 9) {
         Midiboy.drawBitmap_P(&FONT::DATA_P[('1' + i - ' ') * FONT::WIDTH], FONT::WIDTH, (g_editState == EDITSTATE_TRACK ? !(g_keyModifier & KEYMODIFIER_BLINK) : true));
      } else {
         Midiboy.drawBitmap_P(&FONT::DATA_P[('A' + (i - 9) - ' ') * FONT::WIDTH], FONT::WIDTH, (g_editState == EDITSTATE_TRACK ? !(g_keyModifier & KEYMODIFIER_BLINK) : true));
      }
    } else {
      Midiboy.drawSpace(FONT::WIDTH, false);
    }
    x += 8;

    bool loopSection = false;
    for (uint8_t stepIndex = 0; stepIndex <  SEQUENCE_LENGTH; stepIndex++) {
      step_t *step = &g_tracks[i].steps[stepIndex];

      uint16_t y = GUI_VISIBLE_ROWS - (guiRow - guiRowOffset);
      bool canEdit = false;
      bool invertedDraw = false;
     
      loopSection |= STEP_GET_LOOP_START;
      
      if (guiRow >= guiRowOffset && guiRow < (guiRowOffset + GUI_VISIBLE_ROWS)) {
        
        Midiboy.setDrawPosition(x, y);
        canEdit = (y == editY) && (g_guiTrackIndex == i) && (g_editState == EDITSTATE_SEQUENCER);

        invertedDraw = canEdit;
        if (canEdit && (g_keyModifier & KEYMODIFIER_BLINK)) {
          invertedDraw = false;
        }

        if (stepIndex == (g_tracks[i].len - 1)) {
          Midiboy.drawBitmap_P(BITMAP_TRACKLANE[2], FONT::WIDTH, false);
        } else {
          Midiboy.drawSpace(FONT::WIDTH, false);
        }

        if (g_guiPropertyIndex == STEP_PROPERTY_NOTE || !canEdit) {
          
#ifdef MIDI_IN_ENABLED
          if (canEdit && (g_MIDIinNote & 0x80) && (g_keyModifier & KEYMODIFIER_EDIT)) {
            g_MIDIinNote &= 0x7F; // Consumed
            step->note = g_MIDIinNote;
            g_guiRow = (stepIndex != (SEQUENCE_LENGTH - 1)) ? g_guiRow + (STEP_GET_LENGTH) : 0;
          }
#endif          
          if ((g_keyModifier & KEYMODIFIER_EDIT && canEdit) || !STEP_GET_MUTE) {
            printString_P(LABEL_NOTES[(STEP_GET_NOTE % 12)], invertedDraw);
            Midiboy.drawBitmap_P(&FONT::DATA_P[('0' + (STEP_GET_NOTE / 12 - 2) - ' ') * FONT::WIDTH], FONT::WIDTH, invertedDraw);
          } else {
            Midiboy.drawSpace(FONT::WIDTH, invertedDraw);
            Midiboy.drawBitmap_P(BITMAP_TRACKLANE[3], FONT::WIDTH, invertedDraw);
            Midiboy.drawSpace(FONT::WIDTH, invertedDraw);
          }
        } else { // Editing step properties
          if (g_guiPropertyIndex == STEP_PROPERTY_DURATION) {
            printNotePropertyValue(STEP_GET_DURATION, 'd', invertedDraw);
          } else if (g_guiPropertyIndex == STEP_PROPERTY_VELOCITY) {
            printNotePropertyValue(STEP_GET_VELOCITY_RAW + 1, 'v', invertedDraw);
          } else if (g_guiPropertyIndex == STEP_PROPERTY_LENGTH) {
            printNotePropertyValue(STEP_GET_LENGTH, 'l', invertedDraw);
          } else if (g_guiPropertyIndex == STEP_PROPERTY_LOOP_START) {
            printNotePropertyValue(STEP_GET_LOOP_START ? 1 : 0, 's', invertedDraw);
          } else if (g_guiPropertyIndex == STEP_PROPERTY_LOOP_END) {
            printNotePropertyValue(STEP_GET_LOOP_END, 'e', invertedDraw);
          } else if (g_guiPropertyIndex == STEP_PROPERTY_HOLD) {
            printNotePropertyValue(STEP_GET_HOLD, 'h', invertedDraw);
          } else if (g_guiPropertyIndex == STEP_PROPERTY_FLAG) {
            Midiboy.drawBitmap_P(&FONT::DATA_P[('f' - ' ') * FONT::WIDTH], FONT::WIDTH, invertedDraw);
            Midiboy.drawSpace(FONT::WIDTH * 2, invertedDraw);
          }
        }

        if (loopSection) {
          if (STEP_GET_LOOP_START) {
            if (STEP_GET_LOOP_END) {
              (STEP_GET_LENGTH == 1) ? Midiboy.drawBitmap_P(BITMAP_TRACKLANE[14], FONT::WIDTH / 2, false) : Midiboy.drawBitmap_P(BITMAP_TRACKLANE[11], FONT::WIDTH / 2, false);
            } else {
              Midiboy.drawBitmap_P(BITMAP_TRACKLANE[11], FONT::WIDTH / 2, false);
            }
          } else {
            (STEP_GET_LOOP_END && STEP_GET_LENGTH == 1) ? Midiboy.drawBitmap_P(BITMAP_TRACKLANE[13], FONT::WIDTH / 2, false) :  Midiboy.drawBitmap_P(BITMAP_TRACKLANE[12], FONT::WIDTH / 2, false);
          }
        } else {
          Midiboy.drawSpace(FONT::WIDTH / 2, false);
        }

        if ((g_SequencerState & SEQUENCERSTATE_PLAYING) && g_stepIndexPlaying[i] == stepIndex) {
         if (g_guiTrackIndex == i) {
            Midiboy.drawBitmap_P(BITMAP_FLAG_INV[g_flags], FONT::WIDTH, false);
          } else {
            Midiboy.drawBitmap_P(BITMAP_TRACKLANE[4], FONT::WIDTH, false);
          }
        } else {
          Midiboy.drawSpace(FONT::WIDTH, false);
        }
        if (STEP_GET_FLAG && (g_guiTrackIndex == i)) {
          Midiboy.drawBitmap_P(BITMAP_FLAG[STEP_GET_FLAG], FONT::WIDTH, true); 
        } else {
          Midiboy.drawSpace(FONT::WIDTH, false);
        }
      }

      uint16_t stepLength = STEP_GET_LENGTH;
      if (g_guiTrackIndex == i && g_guiRow >= guiRow && g_guiRow < (guiRow + stepLength)) {
        g_guiStepEditIndex = stepIndex; 
        if (g_keyModifier & KEYMODIFIER_EDIT && !canEdit) {
          g_guiRow = guiRow;
        }
      }
      guiRow++;
      stepLength--;

      if (guiRow + stepLength >= guiRowOffset) {
        if ((guiRow + stepLength) >= (guiRowOffset + GUI_VISIBLE_ROWS)) {
          if (guiRow >= (guiRowOffset + GUI_VISIBLE_ROWS)) {
            break;
          }
        }
        uint16_t stepDuration = STEP_GET_DURATION;
        if (guiRow < guiRowOffset) {
          stepLength -= (guiRowOffset - guiRow);
          if ((guiRowOffset - guiRow) < stepDuration) {
            stepDuration -= (guiRowOffset - guiRow);
          } else {
            stepDuration = 0;
          }
          y = GUI_VISIBLE_ROWS;
        } else {
          y = GUI_VISIBLE_ROWS - (guiRow - guiRowOffset);
        }

        while (stepLength > 0 && y >= 1) {
          canEdit = (y == editY) && (g_guiTrackIndex == i) && (g_editState == EDITSTATE_SEQUENCER);
          invertedDraw = canEdit;
          if (canEdit && (g_keyModifier & KEYMODIFIER_BLINK)) {
            invertedDraw = false;
          }

          Midiboy.setDrawPosition(x, y);
          Midiboy.drawSpace(FONT::WIDTH * 2,  false);
          if (stepDuration > 1) {
            (STEP_GET_MUTE) ?  Midiboy.drawSpace(FONT::WIDTH, canEdit) : Midiboy.drawBitmap_P(BITMAP_TRACKLANE[0], FONT::WIDTH, invertedDraw);
            stepDuration--;
          } else {
            Midiboy.drawSpace(FONT::WIDTH, invertedDraw);
          }
          Midiboy.drawSpace(FONT::WIDTH,  false);
          if (loopSection == true) {
            (stepLength == 1 && STEP_GET_LOOP_END) ? Midiboy.drawBitmap_P(BITMAP_TRACKLANE[13], FONT::WIDTH / 2, false) : Midiboy.drawBitmap_P(BITMAP_TRACKLANE[12], FONT::WIDTH / 2, false);
          } else {
            Midiboy.drawSpace(FONT::WIDTH / 2,  false);
          }
          Midiboy.drawSpace(FONT::WIDTH * 2, false);
          stepLength--;
          y--;
        }
      }
      guiRow = (guiRow + STEP_GET_LENGTH) - 1;
      if (STEP_GET_LOOP_END) {
        loopSection = false;
      }
    }

    while (guiRow >= guiRowOffset && guiRow < (guiRowOffset + GUI_VISIBLE_ROWS)) {
      Midiboy.setDrawPosition(x, (GUI_VISIBLE_ROWS - (guiRow - guiRowOffset)));
      Midiboy.drawSpace(FONT::WIDTH * 6, false);
      guiRow++;
    }
  }
}

void handleInput() {
  MidiboyInput::Event event;
  while (Midiboy.readInputEvent(event)) {
    if (event.m_type == MidiboyInput::EVENT_DOWN) {

      track_t *track = &g_tracks[g_guiTrackIndex];
      step_t *step = &g_tracks[g_guiTrackIndex].steps[g_guiStepEditIndex];

      bool increase = true;
      
      switch (event.m_button) {
        case MidiboyInput::BUTTON_RIGHT:
          if (g_keyModifier & KEYMODIFIER_B) {
            if (g_editState == EDITSTATE_SEQUENCER && !(g_keyModifier & KEYMODIFIER_EDIT)) {
              STEP_TOGGLE_MUTE;
              g_keyModifier |= KEYMODIFIER_CANCEL;
            }
          } else if (g_keyModifier & KEYMODIFIER_EDIT) {
            if (g_editState == EDITSTATE_SEQUENCER) {
              g_guiPropertyIndex = (g_guiPropertyIndex == (STEP_PROPERTY_LAST - 1)) ? g_guiPropertyIndex : g_guiPropertyIndex + 1;
            } else if (g_editState == EDITSTATE_TRACK) {
              g_guiPropertyIndex = (g_guiPropertyIndex == (TRKPROP_LAST - 1)) ? g_guiPropertyIndex : g_guiPropertyIndex + 1;
            } else if (g_guiSongMenuIndex == SONGMENU_OPTIONS) {
              g_guiPropertyIndex = (g_guiPropertyIndex == (SONGOPTIONS_LAST - 1)) ? g_guiPropertyIndex : g_guiPropertyIndex + 1;
            }
          } else if (g_editState != EDITSTATE_SONG) {
            g_guiTrackIndex = (g_guiTrackIndex == (N_TRACKS - 1)) ? g_guiTrackIndex : g_guiTrackIndex + 1;
            uint16_t l = getTrackLength(g_guiTrackIndex);
            if (l <= g_guiRow) {
              g_guiRow = l - 1;
            }
            g_guiPropertyIndex = 0;
          } else {
            g_guiSongMenuIndex = (g_guiSongMenuIndex == (SONGMENU_LAST - 1)) ? 0 : g_guiSongMenuIndex + 1;
          }
          break;

        case MidiboyInput::BUTTON_LEFT:
          if (g_keyModifier & KEYMODIFIER_B) {
            if (!(g_keyModifier & KEYMODIFIER_EDIT)) {
              if (g_editState == EDITSTATE_SEQUENCER) {
                g_tracks[g_guiTrackIndex].len = g_guiStepEditIndex + 1;
              }
              g_keyModifier |= KEYMODIFIER_CANCEL;
            }
          } else if (g_keyModifier & KEYMODIFIER_EDIT) {
            if (g_editState == EDITSTATE_SEQUENCER) {
              g_guiPropertyIndex = (g_guiPropertyIndex == 0) ? g_guiPropertyIndex : g_guiPropertyIndex - 1;
            } else if (g_editState == EDITSTATE_TRACK) {
              g_guiPropertyIndex = (g_guiPropertyIndex == 0) ? g_guiPropertyIndex : g_guiPropertyIndex - 1;
            } else if (g_guiSongMenuIndex == SONGMENU_OPTIONS) {
              g_guiPropertyIndex = (g_guiPropertyIndex == 0) ? g_guiPropertyIndex : g_guiPropertyIndex - 1;
            }
          } else if (g_editState != EDITSTATE_SONG) {
            g_guiTrackIndex = (g_guiTrackIndex == 0) ? g_guiTrackIndex : g_guiTrackIndex - 1;
            uint16_t l = getTrackLength(g_guiTrackIndex);
            if (l <= g_guiRow) {
              g_guiRow = l - 1;
            }
            g_guiPropertyIndex = 0;
          } else {
            g_guiSongMenuIndex = (g_guiSongMenuIndex == 0) ? (SONGMENU_LAST - 1) : g_guiSongMenuIndex - 1;
          }
          break;

        case MidiboyInput::BUTTON_DOWN:
          increase = false;
        case MidiboyInput::BUTTON_UP:
          if (g_keyModifier & KEYMODIFIER_EDIT) {
            if (g_editState == EDITSTATE_SEQUENCER) {
              updateStepProperties(step, increase);
            } else if (g_editState == EDITSTATE_TRACK) {
              updateTrackProperties(track, increase);
            } else if (g_editState == EDITSTATE_SONG && g_guiSongMenuIndex == SONGMENU_OPTIONS) {
              updateSongProperties(increase);
            }
          } else if (g_editState == EDITSTATE_SEQUENCER) {
            if (g_keyModifier & KEYMODIFIER_B) {
              if (increase) {
                insertStep(&g_tracks[g_guiTrackIndex], g_guiStepEditIndex);
              } else {
                deleteStep(&g_tracks[g_guiTrackIndex], g_guiStepEditIndex);
              }
              g_keyModifier |= KEYMODIFIER_CANCEL;
            } else {
              if (increase) {
                if (g_guiRow > 0) {
                  g_guiPropertyIndex = 0;
                  g_guiRow--;
                }
              } else {
                if (g_guiStepEditIndex != (SEQUENCE_LENGTH - 1)) {
                  g_guiPropertyIndex = 0;
                  g_guiRow++;
                } 
              } 
            }
          } else if (g_editState == EDITSTATE_TRACK || g_editState == EDITSTATE_SONG) {
            g_editState = (g_editState == EDITSTATE_TRACK) ? EDITSTATE_SONG : EDITSTATE_TRACK;
          }
          break;
          
        case MidiboyInput::BUTTON_A:
          g_keyModifier |= KEYMODIFIER_A;
          if (g_keyModifier & KEYMODIFIER_B) {
            if (g_editState == EDITSTATE_SEQUENCER) {
              g_editState = EDITSTATE_TRACK;
            } else if (g_editState == EDITSTATE_TRACK || g_editState == EDITSTATE_SONG) {
              g_editState = EDITSTATE_SEQUENCER;
            }
            g_keyModifier = (g_keyModifier & (0xff ^ KEYMODIFIER_EDIT)) | KEYMODIFIER_CANCEL;
            g_guiPropertyIndex = 0;
          } else {
            if (!(g_SequencerState & SEQUENCERSTATE_PENDING_STOP) && !(g_SequencerState & SEQUENCERSTATE_PENDING_PLAY)) {
              if (g_SequencerState & SEQUENCERSTATE_PLAYING) {
                g_SequencerState |= SEQUENCERSTATE_PENDING_STOP;
              } else {
                g_SequencerState |= SEQUENCERSTATE_PENDING_PLAY;
              }
            }
          }
          break;
          
        case MidiboyInput::BUTTON_B:
          g_keyModifier |= KEYMODIFIER_B;
          break;
      }
    } else if (event.m_type == MidiboyInput::EVENT_UP) {
      switch (event.m_button) {
        case MidiboyInput::BUTTON_A:
          g_keyModifier = (g_keyModifier & (0xff ^ KEYMODIFIER_A));
          break;
        case MidiboyInput::BUTTON_B:
          if (g_keyModifier & KEYMODIFIER_B && !(g_keyModifier & KEYMODIFIER_CANCEL)) {
            if (g_editState == EDITSTATE_SONG && g_guiSongMenuIndex == SONGMENU_LOAD) {
              if (g_SequencerState & SEQUENCERSTATE_PLAYING) {
                g_SequencerState |= SEQUENCERSTATE_PENDING_STOP;
              }
              loadFromEEPROM();
              g_refreshSongMask = REFRESHSONG_ALL;

              g_editState = EDITSTATE_SEQUENCER;
              g_guiTrackIndex = 0;
            } else if (g_editState == EDITSTATE_SONG && g_guiSongMenuIndex == SONGMENU_SAVE) {
              saveToEEPROM();
              g_editState = EDITSTATE_SEQUENCER;
            } else {
              g_keyModifier ^= KEYMODIFIER_EDIT;
              if (g_keyModifier & KEYMODIFIER_EDIT) {
                 g_MIDIinNote &= 0x7F;
              }
            }
          }
          g_guiPropertyIndex = 0;
          g_keyModifier = (g_keyModifier & (0xff ^ (KEYMODIFIER_B | KEYMODIFIER_CANCEL)));
          break;
      }
    }
  }
}

void handleMidiIn() {
  while (Midiboy.dinMidi().available()) {
    uint8_t b = Midiboy.dinMidi().read();
    if (g_MIDIinData == 0x80) {
      g_MIDIinNote = b | 0x80;
      g_MIDIinData = 1;
    } else if (g_MIDIinData == 0) {
      g_MIDIinData = ((b & 0x80) && (b & 0xf0) == 0x90) ? 0x80 : 0;
    } else {
      g_MIDIinData--;
    }
  }
}

void loop() {
  unsigned long blinkTimestamp = millis();
  if (blinkTimestamp - g_blinkTimestamp >= (60 * 4)) {
    if (g_keyModifier & KEYMODIFIER_EDIT) {
      g_keyModifier = g_keyModifier & (0xff ^ KEYMODIFIER_BLINK);
    } else {
      g_keyModifier ^= KEYMODIFIER_BLINK;
    }
    g_blinkTimestamp = blinkTimestamp;
    if (g_logoCount > 0) {
      g_logoCount--;
      if (g_logoCount == 0) {
        Midiboy.clearScreen();
      }
    }
  }

  Midiboy.think();

#ifdef MIDI_IN_ENABLED
  handleMidiIn();
#endif

  if (g_logoCount > 0) {
    return;
  }
  handleInput();

  if (g_refreshSongMask != REFRESHSONG_CLEAR) {
    if (g_refreshSongMask & REFRESHSONG_TEMPO) {
      setTimer(g_song.bpm);
    }
    if (g_refreshSongMask & REFRESHSONG_TRACKORDER) {
      setTrackOrder();
    }
    if (g_refreshSongMask & REFRESHSONG_SOLOTRACKS) {
      setSoloTracks();
    }
    g_refreshSongMask = REFRESHSONG_CLEAR;
  }

  renderTracks();

  Midiboy.setDrawPosition(0, 0);
  bool invertedDraw = (!(g_keyModifier & KEYMODIFIER_EDIT) && (g_keyModifier & KEYMODIFIER_BLINK)) ? false : true;
  if (g_editState == EDITSTATE_SONG) {
     renderSongBar(invertedDraw);
     Midiboy.drawSpace((128) - Midiboy.getDrawPositionX(), false);
     g_stepDetailsTimestamp = blinkTimestamp;
  } else {
    if (g_editState == EDITSTATE_SEQUENCER) {
      if (g_lastRowTrack != ((g_guiStepEditIndex & 0x01) | ((g_guiTrackIndex & 0x01) << 1))) {
        g_stepDetailsTimestamp = blinkTimestamp;
        g_lastRowTrack = (g_guiStepEditIndex & 0x01) | ((g_guiTrackIndex & 0x01) << 1);
      }
      if (blinkTimestamp - g_stepDetailsTimestamp < (60 * 10)) {
        renderStepTrackDetails();
        Midiboy.drawSpace((128) - Midiboy.getDrawPositionX(), false);
      } else {
        renderPlayingDetails();
        renderPatterns();
      }
    } else if (g_editState == EDITSTATE_TRACK) {
      renderTrackBar(invertedDraw);
      renderPatterns();
      g_stepDetailsTimestamp = blinkTimestamp;
    }
  }
}
