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
	set_history(24);
	m_counter = 0;
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

float sprite_soft_decoder_f::softdecode(const float *buffer, char *output)
{
	int index = 0;
	float max_corr = 0;
	float corr;
	float mag;

	//Calculate the magnitude of the vector
	mag = 0;
	for(int k = 0; k < 22; ++k)
	{
		mag += buffer[k]*buffer[k];
	}
	mag = sqrt(22*mag);

	//Multiply received vector by codeword matrix and look for maximum
	for(int i = 0; i < 256; ++i)
	{
		corr = 0;
		for(int j = 0; j < 22; ++j)
		{
			corr += C[i][j]*buffer[j]/mag;
		}
		if(corr > max_corr)
		{
			max_corr = corr;
			index = i;
		}
	}

	*output = (char)index;
	return max_corr;
}

int sprite_soft_decoder_f::work(int noutput_items,
		  gr_vector_const_void_star &input_items,
		  gr_vector_void_star &output_items)
{
	const float *in = (const float *) input_items[0];

	for(int k = 0; k < noutput_items; ++k)
	{
		//Compute the energy in 3 overlapping 22-sample intervals
		for(int j = 0; j < 24; ++j)
		{
			m_squares[j] = in[k+j]*in[k+j];
		}

		float energy1 = 0;
		float energy2 = 0;
		float energy3 = 0;
		for(int j = 0; j < 22; ++j)
		{
			energy1 += m_squares[j];
			energy2 += m_squares[j+1];
			energy3 += m_squares[j+2];
		}
		
		//Use the first second (125 samples) to characterize the noise
		if(m_initialized)
		{
			//Updoate the energies with the new sample and increment the counter
			m_counter %= 125;
			m_energies[m_counter] = energy1;
			++m_counter;

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
				char result1;
				float corr1 = softdecode(&in[k], &result1);

				char result2;
				float corr2 = softdecode(&in[k+1], &result2);

				char result3;
				float corr3 = softdecode(&in[k+2], &result3);

				char result;
				float corr;
				if(corr1 > corr2 && corr1 > corr3)
				{
					corr = corr1;
					result = result1;
				}
				else if(corr3 > corr2 && corr3 > corr1)
				{
					corr = corr3;
					result = result3;
				}
				else
				{
					corr = corr2;
					result = result2;
				}
				
				/* Debug stuff */
				cout << '<';
				cout << setiosflags(ios::fixed) << setprecision(2) << corr;
				cout << '>';

				cout << result;
			}
		}
		else
		{
			//Use the first second of data (125 samples) to initialize the median calculation
			m_energies[m_counter] = energy1;
			++m_counter;

			if(m_counter >= 125)
			{
				m_initialized = 1;
			}
		}
	}

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

