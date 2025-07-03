#pragma once
#include <stdexcept>

template <typename Tx>
class Matrix
{
public:
	Matrix()
		:_m(0), _n(0)
	{		
	}

	Matrix(size_t m, size_t n)
		:_m(m), _n(n)
	{
		memAlocate();
	}

	Matrix(size_t m, size_t n, double f)
		:_m(m), _n(n)
	{
		memAlocate();
		fill(f);
	}

	Matrix(const Matrix&) = delete;

	Matrix& operator= (const Matrix& rhs){
		if (nRows() != rhs.nRows() || nCols() != rhs.nCols()){
			throw std::runtime_error("Invlid matrix assined");
		}

		for (size_t i = 0; i < _m; i++)
		{
			for (size_t j = 0; j < _n; j++)
				_data[i][j] = rhs(i, j);			
		}
	}

	virtual ~Matrix(){
		for (size_t i = 0; i < _m; i++)
			delete[] _data[i];
		delete[] _data;
	}

	const Tx& operator()(size_t i, size_t j) const{
		return _data[i][j];
	}

	Tx& operator()(size_t i, size_t j){
		return _data[i][j];
	}

	void setSize(size_t m, size_t n)
	{
		setSize(m, n, Tx());
	}

	void setSize(size_t m, size_t n, Tx f)
	{
		_m = m;
		_n = n;
		memAlocate();
		fill(f);
	}

	void setSize(size_t n)
	{
		setSize(n, Tx());
	}

	void setSize(size_t n, Tx f)
	{
		setSize(n, n, f);
	}

	void fill(const double f){
		for (size_t i = 0; i < _m; i++)
		{
			for (size_t j = 0; j < _n; j++)
			{
				_data[i][j] = f;
			}
		}
	}
	
	size_t nRows() const
	{
		return _m;
	}

	size_t nCols() const
	{
		return _n;
	}

private:
	size_t _m, _n;
	Tx** _data;

	void memAlocate()
	{
		_data = new double* [_m];
		for (size_t i = 0; i < _m; i++)
		{
			_data[i] = new double[_n];
		}
	}
};

