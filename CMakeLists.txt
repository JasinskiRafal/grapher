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

# Modules within this project:
# csv      - deals with taking in CSV files and translating
#            them to C++ data structures
# cli      - runs the command line interface to the user
# graph    - Builds graphs according to the directions from CLI 
# driver   - the end executable that makes use of all of the
#            other sub-projects' logic

set(csv_DIR "src/csv")
file(GLOB csv_SRC "${csv_DIR}/*.cpp")
# Our CSV parser is going to be a library
add_library(csvparse ${csv_SRC})

set(cli_DIR "src/cli")
file(GLOB cli_SRC "${cli_DIR}/*.cpp")

set(graph_DIR "src/graph")
file(GLOB graph_SRC "${graph_DIR}/*.cpp")

file(GLOB main_SRC "src/main.cpp")

set(driver_SRC ${cli_SRC} ${graph_SRC} ${main_SRC})
set(driver_include_dirs ${csv_DIR} ${cli_DIR} ${graph_DIR})

### Doxygen Logic ####
find_package(Doxygen)
# Check if we can make doxygen output
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${PROJECT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Building doxygen...")

    add_custom_target(doxygen ALL
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
                      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                      COMMENT "Generating API documentation with Doxygen"
                      VERBATIM)
endif (DOXYGEN_FOUND)

# Add gnuplot interface header
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libraries/gnuplot-iostream)
include_directories(BEFORE ${Boost_INCLUDE_DIRS})
include_directories(BEFORE ${driver_include_dirs})

add_executable(driver ${driver_SRC})
add_executable(debug_driver ${driver_SRC})

target_compile_options(driver PUBLIC -O2)
target_compile_options(debug_driver PUBLIC -g)

# Link ncurses since the CLI lib will need it
# gnuplot-iostream requires Boost
# The driver needs csvparse to take in csv files
set(driver_LIBS csvparse ncurses ${Boost_LIBRARIES} Threads::Threads)

target_link_libraries(driver ${driver_LIBS}) 
target_link_libraries(debug_driver ${driver_LIBS}) 