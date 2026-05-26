# ==========================================
# 1. CONFIGURACIÓN INICIAL
# ==========================================
cmake_minimum_required(VERSION 3.10)
project(robotarm C CXX)


find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(dynamixel_sdk REQUIRED)

add_subdirectory(include/dynamixel_ros2)


target_include_directories(dynamixel_ros2_lib PUBLIC 
    ${dynamixel_sdk_INCLUDE_DIRS}
    include/dynamixel_ros2/include
)

# ==========================================
# 2. BUSCAR TODOS LOS ARCHIVOS .C
# ==========================================
# file(GLOB ...) busca archivos en la carpeta actual.
# GLOB_RECURSE busca también dentro de subcarpetas si las tuvieras.
# Guarda la lista de todos los .c que encuentre en la variable FUENTES_C
file(GLOB_RECURSE FUENTES_CC "src/*.cpp")
file(GLOB_RECURSE FUENTES_C "src/*.c")
add_library(mifunciones STATIC ${FUENTES_C} ${FUENTES_CC})
# ==========================================
# 3. CREAR LA LIBRERÍA DE C
# ==========================================
# Ahora, en lugar de nombrar los archivos, le pasamos la variable ${FUENTES_C}


find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)


find_library(ROZUM_LIB NAMES servo_api PATHS ${CMAKE_CURRENT_SOURCE_DIR}/include/Rozum-Servo-Drives-API/c/build NO_DEFAULT_PATH)
target_link_libraries(mifunciones PRIVATE ${ROZUM_LIB} ${SDL2_TTF_LIBRARIES})


target_include_directories(mifunciones PUBLIC 
    include 
    include/dynamixel_ros2/include
    "/opt/ros/jazzy/include"  # <--- Forzamos la ruta del SDK de ROS aquí
    ${SDL2_TTF_INCLUDE_DIRS}
)


# ==========================================
# 4. CREAR EL EJECUTABLE DE C++ Y ENLAZAR
# ==========================================
add_executable(tester test/main_tester.cpp)
target_link_libraries(tester 
    mifunciones 
    dynamixel_ros2_lib
    ${SDL2_TTF_LIBRARIES}
)

target_include_directories(tester PRIVATE 
    include 
    include/dynamixel_ros2/include
    "/opt/ros/jazzy/include"  # <--- Y aquí también
)

ament_target_dependencies(tester rclcpp dynamixel_sdk)

install(TARGETS 
        tester
        dynamixel_ros2_lib
        DESTINATION lib/${PROJECT_NAME})

install(
  FILES include/Rozum-Servo-Drives-API/c/build/libservo_api.so
  DESTINATION lib/${PROJECT_NAME}
)

set_target_properties(tester PROPERTIES 
  INSTALL_RPATH "$ORIGIN"
)

ament_package() 