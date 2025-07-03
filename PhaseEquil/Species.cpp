#include "Species.h"

#include <algorithm>

const Specie& Species::GetSpecie(int k) const
{
    return _species.at(k);
}

int Species::SpecieId(std::string name) const
{
    return _speciesMap.at(name);
}

const std::string& Species::SpecieName(int k) const
{
    return _species.at(k).Name();
}

int Species::Size() const
{
    return _species.size();
}

double Species::NRTLbinarydGCoeffs(int i, int j) const
{
    return _NRTLbinarydGCoeffs(i,j);
}

double Species::NRTLbinaryalphaCoeffs(int i, int j) const
{
    return _NRTLbinaryalphaCoeffs(i,j);
}

const Matrix<double>& Species::NRTLbinarydGCoeffs() const
{
    return _NRTLbinarydGCoeffs;
}

const Matrix<double>& Species::NRTLbinaryalphaCoeffs() const
{
    return _NRTLbinaryalphaCoeffs;
}

void Species::GetpSat(const double T, double* const outVec)
{
    for (int k = 0; k < Size(); ++k)
        outVec[k] = GetSpecie(k).VaporPressure()->pSat(T);
}

void Species::GetTSat(const double p, double* const outVec)
{
    for (int k = 0; k < Size(); ++k)
        outVec[k] = GetSpecie(k).VaporPressure()->TSat(p);
}
