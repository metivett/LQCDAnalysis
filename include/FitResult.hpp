/*
 * FitResult.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_RESULT_HPP_
#define FIT_RESULT_HPP_

#include "fit_data.hpp"
#include "models.hpp"
#include "model_parameters.hpp"
#include "io.hpp"

namespace LQCDA {

    class FitResult
    {
    private:
	FitModel* _Model;
	FitDataBase* _Data;

	ModelParameters _FitParameters;

    public:
	FitResult(FitDataBase* data, FitModel* model, const ModelParameters& par) :
	    _Model(model),
	    _Data(data),
	    _FitParameters(par) {}

	const ModelParameters& FittedParameters() const { return _FitParameters; }
    };

    std::ostream& operator<< (std::ostream& out, const FitResult& res);

} // namespace LQCDA

#endif	// FIT_RESULT_HPP_
