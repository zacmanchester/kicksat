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
#include "sprite_bitdetector_fb.h"


sprite_bitdetector_fb_sptr
sprite_make_bitdetector_fb (float threshold)
{
	return gnuradio::get_initial_sptr (new sprite_bitdetector_fb(threshold));
}

/*
 * The private constructor
 */
sprite_bitdetector_fb::sprite_bitdetector_fb (float threshold)
  : gr_sync_decimator ("bitdetector_fb",
		   gr_make_io_signature(1, 1, sizeof (float)),
		   gr_make_io_signature(1, 1, sizeof (char)), 512)
{
	m_threshold = threshold;
}

/*
 * Our virtual destructor.
 */
sprite_bitdetector_fb::~sprite_bitdetector_fb()
{
	// Put in <+destructor stuff+> here
}


int
sprite_bitdetector_fb::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const float *in = (const float *) input_items[0];
	char *out = (char *) output_items[0];

	//Detect peaks bigger than the threshold
	for(int k = 0; k < noutput_items; ++k)
	{
		char bit_out = 0;
		unsigned int start_index = 512*k;
		for (int j = 0; j < 512; ++j)
		{
			if(in[j+start_index] > m_threshold)
			{
				bit_out = 1;
			}
			else if (in[j+start_index] < -m_threshold)
			{
				bit_out = -1;
			}
		}

		out[k] = bit_out;
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

