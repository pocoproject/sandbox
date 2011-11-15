set(LIBNAME "PocoDataPgSQL")
set(LIBNAMED "${LIBNAME}d")

set(SRCS "")
aux_source_directory(src SRCS)

add_definitions(-DTHREADSAFE -DNO_TCL)

include_directories( include ${PGSQL_INCLUDE_DIR} ${PGSQL_INCLUDE_DIR}/postgresql )

#link_directories( )

add_library( ${LIBNAME} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAME} 
	PROPERTIES COMPILE_FLAGS ${RELEASE_CXX_FLAGS}
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAME} PocoData PocoFoundation ${PGSQL_LIBRARIES})

add_library( ${LIBNAMED} ${LIB_MODE} ${SRCS} )
set_target_properties( ${LIBNAMED}  
	PROPERTIES COMPILE_FLAGS "${DEBUG_CXX_FLAGS}" 
	VERSION ${SHARED_LIBRARY_VERSION} SOVERSION ${SHARED_LIBRARY_VERSION} )
target_link_libraries( ${LIBNAMED} PocoDatad PocoFoundationd ${PGSQL_LIBRARIES})

install(
    DIRECTORY include/Poco
    DESTINATION include
    PATTERN ".svn" EXCLUDE
    )
    
install(
    TARGETS ${LIBNAME} ${LIBNAMED}
    DESTINATION lib
    )

if (ENABLE_TESTS)
  add_subdirectory(testsuite)
endif ()
