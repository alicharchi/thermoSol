#pragma once
#include "dllHeader.h"

struct PHASEEQUIL_API VaporPressureModel
{
	VaporPressureModel() = default;
	virtual ~VaporPressureModel() = default;

	virtual double pSat(const double T) = 0;
	virtual double TSat(const double p) = 0;
};