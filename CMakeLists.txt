cmake_minimum_required (VERSION 3.8)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endif()

project (google-domains-ddns)

find_package(fmt CONFIG REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

add_executable (google-domains-ddns "google-domains-ddns.cpp" "google-domains-ddns.h")
target_link_libraries(google-domains-ddns PRIVATE fmt::fmt fmt::fmt-header-only)
target_link_libraries(google-domains-ddns PRIVATE unofficial::curlpp::curlpp)
target_link_libraries(google-domains-ddns PRIVATE yaml-cpp)
