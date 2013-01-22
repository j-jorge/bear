/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Some tool functions for the xml writers.
 * \author Julien Jorge
 */
#ifndef __BF_XML_UTIL_HPP__
#define __BF_XML_UTIL_HPP__

#include <string>
#include <map>

namespace bf
{
  namespace xml
  {
    /**
     * \brief Some tool functions to manipulate xml.
     * \author Julien Jorge
     */
    class util
    {
    private:
      /** \brief The type of the map in which we store the xml replacement for
          the special characters. */
      typedef
      std::map< std::string::value_type, std::string > replacement_map_type;

    public:
      static std::string replace_special_characters( const std::string& str );

    private:
      static void build_replacement_map();
  
    private:
      /** \brief The map in which we store the xml replacement for the special
          characters. */
      static replacement_map_type s_replacement_map;

    }; // class util
  } // namespace xml
} // namespace bf

#endif // __BF_XML_UTIL_HPP__
