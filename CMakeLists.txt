project(grapher)
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
find_package(PythonLibs 3.5)

# Sub-projects in this project:
# csvparse - deals with taking in CSV files and translating
#            them to C++ data structures
# cli_     - runs the command line interface to the user
# driver   - the end executable that makes use of all of the
#            other sub-projects' logic
# graph    - Builds graphs according to the directions from CLI 

# Our CSV parser is going to be a library
file(GLOB csv_SRC "src/csv/*.cpp")
add_library(csvparse ${csv_SRC})

# Our actual binary executable is 'driver'
file(GLOB exec_SRC "src/*.cpp")
add_executable(driver ${exec_SRC})
add_executable(debug_driver ${exec_SRC})

# Use /include for linking the libs
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
# Add on matplotlib interface headers
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libraries/matplotlib-cpp)

# TODO Fix odd numpy inclusion
target_include_directories(driver PRIVATE /usr/lib/python3.7/site-packages/numpy/core/include)
target_include_directories(driver PRIVATE ${PYTHON_INCLUDE_DIRS})

# Link ncurses since the CLI lib will need it
target_link_libraries(driver csvparse ncurses ${PYTHON_LIBRARIES}) 

target_include_directories(debug_driver PRIVATE /usr/lib/python3.7/site-packages/numpy/core/include)
target_include_directories(debug_driver PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(debug_driver csvparse ncurses ${PYTHON_LIBRARIES}) 
