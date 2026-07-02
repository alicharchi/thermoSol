#pragma once
#include "ExcessGibbsEnergyModel.h"
#include "Species.h"
#include "Matrix.h"
#include "dllHeader.h"

class PHASEEQUIL_API NRTL : public ExcessGibbsEnergyModel
{
public:
	NRTL(Species * const species);
	~NRTL();

	virtual void Gamma(std::span<const double> x, const double T, std::span<double> gamma) override;
	virtual double ExcessGibbsEnergy(std::span<const double> x, const double T) override;

private:	
	void UpdateCoeffs(const double T);
	double TauGx(int i, std::span<const double> x) const;
	double Gx(int k, std::span<const double> x) const;

	Species* const _species;
	const int _n;
	Matrix<double> _tau;
	Matrix<double> _G;
};

