/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A class that contains an animation or an animation_file_type, only one
 *        at once.
 * \author Julien Jorge
 */
#ifndef __BF_ANY_ANIMATION_HPP__
#define __BF_ANY_ANIMATION_HPP__

#include "bf/animation_file_type.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief A class that contains an animation or an animation_file_type, only
   *        one at once.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT any_animation
  {
  public:
    /** \brief The type of the animation effectively stored. */
    enum content_type
      {
        /** \brief A completely detailed animation. */
        content_animation,

        /** \brief An animation file.*/
        content_file

      }; // enum content_type

  public:
    static std::string content_to_string( content_type c );
    static content_type string_to_content( const std::string& c );

    any_animation( content_type c = content_animation );

    void set_animation_file( const animation_file_type& a );
    void set_animation( const animation& a );

    void reload();
    const animation_file_type& get_animation_file() const;
    const animation& get_animation() const;

    animation get_current_animation() const;

    void switch_to( content_type c );
    content_type get_content_type() const;

    void compile( compiled_file& f, compilation_context& c ) const;

    bool operator==( const any_animation& that ) const;
    bool operator!=( const any_animation& that ) const;
    bool operator<( const any_animation& that ) const;

  private:
    /** \brief The type of the animation on which we are working. */
    content_type m_content_type;

    /** \brief The path of the animation file. */
    animation_file_type m_animation_file;

    /** \brief The animation returned by original_animation(). */
    animation m_animation;

  }; // class any_animation
} // namespace bf

#endif // __BF_ANY_ANIMATION_HPP__
