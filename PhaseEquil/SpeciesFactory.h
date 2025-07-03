#pragma once
#include "dllHeader.h"
#include <memory>
#include <string>
#include <initializer_list>
#include "Species.h"

using namespace std;

class PHASEEQUIL_API SpeciesFactory
{
public:
	static unique_ptr<Species> MakeSpecies
	(
		const string& speciesPath,
		initializer_list<string> selectedSpecies
	);

private:

};

