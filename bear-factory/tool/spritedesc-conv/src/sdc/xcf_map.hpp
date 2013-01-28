/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The map associating the name of the xcf with their descriptions.
 * \author Julien Jorge
 */
#ifndef __SDC_XCF_MAP_HPP__
#define __SDC_XCF_MAP_HPP__

#include "xcf_info.hpp"

#include <string>
#include <map>

namespace sdc
{
  /**
   * \brief The xcf_map class stores the informations of the xcf files.
   * \author Julien Jorge
   */
  class xcf_map
  {
  private:
    /** \brief The map associating the name of the xcf with their
        descriptions. */
    typedef std::map<std::string, xcf_info> name_to_info_type;

  public:
    xcf_map( std::string xcf_directory, std::string xcfinfo_program );

    void load( std::string name );

    bool has_info( std::string name ) const;
    xcf_info get_info( std::string name ) const;

  private:
    std::string execute_xcfinfo_process( std::string filename ) const;

    void parse_xcf_info_header( xcf_info& info, std::string header ) const;
    void parse_xcf_info_layer( xcf_info& info, std::string layer ) const;

  private:
    /** \brief The map associating the name of the xcf with their
        descriptions. */
    name_to_info_type m_xcf_info;

    /** \brief The directory where the xcf files are taken. */
    std::string m_xcf_directory;

    /** \brief The path to the xcfinfo program. */
    std::string m_xcfinfo_program;

  }; // class xcf_map

} // namespace sdc

#endif // __SDC_XCF_MAP_HPP__
