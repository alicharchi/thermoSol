#include "Utility.h"

void Thermo::Normalize(double* const x, size_t n)
{
	double s = 0;
	for (int k = 0; k < n; ++k)
		s += x[k];

	for (int k = 0; k < n; ++k)
		x[k] /= s;
}

double Thermo::DotProd(const double* const a, const double* const b, size_t n)
{
	double s = 0;
	for (int k = 0; k < n; ++k)
		s += a[k] * b[k];

	return s;
}

bool Thermo::IsConverged(const double* const a, const double* const b, const double tol, size_t n)
{
	for (int k = 0; k < n; ++k)
		if (std::abs(a[k] - b[k]) > tol) return false;

	return true;
}
