#include "Utility.h"
#include <numeric>

void Thermo::Normalize(std::span<double> x)
{
	const double s = std::accumulate(x.begin(),x.end(),0.0);
	if (s == 0.0) return; 
	std::transform(x.begin(), x.end(), x.begin(), [s](auto& a) {return a / s; });
}

double Thermo::DotProd(const std::span<const double> a, const std::span<const double> b)
{
	return std::inner_product(std::begin(a), std::end(a), std::begin(b),0.0);
}

bool Thermo::IsConverged(const std::span<const double> a, const std::span<const double> b, const double tol)
{
	for (int k = 0; k < a.size(); ++k)
		if (std::abs(a[k] - b[k]) > tol) return false;

	return true;
}
