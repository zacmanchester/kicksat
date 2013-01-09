/* -*- c++ -*- */

#define SPRITE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "sprite_swig_doc.i"


%{
#include "sprite_correlator_cf.h"
#include "sprite_stddev_ff.h"
#include "sprite_bit_threshold_fb.h"
%}


GR_SWIG_BLOCK_MAGIC(sprite,correlator_cf);
%include "sprite_correlator_cf.h"

GR_SWIG_BLOCK_MAGIC(sprite,stddev_ff);
%include "sprite_stddev_ff.h"

GR_SWIG_BLOCK_MAGIC(sprite,bit_threshold_fb);
%include "sprite_bit_threshold_fb.h"
