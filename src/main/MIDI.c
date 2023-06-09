#include "MIDI.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t* read_file_to_byte_array(char* filename, int* length) {
  FILE *fileptr = fopen(filename, "rb");
  fseek(fileptr, 0, SEEK_END);
  *length = ftell(fileptr);
  rewind(fileptr);

  uint8_t *buffer = (uint8_t *) malloc(*length * sizeof(uint8_t));
  fread(buffer, *length, 1, fileptr);
  fclose(fileptr);
  
  return buffer;
}
