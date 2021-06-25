/*
 * Prima.cpp
 *
 *  Created on: Nov 25, 2020
 *      Author: babis
 */

#include <iostream>
#include <omp.h>
#include <chrono> // whenever you include chrono library you have to include iostream as well
#include "../hpp/prima.hpp"

using namespace std;
/* q	: the number of moments */
void Prima(MatrixXd& C_r, MatrixXd& G_r, MatrixXd& B_r, MatrixXd& D_r, SpMat& C, SpMat& G, SpMat& B, SpMat& D, int q)
{
	/* !Caution: G has to be positive definite in order to use SimplicialLLT solver.
	 * If it isn't, one option is to use FullPivLU */

	SimplicialLLT<SpMat> solver;

	solver.compute(G);

	int N = B.cols();
	int n = G.cols();
	int r = q*N; // the order of the reduced system

	/* G*R = B */
	MatrixXd R(n,N);

	#pragma omp parallel for schedule(dynamic)
	for( int j=0; j<N; j++ )
		R.col(j) = solver.solve( spMat_col(B,j) );

	/* the projection matrix */
	MatrixXd X(n,r);
	MatrixXd Q;

	/* numerically unstable but fast.
	 * When stability is an issue run FullPivHouseholderQR */
	HouseholderQR<MatrixXd> qr(R); // try maybe inplace decomposition (?)
	Q = qr.householderQ();

#if dbg
	std::cout << "The matrix Q is of size " << Q.rows() << "x" << Q.cols() << std::endl;
#endif

	X.topLeftCorner(n,N) = Q.topLeftCorner(n,N);

	/* loop until you reach the number of moments */
	for( int q_i=1; q_i<q; ++q_i ) // k = 1, 2, ..., q
	{
		MatrixXd V;
		V = C * X.block( 0, (q_i-1)*N, n, N ); // V = C*X(k-1)

		/* Xk = G^(-1)*V */
		#pragma omp parallel for schedule(dynamic)
		for( int j=0; j<N; j++ )
			X.col( q_i*N+j ) = solver.solve( V.col(j) );

		for( int q_j=0; q_j<q_i; q_j++ ) // j = 1, 2, ..., k
		{
			MatrixXd H = X.block(0, (q_i-q_j-1)*N, n, N).transpose();
			H = H * X.block(0, q_i*N, n, N); // H = X(k-j)'*Xk
			X.block( 0, q_i*N, n, N) = X.block( 0, q_i*N, n, N) - X.block(0, (q_i-q_j-1)*N, n, N) * H; // Xk = Xk-X(k-j)*H
		}

		HouseholderQR<MatrixXd> qr( X.block( 0, q_i*N, n, N) );
		Q = qr.householderQ();

		X.block( 0, q_i*N, n, N) = Q.topLeftCorner(n,N);
	}

	C_r = X.transpose() * C * X;
	G_r = X.transpose() * G * X;
	B_r = X.transpose() * B;
	D_r = X.transpose() * D;
}



/* a routine that returns the i-th col of sparse matrix sm in dense format */
VectorXd spMat_col(SpMat& sm, int col_i)
{
	VectorXd x = VectorXd::Zero(sm.rows(),1);

	/* loop over the rows of the i-th column of sm */
	for(SpMat::InnerIterator it(sm,col_i); it; ++it)
		x(it.row()) = it.value();

	return x;
}
