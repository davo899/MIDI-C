#pragma once

#include <stdint.h>

struct MIDI_file {
  uint8_t* bytes;
  int index;
  int length;
};

uint8_t* read_file_to_byte_array(char* filename, int* length);
