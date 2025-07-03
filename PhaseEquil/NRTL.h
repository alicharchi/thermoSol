#pragma once
#include "ExcessGibbsEnergyModel.h"
#include "Species.h"
#include "Matrix.h"
#include "dllHeader.h"

using namespace std;

class PHASEEQUIL_API NRTL : public ExcessGibbsEnergyModel
{
public:
	NRTL(Species * const species);
	~NRTL();

	virtual void Gamma(const double* const x, const double T, double* const gamma) override;
	virtual double ExcessGibbsEnergy(const double* const x, const double T) override;

private:	
	void UpdateCoeffs(const double T);
	double TauGx(int i, const double* const x) const;
	double Gx(int k, const double* const x) const;

	Species* const _species;
	const int _n;
	Matrix<double> _tau;
	Matrix<double> _G;
};

