/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The value of a field of type "animation_file_type".
 * \author Julien Jorge
 */
#ifndef __BF_ANIMATION_FILE_TYPE_HPP__
#define __BF_ANIMATION_FILE_TYPE_HPP__

#include "bf/animation.hpp"
#include "bf/image_pool.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief The file of an animation, associated with some rendering attributes.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT animation_file_type:
    public bitmap_rendering_attributes
  {
  public:
    void set_path( const std::string& p, const image_pool& pool );
    const std::string& get_path() const;

    animation get_animation() const;
    animation original_animation() const;
    void compile( compiled_file& f ) const;

    bool operator==( const animation_file_type& that ) const;
    bool operator!=( const animation_file_type& that ) const;
    bool operator<( const animation_file_type& that ) const;

  private:
    /** \brief The path of the animation file. */
    std::string m_path;

    /** \brief The animation returned by original_animation(). */
    animation m_animation;

  }; // class animation_file_type
} // namespace bf

#endif // __BF_ANIMATION_FILE_TYPE_HPP__
