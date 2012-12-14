/* -*- c++ -*- */
/* 
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include "sprite_bit_threshold_fb.h"


sprite_bit_threshold_fb_sptr
sprite_make_bit_threshold_fb ()
{
	return gnuradio::get_initial_sptr (new sprite_bit_threshold_fb());
}

/*
 * The private constructor
 */
sprite_bit_threshold_fb::sprite_bit_threshold_fb ()
  : gr_sync_block ("bit_threshold_fb",
		   gr_make_io_signature(2, 2, sizeof (float)),
		   gr_make_io_signature(1, 1, sizeof (char)))
{
	// Put in <+constructor stuff+> here
}


/*
 * Our virtual destructor.
 */
sprite_bit_threshold_fb::~sprite_bit_threshold_fb()
{
	// Put in <+destructor stuff+> here
}


int
sprite_bit_threshold_fb::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const float *in = (const float *) input_items[0];
	const float *threshold = (const float *) input_items[1];
	float *out = (float *) output_items[0];

	for(int k = 0; k < noutput_items; ++k)
	{
		if(in[k] > threshold[k])
		{
			current_output = 1;
			counter = 512;
		}
		else if(in[k] < -threshold[k])
		{
			current_output = -1;
			counter = 512;
		}

		if(counter)
		{
			out[k] = current_output;
			--counter;
		}
		else
		{
			out[k] = 0;
		}
	}

	return noutput_items;
}

