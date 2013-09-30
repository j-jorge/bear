/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Predicates to tell if a given type represents a visual value (i.e. a
 *        sprite, an animation).
 * \author Julien Jorge
 */
#ifndef __BF_IS_VISUAL_TYPE__
#define __BF_IS_VISUAL_TYPE__

namespace bf
{
  /**
   * \brief Tells if a given type represents a visual value (i.e. a sprite, an
   *        animation).
   */
  template<typename T>
  struct is_visual_type
  {
    enum result
      {
        value = false
      };
  };

  template<>
  struct is_visual_type<sprite>
  {
    enum result
      {
        value = true
      };
  };

  template<>
  struct is_visual_type<animation>
  {
    enum result
      {
        value = true
      };
  };

  template<>
  struct is_visual_type<animation_file_type>
  {
    enum result
      {
        value = true
      };
  };

  template<>
  struct is_visual_type<any_animation>
  {
    enum result
      {
        value = true
      };
  };
} // namespace bf

#endif // __BF_IS_VISUAL_TYPE__
