#include "test.h"
#include "../main/MIDI_internal.h"

static int reads_system_exclusive() {
  uint8_t bytes[] = { 0x7C, 0xF0, 0x42, 0x11, 0x23, 0xF7 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x7C);
  ASSERT(event->type == SYSTEM_EXCLUSIVE);
  struct system_exclusive_message* system_exclusive_message = (struct system_exclusive_message*)event->body;
  ASSERT(system_exclusive_message->id == 0x42);
  ASSERT(system_exclusive_message->length == 2);
  ASSERT(system_exclusive_message->message[0] == 0x11);
  ASSERT(system_exclusive_message->message[1] == 0x23);
  return TEST_PASS;
}

static struct test tests[] = {
  { .name = "Reads system exclusive event", .function = &reads_system_exclusive },
};

INIT_TEST_GROUP(system_event);
