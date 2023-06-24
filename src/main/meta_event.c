#include "MIDI_internal.h"

static struct text_buffer* text_buffer_reader(struct MIDI_file* MIDI_file, int length) {
  struct text_buffer* text_buffer = (struct text_buffer*)malloc(sizeof(struct text_buffer));
  text_buffer->length = length;
  text_buffer->text = (char*)malloc(length * sizeof(char));
  for (int i = 0; i < length; i++) text_buffer->text[i] = (char)next_byte(MIDI_file);
  return text_buffer;
}

struct event* meta_event_reader(struct MIDI_file* MIDI_file) {
  struct event* event = (struct event*)malloc(sizeof(struct event));
  int event_code = next_byte(MIDI_file);
  int length = next_variable_length_quantity(MIDI_file);

  switch (event_code) {
    case 0x00:
      event->type = SEQUENCE_NUMBER;
      event->body = malloc(sizeof(uint16_t));
      *((uint16_t*)event->body) = next_half_word(MIDI_file);
      break;

    case 0x01:
      event->type = TEXT;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x02:
      event->type = COPYRIGHT_NOTICE;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x03:
      event->type = TRACK_NAME;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x04:
      event->type = INSTRUMENT_NAME;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x05:
      event->type = LYRIC;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x06:
      event->type = MARKER;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x07:
      event->type = CUE_POINT;
      event->body = text_buffer_reader(MIDI_file, length);
      break;

    case 0x20:
      event->type = CHANNEL_PREFIX;
      event->body = malloc(sizeof(uint8_t));
      *(uint8_t*)event->body = next_byte(MIDI_file);
      break;

    case 0x2F:
      event->type = END_OF_TRACK;
      break;

    case 0x51:
      event->type = SET_TEMPO;
      event->body = malloc(sizeof(uint64_t));
      uint64_t byte1 = next_byte(MIDI_file);
      uint64_t byte2 = next_byte(MIDI_file);
      *(uint64_t*)event->body = (byte1 << 16) | (byte2 << 8) | next_byte(MIDI_file);
      break;

    case 0x54:
      event->type = SMPTE_OFFSET;
      struct SMPTE_offset* SMPTE_offset = (struct SMPTE_offset*)malloc(sizeof(struct SMPTE_offset));
      uint8_t byte = next_byte(MIDI_file);
      SMPTE_offset->frame_rate = byte >> 5;
      SMPTE_offset->hours = byte & 0b00011111;
      SMPTE_offset->minutes = next_byte(MIDI_file);
      SMPTE_offset->seconds = next_byte(MIDI_file);
      SMPTE_offset->frames = next_byte(MIDI_file);
      SMPTE_offset->subframes = next_byte(MIDI_file);
      event->body = SMPTE_offset;
      break;

    case 0x58:
      event->type = TIME_SIGNATURE;
      struct time_signature* time_signature = (struct time_signature*)malloc(sizeof(struct time_signature));
      time_signature->numerator = next_byte(MIDI_file);
      time_signature->denominator = next_byte(MIDI_file);
      time_signature->clocks = next_byte(MIDI_file);
      time_signature->notes_per_quarter = next_byte(MIDI_file);
      event->body = time_signature;
      break;

    case 0x59:
      event->type = KEY_SIGNATURE;
      struct key_signature* key_signature = (struct key_signature*)malloc(sizeof(struct key_signature));
      key_signature->flats = next_byte(MIDI_file);
      key_signature->scale = next_byte(MIDI_file);
      event->body = key_signature;
      break;

    default:
      event = unimplemented_event_reader(MIDI_file);
  }

  return event;
}
