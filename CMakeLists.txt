cmake_minimum_required(VERSION 3.19)
project(Shexy)

# set the variables
set(CMAKE_BUILD_TYPE None)
set(CMAKE_C_FLAGS "-O2")
find_program(LSB_RELEASE_EXEC lsb_release)
execute_process(COMMAND ${LSB_RELEASE_EXEC} -is
    OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (${LSB_RELEASE_ID_SHORT} STREQUAL "Arch") 
	set(CMAKE_INSTALL_PREFIX /usr)
	set(CMAKE_INSTALL_MANDIR /usr/share/man)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Shexy_BINARY_DIR}/bin)
set(RUNTIME_OUTPUT_NAME shexy)
set(IN_MAN ${Shexy_SOURCE_DIR}/shexy.1)
set(OUT_MAN ${Shexy_BINARY_DIR}/shexy.1.gz)
set(UTIL_DIR ${Shexy_SOURCE_DIR}/util)
set(UTIL_COLOR_DIR ${UTIL_DIR}/color)
set(UTIL_ARGS_DIR ${UTIL_DIR}/args)

# call others CMakeLists
add_subdirectory(src)
add_subdirectory(util)

add_custom_command(OUTPUT ${OUT_MAN} COMMAND gzip -c ${IN_MAN} > ${OUT_MAN} DEPENDS ${MAN})

# install target
install(TARGETS ${RUNTIME_OUTPUT_NAME} RUNTIME DESTINATION bin)
add_custom_target(gzip_man ALL DEPENDS ${OUT_MAN})
install(FILES ${OUT_MAN} DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)

# uninstall target
if(NOT TARGET uninstall)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

  add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

