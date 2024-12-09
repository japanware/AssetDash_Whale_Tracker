cmake_minimum_required(VERSION 3.30)
project(MomSexxer3000)

set(CMAKE_CXX_STANDARD 20)

add_executable(MomSexxer3000 main.cpp)

find_package(libcurl-simple-https CONFIG REQUIRED)
target_link_libraries(MomSexxer3000 PRIVATE libcurl-simple-https)

find_package(CURL CONFIG REQUIRED)
target_link_libraries(MomSexxer3000 PRIVATE CURL::libcurl)

find_package(unofficial-gumbo CONFIG REQUIRED)
target_link_libraries(MomSexxer3000 PRIVATE unofficial::gumbo::gumbo)