if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup(TARGETS)
endif()

set(SRCS 
    Hello_Triangle.c 
    "${PROJECT_SOURCE_DIR}/Common/esShader.c"
    "${PROJECT_SOURCE_DIR}/Common/esTransform.c"
    "${PROJECT_SOURCE_DIR}/Common/esShapes.c"
    "${PROJECT_SOURCE_DIR}/Common/esUtil.c"
)

add_executable(Hello_Triangle ${SRCS})

target_include_directories(Hello_Triangle PUBLIC 
    "${PROJECT_SOURCE_DIR}/Common"
    "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(
    Hello_Triangle
    CONAN_PKG::mesa
)
