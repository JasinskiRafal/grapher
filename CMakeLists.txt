project(grapher)
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
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
file(GLOB cli_SRC "src/cli/*.cpp")
file(GLOB graph_SRC "src/graph/*.cpp")
file(GLOB main_SRC "src/main.cpp")
set(driver_SRC ${cli_SRC} ${graph_SRC} ${main_SRC})

add_executable(driver ${driver_SRC})
add_executable(debug_driver ${driver_SRC})

# Link ncurses since the CLI lib will need it
# gnuplot-iostream requires Boost
# The driver needs csvparse to take in csv files
set(driver_LIBS csvparse ncurses ${Boost_LIBRARIES} Threads::Threads)

target_link_libraries(driver ${driver_LIBS}) 
target_link_libraries(debug_driver ${driver_LIBS}) 