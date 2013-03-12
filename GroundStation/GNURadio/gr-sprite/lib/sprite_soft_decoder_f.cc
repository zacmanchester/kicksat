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

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

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
		   gr_make_io_signature(1, 1, sizeof(float)),
		   gr_make_io_signature(0, 0, 0))
{
	set_history(48);
	set_output_multiple(2);
	m_energy_counter = 0;
	m_initialized = 0;
	m_median_buffer = vector<float>(63);
}

/*
 * Our virtual destructor.
 */
sprite_soft_decoder_f::~sprite_soft_decoder_f()
{
	// Put in <+destructor stuff+> here
}

int sprite_soft_decoder_f::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const float *in = (const float*)input_items[0];

	for(int k = 0; k < noutput_items; k+=2)
	{
		//Compute the energy in 3 overlapping 44-sample intervals
		for(int j = 0; j < 48; ++j)
		{
			m_squares[j] = in[k+j]*in[k+j];
		}

		float energy1 = 0;
		float energy2 = 0;
		float energy3 = 0;
		for(int j = 0; j < 44; ++j)
		{
			energy1 += m_squares[j];
			energy2 += m_squares[j+2];
			energy3 += m_squares[j+4];
		}
		
		if(m_initialized)
		{
			//Updoate the energies with the new sample and increment the counter
			m_energy_counter %= 125;
			m_energies[m_energy_counter] = energy1;
			++m_energy_counter;

			//Calculate the median energy to get an estimate of the noise
			partial_sort_copy(m_energies, m_energies+125, m_median_buffer.begin(), m_median_buffer.end());
			float medE = m_median_buffer[62];

			/* Debug Stuff
			cout << "median = ";
			cout << medE;
			cout << "\n"; */

			//If the SNR is > 10 and the energy is a local max
			if(energy2 > 10*medE && energy1 < energy2 && energy3 < energy2)
			{
				char result;
				float corr = softdecode(&in[k], &result);
				
				/* Debug stuff
				cout << '<';
				cout << setiosflags(ios::fixed) << setprecision(2) << corr;
				cout << ", ";
				cout << k;
				cout << ", ";
				cout << noutput_items;
				cout << '>'; */

				cout << result;
			}
		}
		else
		{
			//Use the first half-second of data (125 samples) to initialize the median calculation
			m_energies[m_energy_counter] = energy1;
			++m_energy_counter;

			if(m_energy_counter >= 125)
			{
				m_initialized = 1;
			}
		}
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

float sprite_soft_decoder_f::softdecode(const float *buffer, char *output)
{
	int index = 0;
	float max_corr = 0;
	float corr;
	float mag;

	for(int k = 0; k < 44; k+=2)
	{
		m_buffer[0][k/2] = buffer[k] + buffer[k+1];
		m_buffer[1][k/2] = buffer[k+2] + buffer[k+3];
		m_buffer[2][k/2] = buffer[k+4] + buffer[k+5];
		m_buffer[3][k/2] = buffer[k] + buffer[k+3];
		m_buffer[4][k/2] = buffer[k+1] + buffer[k+2];
		m_buffer[5][k/2] = buffer[k+2] + buffer[k+5];
		m_buffer[6][k/2] = buffer[k+3] + buffer[k+4];
	}

	for(int k = 0; k < 7; ++k)
	{
		//Calculate the magnitude for normalization
		mag = 0;
		for(int i = 0; i < 22; ++i)
		{
			mag += m_buffer[k][i]*m_buffer[k][i];
		}
		mag = sqrt(22*mag);

		//Multiply received vector by codeword matrix and look for maximum
		for(int i = 0; i < 256; ++i)
		{
			corr = 0;
			for(int j = 0; j < 22; ++j)
			{
				corr += s_C[i][j]*m_buffer[k][j]/mag;
			}
			if(corr > max_corr)
			{
				max_corr = corr;
				index = i;
			}
		}
	}

	*output = (char)index;
	return max_corr;
}

