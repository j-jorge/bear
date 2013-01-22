include( FindGettext OPTIONAL )

if( GETTEXT_FOUND )
  subdirs( bear-engine )
endif( GETTEXT_FOUND )
