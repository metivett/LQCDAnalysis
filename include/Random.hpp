/*
 * random.hpp
 *
 *  Created on: Feb 22, 2013
 *      Author: Thibaut Metivet
 */

#ifndef RANDOM_HPP_
#define RANDOM_HPP_

#include <set>

namespace LQCDA {

const int RG_STATE_SIZE = 105;

/**
 * class RandGen
 * Generates random numbers with different distributions.
 * The internal code is an adaptation of Martin Luscher's "ranlxd" :
 * 		Copyright (C) 2005 Martin Luescher (GPL)
 * 		This software is distributed under the terms of the GNU General Public
 * 		License (GPL)
 *
 * 		Random number generator "ranlxd". See the notes
 *
 *   	"User's guide for ranlxs and ranlxd v3.2" (December 2005)
 *
 *	    "Algorithms used in ranlux v3.0" (May 2001)
 *
 * 		for a detailed description.
 *
 */
class RandGen
{
public:
	// Typedefs
	typedef int rg_state[RG_STATE_SIZE];

	// Constructors
	/**
	 * Default constructor.
	 * Initialize the random generator with time, and level 1.
	 */
	RandGen();
	/**
	 * Initialize with seed and level. The level has to be 1 or 2, and controls the statistical quality
	 * of the generator. Usually, level=1 is sufficient to guarantee very small statistical correlations.
	 * Level = 2 decrease these correlations by several orders of magnitude, at the expense of a doubling
	 * of the execution time.
	 */
	RandGen(const int seed, const int level =1);

	// Random generator state getter and setter
	void setState(rg_state state);
	void getState(rg_state state);

	// Distribution dependant random numbers providers
	double getUniform(double a, double b);
	unsigned int getUniformInt(const unsigned int max);
	double getNormal(const double mean, const double sigma);
	std::set<unsigned int> getUniformIntSample(const unsigned int max, const unsigned int n);

	int size() { return RG_STATE_SIZE; }
private:
	void init(int level,int seed);
	void update();
	void define_constants();
	void ranlxd(double r[],int n);

#ifdef HAVE_SSE

	typedef struct
	{
	   float c1,c2,c3,c4;
	} vec_t __attribute__ ((aligned (16)));

	typedef struct
	{
	   vec_t c1,c2;
	} dble_vec_t __attribute__ ((aligned (16)));

	int pr,prm,ir,jr,is,is_old,next[96];
	vec_t one,one_bit,carry;

	union
	{
	   dble_vec_t vec[12];
	   float num[96];
	} x __attribute__ ((aligned (16)));

#else	// HAVE_SSE

	typedef struct
	{
	   int c1,c2,c3,c4;
	} vec_t;

	typedef struct
	{
	   vec_t c1,c2;
	} dble_vec_t;

	int pr,prm,ir,jr,is,is_old,next[96];
	double one_bit;
	vec_t carry;

	union
	{
	   dble_vec_t vec[12];
	   int num[96];
	} x;

#endif	// HAVE_SSE
};

}	// namespace LQCDA


#endif /* RANDOM_HPP_ */
