#pragma once
#include "dllHeader.h"
#include <memory>
#include <string>
#include <initializer_list>
#include "Species.h"

class PHASEEQUIL_API SpeciesFactory
{
public:
	static std::unique_ptr<Species> MakeSpecies
	(
		const std::string& speciesPath,
		std::initializer_list<std::string> selectedSpecies
	);

private:

};

