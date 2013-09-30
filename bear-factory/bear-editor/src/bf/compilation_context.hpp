/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The compilation context keep some data used during the compilation of
 *        the resources.
 * \author Julien Jorge
 */
#ifndef __BF_COMPILATION_CONTEXT_HPP__
#define __BF_COMPILATION_CONTEXT_HPP__

#include "bf/image_pool.hpp"
#include "bf/sprite_image_cache.hpp"
#include "bf/libeditor_export.hpp"

#include <map>
#include <string>

#include <claw/rectangle.hpp>

namespace bf
{
  class sprite;

  /**
   * \brief The compilation context keep some data used during the compilation
   *        of the resources.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT compilation_context
  {
  public:
    /** \brief The type of the opaque rectangles. */
    typedef claw::math::box_2d<unsigned int> rectangle;

  private:
    /** \brief The type of the structure associating its opaque rectangle to
        each sprite. */
    typedef std::map<sprite, rectangle> opaque_rectangle_map;

    /** \brief The type of the structure associating its the compiled identifier
        to each item identifier. */
    typedef std::map<std::string, unsigned int> identifier_map;

  public:
    compilation_context
      ( unsigned int optimization_level, const image_pool& pool );

    rectangle get_opaque_rectangle
      ( const sprite& s, const std::string& image_name );

    unsigned int get_compiled_identifier( const std::string& item ) const;
    void set_compiled_identifier( const std::string& item, unsigned int id );
    void clear_compiled_identifiers();

  private:
    rectangle compute_opaque_rectangle( const sprite& s );

  private:
    /** \brief The sprite image cache stores the images used to compute the
        opaque rectangles for the sprites. */
    sprite_image_cache m_image_cache;

    /** \brief The opaque rectangle associated with each sprite. */
    opaque_rectangle_map m_opaque_rectangle;

    /** \brief The compiled identifier associated with each item identifier. */
    identifier_map m_identifier;

    /** \brief The optimisation level during the compilation. */
    unsigned int m_optimization_level;

  }; // class compilation_context
} // namespace bf

#endif // __BF_COMPILATION_CONTEXT_HPP__
