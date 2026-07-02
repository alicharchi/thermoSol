#pragma once
#include <stdexcept>
#include <vector>
#include <cassert>
#include <algorithm>

template <typename Tx>
class Matrix
{
public:
	Matrix()
		:
		_m(0), 
		_n(0)		
	{		
	}

	Matrix(size_t m, size_t n)
		:_m(m), _n(n)
	{
		_data.resize(_n * _m);
	}

	Matrix(size_t m, size_t n, Tx f)
		:_m(m), _n(n)
	{
		_data.resize(_n * _m, f);
	}

	Matrix(const Matrix&) = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(Matrix&&) = default;

	Matrix& operator= (const Matrix& rhs){

		if (this == &rhs) return *this;

		if (nRows() != rhs.nRows() || nCols() != rhs.nCols()){
			throw std::runtime_error("Invlid matrix assigned");
		}

		std::copy(rhs.begin(), rhs.end(), this->begin());
		
		return *this;
	}

	~Matrix(){}

	const Tx& operator()(size_t i, size_t j) const{
		assert(i < _m && j < _n);
		return _data[i * _n + j];
	}

	Tx& operator()(size_t i, size_t j){
		assert(i < _m && j < _n);
		return _data[i * _n + j];
	}

	void setSize(size_t m, size_t n, Tx f = Tx()) 
	{ 
		_m = m; 
		_n = n; 
		_data.assign(_m * _n, f);
	}

	void setSizeSquare(size_t n, Tx f = Tx()) 
	{ 
		setSize(n, n, f); 
	}

	void fill(const Tx& f)
	{
		std::fill(_data.begin(), _data.end(), f);
	}	

	size_t nRows() const
	{
		return _m;
	}

	size_t nCols() const
	{
		return _n;
	}

	std::vector<Tx>::iterator begin()
	{
		return std::begin(_data);
	}

	std::vector<Tx>::iterator end()
	{
		return std::end(_data);
	}

	std::vector<Tx>::const_iterator begin() const 
	{ 
		return std::begin(_data); 
	}

	std::vector<Tx>::const_iterator end() const 
	{ 
		return std::end(_data); 
	}


private:
	size_t _m, _n;
	std::vector<Tx> _data;
};

