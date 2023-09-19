# nebula-demo

# 3.25 required for using FindVulkan
cmake_minimum_required(VERSION 3.25)

get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include(${FIPS_ROOT_DIR}/cmake/fips.cmake)

project(nebula-demo)
fips_setup()

add_subdirectory(code)
