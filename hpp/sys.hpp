/*
 * tf.hpp
 *
 *  Created on: Dec 3, 2020
 *      Author: babis
 */

#ifndef HPP_SYS_HPP_
#define HPP_SYS_HPP_

#include <complex>

#include "types.hpp"

using namespace std;

class sys
{
	private:
			MatrixXd B;
			MatrixXd D;
			MatrixXcd iS;

			LLT<MatrixXd> lltG;

			EigenSolver<MatrixXd> eig_iGC; // you may use svd instead of eig

			int no_in_ports;
			int no_out_ports;
			int sys_size;

			complex<double> * poles;
			complex<double> ** res;

	public:
			sys(MatrixXd &, MatrixXd &, MatrixXd &, MatrixXd &, int, int, int); // may create a template constructor
			~sys();

			complex<double> * get_poles();
			tf get_tf(int, int);
			int get_no_in_ports();
			int get_no_out_ports();
			int get_sys_size();
};

#endif /* HPP_SYS_HPP_ */
