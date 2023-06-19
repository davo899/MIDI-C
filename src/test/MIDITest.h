#pragma once

int can_read_file_to_byte_array();
int reading_from_non_existent_file_returns_null();

int can_read_MIDI_file();

int can_read_VLQ_zero();
int can_read_VLQ_single();
int can_read_VLQ_full();

int matching_chunk_type_returns_true();
int non_matching_chunk_type_returns_false();

int next_byte_advances_index();
int next_byte_returns_next_byte();
int next_byte_returns_zero_past_max_index();
