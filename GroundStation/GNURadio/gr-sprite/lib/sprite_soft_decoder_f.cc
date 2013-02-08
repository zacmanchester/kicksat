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
#include "sprite_soft_decoder_f.h"


sprite_soft_decoder_f_sptr
sprite_make_soft_decoder_f ()
{
	return gnuradio::get_initial_sptr(new sprite_soft_decoder_f());
}

/*
 * The private constructor
 */
sprite_soft_decoder_f::sprite_soft_decoder_f()
  : gr_sync_block ("soft_decoder_f",
		   gr_make_io_signature(2, 2, sizeof(float)),
		   gr_make_io_signature(0, 0, 0))
{
	set_history(15);
	m_counter = 0;
}


/*
 * Our virtual destructor.
 */
sprite_soft_decoder_f::~sprite_soft_decoder_f()
{
	// Put in <+destructor stuff+> here
}

char sprite_soft_decoder_f::softdecode(const float *buffer)
{

}

int sprite_soft_decoder_f::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const float *in = (const float *) input_items[0];
	const float *threshold = (const float *) input_items[1];

	for(int k = 0; k < noutput_items; ++k)
	{
		if(!m_counter)
		{
			if(in[k] > threshold[k] || in[k] < -threshold[k])
			{
				//we found a bit!
				m_counter = 14;

				//Decode the byte
				char m = softdecode(&in[k]);

				//For now just write stuff to the console as we get it
				std::cout << m;
			}
		}
		else
		{
			--m_counter;
		}
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

