/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Defines how the informations related to the alignment are set.
 * \author Julien Jorge
 */
#ifndef __UNIVERSE_COLLISION_ALIGN_POLICY_HPP__
#define __UNIVERSE_COLLISION_ALIGN_POLICY_HPP__

#include "universe/contact_mode.hpp"
#include "universe/class_export.hpp"

namespace bear
{
  namespace universe
  {
    /**
     * \brief The collision_align_policy classe defines how the informations
     *        related to the alignment are set.
     * \author Julien Jorge
     */
    class UNIVERSE_EXPORT collision_align_policy
    {
    public:
      explicit collision_align_policy
        ( contact_mode::value_type c = contact_mode::range_contact );

      contact_mode::value_type get_contact_mode() const;

    private:
      /** \brief Tell how the contacts are set. */
      contact_mode::value_type m_contact_mode;

    }; // class collision_align_policy
  } // namespace universe
} // namespace bear

#endif // __UNIVERSE_COLLISION_ALIGN_POLICY_HPP__
