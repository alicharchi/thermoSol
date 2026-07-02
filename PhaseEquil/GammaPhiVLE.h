#pragma once
#include "VLE.h"
#include "dllHeader.h"
#include "Species.h"
#include "ExcessGibbsEnergyModel.h"
#include "FugacityCoefficientModel.h"

class PHASEEQUIL_API GammaPhiVLE : public VLE
{
public:
	GammaPhiVLE
	(
		Species* const species,
		ExcessGibbsEnergyModel* const acModel,
		FugacityCoefficientModel* const fgModel
	);

	~GammaPhiVLE();

	virtual int nSpecies() const override;

	virtual int BublP(const double T, const std::span<const double> x, double& p, std::span<double> y) override;

	virtual int BublT(const double p, const std::span<const double> x, double& T, std::span<double> y) override;

	virtual int DewP(const double T, const std::span<const double> y, double& p, std::span<double> x) override;
  
	virtual int DewT(const double p, const std::span<const double> y, double& T, std::span<double> x) override;

	virtual int Flash(const double T, const double p, const std::span<const double> z, double& v, std::span<double> x, std::span<double> y) override;

	virtual void SetMaxInnerIters(int n) override;
	virtual void SetMaxOuterIters(int n) override;

	virtual void SetInnerTol(double a) override;
	virtual void SetOuterTol(double a) override;

private:

	Species* const _species;
	ExcessGibbsEnergyModel* const _acModel;
	FugacityCoefficientModel* const _fgModel;		

	std::vector<double> _pSat, _TSat, _gamma, _Phi, _gammaNew, _K, _xNew, _yNew;

	int _maxInerIters, _maxOuterIters;
	double _innerTol, _outerTol;

	void PhiToOne();
	void GammaToOne();

	double PsatJx(const int j, const std::span<const double> x, const double p);
	double PsatJy(const int j, const std::span<const double> y, const double p);

	double CalcPx(const std::span<const double> x);
	double CalcPy(const std::span<const double> y);

	void CalcX(const std::span<const double> y, const double p, const std::span<double> x);
	void CalcY(const std::span<const double> x, const double p, const std::span<double> y);
};

