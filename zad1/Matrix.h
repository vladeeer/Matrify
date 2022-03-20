#pragma once
#include<iostream>
#include<sstream>
#include<string>

template<typename T>
class Matrix
{
public: 
	// Constructors/Destructors
	Matrix(const int height, const int width);
	Matrix(const Matrix<T>& m);
	~Matrix();

private: 
	// Variables
	int height;
	int width;
	int size;
	T* arr;
	T* tempCol;
 
	// Utilitary Functions
	std::string toStr(T a, unsigned int l) const;
	std::string arrToStr(T* arr, int n, int printSize) const;
	int determineStrLength(T a) const;
	int determineMaxStrLength() const;

public:  
	// Accessors
	T& at(const int row, const int col);
	T get(const int row, const int col) const;
	T* getRow(const int row) const;
	T* getCol(const int col) const;
	T* getArr() const;
	int getHeight() const;
	int getWidth() const;

	// Modifiers
	void set(const int row, const int col, const T val);
	void setRow(const int row, T* arr);
	void setCol(const int col, T* arr);

	// Functions
	void initWithZeros();
	void initWithOnes();
	void initWithArray(const T* arr);
	void initWithMatrix(const Matrix<T>& m);

	void map(T(*func)(T));

	void add(const Matrix<T>& m);
	void sub(const Matrix<T>& m);
	void dot(const Matrix<T>& m);
	void mul(const int n);
	void div(const int n);

	std::string string(const int printSize = -1) const;
	std::string elementString(const int row, const int col, const int printSize = -1) const;

public: 
	// Operators
	T& operator[](const int i) const;
	Matrix<T> operator=(const Matrix<T>& m);
	Matrix<T> operator+(const Matrix<T>& m) const;
	Matrix<T> operator-(const Matrix<T>& m) const;
	Matrix<T> operator*(const Matrix<T>& m) const;
	Matrix<T> operator*(const int& n) const;
	Matrix<T> operator/(const int& n) const;
};

template<typename T>
Matrix<T> operator*(const int& n, Matrix<T>& m);

