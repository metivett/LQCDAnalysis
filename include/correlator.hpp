/*
 * correlator.hpp
 *
 *  Created on: Feb 11, 2013
 *      Author: Thibaut Metivet
 */

#ifndef CORRELATOR_HPP_
#define CORRELATOR_HPP_

#include <vector>
#include <iostream>
#include "exceptions.hpp"
#include "interpolator.hpp"

namespace LQCDA {

/**
 * class BaseCorrelator
 * Time and configuration dependent correlator.

class BaseCorrelator
{
public:
	BaseCorrelator (const Interpolator * i)
	: T(), data()
	{
		m_i = i;
	}
	BaseCorrelator (const Interpolator * i, const mreal * d, int Nt)
	{
		T = Nt;
		m_i = i;
		data.Set(d, Nt);
	}
	BaseCorrelator (const Interpolator * i, const std::vector<mreal>& d)
	: data(d.size())
	{
		T = data.nx;
		m_i = i;
		for (int i = 0; i < T; ++i)
			data.a[i] = d[i];
	}
	BaseCorrelator (const mglData &d) {}// NOTE: must be constructor for mglData& to exclude copy one
	BaseCorrelator (const mglDataA *d) {a=0; mgl_data_set(this, d);}inline mglData(bool, mglData *d)	// NOTE: Variable d will be deleted!!!
	{	if(d)
		{	nx=d->nx; ny=d->ny; nz=d->nz; a=d->a; d->a=0;
			id=d->id; link=d->link; delete d;}
		else {a=0; Create(1);}}
	/// Initiate by flat array
	inline mglData(int size, const float *d) {a=0; Set(d,size);}inline mglData(int rows, int cols, const float *d) {a=0; Set(d,cols,rows);}inline mglData(int size, const double *d) {a=0; Set(d,size);}inline mglData(int rows, int cols, const double *d) {a=0; Set(d,cols,rows);}inline mglData(const double *d, int size) {a=0; Set(d,size);}inline mglData(const double *d, int rows, int cols) {a=0; Set(d,cols,rows);}
	/// Read data from file
	inline mglData(const char *fname) {a=0; Read(fname);}
	/// Allocate the memory for data array and initialize it zero
	inline mglData(long xx=1,long yy=1,long zz=1) {a=0;
	Create(xx,yy,zz);}
	/// Delete the array
	virtual ~mglData() {if(!link && a) delete []a;}
	inline mreal GetVal(long i, long j = 0, long k = 0) {
		return mgl_data_get_value(this, i, j, k);
	}
	inline void SetVal(mreal f, long i, long j = 0, long k = 0) {
		mgl_data_set_value(this, f, i, j, k);
	}
	/// Get sizes
	inline long GetNx() const {
		return nx;
	}
	inline long GetNy() const {
		return ny;
	}
	inline long GetNz() const {
		return nz;
	}

	/// Link external data array (don't delete it at exit)
	inline void Link(mreal *A, long NX, long NY = 1, long NZ = 1) {
		mgl_data_link(this, A, NX, NY, NZ);
	}
	inline void Link(mglData &d) {
		Link(d.a, d.nx, d.ny, d.nz);
	}
	/// Allocate memory and copy the data from the gsl_vector
	inline void Set(gsl_vector *m) {
		mgl_data_set_vector(this, m);
	}
	/// Allocate memory and copy the data from the gsl_matrix
	inline void Set(gsl_matrix *m) {
		mgl_data_set_matrix(this, m);
	}

	/// Allocate memory and copy the data from the (float *) array
	inline void Set(const float *A, long NX, long NY = 1, long NZ = 1) {
		mgl_data_set_float(this, A, NX, NY, NZ);
	}
	/// Allocate memory and copy the data from the (double *) array
	inline void Set(const double *A, long NX, long NY = 1, long NZ = 1) {
		mgl_data_set_double(this, A, NX, NY, NZ);
	}

	void setData (const mreal * d, int Nt)
	{
		T = Nt;
		data = mglData(Nt, d);
	}

	virtual void print (std::ostream& out = std::cout)
	{
		out << "Base correlator \n";
		out << "Interpolator : \n" << (*m_i) << '\n';
		out << "Time extent : " << T << '\n';
		out << "Data : \n" << std::scientific;
		for(int i = 0; i<T; ++i)
			out << data[i] << '\n';
	}
	virtual void plot (mglGraph* gr)
	{
		gr->Box();
		gr->SetRange('y', data);
		gr->Plot(data, " +");
	}
protected:
	int T;	// Time-extent of the correlator
	mglData data;	// Data
	const Interpolator * m_i;	// pointer (to save memory) to interpolator
private:
};*/

/**
 * class Correlator
 * Time dependent (only) correlator. Averaged on configurations.
 */
/*class Correlator : public BaseCorrelator
{
public:
	Correlator(const Interpolator * interpolator) :
		BaseCorrelator(interpolator) {};
	Correlator (const Interpolator * interpolator, const mreal * d, const mreal * err, int Nt) :
		BaseCorrelator(interpolator, d, Nt)
	{
		errors.Set(err, Nt);
	}
	Correlator (const Interpolator * interpolator, const std::vector<mreal>& d, const std::vector<mreal>& err)	:
		BaseCorrelator(interpolator, d), errors(T)
	{
		if (err.size() != T)
			throw OutOfRange(err.size());
		else
		{
			for (int i = 0; i < T; ++i)
				errors.a[i] = err[i];
		}
	}

	void plot (mglGraph * gr)
	{
		gr->Box();
		gr->SetRange('y', data);
		gr->Error(data, errors);
	}
protected:
	mglData errors;	// Errors found through bootstrapping on configurations.
private:
};*/

} // namespace LQCDA

#endif /* CORRELATOR_HPP_ */