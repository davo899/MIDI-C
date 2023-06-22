#include "event.h"
#include <stdio.h>
#include <stdlib.h>

struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code) {
  printf("Error: encountered unknown event code 0x%x", event_code);
  exit(1);
}
