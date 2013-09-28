/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A mark in the action.
 * \author Julien Jorge
 */
#ifndef __BF_MARK_HPP__
#define __BF_MARK_HPP__

#include <string>
#include <map>

#include "bf/custom_type.hpp"
#include "bf/animation.hpp"
#include "bf/any_animation.hpp"

namespace bf
{
  class compiled_file;

  /**
   * \brief A mark in the model.
   *
   * A mark is present in all actions of a model.
   *
   * \author Julien Jorge
   */
  class mark
  {
  public:
    mark();

    void set_label( const std::string& label );
    const std::string& get_label() const;

    void set_animation( const any_animation& anim );
    const any_animation& get_animation() const;

    bool has_animation() const;
    animation get_animation_data() const;

    void apply_angle_to_animation( bool b );
    bool apply_angle_to_animation() const;

    void pause_animation_when_hidden( bool b );
    bool pause_animation_when_hidden() const;

    void reset_animation_with_action( bool b );
    bool reset_animation_with_action() const;

    void compile
    ( compiled_file& f,
      const std::map<any_animation, std::size_t>& anim_ref ) const;

  private:
    /** \brief The label of the mark. */
    std::string m_label;

    /** \brief The animation centered on this mark. */
    any_animation m_animation;

    /** \brief Tells if the angle applied to the mark is also applied to the
        animation. */
    bool m_apply_angle_to_animation;

    /** \brief Tells if the animation must be paused when the mark is hidden. */
    bool m_pause_animation_when_hidden;

    /** \brief Tells if the animation must be reset when an action begins. */
    bool m_reset_animation_with_action;

  }; // class mark
} // namespace bf

#endif // __BF_MARK_HPP__
