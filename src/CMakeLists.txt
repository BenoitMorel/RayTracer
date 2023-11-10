
add_executable(raytracer main.cpp)
target_compile_options(raytracer PRIVATE -Wall -Wextra -g)
target_link_libraries(raytracer 
  PRIVATE
  -pthread -g)

