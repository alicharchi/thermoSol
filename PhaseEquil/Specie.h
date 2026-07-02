#pragma once
#include <string>
#include "dllHeader.h"
#include "VaporPressureModel.h"

class PHASEEQUIL_API Specie
{
public:
	Specie(std::string name, std::string formula, VaporPressureModel* const vaporPressureModel);
	~Specie();
	Specie(const Specie&) = default;
	Specie& operator=(const Specie&) = default;

	VaporPressureModel * const VaporPressure() const;
	const std::string& Name() const;
	const std::string& Formula() const;

private:
	const std::string _name;
	const std::string _formula;
	VaporPressureModel* const _vaporPressureModel;
};
