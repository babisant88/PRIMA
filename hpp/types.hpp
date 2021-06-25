/*
 * types.hpp
 *
 *  Created on: Nov 30, 2018
 *      Author: babis
 */

#ifndef HPP_TYPES_HPP_
#define HPP_TYPES_HPP_

//#define EIGEN_USE_MKL_ALL

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>
#include <eigen3/Eigen/SparseCholesky>
#include <eigen3/Eigen/Eigenvalues>

using namespace Eigen;

enum _bench_type_ {RC = 1, RLC = 2};

//typedef Matrix<double, Dynamic, Dynamic> MatrixXd;
typedef Matrix<double, Dynamic, 1> VectorXd;
typedef SparseMatrix<double> SpMat;
//typedef Matrix<, Dynamic, Dynamic> MatrixXcd;
typedef Triplet<double> T;

typedef struct tfm
{
	std::complex<double> * poles;
	std::complex<double> * res;
}
tf;

#endif /* HPP_TYPES_HPP_ */
