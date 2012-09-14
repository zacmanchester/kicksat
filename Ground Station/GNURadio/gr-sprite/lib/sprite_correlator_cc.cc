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
#include <cmath>
#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>
#define EIGEN_FFTW_DEFAULT //Use FFTW instead of built in Eigen FFT

using namespace Eigen;

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
	generate_prn(prn_id);
	m_template = (cc430_modulator(m_prn)).conjugate();
	m_fft.SetFlag(m_fft.Unscaled);
}


sprite_correlator_cc::~sprite_correlator_cc ()
{
}

void sprite_correlator_cc::generate_prn(int prn_id)
{
	if(prn_id == -2)
	{	
		//Deep copy M-sequence
		for (int k = 0; k < 512; k++)
		{
			m_prn[k] = mseq1[k];
		}
	}
	else if(prn_id == -1)
	{	
		//Deep copy M-sequence
		for (int k = 0; k < 512; k++)
		{
			m_prn[k] = mseq2[k];
		}
	}
	else //if(prn_id >= 0 && prn_id < 512)
	{	
		//Generate Gold Codes by xor'ing 2 M-sequences in different phases
		for (int k = 0; k < 512-prn_id; k++)
		{
			m_prn[k] = mseq1[k] ^ mseq2[k+prn_id];
		}
		for (int k = 512-prn_id; k < 512; k++)
		{
			m_prn[k] = mseq1[k] ^ mseq2[k-512+prn_id];
		}
	}
}

Vector512c sprite_correlator_cc::cc430_modulator(int* prnBits)
{
	Vector512f diffs;
	Vector512f iBB;
	Vector512f qBB;
	Vector512c baseBand;
	
	//Differentially encode with +/-1 values
	diffs(0) = -2*prnBits[0] + 1;
	for (int k = 1; k < 512; k++)
	{
		char diff = prnBits[k]-prnBits[k-1];
		if(diff == 0)
		{
			diffs(k) = 1;
		}
		else
		{
			diffs(k) = -1;
		}
	}
	
	//Initialize with offset between I and Q
	iBB(0) = 1;
	qBB(0) = diffs(0);
	qBB(1) = diffs(0);
	
	for(int k = 1; k < 510; k+=2)
	{
		iBB(k) = diffs(k)*iBB(k-1);
		iBB(k+1) = iBB(k);
	}
	iBB(511) = diffs(511)*iBB(510);
	
	for(int k = 2; k < 512; k+=2)
	{
		qBB(k) = diffs(k)*iBB(k-1);
		qBB(k+1) = qBB(k);
	}
	
	for(int k = 0; k < 512; k++)
	{
		baseBand(k) = iBB(k)*cos(M_PI/2*k) + 1i*qBB(k)*sin(M_PI/2*k);
	}
	
	return baseBand;
}

int sprite_correlator_cc::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const gr_complex *in = (const gr_complex *) input_items[0];
	gr_complex *out = (gr_complex *) output_items[0];

	// Do <+signal processing+>
	for(int k = 0; k < noutput_items; k++) {
		
		//wrap the input vector in an Eigen matrix
		Map<const Vector512c> invec(&in[k]);
		
		//Pointwise multiply by baseband template
		m_temp1 = invec.cwiseProduct(m_template);
		
		//Take FFT
		m_temp2 = m_fft.fwd(m_temp1);
		
		//Output is largest value in FFT
		out[k] = m_temp2.cwiseAbs().maxCoeff();
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

