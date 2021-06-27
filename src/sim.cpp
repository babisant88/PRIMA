/*
 * sim.cpp
 *
 *  Created on: Dec 9, 2018
 *      Author: babis
 */

#include "../hpp/sim.hpp"

#include <fstream>
#include <iostream>
#include <string>

#include <eigen3/Eigen/SparseCholesky>
#include "../hpp/aux.hpp"

using namespace Eigen;
using namespace std;

void load_wv(const char *, int, VectorXd&, VectorXd&);


MatrixXd rc_tr_sim_d( MatrixXd& C, MatrixXd& G, MatrixXd& B, MatrixXd& D, MatrixXd& u, VectorXd& x0, double h, VectorXd& t)
{
	MatrixXd y(D.cols(), t.rows());

	MatrixXd C_h = C/h;

	MatrixXd A = G + C_h;

	VectorXd v = x0;

	MatrixXd Dt = D.transpose();

	LLT<MatrixXd> solver;

	solver.compute(A);

	for (int t_i=0; t_i<t.rows(); ++t_i)
	{
		VectorXd b = (B*u.col(t_i)) + C_h*v;

		v = solver.solve(b);

		y.col(t_i) = Dt*v;
	}

	return y;
}



MatrixXd rc_tr_sim_sp( SpMat& C, SpMat& G, SpMat& B, SpMat& D, MatrixXd& u, VectorXd& x0, double h, VectorXd& t)
{
	MatrixXd y(D.cols(), t.rows());

	SpMat C_h = C/h;

	SpMat A = G + C_h;

	VectorXd v = x0;

	SpMat Dt = D.transpose();

	SimplicialLLT<SpMat> solver;

	solver.compute(A);

	for (int t_i=0; t_i<t.rows(); ++t_i)
	{
		VectorXd b = (B*u.col(t_i)) + C_h*v;

		v = solver.solve(b);

		y.col(t_i) = Dt*v;
	}

	return y;
}



void load_excitation(int sel, VectorXd& t, VectorXd& u)
{
	VectorXd e(t.rows());

	string filename;

	if (sel == 1)
	{
		filename = "wv1.dat";
		load_wv(filename.c_str(), sel, t, e);
	}
	else if (sel == 2)
	{
		filename = "wv2.dat";
		load_wv(filename.c_str(), sel, t, e);
	}
	else
	{
		filename = "wv3.dat";
		load_wv(filename.c_str(), sel, t, e);
	}

	#if dbg
		cout << "t = " << endl;
		printVectorXd(t);

		cout << "e = " << endl;
		printVectorXd(e);
	#endif

	for(int col_i=0; col_i<u.cols(); col_i++)
		u.col(col_i) = e;
}



void load_wv(const char * filename, int sel, VectorXd& t, VectorXd& e)
{
	ifstream myfile (filename);
	string line;

	int i=0;

	if (myfile.is_open())
	{
		while(getline(myfile, line))
		{
			char * pch;
			pch = strtok((char *)line.c_str(), " " );

			t(i) = atof(pch);
			pch = strtok(NULL, " ");
			e(i) = atof( pch );
			pch = strtok (NULL, " ");

			if(pch != NULL)
			{
				cout << "unable to load wv" << sel << ".dat...wrong file format" << endl;
				exit(-1);
			}

			i++;
		}

		myfile.close();
	}
	else
		cout << "Unable to open file" << "wv1.dat" << endl;
}
