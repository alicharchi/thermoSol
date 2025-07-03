#pragma once
#include "dllHeader.h"

struct PHASEEQUIL_API VLE
{
	VLE() = default;
	virtual ~VLE() = default;
	virtual int nSpecies() const = 0;
	virtual int BublP(const double T, const double* const x, double& p, double* const y) = 0;
	virtual int BublT(const double p, const double* const x, double& T, double* const y) = 0;
	virtual int DewP(const double T, const double* const y, double& p, double* const x) = 0;
	virtual int DewT(const double p, const double* const y, double& T, double* const x) = 0;
	virtual int Flash(const double T, const double p, const double* const z, double& v, double* const x, double* const y) = 0;

	virtual void SetMaxInnerIters(int n) = 0;
	virtual void SetMaxOuterIters(int n) = 0;
	virtual void SetInnerTol(double a) = 0;
	virtual void SetOuterTol(double a) = 0;
};


