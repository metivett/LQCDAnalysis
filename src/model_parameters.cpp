/*
 * model_parameters.cpp
 *
 *  Created on: May 17, 2013
 *      Author: Thibaut Metivet
 */

#include <ostringstream>
#include <assert.h>
#include <algorithm>

using namespace LQCDA;

class ParNameCmp
{
private:
const std::string& _Name;

public:
ParNameCmp(const std::string& name) : _Name(name) {}

bool operator() (const FunctionParameter& par) const {
return par.Name() == _Name;
}
};

ModelParameters::ModelParameters(size_t nparams) :
    _FunctionParameters()
{
for(int n = 0; n < nparams; ++n) {
std::ostringstream oss;
oss<<"p"<<n;
_FunctionParameters.push_back(FunctionParameter(n,oss.str());
}
    }

ModelParameters::ModelParameters(const std::vector<double>& vals, const std::vector<double>& errs) :
    _FunctionParameters()
{
assert(vals.size() == errs.size())
for(int n = 0; n < vals.size(); ++n) {
std::ostringstream buf;
buf << "p" << n;
_FunctionParameters.push_back(FunctionParameter(n,buf.str(),vals[n],errs[n]);
}
    }

bool ModelParameters::Add(const std::string& name, double val, double err, double low, double up)
{
if(std::find_if(_FunctionParameters.begin(), _FunctionParameters.end(), ParNameCmp(name)) != _FunctionParameters.end())
    return false;
else {
_FunctionParameters.push_back(FunctionParameter(_FunctionParameters.size(), name, val, err, low, up));
return true;
}
}
bool ModelParameters::Add(const std::string& name, double val, double err)
{
if(std::find_if(_FunctionParameters.begin(), _FunctionParameters.end(), ParNameCmp(name)) != _FunctionParameters.end())
    return false;
else {
_FunctionParameters.push_back(FunctionParameter(_FunctionParameters.size(), name, val, err));
return true;
}
}
bool ModelParameters::Add(const std::string& name, double val)
{
if(std::find_if(_FunctionParameters.begin(), _FunctionParameters.end(), ParNameCmp(name)) != _FunctionParameters.end())
    return false;
else {
_FunctionParameters.push_back(FunctionParameter(_FunctionParameters.size(), name, val));
return true;
}
}

inline void ModelParameters::SetValue(unsigned int n, double val)
{
assert(n < _FunctionParameters.size());
_FunctionParameters[n].SetValue(val);
}
inline void ModelParameters::SetError(unsigned int n, double err)
{
assert(n < _FunctionParameters.size());
_FunctionParameters[n].SetError(err);
}
inline void ModelParameters::SetLimits(unsigned int n, double low, double up)
{
assert(n < _FunctionParameters.size());
_FunctionParameters[n].SetLimits(low, up);
}
inline void ModelParameters::SetLowerLimit(unsigned int n, double low)
    {
assert(n < _FunctionParameters.size());
_FunctionParameters[n].SetLowerLimit(low);
}
inline void ModelParameters::SetUpperLimit(unsigned int n, double up)
    {
assert(n < _FunctionParameters.size());
_FunctionParameters[n].SetUpperLimit(up);
}
inline void ModelParameters::RemoveLimits(unsigned int n)
    {
assert(n < _FunctionParameters.size());
_FunctionParameters[n].RemoveLimits();
}

std::vector<double> ModelParameters::ParamValues() const
{
unsigned int nPar = _FunctionParameters.size();
std::vector<double> result(nPar);
for(int n = 0; n < nPar; ++n) {
result[n] = _FunctionParameters[n].Value();
}
return result;
}
std::vector<double> ModelParameters::ParamValues(const std::vector<bool>& mask) const
{
unsigned int nPar = _FunctionParameters.size();
assert(mask.size() == nPar);
std::vector<double> result();
result.reserve(nPar);
for(int n = 0; n < nPar; ++n) {
if(mask[n])
result[n].push_back(_FunctionParameters[n].Value());
}
return result;
}
std::vector<double> ModelParameters::ParamErrors() const
{
unsigned int nPar = _FunctionParameters.size();
std::vector<double> result(nPar);
for(int n = 0; n < nPar; ++n) {
result[n] = _FunctionParameters[n].Error();
}
return result;
}
std::vector<double> ModelParameters::ParamErrors(const std::vector<bool>& mask) const
{
unsigned int nPar = _FunctionParameters.size();
assert(mask.size() == nPar);
std::vector<double> result();
result.reserve(nPar);
for(int n = 0; n < nPar; ++n) {
if(mask[n])
result[n].push_back(_FunctionParameters[n].Error());
}
return result;
}
inline const std::vector<FunctionParameter>& ModelParameters::Parameters() const
{
return _FunctionParameters;
}

inline double ModelParameters::Value(unsigned int n)
{
assert(n < _FunctionParameters.size());
return _FunctionParameters[n].Value();
}
inline double ModelParameters::Error(unsigned int n)
    {
assert(n < _FunctionParameters.size());
return _FunctionParameters[n].Error();
}
