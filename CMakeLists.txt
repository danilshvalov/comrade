cmake_minimum_required(VERSION 3.25)
project(sioyek C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenGL REQUIRED)

find_package(Qt6 COMPONENTS Core Widgets Gui Network OpenGLWidgets REQUIRED)
find_package(SQLite3 REQUIRED)

set(TARGET_NAME sioyek)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/pdf_viewer/*.cpp ${CMAKE_SOURCE_DIR}/pdf_viewer/*.c)

add_executable(${TARGET_NAME} ${SRCS})
target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/pdf_viewer)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
target_include_directories(${TARGET_NAME} PRIVATE mupdf/include)
target_link_libraries(${TARGET_NAME} Qt6::Widgets Qt6::Core Qt6::Network Qt6::Gui Qt6::OpenGLWidgets SQLite3 OpenGL::GL)

target_link_libraries(${TARGET_NAME} -ldl -L${CMAKE_SOURCE_DIR}/mupdf/build/release -lmupdf -lmupdf-third -lmupdf-threads -lz)

set_target_properties(${TARGET_NAME} PROPERTIES
  MACOSX_BUNDLE TRUE
)

# # set(CMAKE_MACOSX_BUNDLE ON)
# # set(CMAKE_MACOSX_RPATH ON)
# install(TARGETS ${TARGET_NAME} BUNDLE DESTINATION . RUNTIME DESTINATION .)
# # set_source_files_properties(resources/icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
# set_target_properties(${TARGET_NAME}
#   PROPERTIES
#   MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
#   MACOSX_BUNDLE_INFO_STRING "${PROJECT_NAME} Copyright (c) 2019 Example@Example.com"
#   # MACOSX_BUNDLE_ICON_FILE Icon.icns
#   MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_NAME}"
#   MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
#   MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
#   MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
#   MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2019 Example@Example.com"
# )

target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address)
target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address)
