#pragma once
#include "dllHeader.h"

struct PHASEEQUIL_API FugacityCoefficientModel
{
	FugacityCoefficientModel() = default;
	virtual ~FugacityCoefficientModel() = default;

	virtual void Phi(const double* const y, const double T, const double p, double* const phi) = 0;
};