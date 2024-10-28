cmake_minimum_required(VERSION 3.16...3.27)

project(Simulation)

set(
  Simulation
  macros/prova01.mac
)

foreach(_script ${Simulation})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(sim sim.cc ${sources} ${headers})
target_link_libraries(sim ${Geant4_LIBRARIES})

file(GLOB MACRO_FILES
  "*.mac"
)
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

file(GLOB DATA_FILES
  "*.dat"
)
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_custom_target(Simulation DEPENDS sim)

option(GEANT4_V10 "Geant4 v10" OFF)
if(GEANT4_V10)
   message("-- Building Simulation using Geant4 v10")
   add_definitions(-DGEANT4_V10)
endif()

option(GEANT4_V11 "Geant4 v11" ON)
if(GEANT4_V11)
   message("-- Building Simulation using Geant4 v11")
   add_definitions(-DGEANT4_V11)
endif()