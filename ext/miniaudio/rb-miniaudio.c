#include <ruby.h>

#include "read_functions.h"
#include "convert_functions.h"

void Init_miniaudio(void) {
#ifdef HAVE_RUBY_RACTOR_H
  rb_ext_ractor_safe(true);
#endif
  VALUE rb_cMod = rb_const_get(rb_cObject, rb_intern("Miniaudio"));
  define_read_functions_under(rb_cMod);
  define_convert_functions_under(rb_cMod);
}
