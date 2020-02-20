function( prepend_string dest prefix )
  set( result "" )

  foreach( suffix ${ARGN} )
    set( result "${result}" "${prefix}${suffix}" )
  endforeach()

  set( ${dest} "${result}" PARENT_SCOPE )
endfunction()
