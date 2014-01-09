/*
 * FitModel.hpp
 *
 *  Created on: Mar 8, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FITMODEL_HPP
#define FITMODEL_HPP

#include "TypeTraits.hpp"
#include "MetaProgUtils.hpp"
#include <array>

namespace LQCDA {

    template<class XT, class YT>
    class FitModel
    {
    public:
		typedef YT y_type;
		typedef std::remove_reference<XT>::type x_type;

		virtual y_type operator() (XT x) const =0;
		virtual unsigned int NParams() const =0;

    };

    template<class XT, class YT, unsigned int NPar>
    class StaticFitModel: public FitModel<XT,YT>
    {
    protected:
    	std::array<double, NPar> _Parameters;

    public:
    	StaticFitModel(): _Parameters{} { _Parameters.fill(0.); }
    	StaticFitModel(double p[NPar]): _Parameters{} { for(int i=0; i<NPar; i++) _Parameters[i] = p[i]; }
    	StaticFitModel(std::initializer_list<double> p): _Parameters{p} { std::assert(p.size()==NPar); }

    	virtual unsigned int NParams() const { return NPar; }

    	const std::array<double, NPar>& GetParameters() const { return _Parameters; }
    	std::vector<double> GetParameters() const { return std::vector<double>(_Parameters, _Parameters+NPar); }
    	double GetParameter(unsigned int i) const { std::assert(i<NPar); return _Parameters[i]; }

    	void SetParameters(double p[NPar]) { for(int i=0; i<NPar; i++) _Parameters[i] = p[i]; }
    	void SetParameters(const std::array<double, NPar>& p) { _Parameters = p; }
    	void SetParameters(const std::vector<double>& p) { std::assert(p.size()==NPar); for(int i=0; i<NPar; i++) _Parameters[i] = p[i]; }
    	void SetParameter(unsigned int i, double p) { std::assert(i<NPar); _Parameters[i] = p; }
    };

    template<class XT, class YT, typename... ARGS>
    class FunctionFitModel: public StaticFitModel<XT, YT, sizeof...(ARGS)>
    {
    private:
    	y_type (*_Function)(XT, ARGS...);

    public:
    	FunctionFitModel(YT (*f)(XT, ARGS...)) : _Function(f), StaticFitModel()
    	{
    		std::static_assert(are_floating_points<ARGS...>::value, 
    			"The parameters of the fit model function must be floating-point.");
    	}

    	virtual y_type operator() (XT x) const {
    		return apply_(index_bind<1>(_Function, x), _Parameters);
    	}
    };
    

}	// namespace LQCDA


#endif /* MODELS_HPP_ */
