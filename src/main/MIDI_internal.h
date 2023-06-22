#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define SINGLE_TRACK 0
#define MULTI_TRACK_SIMULTANEOUS 1
#define MULTI_TRACK_INDEPENDENT 2

struct MIDI_file {
  uint8_t* bytes;
  int index;
  int length;
};

struct MIDI_header {
  int format;
  int tracks;
  uint16_t division;
};

uint8_t* read_file_to_byte_array(char* filename, int* length);
struct MIDI_file* read_MIDI_file(char* filename);

uint8_t next_byte(struct MIDI_file* MIDI_file);
uint32_t next_variable_length_quantity(struct MIDI_file* MIDI_file);
bool match_chunk_type(struct MIDI_file* MIDI_file, char* chunk_type);
struct MIDI_header* next_MIDI_header(struct MIDI_file* MIDI_file);

enum event_type {
  // MIDI Events
  NOTE_OFF, NOTE_ON, POLYPHONIC_KEY_PRESSURE, CONTROL_CHANGE,
  PROGRAM_CHANGE, CHANNEL_PRESSURE, PITCH_WHEEL_CHANGE,

  // System Events
  SYSTEM_EXCLUSIVE, SONG_POSITION_POINTER, SONG_SELECT,
  TUNE_RQUEST, END_OF_EXCLUSIVE, TIMING_CLOCK, START, STOP,
  ACTIVE_SENSING, RESET,

  // Meta Events
  SEQUENCE_NUMBER, TEXT_EVENT, COPYRIGHT_NOTICE, TRACK_NAME,
  INSTRUMENT_NAME, LYRIC, MARKER, CUE_POINT, CHANNEL_PREFIX,
  END_OF_TRACK, SET_TEMPO, SMPTE_OFFSET, TIME_SIGNATURE,
  KEY_SIGNATURE, SEQUENCER_SPECIFIC_META_EVENT
};

struct event {
  uint32_t deltatime;
  enum event_type type;
  void* body;
};

struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);
struct event* system_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);
struct event* MIDI_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);
struct event* meta_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);

struct MIDI_event {
  int channel;
  void* body;
};

struct note_toggle {
  int key;
  int velocity;
};

struct note_toggle* note_toggle_reader(struct MIDI_file* MIDI_file);
