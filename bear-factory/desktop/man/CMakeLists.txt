if( BF_NO_MANPAGES )
  message("Ignoring manpages, on user's demand.")
else( BF_NO_MANPAGES )
  if(DOCBOOK_TO_MAN_FOUND)
    docbook_create_man(
      bf-man 1
      bf-level-editor.sgml
      bf-model-editor.sgml
      bf-animation-editor.sgml
    )
  else(DOCBOOK_TO_MAN_FOUND)
    message("docbook-to-man was not found on your system. The manpages of the "
      "Bear Factory are not generated.")
  endif(DOCBOOK_TO_MAN_FOUND)
endif( BF_NO_MANPAGES )

