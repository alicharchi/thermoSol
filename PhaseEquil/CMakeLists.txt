project(PhaseEquil CXX)

set(Public_Header_Files
	"AntoineModel.h"
	"Constants.h"
	"ExcessGibbsEnergyModel.h"
	"FugacityCoefficientModel.h"
	"GammaPhiVLE.h"
	"IdealGasMixtureModel.h"
	"Matrix.h"
	"NRTL.h"
	"Specie.h"
	"SpeciesFactory.h"
	"Species.h"
	"Utility.h"
	"VaporPressureModel.h"
	"VLE.h"
)

source_group("Header Files" FILES ${Public_Header_Files})

set(Source_Files
	"AntoineModel.cpp"
	"GammaPhiVLE.cpp"
	"IdealGasMixtureModel.cpp"
	"NRTL.cpp"
	"Specie.cpp"
	"Species.cpp"
	"SpeciesFactory.cpp"
	"Utility.cpp"
)

source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
${Public_Header_Files}
${Source_Files}
)

include_directories("${CMAKE_SOURCE_DIR}/Json")
include_directories("${CMAKE_SOURCE_DIR}/Json/include")
add_library(${PROJECT_NAME} SHARED ${Source_Files})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
