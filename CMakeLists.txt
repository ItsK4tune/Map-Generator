cmake_minimum_required(VERSION 3.10)
project(MapGenerator)

set(CMAKE_CXX_STANDARD 17)

# Thêm thư mục header
include_directories(include)

# Thêm thư viện GLAD
add_library(glad STATIC src/glad.c)

# Tìm tất cả .cpp trong src và các thư mục con
file(GLOB_RECURSE SRC_FILES src/*.cpp)
list(REMOVE_ITEM SRC_FILES "${CMAKE_SOURCE_DIR}/src/glad.c")

# Thêm main.cpp riêng
set(SOURCES main.cpp ${SRC_FILES})

# Tạo executable
add_executable(MapGenerator ${SOURCES})

# Đường dẫn đến thư viện GLFW
link_directories("${CMAKE_SOURCE_DIR}/lib")

# Liên kết với các thư viện
target_link_libraries(MapGenerator glad glfw3 opengl32)

# Bắt buộc để tránh lỗi WinMain với MinGW
target_link_options(MapGenerator PRIVATE -mconsole)
