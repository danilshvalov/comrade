cmake_minimum_required(VERSION 3.25)
project(sioyek C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(OpenGL REQUIRED)

find_package(Qt6 COMPONENTS Core Widgets Gui Network OpenGLWidgets REQUIRED)
find_package(SQLite3 REQUIRED)

set(TARGET_NAME sioyek)

add_executable(${TARGET_NAME}
    pdf_viewer/synctex/synctex_parser.c
    pdf_viewer/synctex/synctex_parser_utils.c
    pdf_viewer/book.h
    pdf_viewer/config.h
    pdf_viewer/database.h
    pdf_viewer/document.h
    pdf_viewer/document_view.h
    pdf_viewer/fts_fuzzy_match.h
    pdf_viewer/rapidfuzz_amalgamated.hpp
    pdf_viewer/input.h
    pdf_viewer/main_widget.h
    pdf_viewer/pdf_renderer.h
    pdf_viewer/pdf_view_opengl_widget.h
    pdf_viewer/checksum.h
    pdf_viewer/new_file_checker.h
    pdf_viewer/coordinates.h
    pdf_viewer/sqlite3.h
    pdf_viewer/sqlite3ext.h
    pdf_viewer/ui.h
    pdf_viewer/path.h
    pdf_viewer/utf8.h
    pdf_viewer/utils.h
    pdf_viewer/utf8/checked.h
    pdf_viewer/utf8/core.h
    pdf_viewer/utf8/unchecked.h
    pdf_viewer/synctex/synctex_parser.h
    pdf_viewer/synctex/synctex_parser_utils.h
    pdf_viewer/RunGuard.h
    pdf_viewer/OpenWithApplication.h
    pdf_viewer/book.cpp
    pdf_viewer/config.cpp
    pdf_viewer/database.cpp
    pdf_viewer/document.cpp
    pdf_viewer/document_view.cpp
    pdf_viewer/input.cpp
    pdf_viewer/main.cpp
    pdf_viewer/main_widget.cpp
    pdf_viewer/pdf_renderer.cpp
    pdf_viewer/pdf_view_opengl_widget.cpp
    pdf_viewer/checksum.cpp
    pdf_viewer/new_file_checker.cpp
    pdf_viewer/coordinates.cpp
    pdf_viewer/sqlite3.c
    pdf_viewer/ui.cpp
    pdf_viewer/path.cpp
    pdf_viewer/utils.cpp
    pdf_viewer/RunGuard.cpp
    pdf_viewer/OpenWithApplication.cpp
)

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
