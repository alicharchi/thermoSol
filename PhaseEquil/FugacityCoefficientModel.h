#pragma once
#include "dllHeader.h"
#include <span>

struct PHASEEQUIL_API FugacityCoefficientModel
{
	FugacityCoefficientModel() = default;
	virtual ~FugacityCoefficientModel() = default;

	virtual void Phi(const std::span<const double> y, const double T, const double p, const std::span<double> phi) = 0;
};