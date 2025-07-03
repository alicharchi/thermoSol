#pragma once
#include <numeric>
#include <algorithm>
#include "Species.h"

namespace Thermo
{			
	void Normalize(double* const x, size_t n);
	
	double DotProd(const double* const a, const double* const b, size_t n);
		
	bool IsConverged(const double* const a, const double* const b, const double tol, size_t n);
}