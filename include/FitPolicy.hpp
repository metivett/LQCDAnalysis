/*
 * FitPolicy.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: Thibaut Metivet
 */

#ifndef FIT_POLICY_HPP
#define FIT_POLICY_HPP

 #include "TypeTraits.hpp"
 #include "ParametrizedFunction.hpp"
 #include "DataSet.hpp"
 #include "Matrix.hpp"

 namespace LQCDA {

 	template<typename T, int XDIM, int YDIM, unsigned int NPAR>
 	class Chi2Fit
 	{
 		static_assert(std::is_floating_point<T>::value, "Scalar type must be floating point");
 	public:
 		// Typedefs
 		using ParametrizedFunction<T, XDIM, YDIM, NPAR>::x_type;
 		using ParametrizedFunction<T, XDIM, YDIM, NPAR>::y_type;
 		using ParametrizedFunction<T, XDIM, YDIM, NPAR>::p_type;
 		typedef typename if_<std::is_fundamental<p_type>::value, p_type, const p_type&>::result params_type;

 	protected:
 		ParametrizedFunction<T, XDIM, YDIM, NPAR> * _Model;

 		Vector<T, Dynamic> _X;
 		Vector<T, Dynamic> _Y;
 		mutable Vector<T, Dynamic> _MX;

 	public:
 		Chi2Fit()
 		: _Data(data), _Model(model), _Y(data->size() * YDIM), _MX(data->size() * YDIM) 
 		{
 			// Fill _Y
 			for(int i=0; i<_Data->size(); i++)
 				_Y.segment(i*YDIM, YDIM) << _Data->y(i);
 		}

 		double operator() (params_type params) {
 			assert(params.size() == NPAR);
 			// Set model parameters
 			_Model->setParamValues(params);
 			// Fill _MX
 			for(int i=0; i<_Data->size(); i++)
 				_MX.segment(i*YDIM, YDIM) << (*_Model)(_Data->x(i));
 			// Compute and return Chi2
 			return Chi2(_Y - _MX);
 		}

 	private:
 		template<typename Derived1>
 		static double Chi2(const Eigen::MatrixBase<Derived1>& v)
 		{
 			return v.transpose() * v;
 		}

 	};


 	template<typename T, int XDIM, int YDIM, unsigned int NPAR>
 	class YWChi2Fit:public Function<std::vector<double>, double>
 	{
 		static_assert(std::is_floating_point<T>::value, "Scalar type must be floating point");

 	protected:
 		typedef DataSet<T, XDIM, YDIM> DataT;
 		typedef VParametrizedFunction<T, XDIM, YDIM, NPAR> ModelT;

 		const DataSet<T, XDIM, YDIM> * _Data;
 		ModelT * _Model;

 		const Matrix<T, Dynamic, Dynamic> * _YW;

 		Vector<T, Dynamic> _Y;
 		mutable Vector<T, Dynamic> _MX;

 	public:
 		YWChi2Fit(const DataT * data, ModelT * model)
 		: _Data(data), _Model(model), _Y(data->size() * YDIM), _MX(data->size() * YDIM) 
 		{
 			// Fill _Y
 			for(int i=0; i<_Data->size(); i++)
 				_Y.segment(i*YDIM, YDIM) << _Data->y(i);
 		}

 		void setYWeight(const Matrix<T, Dynamic, Dynamic>* w) { _YW = w; }

 		double operator() (const std::vector<double>& params) const {
 			assert(params.size() == NPAR);
 			// Set model parameters
 			_Model->setParamValues(params);
 			// Fill _MX
 			for(int i=0; i<_Data->size(); i++)
 				_MX.segment(i*YDIM, YDIM) << (*_Model)(_Data->x(i));
 			// Compute and return Chi2
 			return Chi2(_Y - _MX);
 		}

 	private:
 		template<typename Derived1>
 		double Chi2(const Eigen::MatrixBase<Derived1>& v) const
 		{
 			return v.transpose() * (*_YW) * v;
 		}

 	};

 }

#endif // FIT_POLICY_HPP