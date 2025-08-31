set(LOGGER_SOURCES
    Logger.cpp
    Logs_Composition.cpp
)

if(BUILD_SHARED_LIBS)
    add_library(logger SHARED ${LOGGER_SOURCES})
    set_target_properties(logger PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION}
    )
endif()

if(BUILD_STATIC_LIBS)
    add_library(logger_static STATIC ${LOGGER_SOURCES})
    set_target_properties(logger_static PROPERTIES
        OUTPUT_NAME logger
    )
endif()

foreach(target_name IN ITEMS logger logger_static)
    if(TARGET ${target_name})

        target_include_directories(${target_name}
            PUBLIC 
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
                $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
        
        target_compile_options(${target_name} PRIVATE
            -O3
        )
        
        target_link_libraries(${target_name} PRIVATE
            pthread
        )
        
        install(TARGETS ${target_name}
            EXPORT LoggerTargets
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
     
        install(DIRECTORY ../include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    endif()
endforeach()
