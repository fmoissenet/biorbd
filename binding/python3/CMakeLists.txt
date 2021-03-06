# Include python
find_package(PYTHON3 REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})
find_package(Numpy REQUIRED)
if(WIN32)
	link_directories(${Python3_RUNTIME_LIBRARY_DIRS}/libs)
endif()

# Add swig module
set(CMAKE_SWIG_FLAGS "")
set(I_SOURCE_FILE ${PROJECT_NAME}_python.i)
set_property(SOURCE ${I_SOURCE_FILE} PROPERTY CPLUSPLUS ON)
SWIG_ADD_LIBRARY(${PROJECT_NAME} TYPE MODULE LANGUAGE python SOURCES ${I_SOURCE_FILE})
swig_link_libraries(${PROJECT_NAME} ${PROJECT_NAME} ${PYTHON_LIBRARIES} ${NUMPY_LIBRARIES})

# Add headers
target_include_directories(_${PROJECT_NAME} PRIVATE ${RBDL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR} ${DLIB_INCLUDE_DIR} ${NUMPY_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

SET_TARGET_PROPERTIES(_${PROJECT_NAME} PROPERTIES INSTALL_RPATH ${${PROJECT_NAME}_BIN_FOLDER})
SET_TARGET_PROPERTIES(_${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# Install to the proper directories
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.py 
        DESTINATION ${Python3_SITELIB}/${PROJECT_NAME}
        RENAME __init__.py)

# For some reason, I could not install the library directly into subfolder
# Some day this should be fixed...
INSTALL(TARGETS _${PROJECT_NAME}
		DESTINATION ${Python3_SITELIB})  

