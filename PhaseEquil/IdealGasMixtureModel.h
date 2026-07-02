#pragma once
#include "dllHeader.h"
#include "FugacityCoefficientModel.h"
#include "Species.h"

class PHASEEQUIL_API IdealGasMixtureModel : public FugacityCoefficientModel
{
public:	
	IdealGasMixtureModel(Species* const species);

	virtual void Phi(const std::span<const double> y, const double T, const double p, const std::span<double> phi) override;

private:
	Species* const _species;
};

