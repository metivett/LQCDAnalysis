/*
 * SampleInterface.hpp
 *
 *  Created on: Mar 14, 2014
 *      Author: Thibaut Metivet
 */

#ifndef SAMPLE_INTERFACE_HPP
#define SAMPLE_INTERFACE_HPP

 #include "Vector.hpp"

 namespace LQCDA {

/******************************************************************************
 *                              SampleInterface                               *
 ******************************************************************************/

 	template<typename T>
 	class SampleInterface
 	{
 	protected:
 		std::vector<T> _Samples;

 	public:
 		// Constrcutors/Destructor
 		SampleInterface() {}
 		SampleInterface(unsigned int nsamples)
 		: _Samples(nsamples)
 		{}
 		SampleInterface(const std::vector<T>& samples)
 		: _Samples(samples)
 		{}
 		virtual ~SampleInterface() =0;

 		// Accessors/Setters
 		unsigned int nSamples() const;
 		T& operator[](unsigned int i);
 		const T& operator[](unsigned int i) const;
 		void reserve(unsigned int n);
 		void addSample(const T& s);

 		// Statistical function
 		T mean() const;
 		T var() const;
 	};

 	template<typename T>
 	unsigned int SampleInterface<T>::nSamples() const
 	{
 		return _Samples.size();
 	}
 	template<typename T>
 	T& SampleInterface<T>::operator[](unsigned int i)
 	{
 		return _Samples.at(i);
 	}
 	template<typename T>
	const T& SampleInterface<T>::operator[](unsigned int i) const
	{
		return _Samples.at(i);
	}
	template<typename T>
	void SampleInterface<T>::reserve(unsigned int n)
	{
		_Samples.reserve(n);
	}
	template<typename T>
	void SampleInterface<T>::addSample(const T& s)
	{
		_Samples.push_back(s);
	}

 	template<typename T>
 	T SampleInterface<T>::mean() const 
 	{
 		T m = _Samples[0];
 		for(int i=0; i<nSamples(); ++i)
 			m = m + _Samples[i];

 		return m / nSamples();
 	}
 	template<typename T>
 	T SampleInterface<T>::var() const 
 	{
 		T m = mean();
 		T buf = (_Samples[0] - m);
 		T v = buf * buf;
 		for(int i=0; i<nSamples(); ++i) {
 			buf = (_Samples[i] - m);
 			v = v + buf * buf;
 		}

 		return v / nSamples();
 	}

/******************************************************************************
 *                                   Sample                                   *
 ******************************************************************************/

 	template<typename T>
 	class Sample
 	: public SampleInterface<T>
 	{
 	static_assert(std::is_fundamental<T>::value, "Sample type must be a fundamental type.");

 	};

 }

#endif // SAMPLE_INTERFACE_HPP