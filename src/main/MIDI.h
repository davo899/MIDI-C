#pragma once

#include <stdint.h>
#include <stdbool.h>

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
uint32_t next_variable_length_quantity(struct MIDI_file* MIDI_file);
bool match_chunk_type(struct MIDI_file* MIDI_file, char* chunk_type);
uint8_t next_byte(struct MIDI_file* MIDI_file);
struct MIDI_header* next_MIDI_header(struct MIDI_file* MIDI_file);
