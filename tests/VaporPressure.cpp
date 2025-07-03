

#if defined(_WIN32)
	#include "pch.h"
#else
	#include "gmock/gmock.h"
#endif

#include <AntoineModel.h>

class VaporPressureTest : public testing::Test {
protected:
	VaporPressureTest()		
	{
		const double A = 21.2223;
		const double B = 2756.22;
		const double C = -45.09;

		model = new AntoineModel(A, B, C);
	}

	~VaporPressureTest()
	{
		delete model;
	}

	VaporPressureModel* model;
};

constexpr double tol = 1e-4;

TEST_F(VaporPressureTest, pSat) 
{
	const double T = 50 + 273.15;
	const double ps = model->pSat(T);

	const double ps_expected = 81632.1875;
	EXPECT_NEAR(ps, ps_expected,tol);
}

TEST_F(VaporPressureTest, TSat)
{
	const double ps = 101325;
	const double Ts = model->TSat(ps);	

	const double Ts_expected = 329.3474;
	EXPECT_NEAR(Ts, Ts_expected, tol);
}