/*
 * CostFcn.hpp
 *
 *  Created on: Feb 11, 2014
 *      Author: Thibaut Metivet
 */

#ifndef COST_FCN_HPP
#define COST_FCN_HPP

 #include <memory>

 #include "Globals.hpp"
 #include "XYDataInterface.hpp"
 #include "FitInterface.hpp"
 #include "ParametrizedFunction.hpp"

 namespace LQCDA {

/******************************************************************************
 *                               CostFcn class                                *
 ******************************************************************************/

 	template<typename T>
 	class CostFunction
 	: public ScalarFunction<T>
 	{
 	protected:
 		// Typedefs
 		typedef ParametrizedScalarFunction<T> ScalarModel;

 		// Data
 		const XYDataInterface<T> & _Data;
 		const FitInterface& _Fit;
 		std::vector<const ScalarModel *> _Model;
 		unsigned int _nPar;

 	public:
 		// Constructors
 		explicit CostFunction(
 			const XYDataInterface<T>& data,
 			const FitInterface& fit);
 		CostFunction(
 			const XYDataInterface<T>& data,
 			const FitInterface& fit,
 			const std::vector<const ParametrizedScalarFunction<T> *>& model);
 		// Destructor
 		virtual ~CostFunction() noexcept = default;

 		// Accessors
 		void setModel(const ScalarModel * model, unsigned int i);
 		void setModel(const std::vector<const ScalarModel *>& model);

 		unsigned int nPar() const { return _nPar; }
 		unsigned int nDOF() const;

 	private:
 		void checkModel(const ScalarModel * model);

 	};

 	template<typename T>
 	CostFunction<T>::CostFunction(
 		const XYDataInterface<T>& data,
 		const FitInterface& fit)
 	: ScalarFunction<T>(0)
 	, _Data(data)
 	, _Fit(fit)
 	, _Model(data.yDim(), nullptr)
 	, _nPar{0}
 	{}

 	template<typename T>
 	CostFunction<T>::CostFunction(
 		const XYDataInterface<T>& data,
 		const FitInterface& fit,
 		const std::vector<const ParametrizedScalarFunction<T> *>& model)
 	: ScalarFunction<T>(0)
 	, _Data(data)
 	, _Fit(fit)
 	, _Model(data.yDim(), nullptr)
 	, _nPar{0}
 	{
 		setModel(model);
 	}

 	template<typename T>
 	void CostFunction<T>::setModel(const ScalarModel * model, unsigned int i)
 	{
 		assert(i < _Data->yDim());
 		checkModel(model);
 		_Model[i] = model;
 		this->setXDim(_nPar + _Data.nFitXDim() * _Data.nFitPoints());
 	}

 	template<typename T>
 	void CostFunction<T>::setModel(const std::vector<const ScalarModel *>& model)
 	{
 		unsigned int ydim = _Data.yDim();
 		_nPar = 0;
 		if(model.size() != ydim)
 		{
 			ERROR(SIZE, "wrong number of models provided (expected "
 				+ utils::strFrom(ydim) + ", got " + utils::strFrom(model.size()) + ")");
 		}
 		for(unsigned int i=0; i<ydim; ++i)
 		{
 			if(!model[i])
 			{
 				ERROR(MEMORY, "trying to set a null model pointer (y index: "
 					+ utils::strFrom(i) + ")");
 			}
 			checkModel(model[i]);
 			_Model[i] = model[i];
 		}
 		this->setXDim(_nPar + _Fit.nFitXDim() * _Fit.nFitPoints());
 	}

 	template<typename T>
 	unsigned int CostFunction<T>::nDOF() const
 	{
 		if(_nPar == 0)
 		{
 			ERROR(MEMORY, "no model set");
 		}
 		return _Data.yDim() * _Fit.nFitPoints() - _nPar;
 	}

 	template<typename T>
 	void CostFunction<T>::checkModel(const ScalarModel * model)
 	{
 		if(model->xDim() != _Data.xDim()) 
 		{
 			ERROR(SIZE, "model/data x-dimension mismatch");
 		}
		if(_nPar && model->nPar() != _nPar)
		{
			ERROR(SIZE, "wrong number of parameters in provided model (expected "
				+ utils::strFrom(_nPar) + ", got " + utils::strFrom(model->nPar()) + ")");
		}
		else if (_nPar == 0)
		{
			_nPar = model->nPar();
		}
 	}



/******************************************************************************
 *                             Chi2 CostFunction                              *
 ******************************************************************************/

 	template<typename T>
 	class Chi2CostFunction
 	: public CostFunction<T>
 	{
 		//static_assert(std::is_floating_point<T>::value, "Scalar type must be floating point");
 	public:
 		// Typedefs
 		typedef typename CostFunction<T>::ScalarModel ScalarModel;

 	protected:
 		using CostFunction<T>::_Data;
 		using CostFunction<T>::_Fit;
 		using CostFunction<T>::_Model;
 		using CostFunction<T>::_nPar;

 	private:
 		// Structs
 		struct Helper
 		{
 			// is updated
 			bool is_updated {false};
 			// vector of residuals
 			Vector<T> r;
 			// fitted points indices
 			Vector<index_t> d_ind;
 			// fitted x indices
 			Vector<index_t> x_ind;
 			// inverse covariance matrix
 			Matrix<T> c_inv;
 			// x buffer
 			Vector<T> x_buf;
 		};

 		// Data
 		std::unique_ptr<Helper> _helper;


 	public:
 		// Constructors
 		explicit Chi2CostFunction(
 			const XYDataInterface<T>& data,
 			const FitInterface& fit)
 		: CostFunction<T>(data, fit)
 		, _helper(new Helper)
 		{}
 		Chi2CostFunction(
 			const XYDataInterface<T>& data,
 			const FitInterface& fit,
 			const std::vector<const ParametrizedScalarFunction<T> *>& model)
 		: CostFunction<T>(data, fit, model)
 		, _helper(new Helper)
 		{}
 		// Destructor
 		virtual ~Chi2CostFunction() noexcept = default;

 		// Accessors
 		void requestUpdate() const;

 	protected:
 		virtual T operator()(const T * args) const override;

 	private:
 		void setCov(index_t j1, index_t j2, ConstRef<Matrix<T>> cov) const;
 		void update_helper() const;

 	};

 	template<typename T>
 	void Chi2CostFunction<T>::requestUpdate() const
 	{
 		_helper->is_updated = false;
 	}

 	template<typename T>
 	T Chi2CostFunction<T>::operator()(const T * args) const
 	{
 		// init sizes
 		index_t nFitPoints = _Fit.nFitPoints();
 		index_t xDim = _Data.xDim();
 		index_t yDim = _Data.yDim();
 		index_t ysize = yDim * nFitPoints;

 		// get x "dummy" params
 		ConstMap<Vector<T>> x_par(args + _nPar, this->xDim() - _nPar, 1);

 		// update helper if necessary
 		if(!_helper->is_updated)
 		{
 			update_helper();
 		}

 		// set vector of residuals
 		// y part
 		index_t xpar_ind;
 		for(index_t yk=0; yk<yDim; ++yk)
 		{
 			FOR_VEC(_helper->d_ind, i)
 			{
 				const ScalarModel * f = _Model[yk];
 				if(!f)
 				{
 					ERROR(MEMORY, "null model pointer encountered (at y index = "
 						+ utils::strFrom(yk) + ")");
 				}
 				xpar_ind = 0;
 				for(index_t xk=0; xk<xDim; ++xk)
 				{
 					if(_Fit.isXExact(xk))
 					{
 						_helper->x_buf(xk) = _Data.x(_helper->d_ind(i), xk);
 					}
 					else
 					{
 						_helper->x_buf(xk) = x_par(xpar_ind * nFitPoints + i);
 						xpar_ind++;
 					}
 				}
 				// ryi_k = yi_k - f(xi)
 				_helper->r(yk * nFitPoints + i) = 
 					_Data.y(_helper->d_ind(i), yk) - (*f)(_helper->x_buf.data(), args);
 			}
 		}
 		// x part
 		FOR_VEC(_helper->x_ind, xk)
 		FOR_VEC(_helper->d_ind, i)
 		{
 			// rxi_k = xi_k - x_par i_k
 			_helper->r(ysize + xk * nFitPoints + i) =
 				_Data.x(_helper->d_ind(i), _helper->x_ind(xk)) - x_par(xk * nFitPoints + i);
 		}

 		// compute chi2
 		T chi2 = _helper->r.dot(_helper->c_inv * _helper->r);
 		// std::cout << "chi2 = " << chi2 << '\n';
 		return chi2;
 	}

 	template<typename T>
 	void Chi2CostFunction<T>::setCov(index_t k1, index_t k2, ConstRef<Matrix<T>> cov) const
 	{
 		index_t nFitPoints = _Fit.nFitPoints();
 		FOR_VEC(_helper->d_ind, i1)
 		FOR_VEC(_helper->d_ind, i2)
 		{
 			if(_Fit.isDataCorrelated(_helper->d_ind(i1), _helper->d_ind(i2)))
 			{
 				_helper->c_inv(k1*nFitPoints + i1, k2*nFitPoints + i2) =
 					cov(_helper->d_ind(i1), _helper->d_ind(i2));
 			}
 		}
 	}

 	template<typename T>
 	void Chi2CostFunction<T>::update_helper() const
 	{
 		// Resize
 		index_t nPoints = _Data.nPoints();
 		index_t nFitPoints = _Fit.nFitPoints();
 		index_t xDim = _Data.xDim();
 		index_t nFitXDim = _Fit.nFitXDim();
 		index_t yDim = _Data.yDim();
 		index_t size = (yDim + nFitXDim) * nFitPoints;
 		_helper->r.setConstant(size, T{0});
 		_helper->d_ind.setZero(nFitPoints);
 		_helper->x_ind.setZero(nFitXDim);
 		_helper->c_inv.setConstant(size, size, T{0});
 		_helper->x_buf.setConstant(xDim, T{0});

 		// Build index tables
 		index_t di = 0;
 		for(index_t i=0; i<nPoints; ++i)
 		{
 			if(_Fit.isFitPoint(i))
 			{
 				_helper->d_ind(di) = i;
 				di++;
 			}
 		}
 		index_t xk = 0;
 		for(index_t k=0; k<xDim; ++k)
 		{
 			if(!_Fit.isXExact(k))
 			{
 				_helper->x_ind(xk) = k;
 				xk++;
 			}
 		}

 		// Set inverse covariance matrix
 		// set yy cov
 		for(index_t yk1=0; yk1<yDim; ++yk1)
 		{
 			for(index_t yk2=0; yk2<yDim; ++yk2)
 			{
 				if(_Fit.isYYCorrelated(yk1, yk2))
 				{
 					setCov(yk1, yk2, _Data.yyCov(yk1, yk2));
 				}
 			}
 		}
 		// set xx cov
 		FOR_VEC(_helper->x_ind, xk1)
 		FOR_VEC(_helper->x_ind, xk2)
 		{
 			if(_Fit.isXXCorrelated(_helper->x_ind(xk1), _helper->x_ind(xk2)))
 			{
 				setCov(xk1 + yDim, xk2 + yDim, _Data.xxCov(_helper->x_ind(xk1), _helper->x_ind(xk2)));
 			}
		}

		// set xy cov
		FOR_VEC(_helper->x_ind, xk)
		{
			for(index_t yk=0; yk<yDim; ++yk)
			{
				if(_Fit.isXYCorrelated(_helper->x_ind(xk), yk))
				{
					setCov(xk + yDim, yk, _Data.xyCov(_helper->x_ind(xk), yk));
				}
			}
		}

		// symmetrize
		auto YX = _helper->c_inv.block(0, nFitPoints*yDim, nFitPoints*yDim, nFitPoints*nFitXDim);
		auto XY = _helper->c_inv.block(nFitPoints*yDim, 0, nFitPoints*nFitXDim, nFitPoints*yDim);
		YX = XY.transpose().eval();

		// invert
		_helper->c_inv = _helper->c_inv.inverse().eval();

		_helper->is_updated = true;
 	}

 }

#endif // COST_FCN_HPP