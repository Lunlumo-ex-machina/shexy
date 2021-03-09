include_directories(${Shexy_SOURCE_DIR}/include
					${UTIL_ARGS_DIR}/include
					${UTIL_COLOR_DIR}/include
				    ${UTIL_UTF8_DIR}/include)

link_directories(${UTIL_ARGS_DIR}/src
				 ${UTIL_COLOR_DIR}/src
				 ${UTIL_UTF8_DIR}/src)

add_executable(${RUNTIME_OUTPUT_NAME} shexy.c)

target_link_libraries(${RUNTIME_OUTPUT_NAME} Args Color Utf8)
