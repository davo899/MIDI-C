#pragma once

#include <stdint.h>
#include <stdbool.h>

struct MIDI_file {
  uint8_t* bytes;
  int index;
  int length;
};

uint8_t* read_file_to_byte_array(char* filename, int* length);
struct MIDI_file* read_MIDI_file(char* filename);
uint32_t next_variable_length_quantity(struct MIDI_file* MIDI_file);
bool match_chunk_type(struct MIDI_file* MIDI_file, char* chunk_type);
