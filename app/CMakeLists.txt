add_executable(logs_composition main.cpp)

if(BUILD_SHARED_LIBS AND TARGET logger)
    target_link_libraries(logs_composition PRIVATE logger)
elseif(BUILD_STATIC_LIBS AND TARGET logger_static)
    target_link_libraries(logs_composition PRIVATE logger_static)
else()
    message(FATAL_ERROR "No logger library available for linking")
endif()

install(TARGETS logs_composition
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)