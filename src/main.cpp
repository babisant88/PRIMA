/*
 * main.cpp
 *
 *  Created on: May 27, 2018
 *      Author: babis
 */

#include <iostream>

#include "../hpp/aux.hpp"
#include "../hpp/sys.hpp"
#include "../hpp/load_input.hpp"
#include "../hpp/sim.hpp"
#include "../hpp/prima.hpp"

#include <chrono>
#include <omp.h>

using namespace std;

#define dbg 0

int main(int argc, char *argv[])
{
	int n; // #node voltages
	int N; // #ports

	int q; // #moments

	n = atoi(argv[5]);
	N = atoi(argv[6]);
	q = atoi(argv[7]);

	/* initialize system matrices */
	SpMat C(n,n); // capacitance matrix
	SpMat G(n,n); // conductance matrix
	SpMat B(n,N); // input port-to-node connectivity matrix
	SpMat D(n,N); // output port-to-node connectivity matrix

	load_input(argv, C, G, B, D);
	cout << "input matrices have been loaded" << endl;

	MatrixXd C_r, G_r, B_r, D_r;

	/* call Prima */
	Prima(C_r, G_r, B_r, D_r, C, G, B, D, q);

#if dbg
	cout << "The matrix C_r is of size " << C_r.rows() << "x" << C_r.cols() << endl;
	cout << "The matrix G_r is of size " << G_r.rows() << "x" << G_r.cols() << endl;
	cout << "The matrix B_r is of size " << B_r.rows() << "x" << B_r.cols() << endl;
	cout << "The matrix D_r is of size " << D_r.rows() << "x" << D_r.cols() << endl;
#endif


#if dbg
	printMatrixXdToFile(G_r, "G_r.txt");
	printMatrixXdToFile(C_r, "C_r.txt");
	printMatrixXdToFile(B_r, "B_r.txt");
	printMatrixXdToFile(D_r, "D_r.txt");
#endif

	int red_sys_size = q*N;

	sys red_sys = sys( C_r, G_r, B_r, D_r, N, N, red_sys_size );

	tf mytf = red_sys.get_tf(0, 0);

#if dbg
	for( int pole_i=0; pole_i<red_sys_size; ++pole_i )
		cout << "pole[" << pole_i << "] = " << mytf.poles[pole_i] << endl;

	for( int res_i=0; res_i<red_sys_size; ++res_i )
		cout << "res[" << res_i << "] = " << mytf.res[res_i] << endl;
#endif

#if dbg
	/* load excitation vector */
	int timestamps = 701;

	VectorXd t(timestamps), e(timestamps);

	load_excitation(3, t, e);

	MatrixXd u(N, t.rows());

	for(int row_i=0; row_i<u.rows(); row_i++)
		u.row(row_i) = e;

	double dt = 1e-12; // timestep

	VectorXd x0; // initial condition
	x0 = VectorXd::Zero(n, 1);

	MatrixXd y(N, t.rows());
	auto start = chrono::high_resolution_clock::now();
	y = rc_tr_sim_sp(C, G, B, D, u, x0, dt, t);
	auto finish = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = finish - start;
	cout << "sim. time of the original (sparse) model: " << elapsed.count() << endl;

	x0 = VectorXd::Zero(q*N, 1);

	MatrixXd y_r(N, t.rows());
	start = chrono::high_resolution_clock::now();
	y_r = rc_tr_sim_d(C_r, G_r, B_r, D_r, u, x0, dt, t);
	finish = chrono::high_resolution_clock::now();
	elapsed = finish - start;
	cout << "sim. time of the reduced (dense) model: " << elapsed.count() << endl;

	printVectorXdToFile(t, "t.txt");
	printMatrixXdToFile(y, "y.txt");
	printMatrixXdToFile(y_r, "y_r.txt");
#endif
}
