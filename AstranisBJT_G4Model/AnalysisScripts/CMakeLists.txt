find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

add_executable(DoseAnalysis DoseAnalysis.C)
target_link_libraries(DoseAnalysis ${ROOT_LIBRARIES})

add_executable(TrackAnalysis TrackAnalysis.C)
target_link_libraries(TrackAnalysis ${ROOT_LIBRARIES})

add_executable(Ana Ana.C)
target_link_libraries(Ana ${ROOT_LIBRARIES})
