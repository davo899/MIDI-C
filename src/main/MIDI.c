#include "MIDI_internal.h"

uint8_t* read_file_to_byte_array(char* filename, int* length) {
  FILE *fileptr = fopen(filename, "rb");
  if (fileptr == NULL) return NULL;
  fseek(fileptr, 0, SEEK_END);
  *length = ftell(fileptr);
  rewind(fileptr);

  uint8_t *buffer = (uint8_t *) malloc(*length * sizeof(uint8_t));
  fread(buffer, *length, 1, fileptr);
  fclose(fileptr);
  
  return buffer;
}

struct MIDI_file* read_MIDI_file(char* filename) {
  struct MIDI_file* MIDI_file = malloc(sizeof(struct MIDI_file));
  MIDI_file->index = 0;
  MIDI_file->bytes = read_file_to_byte_array(filename, &MIDI_file->length);
  if (MIDI_file->bytes == NULL) {
    printf("Error: file %s not found", filename);
    exit(1);
  }
  return MIDI_file;
}

uint32_t next_variable_length_quantity(struct MIDI_file* MIDI_file) {
  uint32_t result = 0;
  int bytes_read = 0;
  uint8_t byte;
  do {
    byte = MIDI_file->bytes[MIDI_file->index++];
    result <<= 7;
    result |= byte & 0b01111111;
    bytes_read++;
  } while (bytes_read < 4 && (byte & 0b10000000) > 0);

  return result;
}

uint8_t next_byte(struct MIDI_file* MIDI_file) {
  if (MIDI_file->index >= MIDI_file->length) return 0;
  return MIDI_file->bytes[MIDI_file->index++];
}

uint16_t next_half_word(struct MIDI_file* MIDI_file) {
  uint16_t byte = next_byte(MIDI_file);
  return (byte << 8) | next_byte(MIDI_file);
}

uint32_t next_full_word(struct MIDI_file* MIDI_file) {
  uint32_t half_word = next_half_word(MIDI_file);
  return (half_word << 16) | next_half_word(MIDI_file);
}

bool match_chunk_type(struct MIDI_file* MIDI_file, char* chunk_type) {
  for (int i = 0; chunk_type[i] != '\0'; i++) {
    if ((char)next_byte(MIDI_file) != chunk_type[i]) return false;
  }
  return true;
}

struct MIDI_header* next_MIDI_header(struct MIDI_file* MIDI_file) {
  if (!match_chunk_type(MIDI_file, "MThd")) return NULL;
  if (next_full_word(MIDI_file) != 6) return NULL;

  struct MIDI_header* MIDI_header = (struct MIDI_header*)malloc(sizeof(struct MIDI_header));
  MIDI_header->format = next_half_word(MIDI_file);
  MIDI_header->tracks = next_half_word(MIDI_file);
  MIDI_header->division = next_half_word(MIDI_file);

  return MIDI_header;
}

struct MIDI_track* next_MIDI_track(struct MIDI_file* MIDI_file) {
  if (!match_chunk_type(MIDI_file, "MTrk")) return NULL;
  int chunk_length = next_full_word(MIDI_file);
  int chunk_end = MIDI_file->index + chunk_length;

  int block_size = 8;
  struct event** events = (struct event**)malloc(block_size * sizeof(struct event*));
  int length = 0;

  bool first = true;
  uint8_t running_status = 0;
  while (MIDI_file->index < chunk_end) {
    if (length >= block_size) {
      block_size *= 2;
      events = (struct event**)realloc(events, block_size * sizeof(struct event*));
    }
    uint32_t deltatime = next_variable_length_quantity(MIDI_file);
    if (first || MIDI_file->bytes[MIDI_file->index] >= 0x80) running_status = next_byte(MIDI_file);
    struct event* event = next_track_event(MIDI_file, running_status);
    if (event != NULL) event->deltatime = deltatime;

    events[length++] = event;
    first = false;
  }

  struct MIDI_track* MIDI_track = (struct MIDI_track*)malloc(sizeof(struct MIDI_track));
  MIDI_track->length = length;
  MIDI_track->events = events;
  return MIDI_track;
}

struct MIDI* read_MIDI(char* filename) {
  struct MIDI_file* MIDI_file = read_MIDI_file(filename);
  struct MIDI* MIDI = (struct MIDI*)malloc(sizeof(struct MIDI));
  MIDI->header = next_MIDI_header(MIDI_file);
  MIDI->tracks = malloc(MIDI->header->tracks * sizeof(struct MIDI_track*));
  for (int i = 0; i < MIDI->header->tracks; i++) MIDI->tracks[i] = next_MIDI_track(MIDI_file);
  free(MIDI_file->bytes);
  free(MIDI_file);
  return MIDI;
}
