#include "AntoineModel.h"
#include <cmath>

AntoineModel::AntoineModel(const double A, const double B, const double C)
    :
    _A(A),_B(B),_C(C)
{
}

double AntoineModel::pSat(const double T)
{
    return exp(_A - _B / (T + _C));
}

double AntoineModel::TSat(const double p)
{
    return _B / (_A - log(p)) - _C;
}