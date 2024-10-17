cmake_minimum_required(VERSION 3.0)
project(aws)

add_library(aws SHARED src/aws.c)
target_include_directories(aws PUBLIC include)

install(TARGETS aws DESTINATION /usr/local/lib)
install(DIRECTORY include/ DESTINATION /usr/local/include)