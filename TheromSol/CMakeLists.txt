project(ThermoSol CXX)

include_directories("../PhaseEquil" "../PhaseEquilC")

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} PhaseEquil)

