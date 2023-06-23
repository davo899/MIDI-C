#include "test.h"
#include "../main/MIDI_internal.h"

static int reads_note_off() {
  uint8_t bytes[] = { 0x03, 0x8A, 0x7F, 0x3A };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == NOTE_OFF);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct note_toggle* note_toggle = (struct note_toggle*)MIDI_event->body;
  ASSERT(note_toggle->key == 0x7F);
  ASSERT(note_toggle->velocity == 0x3A);
  return TEST_PASS;
}

static struct test tests[] = {
  { .name = "Reads note off event", .function = &reads_note_off }
};

INIT_TEST_GROUP(MIDI_event);
