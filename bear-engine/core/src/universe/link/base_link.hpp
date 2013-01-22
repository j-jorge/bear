/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class for links between items.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_BASE_LINK_HPP__
#define __UNIVERSE_BASE_LINK_HPP__

#include "universe/physical_item.hpp"
#include <claw/non_copyable.hpp>

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief Base class for links between items.
     *
     * A link is an abstract entity representing an interaction between two
     * items. For example:
     *  - an elastic connecting two items. It's hard for an item to move from
     *    away from the other.
     *  - a magnetic field attract two items.
     *
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT base_link:
      public claw::pattern::non_copyable
    {
    public:
      base_link( physical_item& first_item, physical_item& second_item );
      base_link
        ( const reference_point& first_item, 
          const reference_point& second_item );
      
      virtual ~base_link();

      virtual void adjust() = 0;

      std::size_t get_id() const;

      void unlink();

      const physical_item& get_first_item() const;
      const physical_item& get_second_item() const;
      
    public:
      /** \brief An incorrect identifier for the links. */
      static std::size_t not_an_id;

    protected:
      /** \brief One of the linked items. */
      reference_point m_first_item;

      /** \brief The second linked item. */
      reference_point m_second_item;

    private:
      /** \brief The identifier of the link. */
      std::size_t m_id;

      /** \brief The next identifier for the links. */
      static std::size_t s_next_id;

    }; // class base_link
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_BASE_LINK_HPP__
