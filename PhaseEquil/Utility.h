#pragma once

#include <algorithm>
#include "Species.h"
#include <span>

namespace Thermo
{			
	void Normalize(std::span<double> x);
	
	double DotProd(const std::span<const double> a, const std::span<const double> b);
		
	bool IsConverged(const std::span<const double> a, const std::span<const double> b, const double tol);
}