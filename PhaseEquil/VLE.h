#pragma once
#include "dllHeader.h"
#include <span>

struct PHASEEQUIL_API VLE
{
	VLE() = default;
	virtual ~VLE() = default;
	virtual int nSpecies() const = 0;
	virtual int BublP(const double T, const std::span<const double> x, double& p, std::span<double> y) = 0;
	virtual int BublT(const double p, const std::span<const double> x, double& T, std::span<double> y) = 0;
	virtual int DewP(const double T, const std::span<const double> y, double& p, std::span<double> x) = 0;
	virtual int DewT(const double p, const std::span<const double> y, double& T, std::span<double> x) = 0;
	virtual int Flash(const double T, const double p, const std::span<const double> z, double& v, std::span<double> x, std::span<double> y) = 0;

	virtual void SetMaxInnerIters(int n) = 0;
	virtual void SetMaxOuterIters(int n) = 0;
	virtual void SetInnerTol(double a) = 0;
	virtual void SetOuterTol(double a) = 0;
};


