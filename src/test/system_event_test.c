#include "test.h"
#include "../main/MIDI_internal.h"

static int reads_system_exclusive() {
  uint8_t bytes[] = { 0x42, 0x11, 0x23, 0xF7 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xF0);
  ASSERT(event != NULL);
  ASSERT(event->type == SYSTEM_EXCLUSIVE);
  struct system_exclusive_message* system_exclusive_message = (struct system_exclusive_message*)event->body;
  ASSERT(system_exclusive_message->id == 0x42);
  ASSERT(system_exclusive_message->length == 2);
  ASSERT(system_exclusive_message->message[0] == 0x11);
  ASSERT(system_exclusive_message->message[1] == 0x23);
  return TEST_PASS;
}

static int reads_song_position_pointer() {
  uint8_t bytes[] = { 0x42, 0x11 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xF2);
  ASSERT(event != NULL);
  ASSERT(event->type == SONG_POSITION_POINTER);
  ASSERT(*(uint16_t*)event->body == 0x08C2);
  return TEST_PASS;
}

static int reads_song_select() {
  uint8_t bytes[] = { 0x42 };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xF3);
  ASSERT(event != NULL);
  ASSERT(event->type == SONG_SELECT);
  ASSERT(*(uint8_t*)event->body == 0x42);
  return TEST_PASS;
}

static int reads_tune_request() {
  uint8_t bytes[] = { };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xF6);
  ASSERT(event != NULL);
  ASSERT(event->type == TUNE_REQUEST);
  return TEST_PASS;
}

static int reads_timing_clock() {
  uint8_t bytes[] = { };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xF8);
  ASSERT(event != NULL);
  ASSERT(event->type == TIMING_CLOCK);
  return TEST_PASS;
}

static int reads_start() {
  uint8_t bytes[] = { };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xFA);
  ASSERT(event != NULL);
  ASSERT(event->type == START);
  return TEST_PASS;
}

static int reads_continue() {
  uint8_t bytes[] = { };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xFB);
  ASSERT(event != NULL);
  ASSERT(event->type == CONTINUE);
  return TEST_PASS;
}

static int reads_stop() {
  uint8_t bytes[] = { };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xFC);
  ASSERT(event != NULL);
  ASSERT(event->type == STOP);
  return TEST_PASS;
}

static int reads_active_sensing() {
  uint8_t bytes[] = { };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file, 0xFE);
  ASSERT(event != NULL);
  ASSERT(event->type == ACTIVE_SENSING);
  return TEST_PASS;
}

static struct test tests[] = {
  { .name = "Reads system exclusive event", .function = &reads_system_exclusive },
  { .name = "Reads song position pointer event", .function = &reads_song_position_pointer },
  { .name = "Reads song select event", .function = &reads_song_select },
  { .name = "Reads tune request event", .function = &reads_tune_request },
  { .name = "Reads timing clock event", .function = &reads_timing_clock },
  { .name = "Reads start event", .function = &reads_start },
  { .name = "Reads continue event", .function = &reads_continue },
  { .name = "Reads stop event", .function = &reads_stop },
  { .name = "Reads active sensing event", .function = &reads_active_sensing },
};

INIT_TEST_GROUP(system_event);
