/*
 * load_input.hpp
 *
 *  Created on: Jun 12, 2018
 *      Author: babis
 */

#ifndef HPP_LOAD_INPUT_HPP_
#define HPP_LOAD_INPUT_HPP_

#include <string>

#include "../hpp/types.hpp"

using namespace std;

void load_sparse_matrix(char *, SpMat&);

void load_input(char* argv[], SpMat&, SpMat&, SpMat&, SpMat&);

#endif /* HPP_LOAD_INPUT_HPP_ */
