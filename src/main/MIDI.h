#pragma once

#include <stdint.h>
#include <stdbool.h>

enum division {
  TICKS_PER_QUATER_NOTE,
  TICKS_PER_FRAME
};

enum format {
  SINGLE_TRACK,
  MULTI_TRACK_SIMULTANEOUS,
  MULTI_TRACK_INDEPENDENT
};

struct MIDI_file {
  uint8_t* bytes;
  int index;
  int length;
};

struct MIDI_header {
  enum format format;
  int tracks;
  enum division division;
};

uint8_t* read_file_to_byte_array(char* filename, int* length);
struct MIDI_file* read_MIDI_file(char* filename);
uint32_t next_variable_length_quantity(struct MIDI_file* MIDI_file);
bool match_chunk_type(struct MIDI_file* MIDI_file, char* chunk_type);
uint8_t next_byte(struct MIDI_file* MIDI_file);
