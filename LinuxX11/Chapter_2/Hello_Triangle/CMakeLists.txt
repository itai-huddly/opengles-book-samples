add_executable(Hello_Triangle Hello_Triangle.c)

target_include_directories(Hello_Triangle PUBLIC 
    "${PROJECT_SOURCE_DIR}/Common"
)