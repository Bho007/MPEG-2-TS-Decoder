project(TSPayloadSections)

set(SOURCE_FILES
        ProgramAssociationSection.cpp
        ProgramMapSection.cpp)

set(HEADER_FILES
        TSPayloadSections.h
        ProgramAssociationSection.h
        ProgramMapSection.h
        )

add_library(TSPayloadSections ${SOURCE_FILES} ${HEADER_FILES})

#adding profiler required debug info
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gstabs+")