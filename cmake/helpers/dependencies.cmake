find_package(
  Boost 1.42 REQUIRED COMPONENTS filesystem regex system thread
  )

find_package(Freetype REQUIRED)
find_package(claw 1.8.0 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2_mixer REQUIRED)

find_package(Intl REQUIRED)

add_library(Intl::Intl INTERFACE IMPORTED)
target_link_libraries(Intl::Intl INTERFACE ${Intl_LIBRARIES})
target_include_directories(Intl::Intl INTERFACE ${Intl_INCLUDE_DIR})

find_package(Gettext)

if(BEAR_EDITORS_ENABLED)
  find_package(wxWidgets REQUIRED)

  add_library(wxWidgets::wxWidgets INTERFACE IMPORTED)

  target_include_directories(
    wxWidgets::wxWidgets
    INTERFACE
    ${wxWidgets_INCLUDE_DIRS}
    )

  target_compile_definitions(
    wxWidgets::wxWidgets
    INTERFACE
    ${wxWidgets_DEFINITIONS}
    )

  target_link_libraries(
    wxWidgets::wxWidgets
    INTERFACE
    ${wxWidgets_LIBRARIES}
    )

  target_compile_options(
    wxWidgets::wxWidgets
    INTERFACE
    ${wxWidgets_CXX_FLAGS}
    )
endif()
