/*
 * Minimizer.cpp
 *
 *  Created on: Mar 13, 2014
 *      Author: Thibaut Metivet
 */

 #include <iostream>

 #include "Minimizer.hpp"

 using namespace LQCDA;

 Minimizer::Result::Result()
 {}


 std::ostream& LQCDA::operator<< (std::ostream& out, const Minimizer::Result& result)
 {
 	std::cout << "Final cost = " << result.final_cost << std::endl;
 	return out;
 }