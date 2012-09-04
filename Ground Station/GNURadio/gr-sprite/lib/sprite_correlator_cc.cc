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
#include <sprite_correlator_cc.h>


sprite_correlator_cc_sptr
sprite_make_correlator_cc (int prn_id)
{
	return sprite_correlator_cc_sptr (new sprite_correlator_cc (prn_id));
}


sprite_correlator_cc::sprite_correlator_cc (int prn_id)
	: gr_sync_block ("correlator_cc",
		gr_make_io_signature (1, 1, sizeof (gr_complex)),
		gr_make_io_signature (1, 1, sizeof (gr_complex)))
{
	set_history(512);
}


sprite_correlator_cc::~sprite_correlator_cc ()
{
}


int
sprite_correlator_cc::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const gr_complex *in = (const gr_complex *) input_items[0];
	gr_complex *out = (gr_complex *) output_items[0];

	// Do <+signal processing+>
	for(int i = 0; i < noutput_items; i++) {
		out[i] = in[i];
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

