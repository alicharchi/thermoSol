#pragma once
#include "VaporPressureModel.h"
#include "dllHeader.h"

class PHASEEQUIL_API AntoineModel : public VaporPressureModel
{
public:
	AntoineModel(const double A, const double B, const double C);

	virtual double pSat(const double T) override;
	virtual double TSat(const double p) override;

private:
	const double _A, _B, _C;
};

