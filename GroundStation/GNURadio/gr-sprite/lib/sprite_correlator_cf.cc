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
#include <sprite_correlator_cf.h>
#include <cmath>
#include <complex>
#include <gri_fft.h>

using namespace std;

sprite_correlator_cf_sptr
sprite_make_correlator_cf (int prn_id)
{
	return sprite_correlator_cf_sptr (new sprite_correlator_cf (prn_id));
}


sprite_correlator_cf::sprite_correlator_cf (int prn_id)
	: gr_sync_block ("correlator_cf",
		gr_make_io_signature (1, 1, sizeof (gr_complex)),
		gr_make_io_signature (1, 1, sizeof (float)))
{
	set_history(640);
	
	generate_prn(prn_id);
	
	cc430_modulator(m_prn, m_template);
	for (int k = 0; k < 640; k++)
	{
		m_template[k] = conj(m_template[k]);
	}
	
	m_fft = new gri_fft_complex(640, true, 1);
	m_fft_buffer_in = m_fft->get_inbuf();
	m_fft_buffer_out = m_fft->get_outbuf();
}

sprite_correlator_cf::~sprite_correlator_cf ()
{
}

void sprite_correlator_cf::generate_prn(int prn_id)
{
	if(prn_id == 0)
	{	
		//Deep copy U-sequence
		for (int k = 0; k < 640; k++)
		{
			m_prn[k] = s_u[k];
		}
	}
	else //if(prn_id >= 0 && prn_id < 640)
	{	
		//Generate SLCE Codes by xor'ing U-sequence in 2 different phases
		for (int k = 0; k < 640-prn_id; k++)
		{
			m_prn[k] = s_u[k] ^ s_u[k+prn_id];
		}
		for (int k = 640-prn_id; k < 640; k++)
		{
			m_prn[k] = s_u[k] ^ s_u[k-640+prn_id];
		}
	}
}

void sprite_correlator_cf::cc430_modulator(int* prnBits, gr_complex* baseBand)
{
	float* diffs = m_buffer_real1;
	float* iBB = m_buffer_real2;
	float* qBB = m_buffer_real3;
	
	//Differentially encode with +/-1 values
	diffs[0] = -2*prnBits[0] + 1;
	for (int k = 1; k < 640; k++)
	{
		char diff = prnBits[k]-prnBits[k-1];
		if(diff == 0)
		{
			diffs[k] = 1;
		}
		else
		{
			diffs[k] = -1;
		}
	}
	
	//Initialize with offset between I and Q
	iBB[0] = 1;
	qBB[0] = diffs[0];
	qBB[1] = diffs[0];
	
	for(int k = 1; k < 638; k+=2)
	{
		iBB[k] = diffs[k]*iBB[k-1];
		iBB[k+1] = iBB[k];
	}
	iBB[639] = diffs[639]*iBB[638];
	
	for(int k = 2; k < 640; k+=2)
	{
		qBB[k] = diffs[k]*qBB[k-1];
		qBB[k+1] = qBB[k];
	}
	
	for(int k = 0; k < 640; k++)
	{
		baseBand[k] = iBB[k]*cos(M_PI/2*k) + 1i*qBB[k]*sin(M_PI/2*k);
	}
}

int
sprite_correlator_cf::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const gr_complex *in = (const gr_complex *) input_items[0];
	float *out = (float *) output_items[0];

	gr_complex* fft_in = m_fft_buffer_in;
	gr_complex* fft_out = m_fft_buffer_out;
	
	// Do <+signal processing+>
	for(int k = 0; k < noutput_items; ++k) {
		
		//Pointwise multiply by baseband template and copy to fft input
		for (int j = 0; j < 640; ++j)
		{
			fft_in[j] = m_template[j]*in[j+k];
		}
		
		//Take FFT
		m_fft->execute();
		
		//Find largest value in FFT
		float mag2 = real(fft_out[0]*conj(fft_out[0]));
		float max = mag2;
		for (int j = 1; j < 640; ++j)
		{
			mag2 = real(fft_out[j]*conj(fft_out[j]));
			if (mag2 > max)
			{
				max = mag2;
			}
		}
		
		out[k] = sqrt(max);
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

