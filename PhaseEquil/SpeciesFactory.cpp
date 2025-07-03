#include "SpeciesFactory.h"

#include <fstream>
#include <iostream>
#include <json.h>
using json = nlohmann::json;

#include "VaporPressureModel.h"

unique_ptr<Species> SpeciesFactory::MakeSpecies
(
	const string& speciesPath,
	initializer_list<string> selectedSpecies
)
{
	std::ifstream file(speciesPath);
	json data = json::parse(file);

	return make_unique<Species>(data, selectedSpecies);
}
