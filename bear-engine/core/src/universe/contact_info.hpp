/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Defines the informations related to the contacts of an item.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_CONTACT_INFO_HPP__
#define __UNIVERSE_CONTACT_INFO_HPP__

#include "universe/contact_range.hpp"
#include "universe/types.hpp"

#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief The contact_info class defines the informations related to the
     *        contacts of an item.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT contact_info
    {
    public:
      contact_info();

      const contact_range& get_top_contact() const;
      bool has_top_contact() const;
      void clear_top_contact();
      void set_top_contact( coordinate_type left, coordinate_type right );

      const contact_range& get_bottom_contact() const;
      bool has_bottom_contact() const;
      void clear_bottom_contact();
      void set_bottom_contact( coordinate_type left, coordinate_type right );

      const contact_range& get_left_contact() const;
      bool has_left_contact() const;
      void clear_left_contact();
      void set_left_contact( coordinate_type bottom, coordinate_type top );

      const contact_range& get_right_contact() const;
      bool has_right_contact() const;
      void clear_right_contact();
      void set_right_contact( coordinate_type bottom, coordinate_type top );

      bool has_middle_contact() const;
      void set_middle_contact( bool contact );

      bool has_contact() const;
      bool has_side_contact() const;

      void clear_contacts();

    private:
      /** \brief The contacts on the top side. */
      contact_range m_top_contact;

      /** \brief The contacts on the bottom side. */
      contact_range m_bottom_contact;

      /** \brief The contacts on the right side. */
      contact_range m_right_contact;

      /** \brief The contacts on the left side. */
      contact_range m_left_contact;

      /** \brief Tell if there is a contact inside the object. */
      bool m_middle_contact;

    }; // class contact_info
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_CONTACT_INFO_HPP__
