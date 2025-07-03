#pragma once
#include "dllHeader.h"

struct PHASEEQUIL_API ExcessGibbsEnergyModel
{
	ExcessGibbsEnergyModel() = default;
	virtual ~ ExcessGibbsEnergyModel() = default;

	virtual void Gamma(const double* const x, const double T, double* const gamma) = 0;
	virtual double ExcessGibbsEnergy(const double* const x, const double T) = 0;
};
