/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The class representing the application.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_EDITOR_HPP__
#define __BF_MODEL_EDITOR_HPP__

#include "bf/base_editor_application.hpp"
#include "bf/configuration.hpp"

namespace bf
{
  class main_frame;
  class model;

  /**
   * \brief The class representing the application.
   * \author Julien Jorge
   */
  class model_editor:
    public base_editor_application
  {
  public:
    model_editor();

    configuration& get_config();
    const configuration& get_config() const;

    void configure();
    void update_image_pool() const;

    void compile( const wxString& path ) const;
    void update( const wxString& path ) const;
    bool compile_model( const model& mdl, const wxString& path ) const;

  private:
    bool do_init_app();
    bool do_command_line_init();
    void init_config();

    bool check_model( const model& mdl ) const;

    void load_models();

  private:
    /** \brief The main window of our program. */
    main_frame* m_main_frame;

    /** \brief The configuration of the program. */
    configuration m_config;

  }; // class model_editor
} // namespace bf

#endif // __BF_MODEL_EDITOR_HPP__
