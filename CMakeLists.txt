
cmake_minimum_required(VERSION 3.20)

project(MedSpase LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# C++ core library
add_library(medspase_lib STATIC
    project/database/Database.cpp

    project/models/User.cpp
    project/models/Patient.cpp
    project/models/MedicalRecord.cpp
    project/models/Analysis.cpp
    project/models/DoctorVisit.cpp
    project/models/MedicalCard.cpp
    project/models/Medication.cpp

    project/repositories/UserRepository.cpp
    project/repositories/PatientRepository.cpp
    project/repositories/AnalysisRepository.cpp
    project/repositories/DoctorVisitRepository.cpp
    project/repositories/MedicationRepository.cpp

    project/utils/DateUtils.cpp
    project/utils/ValidationUtils.cpp

    project/external/sqlite/sqlite3.c
)

target_include_directories(medspase_lib PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/project"
)

set_target_properties(medspase_lib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# Console application
add_executable(medspase_console
    project/main.cpp
)

target_link_libraries(medspase_console PRIVATE
    medspase_lib
)

set_target_properties(medspase_console PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_CURRENT_SOURCE_DIR}/project"
)

# Python bindings

find_package(Python
    COMPONENTS Interpreter Development.Module
    REQUIRED
)

execute_process(
    COMMAND "${Python_EXECUTABLE}" -m pybind11 --cmakedir
    OUTPUT_VARIABLE PYBIND11_CMAKE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    COMMAND_ERROR_IS_FATAL ANY
)

find_package(pybind11 CONFIG REQUIRED
    PATHS "${PYBIND11_CMAKE_DIR}"
    NO_DEFAULT_PATH
)

pybind11_add_module(medspase_core
    api/native/bindings.cpp
)

target_link_libraries(medspase_core PRIVATE
    medspase_lib
)

set_target_properties(medspase_core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/api"
)
