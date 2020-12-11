#pragma once

#define STB_VORBIS_HEADER_ONLY
#include "miniaudio/extras/stb_vorbis.c"    // Enables Vorbis decoding.

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

// The stb_vorbis implementation must come after the implementation of miniaudio.
#undef STB_VORBIS_HEADER_ONLY
#include "miniaudio/extras/stb_vorbis.c"

#define OPEN_FILE(FORMAT, FRAME_TYPE, CODE) \
static VALUE rb_dr ## FORMAT ## _open_file_and_read_pcm_frames_ ## CODE (VALUE mod, VALUE filename) { \
  if (TYPE(filename) != T_STRING) { \
    rb_raise(rb_const_get(rb_cObject, rb_intern("ArgumentError")), "Filename must be a string"); \
  } \
\
  unsigned int num_channels; \
  unsigned int sample_rate; \
  dr ## FORMAT ## _uint64 total_frames; \
\
  FRAME_TYPE * frames = dr ## FORMAT ## _open_file_and_read_pcm_frames_ ## CODE (\
    StringValuePtr(filename),\
    &num_channels,\
    &sample_rate,\
    &total_frames,\
    NULL\
  );\
\
  if (frames == NULL) {\
    return Qnil;\
  }\
\
  VALUE ret_str = rb_str_new((char*)frames, sizeof(FRAME_TYPE)*total_frames);\
  dr ## FORMAT ## _free(frames, NULL);\
  VALUE ret = rb_ary_new();\
  rb_ary_push(ret, ret_str);\
  rb_ary_push(ret, INT2NUM(sample_rate));\
  rb_ary_push(ret, INT2NUM(num_channels));\
  rb_ary_push(ret, INT2NUM(total_frames));\
  return ret;\
}

#define OPEN_MP3_FILE(FRAME_TYPE, CODE) \
static VALUE rb_drmp3_open_file_and_read_pcm_frames_ ## CODE (VALUE mod, VALUE filename) { \
  if (TYPE(filename) != T_STRING) { \
    rb_raise(rb_const_get(rb_cObject, rb_intern("ArgumentError")), "Filename must be a string"); \
  } \
\
  drmp3_uint64 total_frames; \
\
  drmp3_config config;\
  FRAME_TYPE * frames = drmp3_open_file_and_read_pcm_frames_ ## CODE (\
    StringValueCStr(filename),\
    &config,\
    &total_frames,\
    NULL\
  );\
\
  if (frames == NULL) {\
    return Qnil;\
  }\
\
  VALUE ret_str = rb_str_new((char*)frames, sizeof(FRAME_TYPE)*total_frames*config.channels);\
  drmp3_free(frames, NULL);\
  VALUE ary = rb_ary_new_from_args(4, ret_str, INT2NUM(config.sampleRate), INT2NUM(config.channels), INT2NUM(total_frames));\
  return ary;\
}

#define OPEN_MEMORY(FORMAT, FRAME_TYPE, CODE) \
static VALUE rb_dr ## FORMAT ## _open_memory_and_read_pcm_frames_ ## CODE (VALUE mod, VALUE str) { \
  if (TYPE(str) != T_STRING) { \
    rb_raise(rb_const_get(rb_cObject, rb_intern("ArgumentError")), "Memory storage must be a string"); \
  } \
\
  unsigned int num_channels; \
  unsigned int sample_rate; \
  dr ## FORMAT ## _uint64 total_frames; \
\
  FRAME_TYPE * frames = dr ## FORMAT ## _open_memory_and_read_pcm_frames_ ## CODE (\
    StringValuePtr(str),\
    RSTRING_LEN(str),\
    &num_channels,\
    &sample_rate,\
    &total_frames,\
    NULL\
  );\
\
  if (frames == NULL) {\
    return Qnil;\
  }\
\
  VALUE ret_str = rb_str_new((char*)frames, sizeof(FRAME_TYPE)*total_frames);\
  dr ## FORMAT ## _free(frames, NULL);\
  VALUE ret = rb_ary_new();\
  rb_ary_push(ret, ret_str);\
  rb_ary_push(ret, INT2NUM(sample_rate));\
  rb_ary_push(ret, INT2NUM(num_channels));\
  rb_ary_push(ret, INT2NUM(total_frames));\
  return ret;\
}

#define OPEN_MP3_MEMORY(FRAME_TYPE, CODE) \
static VALUE rb_drmp3_open_memory_and_read_pcm_frames_ ## CODE (VALUE mod, VALUE str) { \
  if (TYPE(str) != T_STRING) { \
    rb_raise(rb_const_get(rb_cObject, rb_intern("ArgumentError")), "Memory must be a string"); \
  } \
\
  drmp3_uint64 total_frames; \
\
  drmp3_config config;\
  FRAME_TYPE * frames = drmp3_open_memory_and_read_pcm_frames_ ## CODE (\
    StringValuePtr(str),\
    RSTRING_LEN(str),\
    &config,\
    &total_frames,\
    NULL\
  );\
\
  if (frames == NULL) {\
    return Qnil;\
  }\
\
  VALUE ret_str = rb_str_new((char*)frames, sizeof(FRAME_TYPE)*total_frames);\
  drmp3_free(frames, NULL);\
  VALUE ret = rb_ary_new();\
  rb_ary_push(ret, ret_str);\
  rb_ary_push(ret, INT2NUM(config.sampleRate));\
  rb_ary_push(ret, INT2NUM(config.channels));\
  rb_ary_push(ret, INT2NUM(total_frames));\
  return ret;\
}

