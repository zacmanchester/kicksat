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

#ifndef INCLUDED_SPRITE_CORRELATOR_CC_H
#define INCLUDED_SPRITE_CORRELATOR_CC_H

#include <sprite_api.h>
#include <gr_sync_block.h>
#include <Eigen/Dense>

using namespace Eigen;

class sprite_correlator_cc;
typedef boost::shared_ptr<sprite_correlator_cc> sprite_correlator_cc_sptr;

//Eigen vector/matrix typedefs
typedef Matrix<float, 512, 1> Vector512f;
typedef Matrix<gr_complex, 512, 1> Vector512c;
typedef Matrix<gr_complex, Dynamic, 512> Matrix512c;

SPRITE_API sprite_correlator_cc_sptr sprite_make_correlator_cc (int prn_id);

/*!
 * \brief <+description+>
 *
 */
class SPRITE_API sprite_correlator_cc : public gr_sync_block
{
	private:
	
		friend SPRITE_API sprite_correlator_cc_sptr sprite_make_correlator_cc (int prn_id);

		sprite_correlator_cc (int prn_id);
		
		int m_prn[512];
		void generate_prn(int prn_id);
		
		Vector512c cc430_modulator(int* prnBits);
		
		Matrix512c m_template;
		void generate_template(double minFreqOffset, double maxFreqOffset);
		
		static int mseq1[512];
		static int mseq2[512];

	public:
		~sprite_correlator_cc ();


		int work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);
};

int sprite_correlator_cc::mseq1[] = {
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
		
int sprite_correlator_cc::mseq2[] = {
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

#endif /* INCLUDED_SPRITE_CORRELATOR_CC_H */

