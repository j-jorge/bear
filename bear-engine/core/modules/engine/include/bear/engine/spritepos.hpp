/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The spritepos class describes the content of a spritepos file.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SPRITEPOS_HPP__
#define __ENGINE_SPRITEPOS_HPP__

#include <claw/rectangle.hpp>
#include <list>

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The spritepos class describes the content of a spritepos file.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT spritepos
    {
    public:
      /**
       * \brief A description of a sprite in a spritepos file.
       */
      class sprite_entry
      {
      public:
        /** \brief The type of the rectangle used to describe the region of a
            sprite in the source image. */
        typedef claw::math::rectangle<unsigned int> rectangle_type;

      public:
        sprite_entry( const std::string& name, const rectangle_type& clip );

        const std::string& get_name() const;
        const rectangle_type& get_clip() const;

      private:
        /** \brief The name of the entry in the spritepos file. */
        const std::string m_name;

        /** \brief The region of the sprite in the source image. */
        const rectangle_type m_clip;

      }; // class sprite_entry

    private:
      /** \brief The type of the container in which the sprite entries are
          stored. */
      typedef std::list<sprite_entry> entry_list;

    public:
      /** \brief The type of the iterator on non modifiable entries. */
      typedef entry_list::const_iterator const_iterator;

    public:
      spritepos( std::istream& f );

      const_iterator begin() const;
      const_iterator end() const;

      const_iterator find( const std::string& name ) const;

    private:
      void load( std::istream& f );

    private:
      /** \brief The entries of the spritepos file. */
      entry_list m_entries;

    }; // class spritepos
  } // namespace engine
} // namespace bear

#endif // __ENGINE_SPRITEPOS_HPP__