static VALUE rb_stb_vorbis_decode_filename_s16 (VALUE mod, VALUE filename) {
  if (TYPE(filename) != T_STRING) {
    rb_raise(rb_const_get(rb_cObject, rb_intern("ArgumentError")), "Filename must be a string");
  }

  int num_channels;
  int sample_rate;
  int64_t total_frames;

  int16_t *output;

  total_frames = stb_vorbis_decode_filename (
    StringValueCStr(filename),
    &num_channels,
    &sample_rate,
    &output
  );

  if (total_frames < 0) {
    return Qnil;
  }

  VALUE ret_str = rb_str_new((char*)output, sizeof(int16_t)*total_frames);
  free(output);
  VALUE ret = rb_ary_new();
  rb_ary_push(ret, ret_str);
  rb_ary_push(ret, INT2NUM(sample_rate));
  rb_ary_push(ret, INT2NUM(num_channels));
  rb_ary_push(ret, INT2NUM(total_frames));
  return ret;
}

static VALUE rb_stb_vorbis_decode_memory_s16(VALUE mod, VALUE str) {
  if (TYPE(str) != T_STRING) {
    rb_raise(rb_const_get(rb_cObject, rb_intern("ArgumentError")), "Memory storage must be a string");
  }

  int num_channels;
  int sample_rate;
  int64_t total_frames;

  int16_t *output;

  total_frames = stb_vorbis_decode_memory(
    (uint8*)StringValuePtr(str),
    RSTRING_LEN(str),
    &num_channels,
    &sample_rate,
    &output
  );

  if (total_frames < 0) {
    return Qnil;
  }

  VALUE ret_str = rb_str_new((char*)output, sizeof(int16_t)*total_frames);
  free(output);
  VALUE ret = rb_ary_new();
  rb_ary_push(ret, ret_str);
  rb_ary_push(ret, INT2NUM(sample_rate));
  rb_ary_push(ret, INT2NUM(num_channels));
  rb_ary_push(ret, INT2NUM(total_frames));
  return ret;
}

OPEN_FILE(wav, int16_t, s16)
OPEN_FILE(wav, int32_t, s32)
OPEN_FILE(wav, float, f32)

OPEN_MP3_FILE(int16_t, s16)
OPEN_MP3_FILE(float, f32)

OPEN_FILE(flac, int16_t, s16)
OPEN_FILE(flac, int32_t, s32)
OPEN_FILE(flac, float, f32)

OPEN_MEMORY(wav, int16_t, s16)
OPEN_MEMORY(wav, int32_t, s32)
OPEN_MEMORY(wav, float, f32)

OPEN_MP3_MEMORY(int16_t, s16)
OPEN_MP3_MEMORY(float, f32)

OPEN_MEMORY(flac, int16_t, s16)
OPEN_MEMORY(flac, int32_t, s32)
OPEN_MEMORY(flac, float, f32)

void define_read_functions_under(VALUE mod) {
  rb_define_module_function(mod, "wav_read_file_s16", &rb_drwav_open_file_and_read_pcm_frames_s16, 1);
  rb_define_module_function(mod, "wav_read_file_s32", &rb_drwav_open_file_and_read_pcm_frames_s32, 1);
  rb_define_module_function(mod, "wav_read_file_f32", &rb_drwav_open_file_and_read_pcm_frames_f32, 1);
  rb_define_module_function(mod, "mp3_read_file_s16", rb_drmp3_open_file_and_read_pcm_frames_s16, 1);
  rb_define_module_function(mod, "mp3_read_file_f32", rb_drmp3_open_file_and_read_pcm_frames_f32, 1);
  rb_define_module_function(mod, "flac_read_file_s16", &rb_drflac_open_file_and_read_pcm_frames_s16, 1);
  rb_define_module_function(mod, "flac_read_file_s32", &rb_drflac_open_file_and_read_pcm_frames_s32, 1);
  rb_define_module_function(mod, "flac_read_file_f32", &rb_drflac_open_file_and_read_pcm_frames_f32, 1);
  rb_define_module_function(mod, "vorbis_read_file_s16", &rb_stb_vorbis_decode_filename_s16, 1);
  rb_define_module_function(mod, "wav_read_s16", &rb_drwav_open_memory_and_read_pcm_frames_s16, 1);
  rb_define_module_function(mod, "wav_read_s32", &rb_drwav_open_memory_and_read_pcm_frames_s32, 1);
  rb_define_module_function(mod, "wav_read_f32", &rb_drwav_open_memory_and_read_pcm_frames_f32, 1);
  rb_define_module_function(mod, "mp3_read_s16", rb_drmp3_open_memory_and_read_pcm_frames_s16, 1);
  rb_define_module_function(mod, "mp3_read_f32", rb_drmp3_open_memory_and_read_pcm_frames_f32, 1);
  rb_define_module_function(mod, "flac_read_s16", &rb_drflac_open_memory_and_read_pcm_frames_s16, 1);
  rb_define_module_function(mod, "flac_read_s32", &rb_drflac_open_memory_and_read_pcm_frames_s32, 1);
  rb_define_module_function(mod, "flac_read_f32", &rb_drflac_open_memory_and_read_pcm_frames_f32, 1);
  rb_define_module_function(mod, "vorbis_file_s16", &rb_stb_vorbis_decode_memory_s16, 1);
}
