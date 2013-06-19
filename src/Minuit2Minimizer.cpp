/*
 * Minuit2Minimizer.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: Thibaut Metivet
 */

#include "Minuit2Minimizer.hpp"


MnUserParameters LQCDA::Mn2Minimizer::GetMnUserParameters(const ModelParameters& params)
{
    const std::vector<FunctionParameter>& par = params.Parameters();
    MnUserParameters MnPar;

    for(int p=0; p<par.size(); ++p) {
	// Add parameter p
	MnPar.Add(par[p].Name(), par[p].Value(), par[p].Error());
	MnPar.SetError(p, par[p].Error());
	    
	// Set optional limits on parameters
	if(par[p].HasLimits()) {
	    if(par[p].HasLowerLimit()) {
		if(par[p].HasUpperLimit()) {
		    MnPar.SetLimits(p, par[p].LowerLimit(), par[p].UpperLimit());
		}
		else {
		    MnPar.SetLowerLimit(p, par[p].LowerLimit());
		}
	    }
	    else {
		MnPar.SetUpperLimit(p, par[p].UpperLimit());
	    }
	}
    }

    return MnPar;
}

LQCDA::ModelParameters LQCDA::Mn2Minimizer::GetModelParameters(const MnUserParameters& params)
{
    const std::vector<MinuitParameter>& par = params.Parameters();
    ModelParameters ModPar;

    for(int p=0; p<par.size(); ++p) {
	// Add parameter p
	ModPar.Add(par[p].GetName(), par[p].Value(), par[p].Error());
	
	// Set optional limits on parameters
	if(par[p].HasLimits()) {
	    if(par[p].HasLowerLimit()) {
		if(par[p].HasUpperLimit()) {
		    ModPar.SetLimits(p, par[p].LowerLimit(), par[p].UpperLimit());
		}
		else {
		    ModPar.SetLowerLimit(p, par[p].LowerLimit());
		}
	    }
	    else {
		ModPar.SetUpperLimit(p, par[p].UpperLimit());
	    }
	}
    }

    return ModPar;
}
