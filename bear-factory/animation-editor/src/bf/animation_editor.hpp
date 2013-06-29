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
#ifndef __BF_ANIMATION_EDITOR_HPP__
#define __BF_ANIMATION_EDITOR_HPP__

#include "bf/animation.hpp"
#include "bf/base_editor_application.hpp"
#include "bf/configuration.hpp"

namespace bf
{
  /**
   * \brief The class representing the application.
   * \author Julien Jorge
   */
  class animation_editor:
    public base_editor_application
  {
  public:
    void configure();
    void update_image_pool() const;
    void set_main_rect( const wxRect& r );

    void compile( const wxString& path ) const;
    void update( const wxString& path ) const;
    void compile_animation( const animation& anim, const wxString& path ) const;

  private:
    bool do_init_app();
    void init_config();

  private:
    /** \brief The configuration of the program. */
    configuration m_config;

  }; // class animation_editor
} // namespace bf

#endif // __BF_ANIMATION_EDITOR_HPP__
