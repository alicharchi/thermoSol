#include "IdealGasMixtureModel.h"
#include <algorithm>

IdealGasMixtureModel::IdealGasMixtureModel(Species* const species)
    :
    _species(species)
{
}

void IdealGasMixtureModel::Phi(const std::span<const double> y, const double T, const double p, const std::span<double> phi)
{
    std::fill(phi.begin(), phi.end(), 1.0);
}
