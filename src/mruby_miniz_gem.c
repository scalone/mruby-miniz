#include "mruby.h"

void mrb_init_miniz(mrb_state *mrb);

#define DONE mrb_gc_arena_restore(mrb, 0)

void
mrb_mruby_miniz_gem_init(mrb_state* mrb)
{
  mrb_init_miniz(mrb); DONE;
}

void
mrb_mruby_miniz_gem_final(mrb_state* mrb)
{
}