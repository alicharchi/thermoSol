#include "NRTL.h"
#include "Constants.h"
#include "Utility.h"

#include <cmath>

NRTL::NRTL(Species* const species)
	:
	_species(species),
	_n(_species->Size()),
	_tau(_n, _n, 1.0),
	_G(_n, _n, 1.0)
{	
}

NRTL::~NRTL()
{
}

void NRTL::UpdateCoeffs(const double T)
{
	const int n = _species->Size();
	
	const auto& dg = _species->NRTLbinarydGCoeffs();
	const auto& alpha = _species->NRTLbinaryalphaCoeffs();

	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			_tau(i,j) = dg(i,j) / (Constants::R * T);
			_G(i,j) = std::exp(-alpha(i,j) * _tau(i,j));
		}
	}
}

double NRTL::TauGx(int i, std::span<const double> x) const
{	
	double s = 0;
	for (int j = 0; j < _species->Size(); j++)
		s += _tau(j,i) * _G(j,i) * x[j];	
	
	return s;
}

double NRTL::Gx(int k, std::span<const double> x) const
{	
	double s = 0;
	for (int j = 0; j < _species->Size(); j++)
		s += _G(j,k) * x[j];
	
	return s;
}

void NRTL::Gamma(std::span<const double> x, const double T, std::span<double> gamma)
{
	UpdateCoeffs(T);
	const int n = _species->Size();

	double lnGamma;
	
	double xgs,gx;

	for (int i = 0; i < n; ++i)
	{		
		lnGamma = TauGx(i,x) / Gx(i,x);
		for (int j = 0; j < n; ++j)
		{
			gx = Gx(j, x);
			xgs = x[j] * _G(i,j) / gx;
			lnGamma += xgs * (_tau(i, j) - TauGx(j, x) / gx);
		}
		gamma[i] = std::exp(lnGamma);
	}	
}

double NRTL::ExcessGibbsEnergy(std::span<const double> x, const double T)
{	
	UpdateCoeffs(T);
	const int n = _species->Size();
	double gEbyRT = 0;
	
	for (int i = 0; i < n; i++)
		gEbyRT += x[i] * TauGx(i, x) / Gx(i, x);;

	return gEbyRT * Constants::R * T;

}
