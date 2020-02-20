set(desktop_root ${BEAR_ROOT}/bear-engine/desktop)

if(BEAR_MAN_PAGES_ENABLED)
  if(DOCBOOK_TO_MAN_FOUND)
    docbook_create_man(engine-man 6 ${desktop_root}/man/running-bear.sgml)
  else()
    message("docbook-to-man was not found on your system. The manpages of the "
      "engine are not generated.")
  endif()
endif()

if(GETTEXT_FOUND)
  set(locale_root ${desktop_root}/locale/bear-engine/)
  gettext_create_translations_custom(
    bear-engine-translations
    bear-engine.pot
    ${locale_root}/fr.po
    ${locale_root}/nn.po
    )
endif()
