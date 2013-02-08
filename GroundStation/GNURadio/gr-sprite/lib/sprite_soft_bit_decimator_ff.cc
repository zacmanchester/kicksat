/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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
#include "sprite_soft_bit_decimator_ff.h"

sprite_soft_bit_decimator_ff_sptr
sprite_make_soft_bit_decimator_ff()
{
	return gnuradio::get_initial_sptr(new sprite_soft_bit_decimator_ff());
}

/*
 * The private constructor
 */
sprite_soft_bit_decimator_ff::sprite_soft_bit_decimator_ff()
  : gr_sync_decimator("soft_bit_decimator_ff",
		   gr_make_io_signature(1, 1, sizeof (float)),
		   gr_make_io_signature(1, 1, sizeof (float)), 512)
{
	// Put in <+constructor stuff+> here
}

/*
 * Our virtual destructor.
 */
sprite_soft_bit_decimator_ff::~sprite_soft_bit_decimator_ff()
{
	// Put in <+destructor stuff+> here
}


int
sprite_soft_bit_decimator_ff::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const float *in = (const float *)input_items[0];
	float *out = (float *)output_items[0];

	for(int k = 0; k < noutput_items; ++k)
	{
		m_min = 0;
		m_max = 0;

		for(int j = 512*k; j < 512*(k+1); ++j)
		{
			if(in[j] > m_max)
			{
				m_max = in[j];
			}
			else if(in[j] < m_min)
			{
				m_min = in[j];
			}
		}

		out[k] = m_max > -m_min ? m_max : m_min;
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

