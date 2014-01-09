/*
 * FitResult.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_RESULT_HPP_
#define FIT_RESULT_HPP_

#include "FitData.hpp"
#include "FitModel.hpp"
#include "ModelParameters.hpp"
#include "io.hpp"

namespace LQCDA {

    template<class XT, class YT>
    class FitResult
    {
    private:
		typedef FitData<XT, YT> Data;
		typedef FitModel<XT, YT> Model;
		
		Model* _Model;
		Data* _Data;

		ModelParameters _FitParameters;

    public:
	FitResult(Data* data, Model* model, const ModelParameters& par) :
	    _Model(model),
	    _Data(data),
	    _FitParameters(par) {}

	const ModelParameters& FittedParameters() const { return _FitParameters; }
    };

    template<class DataT, class XT>
    std::ostream& operator<< (std::ostream& out, const FitResult<DataT, XT>& res) {
	out << "\nFitted parameters:\n";
	out << res.FittedParameters()
	    << '\n';
	
	return out;
    }

} // namespace LQCDA

#endif	// FIT_RESULT_HPP_
