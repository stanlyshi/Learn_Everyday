CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(shmq)
aux_source_directory(. DIR_SRCS)

# Add your header folder in here 
include_directories( . )

# All library the execution needed 

find_library(MATH m /usr/lib64) 
find_library(PTHREAD pthread /usr/lib64) 
find_library(CRYPT crypt /usr/lib64) 
find_library(DL crypt /usr/lib64) 
find_library(RT rt /usr/lib64) 
link_libraries(${MATH} ${PTHREAD} ${CRYPT} ${DL} ${RT})

# Definition 
add_definitions(-Os -MD -Wall -DRT_TEST=1  -Wno-strict-aliasing)

# Execute 
ADD_EXECUTABLE(shmq ${DIR_SRCS})
