#include "SpeciesFactory.h"

#include <fstream>
#include <iostream>
#include <json.h>

using json = nlohmann::json;

#include "VaporPressureModel.h"

std::unique_ptr<Species> SpeciesFactory::MakeSpecies
(
	const std::string& speciesPath,
	std::initializer_list<std::string> selectedSpecies
)
{
	std::ifstream file(speciesPath);
	json data = json::parse(file);

	return std::make_unique<Species>(data, selectedSpecies);
}
