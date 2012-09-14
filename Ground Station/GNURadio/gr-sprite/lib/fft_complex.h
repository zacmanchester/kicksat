#ifndef _fft_complex_h_
#define _fft_complex_h_
/*
	A FFT and Inverse FFT C++ class library for complex arrays

	Author: Tim Molteno, tim@physics.otago.ac.nz

	Based on article "A Simple and Efficient FFT Implementation in C++"
        by Volodymyr Myrnyy with an Inverse FFT modification. 

        This class uses Eigen http://eigen.tuxfamily.org
	as the Container class for doing its complex arrays.

	Copyright Tim Molteno, 2008.
	Licensed under the GPL v3.
*/


#include <cmath>
#include <complex>
#include <algorithm>

#include <Eigen/Core>
using namespace Eigen; 

/*!\brief Radix-2 Decimation class
*/
template<unsigned N, typename T>
class Radix2_Decimation
{
	enum { N2 = N>>1 };
	static Radix2_Decimation<N2,T> next;
public:
	/*!\brief Recursive decimation routine.
	\param data The array of complex numbers to decimate.
	\param iSign If +1 we're doing an FFT, if -1 we're doing an inverse FFT
	*/
	template<typename VectorType>
	static void apply(VectorType data, int iSign)
	{
		/*! Call the next-level of recursion on the first half of the data */
		next.apply(data.template segment<N2>(0), iSign);

		/*! Call the next-level of recursion on the second half of the data */
		next.apply(data.template segment<N2>(N2), iSign);

		T wtemp = iSign*std::sin(M_PI/N);
		T wpi = -iSign*std::sin(2*M_PI/N);
		const std::complex<T> wp(-2.0*wtemp*wtemp, wpi);


		std::complex<T> w(1.0,0.0);
		for (unsigned i=0; i<N2; i++)
		{
			std::complex<T> temp(data[i+N2]*w);
			data[i+N2] = data[i]-temp;
			data[i] += temp;	
			w += w*wp;
		}

	}
}; 

/*!\brief N=2 case for decimation.
*/
template<typename T>
class Radix2_Decimation<2,T>
{
public:
	template<typename VectorType>
	static void apply(VectorType data, int iSign)
	{
		std::complex<T> temp(data[1]);
		data[1] = data[0]-temp;
		data[0] += temp;
	}
};


/*!\brief N=1 case for decimation.
*/
template<typename T>
class Radix2_Decimation<1,T>
{
public:
	template<typename VectorType>
	static void apply(VectorType data, int iSign) { }
};


/*!\brief A templated FFT/Inverse FFT object

	\param N The length of the FFT vector. Must be a power of two.

	How to use this FFT.....

	#define LENGTH 8192

	CFFT<LENGTH, double> cfft;
	Matrix<complex<double>, LENGTH, 1> cdata;
	cdata.setRandom();

	cfft.fft(cdata);	// Forward transform
	cfft.ifft(cdata);	// Reverse transform
*/
template<unsigned N, typename T>
class CFFT
{
	static Radix2_Decimation<N,T> decimation;

	/*!\brief Cooley-Tukey factorization 
	*/
	static void factorize(Matrix< std::complex<T>, N, 1>& data)
	{
		unsigned j=1;
		for (unsigned i=1; i<N; i++)
		{
			if (j>i)
			{
				std::swap(data[j-1], data[i-1]);
			}
			unsigned m = N/2;

			while (m>=2 && j>m)
			{
				j -= m;
				m >>= 1;
			}
			j += m;
		}
	}

public:
	/*!\brief Replaces the complex array data[1..N] by its DFT */
	static void fft(Matrix< std::complex<T>, N, 1>& data)
	{
		factorize(data);
		decimation.apply(data.template segment<N>(0),1);
	}

	/*!\brief Replaces complex array data[1..N] by its inverse DFT */
	static void ifft(Matrix< std::complex<T>, N, 1>& data)
	{
		factorize(data);
		decimation.apply(data.template segment<N>(0),-1);

		/** Scale all results by 1/n */
		T scale = static_cast<T>(1)/N;
		data *= scale;
	}
};

#endif /* _fft_complex_h_ */
