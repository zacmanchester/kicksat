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
#include "sprite_decoder_b.h"

#include <iostream> 

sprite_decoder_b_sptr
sprite_make_decoder_b(const std::string &filename)
{
	return gnuradio::get_initial_sptr(new sprite_decoder_b(filename));
}

/*
 * The private constructor
 */
sprite_decoder_b::sprite_decoder_b(const std::string &filename)
  : gr_sync_block ("decoder_b",
		   gr_make_io_signature(1, 1, sizeof(char)),
		   gr_make_io_signature(0, 0, 0))
{
	set_history(15);
	m_counter = 0;
}


/*
 * Our virtual destructor.
 */
sprite_decoder_b::~sprite_decoder_b()
{
	// Put in <+destructor stuff+> here
}

char sprite_decoder_b::bytedecode(const char* c)
{
	char m = 0;
	//For now, just pack the last 8 bits into a char and return
	//TODO: Hamming decoder
	for(int k = 0; k < 8; ++k)
	{
		if(c[7+k] > 0)
		{
			m |= m_bits[k];
		}
	}

	return m;
}

int
sprite_decoder_b::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const char *in = (const char*) input_items[0];

	for(int k = 0; k < noutput_items; ++k)
	{
		if(!m_counter)
		{
			if(in[k])
			{
				//we found a bit!
				m_counter = 14;

				//Decode the byte
				char m = bytedecode(&in[k]);

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

