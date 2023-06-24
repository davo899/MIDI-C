#pragma once

#include "MIDI.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

struct MIDI_file {
  uint8_t* bytes;
  int index;
  int length;
};

uint8_t* read_file_to_byte_array(char* filename, int* length);
struct MIDI_file* read_MIDI_file(char* filename);

uint8_t next_byte(struct MIDI_file* MIDI_file);
uint16_t next_half_word(struct MIDI_file* MIDI_file);
uint32_t next_full_word(struct MIDI_file* MIDI_file);
uint32_t next_variable_length_quantity(struct MIDI_file* MIDI_file);

struct event* next_track_event(struct MIDI_file* MIDI_file);
struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file);
struct event* system_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);
struct event* MIDI_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);
struct event* meta_event_reader(struct MIDI_file* MIDI_file);

bool match_chunk_type(struct MIDI_file* MIDI_file, char* chunk_type);
struct MIDI_header* next_MIDI_header(struct MIDI_file* MIDI_file);
struct MIDI_track* next_MIDI_track(struct MIDI_file* MIDI_file);
