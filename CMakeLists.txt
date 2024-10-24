cmake_minimum_required(VERSION 3.0)
project(aws)

find_package(LibXml2 REQUIRED)

add_library(aws SHARED src/aws.c)
target_include_directories(aws PUBLIC include ${LIBXML2_INCLUDE_DIR})
target_link_libraries(aws PUBLIC ${LIBXML2_LIBRARIES})

install(TARGETS aws DESTINATION /usr/local/lib)
install(DIRECTORY include/ DESTINATION /usr/local/include)