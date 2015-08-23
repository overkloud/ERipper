cmake_minimum_required(VERSION 3.0)
project ("ScrapWeb")


SET(CMAKE_CXX_FLAGS "-std=c++11")
include_directories(/usr/include/libxml2/)

#add_library(curl)
#add_library(xml2)

add_executable(scrap main.cpp
					 xmlutil.cpp
                     util.cpp)
target_link_libraries(scrap curl xml2)
install(TARGETS scrap DESTINATION bin)