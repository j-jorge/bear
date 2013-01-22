/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Some tool functions for the xml readers.
 * \author Julien Jorge
 */
#ifndef __BF_XML_READER_TOOL_HPP__
#define __BF_XML_READER_TOOL_HPP__

#include <wx/xml/xml.h>
#include <string>
#include "bf/trinary_logic.hpp"

namespace bf
{
  namespace xml
  {
    /**
     * \brief Some tool functions for the xml readers.
     * \author Julien Jorge
     */
    class reader_tool
    {
    public:
      static const wxXmlNode* skip_comments( const wxXmlNode* node );

      static int read_int( const wxXmlNode* node, const wxString& prop );
      static unsigned int
      read_uint( const wxXmlNode* node, const wxString& prop );
      static std::string
      read_string( const wxXmlNode* node, const wxString& prop );
      static double read_real( const wxXmlNode* node, const wxString& prop );

      static int read_int_opt
      ( const wxXmlNode* node, const wxString& prop, int def );
      static unsigned int read_uint_opt
      ( const wxXmlNode* node, const wxString& prop, unsigned int def );
      static std::string read_string_opt
      ( const wxXmlNode* node, const wxString& prop, const std::string& def );
      static double
      read_real_opt( const wxXmlNode* node, const wxString& prop, double def );
      static bool
      read_bool_opt( const wxXmlNode* node, const wxString& prop, bool def );
      static trinary_logic::value_type read_trinary_logic_opt
      ( const wxXmlNode* node, const wxString& prop,
        trinary_logic::value_type def );
    }; // class reader_tool
  } // namespace xml
} // namespace bf

#endif // __BF_XML_READER_TOOL_HPP__
