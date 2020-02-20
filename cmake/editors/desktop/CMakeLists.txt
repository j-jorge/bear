set(desktop_root "${BEAR_ROOT}/bear-factory/desktop")

install(
  DIRECTORY
  "${desktop_root}/applications"
  "${desktop_root}/icons"
  "${desktop_root}/pixmaps"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/"
  )

install(
  FILES "${desktop_root}/images/level-splash.png"
  DESTINATION "${BF_IMAGES_PATH}"
)

if(BEAR_MAN_PAGES_ENABLED)
  if(DOCBOOK_TO_MAN_FOUND)
    set(man_root ${desktop_root}/man)
    docbook_create_man(
      bf-man 1
      ${man_root}/bf-animation-editor.sgml
      ${man_root}/bf-level-editor.sgml
      ${man_root}/bf-model-editor.sgml
      )
  else()
    message("docbook-to-man was not found on your system. The manpages of the "
      "Bear Factory are not generated.")
  endif()
endif()

if(GETTEXT_FOUND)
  set(locale_root ${desktop_root}/locale/bear-factory/)
  gettext_create_translations_custom(
    bear-factory-translations
    bear-factory.pot
    ${locale_root}/fr.po
    ${locale_root}/nn.po
    )
endif()
