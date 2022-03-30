#include "matrix.h"

// Constructors/Destructors
template<typename T>
Matrix<T>::Matrix(const int height, const int width)
{
	this->height = height;
	this->width = width;
	this->size = width * height; 
	
	// Allocate dynamic memory
	// Data is stored in 1d array
	this->arr = new T[size];	 

	// Util, used in getCol
	this->tempCol = new T[height]; 
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& m) :Matrix<T>(m.getHeight(), m.getWidth()) 
{
	// Copy Constructor
	for (int i = 0; i < this->size; i++)
		this->arr[i] = m.getArr()[i];
} 

template<typename T>
Matrix<T>::~Matrix()
{
	delete[] this->arr;
	delete[] this->tempCol;
}

// Utilitary Functions
template<typename T>
std::string Matrix<T>::toStr(T a, unsigned int l) const
{
	// Converts a to string with length l
	std::string s = std::to_string(a);

	if (s.find(".") != std::string::npos)
	{
		while (s.back() == '0')
		{
			s.pop_back();
			if (s.back() == '.')
			{
				s.pop_back();
				break;
			}
		}
	}

	while (s.length() < l)
		s.append(" ");

	return s;
}

template<typename T>
std::string Matrix<T>::arrToStr(T* arr, int n, int printSize) const
{
	// Converts array arr with length n to a string with length n*printSize
	std::stringstream ss;
	for (int i = 0; i < n; i++)
		ss << toStr(arr[i], printSize) << ((i == n - 1) ? "" : " ");
	return ss.str();
}

template<typename T>
int Matrix<T>::determineStrLength(T a) const
{
	std::string s = std::to_string(a);
	int l = (int)s.length();

	if (s.find(".") != std::string::npos)
	{
		while (s[l - 1LL] == '0')
		{
			l--;
			if (s[l - 1LL] == '.')
			{
				l--;
				break;
			}
		}
	}
	return l;
}

template<typename T>
int Matrix<T>::determineMaxStrLength() const
{
	// Max number of character per element
	int maxPrintSize = 1;

	// Determine maxPrintSize
	for (int i = 0; i < this->size; i++)
		if (this->determineStrLength(this->arr[i]) > maxPrintSize)
			maxPrintSize = this->determineStrLength(this->arr[i]);

	return maxPrintSize;
}

// Accessors
template<typename T>
T& Matrix<T>::at(const int row, const int col)
{
	return this->arr[row * this->width + col];
}

template<typename T>
T Matrix<T>::get(const int row, const int col) const
{
	// Returns element by indexes
	return this->arr[row * this->width + col];
}

template<typename T>
T* Matrix<T>::getRow(const int row) const
{
	// Returns pointer to selected row
	return this->arr + ((long long)row * this->width);
}

template<typename T>
T* Matrix<T>::getCol(const int col) const
{
	// Returns pointer to a copy of selected column
	for (int i = 0; i < height; i++)
		tempCol[i] = arr[i * width + col];
	return tempCol;
}

template<typename T>
T* Matrix<T>::getArr() const
{
	return this->arr;
}

template<typename T>
int Matrix<T>::getHeight() const
{
	return this->height;
}

template<typename T>
int Matrix<T>::getWidth() const
{
	return this->width;
}

// Functions
template<typename T>
void Matrix<T>::initWithZeros()
{
	// Initialize as zero matrix
	for (int i = 0; i < size; i++)
		this->arr[i] = 0;
}

template<typename T>
void Matrix<T>::initWithOnes()
{
	// Initialize as a matrix of ones
	this->initWithZeros();
	if (this->height == this->width)
		for (int i = 0; i < this->height; i++)
			this->set(i, i, 1);
}

template<typename T>
void Matrix<T>::initWithArray(const T* arr)
{
	for (int i = 0; i < this->size; i++)
		this->arr[i] = arr[i];
}

template<typename T>
void Matrix<T>::initWithMatrix(const Matrix& m)
{
	// Reinitialize matrix with other matrix

	// Free dynamic memory
	delete[] this->arr;
	delete[] this->tempCol;

	// Initialize
	this->height = m.getHeight();
	this->width = m.getWidth();
	this->size = this->height * this->width;

	// Allocate dynamic memory
	this->arr = new T[size];
	this->tempCol = new T[height];

	// Copy array data
	this->initWithArray(m.getArr());
}

// Modifiers
template<typename T>
void Matrix<T>::set(const int row, const int col, const T val)
{
	this->at(row, col) = val;
}

template<typename T>
void Matrix<T>::setRow(const int row, T* arr)
{
	for (int i = 0; i < this->width; i++)
		this->at(row, i) = arr[i];
}

template<typename T>
void Matrix<T>::setCol(const int col, T* arr)
{
	for (int i = 0; i < this->height; i++)
		this->at(i, col) = arr[i];
}

