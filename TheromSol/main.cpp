#include <iostream>
#include <string>
#include <fstream>

#include "Species.h"
#include "NRTL.h"
#include "IdealGasMixtureModel.h"
#include "GammaPhiVLE.h"

#include "SpeciesFactory.h"

using namespace std;

template <typename T>
void MakeTxyPlot(T& vle)
{
	const int n = 100;
	const double dx = (double)1.0 / n;
	double x[2], y[2];

	ofstream myfile("out.csv", ios::out);
	myfile << "x" << "," << "Tb" << "," << "Td" << "," << "p" << "\n";

	for (const double c : {0.2, 0.4, 0.6, 0.8, 1.0})
	{
		
		const double p = 101325 * c * 4;
		vector<double> Tdew(n + 1, 0.0), Tbubl(n + 1, 0.0);

		{
			//Make bubble curve
			double x1 = 0;
			int i = 0;
			do
			{
				x[0] = x1; x[1] = 1 - x1;
				vle->BublT(p, x, Tbubl[i], y);

				i++;
				x1 = i * dx;
			} while (x1 <= 1.0);
		}

		{
			//Make dew curve
			double y1 = 0;
			int i = 0;
			do
			{
				y[0] = y1; y[1] = 1 - y1;
				vle->DewT(p, y, Tdew[i], x);

				i++;
				y1 = i * dx;
			} while (y1 <= 1.0);
		}

		double xi;
		for (int i = 0; i < n + 1; i++)
		{
			xi = i * dx;
			myfile << xi << "," << Tbubl[i] << "," << Tdew[i] << "," << p << "\n";
		}
	}

	myfile.close();
}

template <typename T>
void VLEExample(T& vle)
{
	const double p0 = 101.33*1000;
	const double T0 = 300;
	const double z0[2] = { 0.3, 0.7 };
		
	double x[2], y[2];	
	int r;

	double pB;
	r = vle->BublP(T0, z0, pB, y);
	cout << "BP r = " << r << "\n";
	cout << "   y = " << y[0] << " , " << y[1] << "\n";
	cout << "   p = " << pB << "\n";
	cout << "\n";

	double TB;
	r = vle->BublT(p0, z0, TB, y);
	cout << "BT r = " << r << "\n";
	cout << "   y = " << y[0] << " , " << y[1] << "\n";
	cout << "   T = " << TB << "\n";
	cout << "\n";

	double pD;
	r = vle->DewP(T0, z0, pD, x);
	cout << "DP r = " << r << "\n";
	cout << "   x = " << x[0] << " , " << x[1] << "\n";
	cout << "   p = " << pD << "\n";
	cout << "\n";

	double TD;
	r = vle->DewT(p0, z0, TD, x);
	cout << "DT r = " << r << "\n";
	cout << "   x = " << x[0] << " , " << x[1] << "\n";
	cout << "   T = " << TD << "\n";
	cout << "\n";

	double v;
	r = vle->Flash((TD+TB)/2, p0, z0, v, x, y);
	cout << "FL r = " << r << "\n";
	cout << "   v = " << v << "\n";
	cout << "   x = " << x[0] << " , " << x[1] << "\n";
	cout << "   y = " << y[0] << " , " << y[1] << "\n";		

}

int main()
{
	const auto species = SpeciesFactory::MakeSpecies("dataBank.json",{"water","acetone"});
	auto* const spPtr = species.get();

	const auto ac = make_unique<NRTL>(spPtr);
	const auto fg = make_unique<IdealGasMixtureModel>(spPtr);

	unique_ptr<VLE> vle = make_unique<GammaPhiVLE>(spPtr, ac.get(), fg.get());

	MakeTxyPlot(vle);
	VLEExample(vle);
	
	return 0;
}
