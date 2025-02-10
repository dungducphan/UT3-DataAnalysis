find_package(Geant4 REQUIRED ui_all vis_all multithreaded)
find_package(ROOT REQUIRED Core)

include(${Geant4_USE_FILE})
include(${ROOT_USE_FILE})

file(GLOB sources src/*.cc)
file(GLOB headers inc/*.hh)
file(GLOB GDMLFiles GDML/*.gdml)
file(GLOB MACROS "*.mac")

file(COPY ${MACROS} DESTINATION ${PROJECT_BINARY_DIR}/G4Sim)
file(COPY ${GDMLFiles} DESTINATION ${PROJECT_BINARY_DIR}/G4Sim)

include_directories(${PROJECT_SOURCE_DIR}/G4Sim/inc)
add_executable(BJTDosimeter sim.cc ${sources} ${headers})
target_link_libraries(BJTDosimeter ${Geant4_LIBRARIES})
target_link_libraries(BJTDosimeter ${ROOT_LIBRARIES})
