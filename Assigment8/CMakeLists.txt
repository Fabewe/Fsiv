CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(Assigment)
set(CMAKE_CXX_STANDARD 11) # C++11,C++14,17,20
FIND_PACKAGE(OpenCV 	REQUIRED )
include_directories ("${OpenCV_INCLUDE_DIRS}")
#Required for uco thinstation. May not be required in your own pc
OPTION(THINSTATION	"Set to OFF at home" OFF)

IF(THINSTATION)
  SET(CMAKE_CXX_FLAGS " -D_GLIBCXX_USE_CXX11_ABI=0 -m32")
  SET(XRAND Xrandr)
  link_directories(/usr/local/X11R7/lib)
  MESSAGE( STATUS "USING THINSTATION OPTIONS" )
ENDIF()

LINK_LIBRARIES(${OpenCV_LIBS} ${XRAND})


add_executable(prog main.cpp)
add_executable(fran aug.cpp)
add_executable(calibration calibration.cpp)
add_executable(imagelist_creator imagelist_creator.cpp)

#cv::solvePnP 2Dc & 3Dg -> T || 3Dg X T ->3Dc  || (Rvec,Tvec)=(Rotation vector, Translation vector)

#cv::proyectPoint()
#cv::line()[dibujar en la imagen]