/*
 * utils.hpp
 *
 *  Created on: Mar 19, 2013
 *      Author: Thibaut Metivet
 */

namespace LQCDA {

/*
 * Computes the sum of the elements in the sequence between iterators first and last
 */
    template <typename InputIterator>
    inline typename InputIterator::value_type sum (InputIterator first, InputIterator last)
    {
	typename InputIterator::value_type result = *first++;
	for (; first != last; ++first)
	    result = result + *first;
	return result;
    }

/*
 * Computes the mean value of the elements in the sequence between iterators first and last
 */
    template <typename InputIterator>
    inline typename InputIterator::value_type mean (InputIterator first, InputIterator last)
    {
	typename InputIterator::value_type result = *first++;
	int n(0);
	for (; first != last; ++first) {
	    result = result + *first;
	    n++;
	}
	return result/n;
    }


/*
 * Utility functions to manipulate STL vectors
 */
    template<typename T>
    T sum (const std::vector<T>& v)
    {
	return sum(v.begin(), v.end());
    }
    template<typename T>
    T mean (const std::vector<T>& v)
    {
	return mean(v.begin(), v.end());
    }
    template<typename T>
    std::vector<T> operator+ (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	if(w.size() != n)
	    throw DataException("You're adding vectors of different sizes!");
	std::vector<T> result(n);
	for(int i=0; i<n; ++i)
	    result[i] = v[i] + w[i];
	return result;
    }
    template<typename T>
    std::vector<T> operator+ (const std::vector<T>& v, const T& t)
    {
	int n = v.size();
	std::vector<T> result(v);
	for(int i=0; i<n; ++i)
	    result[i] += t;
	return result;
    }
    template<typename T>
    std::vector<T> operator- (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	if(w.size() != n)
	    throw DataException("You're substracting vectors of different sizes!");
	std::vector<T> result(n);
	for(int i=0; i<n; ++i)
	    result[i] = v[i] - w[i];
	return result;
    }
    template<typename T>
    std::vector<T> operator- (const std::vector<T>& v, const T& t)
    {
	int n = v.size();
	std::vector<T> result(v);
	for(int i=0; i<n; ++i)
	    result[i] -= t;
	return result;
    }
    template<typename T>
    std::vector<T> operator* (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	if(w.size() != n)
	    throw DataException("You're multiplying vectors of different sizes!");
	std::vector<T> result(n);
	for(int i=0; i<n; ++i)
	    result[i] = v[i] * w[i];
	return result;
    }
    template<typename T>
    std::vector<T> operator* (const std::vector<T>& v, const T& t)
    {
	int n = v.size();
	std::vector<T> result(v);
	for(int i=0; i<n; ++i)
	    result[i] *= t;
	return result;
    }



/*
 * Statistical functions to deal with samples of vectors
 */
    template<typename T>
    T variance (const std::vector<T>& v)
    {
	std::vector<T> x = v - mean(v);
	return mean(x*x);
    }
    template<typename T>
    T covariance (const std::vector<T>& v, const std::vector<T>& w)
    {
	int n = v.size();
	int m = w.size();
	if(n != m)
	    throw DataException("You're computing covariance with sample vectors of different sizes!");
	std::vector<T> x = v - mean(v);
	std::vector<T> y = w - mean(w);
	return mean(x*y);
    }
    template<typename Scalar, int Rows, int Cols>
    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> covariance(
	const std::vector<Eigen::Array<Scalar, Rows, Cols> >& v,
	const std::vector<Eigen::Array<Scalar, Rows, Cols> >& w) {
	int n = v.size();
	int m = w.size();
	if(n != m)
	    throw DataException("You're computing covariance with sample vectors of different sizes!");
	std::vector<Eigen::Array<Scalar, Rows, Cols> > x = v - mean(v);
	std::vector<Eigen::Array<Scalar, Rows, Cols> > y = w - mean(w);
	std::vector<Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> > result(n);

	struct v_times_w_transpose {
	    v_times_w_transpose() {}
	    Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> operator() (
		const Eigen::Array<Scalar, Rows, Cols>& a,
		const Eigen::Array<Scalar, Rows, Cols>& b) {
		return a.matrix() * b.matrix().transpose();
	    }
	};
	std::transform(x.begin(), x.end(), y.begin(), result.begin(), v_times_w_transpose());
	return mean(result);
    }

} // namespace LQCDA
