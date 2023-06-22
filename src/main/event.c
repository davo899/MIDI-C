#include "event.h"
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_EVENT_TYPES 32

struct event (*event_readers[NUMBER_OF_EVENT_TYPES])(struct MIDI_file*, uint8_t);

void init_event_readers() {
  for (int i = 0; i < NUMBER_OF_EVENT_TYPES; i++) event_readers[i] = &unimplemented_event_reader;
}

struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code) {
  printf("Error: encountered unknown event code 0x%x", event_code);
  exit(1);
}
