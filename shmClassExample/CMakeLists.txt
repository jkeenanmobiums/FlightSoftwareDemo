#Jeremy Lim
#September 29, 2015
#Example of a basic Cmake file.
cmake_minimum_required(VERSION 2.8)

project(SharedMemExample)
add_executable(Pgrm1 Pgrm1.cpp)
add_executable(Pgrm2 Pgrm2.cpp)

#link in pthread so that semaphores work.
target_link_libraries(Pgrm1 pthread)
target_link_libraries(Pgrm2 pthread)
