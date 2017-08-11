/*
* CSCI203/803 Assignment 4
*
* Student name: Thomas Nieuwland
* Subject code: CSCI203
* Student number: 5110634
* Email ID: tn996
*/

/* REUSED FROM ASSIGNMENT 3 */

#ifndef MATRIX_HPP_INCLUDED
#define MATRIX_HPP_INCLUDED

#include <fstream>
#include <iostream>

class Matrix{
public:
	float** matrix;
	int vertices;

	Matrix();
	~Matrix();

	bool intialise(char[]);
	bool intialise(int);
	void print();
};
#endif // MATRIX_HPP_INCLUDED
