/*
* CSCI203/803 Assignment 4
*
* Student name: Thomas Nieuwland
* Subject code: CSCI203
* Student number: 5110634
* Email ID: tn996
*/

/* REUSED FROM ASSIGNMENT 3 */

#include "matrix.hpp"
using namespace std;

Matrix::Matrix(){
	matrix = NULL;
	vertices = 0;
}

Matrix::~Matrix(){
	if (vertices > 0){
		for (int i = 0; i < vertices; i++){
			delete[] matrix[i];
		}
		delete[] matrix;
	}
}

bool Matrix::intialise(char filename[]){
	//Input file is loaded
	ifstream inf;
	inf.open(filename);
	//Check to make sure file exists
	if(!inf.good()){
		char exit;
		cerr << "File \"" << filename << "\" doesn't exist or cannot be opened!" << endl;
		cerr << "Press return to exit program" << endl;
		cin.get();
		return false;
	}

	//File exists, matrix is allocated memory, and data is loaded

	inf >> vertices;
	matrix = new float* [vertices];
	for (int i = 0; i < vertices; i++){
		matrix[i] = new float[vertices];
	}

	for(int x = 0; x < vertices; x++){
		for(int y = 0; y < vertices; y++){
			inf >> matrix[x][y];
		}
	}
	inf.close();
	return true;
}

bool Matrix::intialise(int vert){
	vertices = vert;
	matrix = new float* [vertices];
	for (int i = 0; i < vertices; i++){
		matrix[i] = new float[vertices];
	}
}

void Matrix::print(){
		cout << "Matrix size: " << vertices << "x" << vertices << endl;
	for(int x = 0; x < vertices; x++){
		for(int y = 0; y < vertices; y++){
			cout << matrix[x][y] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}
