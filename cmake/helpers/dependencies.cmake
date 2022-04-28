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

add_library(Claw::application INTERFACE IMPORTED)
target_include_directories(
  Claw::application INTERFACE ${CLAW_INCLUDE_DIRECTORY}
  )
target_link_libraries(Claw::application INTERFACE ${CLAW_APPLICATION_LIBRARIES})

add_library(Claw::dynamic_library INTERFACE IMPORTED)
target_include_directories(
  Claw::dynamic_library INTERFACE ${CLAW_INCLUDE_DIRECTORY}
  )
target_link_libraries(
  Claw::dynamic_library INTERFACE ${CLAW_DYNAMIC_LIBRARY_LIBRARIES}
  )

add_library(Claw::configuration_file INTERFACE IMPORTED)
target_include_directories(
  Claw::configuration_file INTERFACE ${CLAW_INCLUDE_DIRECTORY}
  )
target_link_libraries(
  Claw::configuration_file INTERFACE ${CLAW_CONFIGURATION_FILE_LIBRARIES}
  )

add_library(Claw::graphic INTERFACE IMPORTED)
target_include_directories(Claw::graphic INTERFACE ${CLAW_INCLUDE_DIRECTORY})
target_link_libraries(Claw::graphic INTERFACE ${CLAW_GRAPHIC_LIBRARIES})

add_library(Claw::logger INTERFACE IMPORTED)
target_include_directories(Claw::logger INTERFACE ${CLAW_INCLUDE_DIRECTORY})
target_link_libraries(
  Claw::logger INTERFACE ${CLAW_LOGGER_LIBRARIES} Boost::system
  )

add_library(Claw::tween INTERFACE IMPORTED)
target_include_directories(Claw::tween INTERFACE ${CLAW_INCLUDE_DIRECTORY})
target_link_libraries(Claw::tween INTERFACE ${CLAW_TWEEN_LIBRARIES})

add_library(Claw::net INTERFACE IMPORTED)
target_include_directories(Claw::net INTERFACE ${CLAW_INCLUDE_DIRECTORY})
target_link_libraries(Claw::net INTERFACE ${CLAW_NET_LIBRARIES})

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
