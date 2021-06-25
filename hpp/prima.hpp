/*
 * prima.hpp
 *
 *  Created on: Nov 25, 2020
 *      Author: babis
 */

#ifndef HPP_PRIMA_HPP_
#define HPP_PRIMA_HPP_

#include "types.hpp"
//#include "aux.hpp"

void Prima(MatrixXd&, MatrixXd&, MatrixXd&, MatrixXd&, SpMat&, SpMat&, SpMat&, SpMat&, int);

VectorXd spMat_col(SpMat&, int);

#endif /* HPP_PRIMA_HPP_ */
