#include "test.h"
#include "../main/MIDI_internal.h"

static int reads_note_off() {
  uint8_t bytes[] = { 0x03, 0x8A, 0x7F, 0x3A };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == NOTE_OFF);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct note_toggle* note_toggle = (struct note_toggle*)MIDI_event->body;
  ASSERT(note_toggle->key == 0x7F);
  ASSERT(note_toggle->velocity == 0x3A);
  return TEST_PASS;
}

static int reads_note_on() {
  uint8_t bytes[] = { 0x03, 0x9A, 0x7F, 0x3A };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == NOTE_ON);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct note_toggle* note_toggle = (struct note_toggle*)MIDI_event->body;
  ASSERT(note_toggle->key == 0x7F);
  ASSERT(note_toggle->velocity == 0x3A);
  return TEST_PASS;
}

static int reads_polyphonic_key_pressure() {
  uint8_t bytes[] = { 0x03, 0xAA, 0x7F, 0x3A };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == POLYPHONIC_KEY_PRESSURE);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct note_toggle* note_toggle = (struct note_toggle*)MIDI_event->body;
  ASSERT(note_toggle->key == 0x7F);
  ASSERT(note_toggle->velocity == 0x3A);
  return TEST_PASS;
}

static int reads_control_change() {
  uint8_t bytes[] = { 0x03, 0xBA, 0x7F, 0x3A };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == CONTROL_CHANGE);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct control_change* control_change = (struct control_change*)MIDI_event->body;
  ASSERT(control_change->controller == 0x7F);
  ASSERT(control_change->value == 0x3A);
  return TEST_PASS;
}

static int reads_program_change() {
  uint8_t bytes[] = { 0x03, 0xCA, 0x7F };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == PROGRAM_CHANGE);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct program_change* program_change = (struct program_change*)MIDI_event->body;
  ASSERT(program_change->program == 0x7F);
  return TEST_PASS;
}

static int reads_channel_pressure() {
  uint8_t bytes[] = { 0x03, 0xDA, 0x7F };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == CHANNEL_PRESSURE);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct channel_pressure* channel_pressure = (struct channel_pressure*)MIDI_event->body;
  ASSERT(channel_pressure->pressure == 0x7F);
  return TEST_PASS;
}

static int reads_pitch_wheel_change() {
  uint8_t bytes[] = { 0x03, 0xEA, 0x7A, 0x7F };
  INIT_MIDI_FILE(MIDI_file);
  struct event* event = next_track_event(&MIDI_file);
  ASSERT(event != NULL);
  ASSERT(event->deltatime == 0x03);
  ASSERT(event->type == PITCH_WHEEL_CHANGE);
  struct MIDI_event* MIDI_event = (struct MIDI_event*)event->body;
  ASSERT(MIDI_event->channel == 0xA);
  struct pitch_wheel_change* pitch_wheel_change = (struct pitch_wheel_change*)MIDI_event->body;
  ASSERT(pitch_wheel_change->value == 0x3FFA);
  return TEST_PASS;
}

static struct test tests[] = {
  { .name = "Reads note off event", .function = &reads_note_off },
  { .name = "Reads note on event", .function = &reads_note_on },
  { .name = "Reads polyphonic key pressure event", .function = &reads_polyphonic_key_pressure },
  { .name = "Reads control change event", .function = &reads_control_change },
  { .name = "Reads program change event", .function = &reads_program_change },
  { .name = "Reads channel pressure event", .function = &reads_channel_pressure },
  { .name = "Reads pitch wheel change event", .function = &reads_pitch_wheel_change },
};

INIT_TEST_GROUP(MIDI_event);
