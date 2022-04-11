cmake_minimum_required(VERSION 3.18)  # CMake version check
project(flatland_nerf)
             
set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(Eigen3 3.4 REQUIRED)


set(CMAKE_CXX_FLAGS "-O3") #needed to make Eigen go brrr (faster)


set(SRC_FILES
    src/utils/CameraCal.cpp
    src/utils/CameraOps.cpp
    src/utils/Renderer.cpp
    src/utils/Camera.cpp
    src/utils/Ray.cpp
    src/utils/SceneObject.cpp
    src/utils/Light.cpp
    src/utils/CameraSimulator.cpp
    #add more cpp files here. Should be relative path from makefile
)

include_directories(
    #shouldn need to change anything. If you add another folder in include *eg include/utils 
    #in your code you should reference it relative to the include eg. #include "utils/file.h"
    include
    ${OpenCV_INCLUDE_DIRS}
    ${EIGEN_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    src/main.cpp
)


target_link_libraries(${PROJECT_NAME}
		${OpenCV_LIBS}
		Eigen3::Eigen
		)

