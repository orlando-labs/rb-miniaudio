#pragma once

#include <ruby.h>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

static const char* MA_SAMPLE_FORMAT_U8 = "u8";
static const char* MA_SAMPLE_FORMAT_S16 = "s16";
static const char* MA_SAMPLE_FORMAT_S24 = "s24";
static const char* MA_SAMPLE_FORMAT_S32 = "s32";
static const char* MA_SAMPLE_FORMAT_F32 = "f32";

static const char* G711_FORMAT_ALAW = "alaw";
static const char* G711_FORMAT_MULAW = "mulaw";

#define CHECK_CHANNELS_NUM(rb, c, err) \
int c = NUM2INT(rb); \
if (c < MA_MIN_CHANNELS || c > MA_MAX_CHANNELS) { \
  rb_raise(rb_eArgError, err, MA_MIN_CHANNELS, MA_MAX_CHANNELS); \
}

#define CHECK_SAMPLE_RATE(rb, c, err) \
int c = NUM2INT(rb); \
if (c < MA_MIN_SAMPLE_RATE || c > MA_MAX_SAMPLE_RATE) { \
  rb_raise(rb_eArgError, err, MA_MIN_SAMPLE_RATE, MA_MAX_SAMPLE_RATE);\
}

#define CHECK_SAMPLE_FORMAT(rb, c, err) \
ma_format c = ma_format_unknown; \
if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_U8)) == rb) { \
  c = ma_format_u8; \
} else if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_S16)) == rb) { \
  c = ma_format_s16; \
} else if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_S24)) == rb) { \
  c = ma_format_s24; \
} else if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_S32)) == rb) { \
  c = ma_format_s32; \
} else if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_F32)) == rb) { \
  c = ma_format_f32; \
} else { \
  rb_raise(rb_eArgError, "Unknown %s sample format: %+"PRIsVALUE". Allowed: :u8, :s16, :s24, :s32, :f32", err, rb); \
}

#define CHECK_MA_G711_CONVERTIBLE_SAMPLE_FORMAT(rb, c, err) \
ma_format c = ma_format_unknown; \
if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_S16)) == rb) { \
  c = ma_format_s16; \
} else if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_S32)) == rb) { \
  c = ma_format_s32; \
} else if (ID2SYM(rb_intern(MA_SAMPLE_FORMAT_F32)) == rb) { \
  c = ma_format_f32; \
} else { \
  rb_raise(rb_eArgError, "Unknown %s sample format: %+"PRIsVALUE". Allowed: :s16, :s32, :f32", err, rb); \
}

#define CHECK_G711_FORMAT(rb, err) \
if (ID2SYM(rb_intern(G711_FORMAT_ALAW)) != rb && ID2SYM(rb_intern(G711_FORMAT_MULAW)) != rb) { \
  rb_raise(rb_eArgError, "Unknown %s format: %+"PRIsVALUE". Allowed: :alaw and :mulaw", err, rb); \
}

static int sample_width(ma_format ma_fmt) {
  switch (ma_fmt) {
    case ma_format_u8:
      return 1;
    case ma_format_s16:
      return 2;
    case ma_format_s24:
      return 3;
    case ma_format_s32:
    case ma_format_f32:
      return 4;
    default:
      return 0;
  }
  return 0;
}

#define FROM_G711_CONVERSION(FROM, TO) \
static VALUE rb_g711_conversion_ ## FROM ## _to_ ## TO ( \
  VALUE mod, \
  VALUE rb_source, \
  VALUE rb_src_format, VALUE rb_dst_format) { \
 \
  CHECK_G711_FORMAT(rb_src_format, "source"); \
  CHECK_SAMPLE_FORMAT(rb_dst_format, dst_sample_format, "target"); \
 \
  int mem_len = RSTRING_LEN(rb_source) * sample_width(dst_sample_format); \
  void *converted = malloc(mem_len); \
 \
  if (converted == NULL) { \
    rb_raise(rb_eNoMemError, "Unable to allocate memory for converted frames"); \
  } \
 \
  drwav_ ## FROM ## _to_ ## TO (converted, (void*)StringValuePtr(rb_source), mem_len); \
 \
  VALUE rb_output = rb_str_new(converted, mem_len); \
  free(converted); \
  return rb_output; \
}

static VALUE rb_ma_convert_frames(
  VALUE mod, VALUE rb_source, 
  VALUE rb_source_format, VALUE rb_source_channels, VALUE rb_source_sample_rate,
  VALUE rb_dst_format, VALUE rb_dst_channels, VALUE rb_dst_sample_rate) {

  CHECK_CHANNELS_NUM(
    rb_source_channels, source_channels, 
    "Source channel count must be in %d..%d range"
  )
  
  CHECK_CHANNELS_NUM(
    rb_dst_channels, dst_channels, 
    "Target channel count must be in %d..%d range"
  )

  CHECK_SAMPLE_RATE(
    rb_source_sample_rate, source_sample_rate, 
    "Source sample rate must be in %d..%d range"
  )
  
  CHECK_SAMPLE_RATE(
    rb_dst_sample_rate, dst_sample_rate, 
    "Target sample rate must be in %d..%d range"
  )

  CHECK_SAMPLE_FORMAT(rb_source_format, source_format, "source")
  CHECK_SAMPLE_FORMAT(rb_dst_format, dst_format, "target")

  int source_sample_width = sample_width(source_format);
  int num_frames = RSTRING_LEN(rb_source) / source_sample_width / source_channels;
  int dst_sample_width = sample_width(dst_format);
  int num_out_frames = ma_calculate_frame_count_after_resampling(dst_sample_rate, source_sample_rate, num_frames);

  int mem_len = num_out_frames * dst_sample_width * dst_channels;
  void *converted = malloc(mem_len);
  
  ma_uint64 res = ma_convert_frames(
    converted, num_out_frames, 
    dst_format, dst_channels, dst_sample_rate,
    StringValuePtr(rb_source), num_frames,
    source_format, source_channels, source_sample_rate
  );
  
  if (res == 0) {
    free(converted);
    return Qnil;
  }

  int bytes_used = num_frames * source_sample_width * source_channels;
  VALUE output = rb_str_new(converted, mem_len);
  free(converted);
  VALUE ret = rb_ary_new();
  rb_ary_push(ret, output);
  rb_ary_push(ret, INT2NUM(bytes_used));
  return ret;
}

FROM_G711_CONVERSION(alaw, s16)
FROM_G711_CONVERSION(alaw, s32)
FROM_G711_CONVERSION(alaw, f32)

FROM_G711_CONVERSION(mulaw, s16)
FROM_G711_CONVERSION(mulaw, s32)
FROM_G711_CONVERSION(mulaw, f32)

void define_convert_functions_under(VALUE mod) {
  rb_define_module_function(mod, "_convert_frames", &rb_ma_convert_frames, 7);
  
  rb_define_module_function(mod, "alaw_to_s16", &rb_g711_conversion_alaw_to_s16, 3);
  rb_define_module_function(mod, "alaw_to_s32", &rb_g711_conversion_alaw_to_s32, 3);
  rb_define_module_function(mod, "alaw_to_f32", &rb_g711_conversion_alaw_to_f32, 3);
  
  rb_define_module_function(mod, "mulaw_to_s16", &rb_g711_conversion_mulaw_to_s16, 3);
  rb_define_module_function(mod, "mulaw_to_s32", &rb_g711_conversion_mulaw_to_s32, 3);
  rb_define_module_function(mod, "mulaw_to_f32", &rb_g711_conversion_mulaw_to_f32, 3);
}
