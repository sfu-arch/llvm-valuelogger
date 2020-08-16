add_library(value-inst
    ValueLogger.cpp
)

set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
add_library(value-lib MODULE
    ValueLogger.cpp
)
