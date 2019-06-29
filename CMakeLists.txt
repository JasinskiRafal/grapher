project(grapher)
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
add_compile_options(-pedantic -Werror)

# Sub-projects in this project:
# csvparse - deals with taking in CSV files and translating
#            them to C++ data structures
# cli_     - runs the command line interface to the user
# driver   - the end executable that makes use of all of the
#            other sub-projects' logic
# graph    - Builds graphs according to the directions from CLI 


# Use /include for linking the libs
# Add gnuplot interface header
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include  ${PROJECT_SOURCE_DIR}/libraries/gnuplot-iostream ${Boost_INCLUDE_DIRS})

# Our CSV parser is going to be a library
file(GLOB csv_SRC "src/csv/*.cpp")
add_library(csvparse ${csv_SRC})


# Our actual binary executable is 'driver'
file(GLOB exec_SRC "src/*.cpp")
add_executable(driver ${exec_SRC})
add_executable(debug_driver ${exec_SRC})

# Link ncurses since the CLI lib will need it
# gnuplot-iostream requires Boost
# The driver needs csvparse to take in csv files
target_link_libraries(driver csvparse ncurses ${Boost_LIBRARIES}) 
target_link_libraries(debug_driver csvparse ncurses ${Boost_LIBRARIES}) 