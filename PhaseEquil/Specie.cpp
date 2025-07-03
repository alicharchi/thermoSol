#include "Specie.h"

Specie::Specie(string name, string formula, VaporPressureModel* const vaporPressureModel)
	:
	_name(name), _formula(formula), _vaporPressureModel(vaporPressureModel)
{
}

Specie::~Specie()
{
	delete _vaporPressureModel;
}

VaporPressureModel* const Specie::VaporPressure() const
{
	return _vaporPressureModel;
}

const string& Specie::Name() const
{
	return _name;
}

const string& Specie::Formula() const
{
	return _formula;
}
