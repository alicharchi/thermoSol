#include "IdealGasMixtureModel.h"
#include <algorithm>

IdealGasMixtureModel::IdealGasMixtureModel(Species* const species)
    :
    _species(species)
{
}

void IdealGasMixtureModel::Phi(const double* const y, const double T, const double p, double* const phi)
{
    for (int k = 0; k < _species->Size(); k++)
        phi[k] = 1.0;    
}
