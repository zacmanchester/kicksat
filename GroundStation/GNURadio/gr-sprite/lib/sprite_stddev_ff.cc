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
#include "sprite_stddev_ff.h"


sprite_stddev_ff_sptr
sprite_make_stddev_ff()
{
	return gnuradio::get_initial_sptr(new sprite_stddev_ff());
}

/*
 * The private constructor
 */
sprite_stddev_ff::sprite_stddev_ff()
  : gr_sync_block ("stddev_ff",
		   gr_make_io_signature(1, 1, sizeof(float)),
		   gr_make_io_signature(1, 1, sizeof(float)))
{
	//Fill buffer with 1's to start
	for(int k = 0; k < 5120; ++k)
	{
		m_buffer[k] = 1.0/5120;
	}
}


/*
 * Our virtual destructor.
 */
sprite_stddev_ff::~sprite_stddev_ff()
{
	// Put in <+destructor stuff+> here
}


int
sprite_stddev_ff::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const float *in = (const float *) input_items[0];
	float *out = (float *) output_items[0];

	// Do <+signal processing+>
	for(int k = 0; k < noutput_items; ++k)
	{
		out[k] = 0;
		for(int j = 0; j < 5119; ++j)
		{
			m_buffer[j] = m_buffer[j+1];
			out[k] += m_buffer[j];
		}
		m_buffer[1019] = in[k]*in[k]/5120;
		out[k] += m_buffer[5119];

		out[k] = sqrt(out[k]);
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

