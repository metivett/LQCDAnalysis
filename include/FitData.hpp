/*
 * FitData.hpp
 *
 *  Created on: Apr 15, 2013
 *      Author: Thibaut Metivet
 */

#ifndef FIT_DATA_HPP_
#define FIT_DATA_HPP_

#include <vector>
#include <type_traits>

namespace LQCDA {

    template<class XT, class YT>
    class FitData
    {
    public:
    	typedef YT y_type;
		typedef std::remove_reference<XT>::type x_type;

    private:
		std::vector<x_type> _X; // "x-points" ie points s.t. we evaluate model(x,params)
		std::vector<y_type> _Y; // data ("y-points") to be fitted

		std::vector<bool> _IsEnabledPoint;
	
    public:
		FitData() {}
		FitData(const std::vector<x_type>& x, 
				const std::vector<y_type>& y) :
		    _X(x), _Y(y),
		    _IsEnabledPoint(x.size(), true)
		    {
		    	std::assert(x.size() == y.size());
		    }

		// Getters
		unsigned int NPoints() const { return _X.size(); }
		// unsigned int XDim() const { return SizeInfo(_X[0]); }
		// unsigned int YDim() const { return SizeInfo(_Data[0]); }

		const x_type& X(unsigned int i) const { return _X[i]; }
		const y_type& Y(unsigned int i) const { return _Y[i]; }
		
		void Add(const x_type& x, const y_type& y) {
		    _Data.push_back(y);
		    _X.push_back(x);
		    _IsEnabledPoint.push_back(true);
		}

		void Reserve(size_t n) {
		    _Data.reserve(n);
		    _X.reserve(n);
		    _EnabledPointsIndexes.reserve(n);
		}

		bool IsEnabled(unsigned int i) const {
			std::assert(i < NPoints());
			return _IsEnabledPoint[i];
		}

		void DisablePoint(unsigned int i) {
		    std::assert(i < NPoints());
		    _IsEnabledPoint[i] = false;
		}
		void EnablePoint(unsigned int i) {
		    std::assert(i < NPoints());
		    _IsEnabledPoint[i] = true;
		}
		void EnableAllPoints() {
		    for(int i = 0; i < NPoints(); ++i)
				_IsEnabledPoint[i] = i;
		}

    protected:
		const std::vector<y_type>& GetY() { return _Y; }
		const std::vector<x_type>& GetX() { return _X; }

		std::vector<x_type> EnabledX() {
		    std::vector<x_type> res;
		    res.reserve(_X.size());
		    for(unsigned int i = 0; i < NPoints(); ++i)
				res[i].push_back(_X[i]);
		    return res;
		}

		std::vector<y_type> EnabledY() {
		    std::vector<y_type> res;
		    res.reserve(_Y.size());
		    for(unsigned int i = 0; i < NPoints(); ++i)
				res[i].push_back(_Y[i]);
		    return res;
		}
		
    };

} // namespace LQCDA


#endif	// FIT_DATA_HPP_
