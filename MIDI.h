#pragma once

#include <stdint.h>

#define SINGLE_TRACK 0
#define MULTI_TRACK_SIMULTANEOUS 1
#define MULTI_TRACK_INDEPENDENT 2

enum event_type {
  // MIDI Events
  NOTE_OFF, NOTE_ON, POLYPHONIC_KEY_PRESSURE, CONTROL_CHANGE,
  PROGRAM_CHANGE, CHANNEL_PRESSURE, PITCH_WHEEL_CHANGE,

  // System Events
  SYSTEM_EXCLUSIVE, SONG_POSITION_POINTER, SONG_SELECT,
  TUNE_REQUEST, END_OF_EXCLUSIVE, TIMING_CLOCK, START, CONTINUE,
  STOP, ACTIVE_SENSING, RESET,

  // Meta Events
  SEQUENCE_NUMBER, TEXT, COPYRIGHT_NOTICE, TRACK_NAME,
  INSTRUMENT_NAME, LYRIC, MARKER, CUE_POINT, CHANNEL_PREFIX,
  END_OF_TRACK, SET_TEMPO, SMPTE_OFFSET, TIME_SIGNATURE,
  KEY_SIGNATURE, SEQUENCER_SPECIFIC_META,

  UNIMPLEMENTED
};

struct note_toggle {
  int key;
  int velocity;
};

struct control_change {
  int controller;
  int value;
};

struct byte_buffer {
  int length;
  uint8_t* bytes;
};

struct SMPTE_offset {
  int frame_rate;
  int hours;
  int minutes;
  int seconds;
  int frames;
  int subframes;
};

struct time_signature {
  int numerator;
  int denominator;
  int clocks;
  int notes_per_quarter;
};

struct key_signature {
  int flats;
  int scale;
};

struct system_exclusive_message {
  int id;
  int length;
  uint8_t* message;
};

struct MIDI_event {
  int channel;
  void* body;
};

struct event {
  uint32_t deltatime;
  enum event_type type;
  void* body;
};

struct MIDI_header {
  int format;
  int tracks;
  uint16_t division;
};

struct MIDI_track {
  int length;
  struct event** events;
};

struct MIDI {
  struct MIDI_header* header;
  struct MIDI_track** tracks;
};

struct MIDI* read_MIDI(char* filename);
