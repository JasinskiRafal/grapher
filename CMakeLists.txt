project(grapher)
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
find_package(PythonLibs 3.5)

# Sub-projects in this project:
# csvparse - deals with taking in CSV files and translating
#            them to C++ data structures
# cli      - runs the command line interface to the user
# driver   - the end executable that makes use of all of the
#            other sub-projects' logic
# graph    - Builds graphs according to the directions from CLI 

# Our CSV parser is going to be a library
add_library(csvparse src/csv_parser.cpp src/csv_logdb.cpp)

# Our actual binary executable is 'driver'
add_executable(driver src/main.cpp src/cli_graph.cpp src/graph.cpp src/cli_pick_fields.cpp)

# Use /include for linking the libs
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
# Add on third party library
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libraries/matplotlib-cpp)

target_include_directories(driver PRIVATE ${PYTHON_INCLUDE_DIRS})
# Odd numpy inclusion
target_include_directories(driver PRIVATE ${PYTHON_LIBRARIES}/site-packages/numpy/core/include)

# Link ncurses since the CLI lib will need it
target_link_libraries(driver csvparse ncurses ${PYTHON_LIBRARIES}) 
