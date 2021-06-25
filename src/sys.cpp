/*
 * sys.cpp
 *
 *  Created on: Dec 3, 2020
 *      Author: babis
 */

#include <iostream>

#include "../hpp/sys.hpp"

sys::sys(MatrixXd& C, MatrixXd& G, MatrixXd& B, MatrixXd& D, int no_in_ports, int no_out_ports, int sys_size)
{
	this->B = B;
	this->D = D;

	this->no_in_ports = no_in_ports;
	this->no_out_ports = no_out_ports;
	this->sys_size = sys_size;

	/* init mem. for poles & residues */
	poles = new complex<double>[sys_size];

	res = new complex<double>*[no_in_ports*no_out_ports];

	for( int p_i=0; p_i < no_in_ports*no_out_ports; ++p_i )
		res[p_i] = NULL;

	/* -iGC = G^(-1)C */
	lltG.compute(G);

	MatrixXd iGC(sys_size, sys_size);

	#pragma omp parallel for schedule(dynamic)
	for( int i=0; i<sys_size; i++ )
		iGC.col(i) = lltG.solve( -C.col(i) );

	/* eig(iGC) */
	eig_iGC.compute(iGC);

#if dbg
	for( int eig_i=0; eig_i<sys_size; ++eig_i )
		std::cout << "eig[" << eig_i << "] = " << eig_iGC.eigenvalues()[eig_i] << std::endl;
#endif

	/* find poles (1/lambda_i) */
	for( int i=0; i<sys_size; ++i )
		poles[i] = conj( eig_iGC.eigenvalues()[i] ) / norm( eig_iGC.eigenvalues()[i] );

	iS = eig_iGC.eigenvectors().inverse();
}



sys::~sys()
{
	delete[] poles;

	for( int p_i=0; p_i < no_in_ports*no_out_ports; ++p_i )
		if ( res[p_i] != NULL )
			delete[] res[p_i];
}



complex<double> *sys::get_poles()
{
	return poles;
}



tf sys::get_tf(int inp_k, int out_l)
{
	tf sys_tf;

	sys_tf.poles = poles;

	if ( res[out_l*no_in_ports+inp_k] != NULL ) // the resisuals for Y(out_l,inp_k) already exists
	{
		sys_tf.res = res[out_l*no_in_ports+inp_k];
	}
	else
	{
		sys_tf.res = new complex<double>[sys_size];

		VectorXd w;
		VectorXcd v, mu;

		w = lltG.solve( B.col(inp_k) );

		mu = eig_iGC.eigenvectors().transpose() * D.col(out_l);

		v = iS * w;

		for ( int n=0; n<sys_size; ++n )
			sys_tf.res[n] = mu(n)*v(n);

		/* make a ref. to the residuals of Y(out_l,inp_k) */
		res[out_l*no_in_ports+inp_k] = sys_tf.res;
	}

	return sys_tf;
}



int sys::get_no_in_ports()
{
	return no_in_ports;
}



int sys::get_no_out_ports()
{
	return no_out_ports;
}



int sys::get_sys_size()
{
	return sys_size;
}
