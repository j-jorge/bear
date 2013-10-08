/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class for reading source model files.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_FILE_HPP__
#define __BF_MODEL_FILE_HPP__

#include <iostream>
#include <wx/xml/xml.h>

namespace bf
{
  class gui_model;
  class workspace_environment;

  namespace xml
  {
    /**
     * \brief A class for reading source model files.
     * \author Julien Jorge
     */
    class model_file
    {
    public:
      gui_model* load
      ( const wxString& file_path, workspace_environment * env ) const;
      void save( const gui_model& mdl, std::ostream& os ) const;

    private:
      gui_model* load_model
      ( const wxXmlNode* node, workspace_environment * env ) const;

    }; // class model_file
  } // namespace xml
} // namespace bf

#endif // __BF_MODEL_FILE_HPP__
