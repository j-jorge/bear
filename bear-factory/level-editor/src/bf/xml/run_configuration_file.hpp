/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class for reading run configuration files.
 * \author Julien Jorge
 */
#ifndef __BF_RUN_CONFIGURATION_FILE_HPP__
#define __BF_RUN_CONFIGURATION_FILE_HPP__

#include <iostream>
#include <wx/xml/xml.h>

#include "bf/item_class_pool.hpp"
#include "bf/run_configuration.hpp"

namespace bf
{
  namespace xml
  {
    /**
     * \brief A class for reading run configuration files.
     * \author Julien Jorge
     */
    class run_configuration_file
    {
    public:
      run_configuration
      load( const item_class_pool& pool, const wxString& file_path ) const;

      void save( const run_configuration& config, std::ostream& os ) const;

    }; // class run_configuration_file
  } // namespace xml
} // namespace bf

#endif // __BF_RUN_CONFIGURATION_FILE_HPP__
