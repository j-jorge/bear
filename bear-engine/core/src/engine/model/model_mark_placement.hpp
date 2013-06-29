/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A placement for a mark in a snapshot.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_MARK_PLACEMENT_HPP__
#define __ENGINE_MODEL_MARK_PLACEMENT_HPP__

#include "universe/types.hpp"
#include "engine/class_export.hpp"

#include <claw/tween/single_tweener.hpp>

namespace bear
{
  namespace engine
  {
    /**
     * \brief A placement for a mark in a snapshot.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_mark_placement
    {
    public:
      typedef claw::tween::single_tweener::easing_function easing_function;

    public:
      model_mark_placement();
      model_mark_placement
      ( unsigned int id, universe::coordinate_type x,
        universe::coordinate_type y, universe::coordinate_type w,
        universe::coordinate_type h, int depth, double angle, bool visible,
        const std::string& collision_function );

      void set_mark_id( unsigned int i );
      unsigned int get_mark_id() const;

      void set_position
      ( universe::coordinate_type x, universe::coordinate_type y );
      void set_position( const universe::position_type& pos );
      const universe::position_type& get_position() const;

      void set_x_position( universe::coordinate_type x );
      void set_y_position( universe::coordinate_type y );

      void set_size( const universe::size_box_type& s );
      const universe::size_box_type& get_size() const;

      void set_width( universe::size_type s );
      void set_height( universe::size_type s );

      void set_depth_position( int z );
      int get_depth_position() const;

      void set_angle( double a );
      double get_angle() const;

      void set_visibility( bool v );
      bool is_visible() const;

      void set_collision_function( const std::string& f );
      const std::string& get_collision_function() const;

      easing_function get_angle_easing() const;
      void set_angle_easing( easing_function f );

      easing_function get_x_position_easing() const;
      void set_x_position_easing( easing_function f );

      easing_function get_y_position_easing() const;
      void set_y_position_easing( easing_function f );

      easing_function get_width_easing() const;
      void set_width_easing( easing_function f );

      easing_function get_height_easing() const;
      void set_height_easing( easing_function f );

    private:
      /** \brief The identifier or the mark concerned by this placement. */
      unsigned int m_mark_id;

      /** \brief The position of the mark. */
      universe::position_type m_position;

      /** \brief The size of the mark. */
      universe::size_box_type m_size;

      /** \brief The depth-position of the mark. */
      int m_depth;

      /** \brief The angle of the mark. */
      double m_angle;

      /** \brief Tell if the mark is visible or not. */
      bool m_visible;

      /** \brief The name of the function to call when there is a collision on
          the mark. */
      std::string m_collision_function;

      /** \brief The easing function for the angle. */
      easing_function m_angle_easing;

      /** \brief The easing function for the x-position. */
      easing_function m_x_easing;

      /** \brief The easing function for the y-position. */
      easing_function m_y_easing;

      /** \brief The easing function for the width. */
      easing_function m_width_easing;

      /** \brief The easing function for the height. */
      easing_function m_height_easing;

    }; // class model_mark_placement
  } // namespace engine
} // namespace bear

#endif // __ENGINE_MODEL_MARK_PLACEMENT_HPP__
