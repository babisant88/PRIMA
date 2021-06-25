/*
 * load_input.cpp
 *
 *  Created on: Jun 12, 2018
 *      Author: babis
 */

#include "../hpp/load_input.hpp"

#include <iostream>
#include <fstream>
#include <cstring>

//#include "../hpp/aux.hpp"

#define dbg 0

using namespace std;


void load_input(char* argv[], SpMat& C, SpMat& G, SpMat& B, SpMat& D)
{
	load_sparse_matrix(argv[1], C);

	#if dbg
		cout << "The matrix C is of size " << C.rows() << "x" << C.cols() << endl;

		cout << "C = " << endl;
		printSpMatXd(C);
	#endif

	load_sparse_matrix(argv[2], G);

	#if dbg
		cout << "The matrix G is of size " << G.rows() << "x" << G.cols() << endl;

		cout << "G = " << endl;
		printSpMatXd(G);
	#endif

	load_sparse_matrix(argv[3], B);

	#if dbg
		cout << "The matrix B is of size " << B.rows() << "x" << B.cols() << endl;

		cout << "B = " << endl;
		printSpMatXd(B);
	#endif

	load_sparse_matrix(argv[4], D);

	#if dbg
		cout << "The matrix D is of size " << D.rows() << "x" << D.cols() << endl;

		cout << "D = " << endl;
		printSpMatXd(D);
	#endif
}



void load_sparse_matrix(char * filename, SpMat& X)
{
	string line;
	ifstream myfile ( filename );

	std::vector<T> triplets;

	if ( myfile.is_open() )
	{
		while( getline( myfile, line ) )
		{
			char * pch;
			pch = strtok( (char *)line.c_str(), " " );

			int i,j;
			double x;

			/* I assume that the indexing of row/col(s)
			 * starts from 1 (same as MATLAB indexing) */
			i = atoi(pch) - 1; // -1 to account for MATLAB-to-C indexing
			pch = strtok ( NULL, " " );
			j = atoi(pch) - 1; // -1 to account for MATLAB-to-C indexing
			pch = strtok ( NULL, " " );
			x = atof(pch);
			pch = strtok ( NULL, " " );

			if( pch != NULL )
			{
				cout << "wrong input file format for sparse matrices" << endl;
				exit(-1);
			}

			triplets.push_back(T(i,j,x));
		}

		myfile.close();
	}
	else
		cout << "Unable to open file" << filename << endl;

	X.setFromTriplets(triplets.begin(), triplets.end());
}
