add_library(common
    Common.cpp
)

set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
add_library(common-lib MODULE
    Common.cpp
)
