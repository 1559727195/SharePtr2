ADD_LIBRARY(pthread-lib SHARED pthread-lib.cpp)

target_link_libraries(pthread-lib log)