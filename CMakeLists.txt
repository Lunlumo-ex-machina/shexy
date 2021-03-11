cmake_minimum_required(VERSION 3.19)
project(Shexy)

# set the variables
set(CMAKE_BUILD_TYPE None)
set(CMAKE_C_FLAGS "-O2")
set(CMAKE_INSTALL_PREFIX /usr)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Shexy_BINARY_DIR}/bin)
set(RUNTIME_OUTPUT_NAME shexy)
set(UTIL_DIR ${Shexy_SOURCE_DIR}/util)
set(UTIL_COLOR_DIR ${UTIL_DIR}/color)
set(UTIL_ARGS_DIR ${UTIL_DIR}/args)

# call others CMakeLists
add_subdirectory(src)
add_subdirectory(util)

# install target
install(TARGETS ${RUNTIME_OUTPUT_NAME} RUNTIME DESTINATION bin)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

