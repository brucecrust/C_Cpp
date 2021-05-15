if (NOT WIN32)
	find_package(Curses REQUIRED)
	include_directories(${CURSES_INCLUDE_DIR})
ENDIF()

file(GLOB src CONFIGURE_DEPENDS "*.h" "*.cpp")

add_executable(${PROJECT_NAME} ${src})

if (NOT WIN32)
	target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
ENDIF()

install(TARGETS ${PROJECT_NAME} DESTINATION ${INSTALL_DIR})
