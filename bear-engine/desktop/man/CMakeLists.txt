if( BEAR_ENGINE_NO_MANPAGES )
  message("Ignoring manpages, on user's demand.")
else( BEAR_ENGINE_NO_MANPAGES )
  if(DOCBOOK_TO_MAN_FOUND)
    docbook_create_man( engine-man 6 running-bear.sgml )
  else(DOCBOOK_TO_MAN_FOUND)
    message("docbook-to-man was not found on your system. The manpages of the "
      "engine are not generated.")
  endif(DOCBOOK_TO_MAN_FOUND)
endif( BEAR_ENGINE_NO_MANPAGES )

