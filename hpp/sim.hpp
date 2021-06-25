/*
 * sim.hpp
 *
 *  Created on: Dec 9, 2018
 *      Author: babis
 */

#ifndef HPP_SIM_HPP_
#define HPP_SIM_HPP_

#include "../hpp/types.hpp"

void load_excitation(int sel, VectorXd&, VectorXd&);
MatrixXd rc_tr_sim_d( MatrixXd&, MatrixXd&, MatrixXd&, MatrixXd&, MatrixXd&, VectorXd&, double, VectorXd& );
MatrixXd rc_tr_sim_sp( SpMat&, SpMat&, SpMat&, SpMat&, MatrixXd&, VectorXd&, double, VectorXd& );

#endif /* HPP_SIM_HPP_ */
