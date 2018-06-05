ADD_LIBRARY(two-lib SHARED two-lib.c)

target_link_libraries(two-lib log)