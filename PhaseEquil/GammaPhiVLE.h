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

	virtual int BublP(const double T, const double* const x, double& p, double* const y) override;

	virtual int BublT(const double p, const double* const x, double& T, double* const y) override;

	virtual int DewP(const double T, const double* const y, double& p, double* const x) override;
  
	virtual int DewT(const double p, const double* const y, double& T, double* const x) override;

	virtual int Flash(const double T, const double p, const double* const z, double& v, double* const x, double* const y) override;

	virtual void SetMaxInnerIters(int n) override;
	virtual void SetMaxOuterIters(int n) override;

	virtual void SetInnerTol(double a) override;
	virtual void SetOuterTol(double a) override;

private:

	Species* const _species;
	ExcessGibbsEnergyModel* const _acModel;
	FugacityCoefficientModel* const _fgModel;		

	double *_pSat, *_TSat, *_gamma, *_Phi, *_gammaNew, *_K, *_xNew, *_yNew;

	int _maxInerIters, _maxOuterIters;
	double _innerTol, _outerTol;

	void PhiToOne();
	void GammaToOne();

	double PsatJx(const int j, const double* const x, const double p);
	double PsatJy(const int j, const double* const y, const double p);

	double CalcPx(const double* const x);
	double CalcPy(const double* const y);

	void CalcX(const double* const y, const double p, double* const x);
	void CalcY(const double* const x, const double p, double* const y);	
};

