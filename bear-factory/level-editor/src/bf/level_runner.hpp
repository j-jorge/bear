/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class runs a level.
 * \author Julien Jorge
 */
#ifndef __BF_LEVEL_RUNNER_HPP__
#define __BF_LEVEL_RUNNER_HPP__

#include <wx/string.h>

#include "bf/level.hpp"

namespace bf
{
  class run_configuration;

  /**
   * \brief This class runs a level.
   * \author Julien Jorge
   */
  class level_runner
  {
  public:
    level_runner
    ( const run_configuration& config, const level& lvl,
      std::size_t layer_index, double x, double y );

    void run();

  private:
    void initialize_run();
    void compile_level( const wxString& p ) const;

    bool add_temporary_items() const;

    wxString get_data_directory() const;

    wxChar** create_argv( const wxString& level_file ) const;
    void delete_argv( wxChar** argv ) const;
    wxChar* alloc_argument( const wxString& arg ) const;

    void set_working_directory() const;

  private:
    /** \brief The configuration to use to run the level. */
    const run_configuration& m_config;

    /** \brief The level to run. */
    level m_level;

    /** \brief The index of the layer in which the items are inserted. */
    std::size_t m_layer_index;

    /** \brief The x-origin of the items to insert. */
    const double m_x;

    /** \brief The y-origin of the items to insert. */
    const double m_y;

  }; // class level_runner
} // namespace bf

#endif // __BF_LEVEL_RUNNER_HPP__
