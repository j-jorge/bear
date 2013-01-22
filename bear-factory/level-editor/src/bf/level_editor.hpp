/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The class representing the application.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_EDITOR_HPP__
#define __BF_LEVEL_EDITOR_HPP__

#include "bf/base_editor_application.hpp"
#include "bf/configuration.hpp"
#include "bf/item_class_pool.hpp"

namespace bf
{
  class item_instance;
  class item_check_result;
  class layer;
  class layer_check_result;
  class level;
  class level_check_result;
  class main_frame;

  /**
   * \brief The class representing the application.
   * \author Julien Jorge
   */
  class level_editor:
    public base_editor_application
  {
  public:
    level_editor();

    configuration& get_config();
    const configuration& get_config() const;

    void configure();
    void update_image_pool() const;
    const item_class_pool& get_item_class_pool() const;

    void compile( const wxString& path ) const;
    void update( const wxString& path ) const;
    void compile_level( const level& lvl, const wxString& path ) const;

  private:
    bool do_init_app();
    bool do_command_line_init();
    void init_config();

    bool check_level( level& lvl ) const;

    void print_layer_check_result
    ( const layer* lay, const layer_check_result& result ) const;
    void print_item_check_result
    ( const item_instance* item, const item_check_result& result ) const;

    void load_levels();

  private:
    /** \brief The main window of our program. */
    main_frame* m_main_frame;

    /** \brief The configuration of the program. */
    configuration m_config;

    /** \brief All the item classes. */
    item_class_pool m_class_pool;

  }; // class level_editor
} // namespace bf

#endif // __BF_LEVEL_EDITOR_HPP__
