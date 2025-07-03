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
	_pSat = new double[n];
	_TSat = new double[n];
	_gamma = new double[n];
	_Phi = new double[n];
	_gammaNew = new double[n];
	_K = new double[n];
	_xNew = new double[n];
	_yNew = new double[n];

	//Set tols and max iters
	_maxInerIters = 20;
	_maxOuterIters = 20;
	_innerTol = 1e-4;
	_outerTol = 1e-3;
}

GammaPhiVLE::~GammaPhiVLE()
{
	delete [] _pSat;
	delete [] _TSat;
	delete [] _gamma;
	delete [] _Phi;
	delete [] _gammaNew;
	delete [] _K;
	delete [] _xNew;
	delete [] _yNew;
}

int GammaPhiVLE::nSpecies() const
{
	return _species->Size();
}

int GammaPhiVLE::BublP(const double T, const double* const x, double& p, double* const y)
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
	catch (const exception& ex)
	{
		cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::BublT(const double p, const double* const x, double& T, double* const y)
{
	try
	{
		const int n = _species->Size();
		double Tnew, dT;
		int iters = 1;
		const int j = 0;

		PhiToOne();

		_species->GetTSat(p, _TSat);				
		T = Thermo::DotProd(x, _TSat, n);
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
	catch (const exception& ex)
	{
		cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::DewP(const double T, const double* const y, double& p, double* const x)
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
				Thermo::Normalize(x, n);
				_acModel->Gamma(x, T, _gammaNew);
				converged = Thermo::IsConverged(_gamma, _gammaNew, _innerTol, n);
				copy_n(_gammaNew,n, _gamma);			
				innerIters += 1;
			} while (!converged && innerIters < _maxInerIters);

			pNew = CalcPy(y);
			dp = abs((pNew - p) / p);
			p = pNew;

			outerIters += 1;

		} while (dp > _outerTol && outerIters < _maxOuterIters);

		return outerIters;
	}
	catch (const exception& ex)
	{
		cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::DewT(const double p, const double* const y, double& T, double* const x)
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
		T = Thermo::DotProd(y, _TSat, n);
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
				Thermo::Normalize(x,n);
				_acModel->Gamma(x, T, _gammaNew);
				converged = Thermo::IsConverged(_gamma, _gammaNew, _innerTol, n);
				copy_n(_gammaNew, n, _gamma);
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
	catch (const exception& ex)
	{
		cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

int GammaPhiVLE::Flash(const double T, const double p, const double* const z, double& v, double* const x, double* const y)
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
			cout << "p should be in the range of [" << pDew << "," << pBubl << "]\n";
			v = 1;
			return -1;
		}
		else if (p > pBubl)
		{
			cout << "p should be in the range of [" << pDew << "," << pBubl << "]\n";
			v = 0;
			return -1;
		}

		std::copy_n(z, n, y);
		std::copy_n(z, n, y);

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

			converged = Thermo::IsConverged(x, _xNew, _outerTol, n) && Thermo::IsConverged(y, _yNew, _outerTol, n);
			std::copy_n(_xNew, n, x);
			std::copy_n(_yNew, n, y);

			_acModel->Gamma(x, T, _gamma);
			_fgModel->Phi(y, T, p, _Phi);

			++outterIters;
		} while (!converged && outterIters < _maxOuterIters);

		return outterIters;
	}
	catch (const exception& ex)
	{
		cerr << "\nError in " << __FUNCTION__ << ex.what() << "\n";
		return -1;
	}
}

void GammaPhiVLE::PhiToOne()
{
	for (int k = 0; k < _species->Size(); k++)
		_Phi[k] = 1.0;
}

void  GammaPhiVLE::GammaToOne()
{
	for (int k = 0; k < _species->Size(); k++)
		_gamma[k] = 1.0;
}

double GammaPhiVLE::PsatJx(const int j, const double* const x, const double p)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += (x[k] * _gamma[k] * _Phi[k]) * (_pSat[k] / _pSat[j]);
	return p / s;
}

double GammaPhiVLE::PsatJy(const int j, const double* const y, const double p)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += (y[k] * _Phi[k] / _gamma[k]) * (_pSat[j] / _pSat[k]);
	return p * s;
}

double GammaPhiVLE::CalcPx(const double* const x)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += x[k] * _gamma[k] * _pSat[k] / _Phi[k];
	return s;
}

double GammaPhiVLE::CalcPy(const double* const y)
{
	double s = 0;
	for (int k = 0; k < _species->Size(); ++k)
		s += y[k] * _Phi[k] / (_gamma[k] * _pSat[k]);

	s = 1.0 / s;
	return s;
}

void GammaPhiVLE::CalcX(const double* const y, const double p, double* const x)
{
	for (int k = 0; k < _species->Size(); ++k)
		x[k] = y[k] * _Phi[k] * p / (_gamma[k] * _pSat[k]);
}

void GammaPhiVLE::CalcY(const double* const x, const double p, double* const y)
{
	for (int k = 0; k < _species->Size(); ++k)
		y[k] = (x[k] * _gamma[k] * _pSat[k]) / (_Phi[k] * p);
}

void GammaPhiVLE::SetMaxInnerIters(int n)
{
	if (n <= 0)
		cerr << "Error: Can not set MaxInnerIters to " << n << "\n";
	else
		_maxInerIters = n;
}

void GammaPhiVLE::SetMaxOuterIters(int n)
{
	if (n <= 0)
		cerr << "Error: Can not set MaxOuterIters to " << n << "\n";
	else
		_maxOuterIters = n;
}

void GammaPhiVLE::SetInnerTol(double a)
{
	if (a <= 0)
		cerr << "Error: Can not set InnerTol to " << a << "\n";
	else
		_innerTol = a;
}

void GammaPhiVLE::SetOuterTol(double a)
{
	if (a <= 0)
		cerr << "Error: Can not set OuterTol to " << a << "\n";
	else
		_outerTol = a;
}
