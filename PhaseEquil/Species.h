#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <list>
#include <tuple>
#include <map>
#include <memory>

#include "dllHeader.h"
#include "Specie.h"
#include "AntoineModel.h"
#include "Matrix.h"

class PHASEEQUIL_API Species
{
public:

	template <typename T, typename S>
	Species(const T& data, const S& selectedSpecies)
	{
		string spName, spFormula;
		
		_species.reserve(selectedSpecies.size());

		double A, B, C;
		int k = 0;
		for (const auto& sp : data["species"])
		{
			spName = sp["name"];

			const auto it = find(selectedSpecies.begin(), selectedSpecies.end(), spName);
			if (it == selectedSpecies.end())
				continue;
			
			spFormula = sp["formula"];

			const auto& antoine = sp["VaporpressureModels"]["antoine"];

			A = antoine["A"];
			B = antoine["B"];
			C = antoine["C"];

			_speciesMap[spName] = k;
			VaporPressureModel* const vapPressModel = new AntoineModel(A, B, C);
			_species.emplace_back(spName, spFormula, vapPressModel);

			++k;
		}

		const auto& binData = data["binaryData"];
		const auto& dgData = binData["NRTL"]["dg"]["pairs"];
		const auto& alphaData = binData["NRTL"]["alpha"]["pairs"];

		_NRTLbinarydGCoeffs.setSize(selectedSpecies.size(),1.0);
		_NRTLbinaryalphaCoeffs.setSize(selectedSpecies.size(),1.0);

		string p1Name, p2Name;
		int p1Id, p2Id;
		{
			double a;
			for (const auto& pair : dgData)
			{
				p1Name = pair["p1"];
				p1Name = pair["p2"];
				if (_speciesMap.count(p1Name) == 1 && _speciesMap.count(p2Name) == 1)
				{
					a = pair["a"];
					p1Id = _speciesMap[p1Name];
					p2Id = _speciesMap[p2Name];
					_NRTLbinarydGCoeffs(p1Id,p2Id) =  a;
				}
			}
		}

		{
			double alpha;
			for (const auto& pair : alphaData)
			{
				p1Name = pair["p1"];
				p1Name = pair["p2"];
				if (_speciesMap.count(p1Name) == 1 && _speciesMap.count(p2Name) == 1)
				{
					alpha = pair["alpha"];
					p1Id = _speciesMap[p1Name];
					p2Id = _speciesMap[p2Name];
					_NRTLbinaryalphaCoeffs(p1Id, p2Id) = alpha;
					_NRTLbinaryalphaCoeffs(p2Id, p1Id) = alpha;
				}
			}
		}
	}

	virtual ~Species() = default;	
	Species(const Species&) = delete;
	Species& operator=(const Species&) = delete;

	const Specie& GetSpecie(int k) const;
	int SpecieId(std::string name) const;
	const std::string& SpecieName(int k) const;
	int Size() const;

	double NRTLbinarydGCoeffs(int i,int j) const;
	double NRTLbinaryalphaCoeffs(int i, int j) const;

	const Matrix<double>& NRTLbinarydGCoeffs() const;
	const Matrix<double>& NRTLbinaryalphaCoeffs() const;

	void GetpSat(const double T, double* const outVec);
	void GetTSat(const double p, double* const outVec);

private:
	std::vector<Specie> _species;
	std::map<std::string, int> _speciesMap;
	Matrix<double> _NRTLbinarydGCoeffs;
	Matrix<double> _NRTLbinaryalphaCoeffs;
};

