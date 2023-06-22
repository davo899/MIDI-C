#pragma once

#include "MIDI.h"
#include <stdint.h>
#include <stdlib.h>

enum event_type {
  // MIDI Events
  NOTE_OFF, NOTE_ON, POLYPHONIC_KEY_PRESSURE, CONTROL_CHANGE,
  PROGRAM_CHANGE, CHANNEL_PRESSURE, PITCH_WHEEL_CHANGE,

  // System Events
  SYSTEM_EXCLUSIVE, SONG_POSITION_POINTER, SONG_SELECT,
  TUNE_RQUEST, END_OF_EXCLUSIVE, TIMING_CLOCK, START, STOP,
  ACTIVE_SENSING, RESET,

  // Meta Events
  SEQUENCE_NUMBER, TEXT_EVENT, COPYRIGHT_NOTICE, TRACK_NAME,
  INSTRUMENT_NAME, LYRIC, MARKER, CUE_POINT, CHANNEL_PREFIX,
  END_OF_TRACK, SET_TEMPO, SMPTE_OFFSET, TIME_SIGNATURE,
  KEY_SIGNATURE, SEQUENCER_SPECIFIC_META_EVENT
};

struct event {
  uint32_t deltatime;
  enum event_type type;
  void* body;
};

struct event* unimplemented_event_reader(struct MIDI_file* MIDI_file, uint8_t event_code);
