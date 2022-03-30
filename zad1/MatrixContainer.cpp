#include "MatrixContainer.h"

// Constructors/Destructors
MatrixContainer::MatrixContainer(int height, int width,
	std::string name)
{
	this->matrix = new Matrix<double>(height, width);
	this->matrix->initWithZeros();
	this->name = name;
}

MatrixContainer::MatrixContainer(const Matrix<double>& matrix, 
	std::string name) 
{
	this->matrix = new Matrix<double>(matrix);
	this->name = name;
}

MatrixContainer::MatrixContainer(double* arr, int height, 
	int width, std::string name)
	:MatrixContainer(height, width, name)
{
	this->matrix->initWithArray(arr);
}

MatrixContainer::MatrixContainer(MatrixContainer& m)
	: MatrixContainer(m.matrix->getHeight(), m.matrix->getWidth(), m.name)
{
	this->matrix->initWithMatrix(*m.matrix);
}

MatrixContainer::MatrixContainer()
{
	this->matrix = new Matrix<double>(1, 1);
	this->matrix->initWithZeros();
	this->name = "Empty";
}

MatrixContainer::~MatrixContainer()
{
	delete this->matrix;
}

