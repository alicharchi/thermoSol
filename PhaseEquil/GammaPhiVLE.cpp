#include "GammaPhiVLE.h"
#include <exception>
#include <iostream>
#include <cmath>

#include "Utility.h"

GammaPhiVLE::GammaPhiVLE(
	Species* const species,
	ExcessGibbsEnergyModel* const acModel,
	FugacityCoefficientModel* const fgModel
)
	:
	_species(species),
	_acModel(acModel),
	_fgModel(fgModel)
{
	const int n = _species->Size();

	//Allocate memory for arrays
	_pSat.resize(n, 0);
	_TSat.resize(n, 0);
	_gamma.resize(n, 0);
	_Phi.resize(n, 0);
	_gammaNew.resize(n, 0);
	_K.resize(n, 0);
	_xNew.resize(n, 0);
	_yNew.resize(n, 0);

	//Set tols and max iters
	_maxInerIters = 20;
	_maxOuterIters = 20;
	_innerTol = 1e-4;
	_outerTol = 1e-3;
}

GammaPhiVLE::~GammaPhiVLE()
{

}

int GammaPhiVLE::nSpecies() const
{
	return _species->Size();
}

int GammaPhiVLE::BublP(const double T, const std::span<const double> x, double& p, std::span<double> y)
{
	try
	{
		const int n = _species->Size();
		double pNew, dp;
		
		PhiToOne();		
		_acModel->Gamma(x, T, _gamma);		
		_species->GetpSat(T, _pSat);
		p = CalcPx(x);
		int	iters = 1;
		do
		{
			CalcY(x, p, y);			
			_fgModel->Phi(y, T, p, _Phi);			
			pNew = CalcPx(x);
			dp = abs((pNew - p) / p);
			p = pNew;			
			iters += 1;
		} while (dp > _outerTol && iters < _maxOuterIters);

		return iters;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::BublT(const double p, const std::span<const double> x, double& T, std::span<double> y)
{
	try
	{
		const int n = _species->Size();
		double Tnew, dT;
		int iters = 1;
		const int j = 0;

		PhiToOne();

		_species->GetTSat(p, _TSat);				
		T = Thermo::DotProd(x, _TSat);
		_acModel->Gamma(x, T, _gamma);		

		do {
			_species->GetpSat(T, _pSat);
			CalcY(x, p, y);
			_fgModel->Phi(y, T, p, _Phi);
			_pSat[j] = PsatJx(j, x, p);
			Tnew = _species->GetSpecie(j).VaporPressure()->TSat(_pSat[j]);
			dT = abs((Tnew - T) / T);
			T = Tnew;
			iters += 1;
		} while (dT > _outerTol && iters < _maxOuterIters);

		return iters;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::DewP(const double T, const std::span<const double> y, double& p, std::span<double> x)
{
	try
	{
		bool converged = false;
		double dp, pNew;
		const int n = _species->Size();		

		PhiToOne();
		GammaToOne();
				
		_species->GetpSat(T, _pSat);
		p = CalcPy(y);
		CalcX(y, p, x);
		_acModel->Gamma(x, T, _gamma);
		p = CalcPy(y);

		int outerIters = 1;
		do {
			_fgModel->Phi(y, T, p, _Phi);
			int innerIters = 1;
			do {				
				CalcX(y, p, x);
				Thermo::Normalize(x);
				_acModel->Gamma(x, T, _gammaNew);
				converged = Thermo::IsConverged(_gamma, _gammaNew, _innerTol);				
				std::copy(_gammaNew.begin(), _gammaNew.end(), _gamma.begin());
				innerIters += 1;
			} while (!converged && innerIters < _maxInerIters);

			pNew = CalcPy(y);
			dp = abs((pNew - p) / p);
			p = pNew;

			outerIters += 1;

		} while (dp > _outerTol && outerIters < _maxOuterIters);

		return outerIters;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::DewT(const double p, const std::span<const double> y, double& T, std::span<double> x)
{
	try
	{
		bool converged = false;
		const int n = _species->Size();
		double Tnew, dT;
		const int j = 0;

		PhiToOne();
		GammaToOne();

		const auto& spJ = _species->GetSpecie(j);

		_species->GetTSat(p, _TSat);
		T = Thermo::DotProd(y, _TSat);
		_species->GetpSat(T, _pSat);
		_pSat[j] = PsatJy(j, y, p);
		
		T = spJ.VaporPressure()->TSat(_pSat[j]);
		_species->GetpSat(T, _pSat);
		CalcX(y, p, x);
		_acModel->Gamma(x, T, _gamma);		
		_pSat[j] = PsatJy(j, y, p);
		T = spJ.VaporPressure()->TSat(_pSat[j]);

		int outerIters = 1;
		do {
			_species->GetpSat(T, _pSat);
			_fgModel->Phi(y, T, p, _Phi);
			int innerIters = 1;
			do {
				CalcX(y, p, x);
				Thermo::Normalize(x);
				_acModel->Gamma(x, T, _gammaNew);
				converged = Thermo::IsConverged(_gamma, _gammaNew, _innerTol);				
				std::copy(_gammaNew.begin(), _gammaNew.end(), _gamma.begin());
				innerIters += 1;
			} while (!converged && innerIters < _maxInerIters);

			_pSat[j] = PsatJy(j, y, p);
			Tnew = spJ.VaporPressure()->TSat(_pSat[j]);
			dT = abs((Tnew - T) / T);
			T = Tnew;
			outerIters += 1;

		} while (dT > _outerTol && outerIters < _maxOuterIters);

		return outerIters;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::Flash(const double T, const double p, const std::span<const double> z, double& v, std::span<double> x, std::span<double> y)
{
	try
	{
		bool converged = false;
		double pDew, pBubl, vNew, dv, f, df;
		const int n = _species->Size();
		int i;

		PhiToOne();
		GammaToOne();

		DewP(T, z, pDew, x);
		BublP(T, z, pBubl, y);

		if (p < pDew)
		{
			std::cout << "p should be in the range of [" << pDew << "," << pBubl << "]\n";
			v = 1;
			return -1;
		}
		else if (p > pBubl)
		{
			std::cout << "p should be in the range of [" << pDew << "," << pBubl << "]\n";
			v = 0;
			return -1;
		}

		std::copy(z.begin(), z.end(), y.begin());		

		_acModel->Gamma(x, T, _gamma);
		_fgModel->Phi(y, T, p, _Phi);

		//initial guess for v
		v = (pBubl - p) / (pBubl - pDew);

		int outterIters = 1;
		do
		{
			_species->GetpSat(T, _pSat);
			
			for(i = 0;i<n;i++)
				_K[i] = (_gamma[i] * _pSat[i]) / (_Phi[i] * p);

			int innerIters = 1;
			do
			{				
				f = 0; df = 0;
				for (i = 0; i < n; i++)
				{
					f += z[i] * (_K[i] - 1) / (1 + v * (_K[i] - 1));
					df -= z[i] * pow(_K[i] - 1, 2) / pow(1 + v * (_K[i] - 1), 2);					
				}
				vNew = v - f / df;
				dv = abs(vNew - v);
				v = vNew;
				++innerIters;
			} while (dv > _innerTol && innerIters < _maxInerIters);
			
			for (i = 0; i < n; i++)
			{
				_xNew[i] = z[i] / (1 + v * (_K[i] - 1));
				_yNew[i] = x[i] * _K[i];			
			}

			converged = Thermo::IsConverged(x, _xNew, _outerTol) && Thermo::IsConverged(y, _yNew, _outerTol);
			
			std::copy(_xNew.begin(), _xNew.end(), x.begin());
			std::copy(_yNew.begin(), _yNew.end(), y.begin());
			

			_acModel->Gamma(x, T, _gamma);
			_fgModel->Phi(y, T, p, _Phi);

			++outterIters;
		} while (!converged && outterIters < _maxOuterIters);

		return outterIters;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

void GammaPhiVLE::PhiToOne()
{
	std::fill(_Phi.begin(), _Phi.end(), 1.0);
}

void  GammaPhiVLE::GammaToOne()
{
	std::fill(_gamma.begin(), _gamma.end(), 1.0);
}

double GammaPhiVLE::PsatJx(const int j, const std::span<const double> x, const double p)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += (x[k] * _gamma[k] * _Phi[k]) * (_pSat[k] / _pSat[j]);
	return p / s;
}

double GammaPhiVLE::PsatJy(const int j, const std::span<const double> y, const double p)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += (y[k] * _Phi[k] / _gamma[k]) * (_pSat[j] / _pSat[k]);
	return p * s;
}

double GammaPhiVLE::CalcPx(const std::span<const double> x)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += x[k] * _gamma[k] * _pSat[k] / _Phi[k];
	return s;
}

double GammaPhiVLE::CalcPy(const std::span<const double> y)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += y[k] * _Phi[k] / (_gamma[k] * _pSat[k]);

	s = 1.0 / s;
	return s;
}

void GammaPhiVLE::CalcX(const std::span<const double> y, const double p, const std::span<double> x)
{
	for (int k = 0; k < _species->Size(); ++k)
		x[k] = y[k] * _Phi[k] * p / (_gamma[k] * _pSat[k]);
}

void GammaPhiVLE::CalcY(const std::span<const double> x, const double p, const std::span<double> y)
{
	for (int k = 0; k < _species->Size(); ++k)
		y[k] = (x[k] * _gamma[k] * _pSat[k]) / (_Phi[k] * p);
}

void GammaPhiVLE::SetMaxInnerIters(int n)
{
	if (n <= 0)
		std::cerr << "Error: Can not set MaxInnerIters to " << n << "\n";
	else
		_maxInerIters = n;
}

void GammaPhiVLE::SetMaxOuterIters(int n)
{
	if (n <= 0)
		std::cerr << "Error: Can not set MaxOuterIters to " << n << "\n";
	else
		_maxOuterIters = n;
}

void GammaPhiVLE::SetInnerTol(double a)
{
	if (a <= 0)
		std::cerr << "Error: Can not set InnerTol to " << a << "\n";
	else
		_innerTol = a;
}

void GammaPhiVLE::SetOuterTol(double a)
{
	if (a <= 0)
		std::cerr << "Error: Can not set OuterTol to " << a << "\n";
	else
		_outerTol = a;
}
