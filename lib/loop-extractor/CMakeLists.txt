add_library(loopext
    LoopExtractor.cpp
)

set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
add_library(loopext-lib MODULE
    LoopExtractor.cpp
)
