project(driver)
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)

add_library(csvparse src/csv_iterator.cpp src/csv_row.cpp src/csv_logdb.cpp)
add_executable(driver src/driver.cpp)
target_include_directories(csvparse PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_include_directories(driver PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(driver csvparse) 