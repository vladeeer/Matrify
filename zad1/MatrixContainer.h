#pragma once

#include<string>
#include"Matrix.h"

class MatrixContainer
{
public:
	// Constructors/Destructors
	MatrixContainer(int height, int width,
		std::string name);
	MatrixContainer(const Matrix<double>& matrix,
		std::string name);
	MatrixContainer(double* arr, int height, int width,
		std::string name);
	~MatrixContainer();

public:
	// Variables
	Matrix<double>* matrix;
	std::string name;
};

