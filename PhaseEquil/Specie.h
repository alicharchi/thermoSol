#pragma once
#include <string>
#include "dllHeader.h"
#include "VaporPressureModel.h"

using namespace std;

class PHASEEQUIL_API Specie
{
public:
	Specie(string name, string formula, VaporPressureModel* const vaporPressureModel);
	~Specie();
	Specie(const Specie&) = default;
	Specie& operator=(const Specie&) = default;

	VaporPressureModel * const VaporPressure() const;
	const string& Name() const;
	const string& Formula() const;

private:
	const string _name;
	const string _formula;
	VaporPressureModel* const _vaporPressureModel;
};
