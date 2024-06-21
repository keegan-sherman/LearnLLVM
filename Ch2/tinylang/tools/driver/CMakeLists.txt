set(LLVM_LINK_COMPONENTS
    Support
)

# add a new installable application which we name tinylang
# whose source is stored in Driver.cpp
add_tinylang_tool(tinylang
    Driver.cpp
)

# make sure we link against our own libraries.
target_link_libraries(tinylang
    PRIVATE
    tinylangBasic
)
