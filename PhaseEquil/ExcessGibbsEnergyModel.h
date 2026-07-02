#pragma once
#include "dllHeader.h"
#include <span>

struct PHASEEQUIL_API ExcessGibbsEnergyModel
{
	ExcessGibbsEnergyModel() = default;
	virtual ~ ExcessGibbsEnergyModel() = default;

	virtual void Gamma(std::span<const double> x, const double T, std::span<double> gamma) = 0;
	virtual double ExcessGibbsEnergy(std::span<const double> x, const double T) = 0;
};
