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


#ifndef INCLUDED_SPRITE_DECODER_B_H
#define INCLUDED_SPRITE_DECODER_B_H

#include <sprite_api.h>
#include <gr_sync_block.h>

class sprite_decoder_b;

typedef boost::shared_ptr<sprite_decoder_b> sprite_decoder_b_sptr;

SPRITE_API sprite_decoder_b_sptr sprite_make_decoder_b(const std::string &filename);

/*!
 * \brief <+description+>
 * \ingroup block
 *
 */
class SPRITE_API sprite_decoder_b : public gr_sync_block
{
 private:
	friend SPRITE_API sprite_decoder_b_sptr sprite_make_decoder_b(const std::string &filename);
	sprite_decoder_b(const std::string &filename);
	char bytedecode(const char *buffer);
	unsigned int m_counter;
	static char H[4][15];
	static char m_bits[8];

 public:
 	~sprite_decoder_b();

	// Where all the action really happens
	int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);
};

char sprite_decoder_b::m_bits[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000, 0b10000000};

//Parity check matrix goes here
/*
char sprite_decoder_b::H[][] = {{},
								{},
								{},
								{}};
*/

#endif /* INCLUDED_SPRITE_DECODER_B_H */

