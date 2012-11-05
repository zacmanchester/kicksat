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

#ifndef INCLUDED_SPRITE_CORRELATOR_CF_H
#define INCLUDED_SPRITE_CORRELATOR_CF_H

#include <sprite_api.h>
#include <gr_sync_block.h>
#include <gri_fft.h>

class sprite_correlator_cf;
typedef boost::shared_ptr<sprite_correlator_cf> sprite_correlator_cf_sptr;

SPRITE_API sprite_correlator_cf_sptr sprite_make_correlator_cf (int prn_id);

/*!
 * \brief <+description+>
 *
 */
class SPRITE_API sprite_correlator_cf : public gr_sync_block
{
	friend SPRITE_API sprite_correlator_cf_sptr sprite_make_correlator_cf (int prn_id);

	sprite_correlator_cf (int prn_id);
	
	int m_prn[512];
	void generate_prn(int prn_id);
		
	gr_complex m_template[512];
	void cc430_modulator(int* prnBits, gr_complex* baseBand);
		
	float m_buffer_real1[512];
	float m_buffer_real2[512];
	float m_buffer_real3[512];
		
	gr_complex* m_fft_buffer_in;
	gr_complex* m_fft_buffer_out;
		
	gri_fft_complex* m_fft;
		
	static int mseq1[512];
	static int mseq2[512];

 public:
	~sprite_correlator_cf ();

	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

int sprite_correlator_cf::mseq1[] = {
	1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,1,1,1,1,0,0,1,0,
	1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,0,0,1,1,1,0,1,0,0,1,0,0,1,1,1,1,
	0,1,0,1,1,1,0,1,0,1,0,0,0,1,0,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,0,0,
	1,0,1,1,1,1,0,1,1,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,0,1,1,1,1,0,0,
	0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,1,0,1,
	1,1,0,0,1,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,0,0,1,1,1,0,1,1,0,1,0,0,
	0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,0,0,
	0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0,1,
	0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,1,1,
	1,0,0,1,0,1,0,1,0,1,1,0,0,0,0,1,1,0,1,1,1,1,0,1,0,0,1,1,0,1,1,1,
	0,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,0,1,1,0,1,0,0,1,1,1,1,1,1,0,1,1,
	0,0,1,0,0,1,0,0,1,0,1,1,0,1,1,1,1,1,1,0,0,1,0,0,1,1,0,1,0,1,0,0,
	1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,1,0,1,0,0,0,1,1,0,1,
	0,0,1,0,1,1,1,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,0,1,0,1,1,0,
	0,1,0,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,1,0,0,0,0,0,1,1,
	0,1,0,1,1,0,1,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,1,1,0,1,0,1,1,1,1,0
};
		
int sprite_correlator_cf::mseq2[] = {
	1,0,1,0,1,0,1,0,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,
	1,1,1,1,1,0,0,1,0,1,1,1,1,1,0,1,0,0,0,0,0,0,1,0,1,1,0,0,0,1,0,0,
	1,1,0,0,1,1,1,0,1,1,1,1,0,1,0,1,1,0,1,1,0,1,1,1,0,1,0,1,0,1,1,0,
	1,0,0,1,0,1,1,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,0,1,0,1,1,1,
	0,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,1,0,
	1,0,0,0,1,1,1,0,1,1,0,1,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,
	0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,1,0,0,1,1,0,0,0,1,1,1,1,0,1,1,0,1,
	1,0,0,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,1,1,1,0,0,1,1,0,0,0,0,1,1,
	0,0,0,1,0,1,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,
	0,0,1,0,1,0,0,0,0,1,1,1,1,0,0,1,0,0,1,1,0,1,1,1,0,0,0,1,1,1,0,0,
	1,0,0,0,1,0,0,1,0,0,0,0,1,0,0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,
	0,0,1,0,0,0,1,1,0,1,1,0,1,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,0,1,0,
	0,1,0,1,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,1,0,1,
	0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,1,0,0,0,1,0,1,1,0,1,0,1,
	1,0,0,1,0,0,1,0,0,1,0,1,1,0,0,1,1,0,1,1,0,0,0,1,1,0,1,0,0,1,1,1,
	1,0,0,0,0,0,0,1,1,1,1,1,0,1,1,0,0,1,0,1,1,0,1,1,1,1,1,0,0,0,0,0
};

#endif /* INCLUDED_SPRITE_CORRELATOR_CF_H */

