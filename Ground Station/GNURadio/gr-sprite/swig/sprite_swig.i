/* -*- c++ -*- */

#define SPRITE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "sprite_swig_doc.i"


%{
#include "sprite_correlator_cc.h"
%}


GR_SWIG_BLOCK_MAGIC(sprite,correlator_cc);
%include "sprite_correlator_cc.h"