template<typename T>
void Matrix<T>::map(T(*func)(T))
{
	//Requires a mapper function as an argument
	//That is going to be applied to every element
	for (int i = 0; i < this->size; i++)
		this->arr[i] = func(this->arr[i]);
}

template<typename T>
void Matrix<T>::add(const Matrix& m)
{
	// Matrix addition
	if (this->height == m.getHeight() && this->width == m.getWidth())
	{
		T* mArrPtr = m.getArr();

		for (int i = 0; i < size; i++)
			this->arr[i] += mArrPtr[i];
	}
}

template<typename T>
void Matrix<T>::sub(const Matrix& m)
{
	// Matrix subtraction
	if (this->height == m.getHeight() && this->width == m.getWidth())
	{
		T* mArrPtr = m.getArr();

		for (int i = 0; i < size; i++)
			this->arr[i] -= mArrPtr[i];
	}
}
	
template<typename T>
void Matrix<T>::dot(const Matrix<T>& m) 
{
	// Matrix dot product
	T* row;
	T* col;
	int mWidth = m.getWidth();
	
	// Temporary matrix to write results to
	Matrix<T> temp(this->height, mWidth);
	temp.initWithZeros();

	if (this->width == m.getHeight())
		for (int j=0; j<this->height; j++)
			for (int i = 0; i < mWidth; i++)
			{
				row = this->getRow(j);
				col = m.getCol(i);
				for (int k = 0; k < this->width; k++)
					temp.at(j, i) += row[k]*col[k];
			}

	// Reinitialize this matrix with temp
	this->initWithMatrix(temp);
}

template<typename T>
void Matrix<T>::mul(const int n)
{
	// Multiply by scalar
	for (int i = 0; i < size; i++)
		this->arr[i] *= n;
}

template<typename T>
void Matrix<T>::div(const int n)
{
	// Divide by scalar
	for (int i = 0; i < size; i++)
		this->arr[i] /= n;
}

template<typename T>
std::string Matrix<T>::string(const std::string linePrefix, const int printSize) const
{
	// Printable result
	std::stringstream ss;		 

	int maxPrintSize;

	// Max number of character per element
	if (printSize == -1)
		maxPrintSize = this->determineMaxStrLength();
	else
		maxPrintSize = printSize;

	if (this->height <= 1)
	{
		if (this->size == 0)
		{	
			// Print empty matrix 
			// [ ]
			ss << "  [ ]" << "\n";
		}
		else
		{			
			// Print row matrix
			// [0 1 2]
			ss << "  [ ";								
			ss << this->arrToStr(this->arr, this->width, maxPrintSize);
			ss << " ]" << "\n";
		}
	}
	else
	{
		// Print rectangular matrix
		// / 0 1 \    
		// | 2 3 |
		// \ 4 5 /
		ss << linePrefix << "/ " << this->arrToStr(this->getRow(0), this->width, maxPrintSize)
			<< " \\" << "\n";
		for (int j = 1; j < this->height-1; j++)
		{
			ss << linePrefix << "| " << this->arrToStr(this->getRow(j), this->width, maxPrintSize)
				<< " |" << "\n";
		}
		ss << linePrefix << "\\ " << this->arrToStr(this->getRow(this->height-1), this->width, maxPrintSize)
			<< " /" << "\n";
	}
	return ss.str();
}

template<typename T>
std::string Matrix<T>::elementString(const int row, const int col, const int printSize) const
{
	if (printSize == -1)
		return this->toStr(this->get(row, col), this->determineMaxStrLength());
	else
		return this->toStr(this->get(row, col), printSize);
}

// Operators
template<typename T>
T& Matrix<T>::operator[](const int i) const
{
	// Access array elements by index
	return this->arr[i];
}

template<typename T>
Matrix<T> Matrix<T>::operator=(const Matrix<T>& m)
{
	this->initWithMatrix(m);
	return *this;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& m) const
{
	Matrix<T> res(*this);
	res.add(m);
	return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& m) const
{
	Matrix<T> res(*this);
	res.sub(m);
	return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& m) const
{
	Matrix<T> res(*this);
	res.dot(m);
	return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const int& n) const
{
	Matrix<T> res(*this);
	res.mul(n);
	return res;
}

template<typename T>
Matrix<T> Matrix<T>::operator/(const int& n) const
{
	Matrix<T> res(*this);
	res.div(n);
	return res;
}

template<typename T>
Matrix<T> operator*(const int& n, Matrix<T>& m)
{
	// Multiply scalar by matrix
	Matrix<T> res(m);
	res.mul(n);
	return res;
}

// Implicit template initialization
//template class Matrix<short>;
//template class Matrix<int>;
//template class Matrix<long>;
//template class Matrix<long long>;
//template class Matrix<float>;
template class Matrix<double>;