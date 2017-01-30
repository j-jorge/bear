/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for all screen implementation.
 * \author Julien Jorge
 */
#ifndef __VISUAL_BASE_SCREEN_HPP__
#define __VISUAL_BASE_SCREEN_HPP__

#include "visual/class_export.hpp"
#include "visual/types.hpp"

#include <claw/coordinate_2d.hpp>
#include <claw/image.hpp>
#include <boost/function.hpp>
#include <boost/signals2/connection.hpp>
#include <vector>

namespace bear
{
  namespace visual
  {
    class sprite;
    class shader_program;

    /**
     * \brief Base class for all screen implementation.
     * \author Julien Jorge
     */
    class VISUAL_EXPORT base_screen
    {
    public:
      virtual ~base_screen() {}

      virtual void set_pause() = 0;
      virtual void unset_pause() = 0;

      virtual void fullscreen( bool b ) = 0;
      virtual claw::math::coordinate_2d<unsigned int> get_size() const = 0;
      virtual claw::math::coordinate_2d<unsigned int>
        get_container_size() const = 0;

      virtual void set_background_color( const color_type& c ) = 0;
      virtual color_type get_background_color() const = 0;

      virtual void begin_render() {}
      virtual void render( const position_type& pos, const sprite& s ) = 0;
      virtual void end_render() { }

      virtual void draw_line
      ( const color_type& color,
        const std::vector<position_type>& p, double w, bool close = false ) = 0;
      virtual void draw_polygon
      ( const color_type& color,
        const std::vector<position_type>& p ) = 0;

      virtual void push_shader( const shader_program& p ) = 0;
      virtual void pop_shader() = 0;

      virtual void shot( claw::graphic::image& img ) const = 0;
      virtual boost::signals2::connection schedule_shot
      ( const boost::function< void( const claw::graphic::image& ) >& f ) = 0;

    }; // class base_screen
  } // namespace visual
} // namespace bear

#endif // __VISUAL_BASE_SCREEN_HPP__
