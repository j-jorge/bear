project( pack-my-sprites )

# where the programs are installed
if( NOT SDC_INSTALL_EXECUTABLE_DIR )
  set( SDC_INSTALL_EXECUTABLE_DIR bin )
endif( NOT SDC_INSTALL_EXECUTABLE_DIR )

# where the resources are installed
if( NOT SDC_INSTALL_RESOURCES_DIR )
  set( SDC_INSTALL_RESOURCES_DIR share/pack-my-sprites )
endif( NOT SDC_INSTALL_RESOURCES_DIR )

subdirs(src)
