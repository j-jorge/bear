/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "link", applies
 *        it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_LINK_CREATOR_HPP__
#define __BEAR_LINK_CREATOR_HPP__

#include "engine/base_item.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates an link between two items then die.
   *
   * The fields of this item are
   *  - \a first_item: (item) \b [required] \c the item at the first end of the
   *    link,
   *  - \a second_item: (item) \b [required] \c the item at the second end of
   *    the link,
   *  - \a strength: (real) \c the strength of the elastic (default = 1),
   *  - \a length.minimal: (real) \c the minimal length of the link
   * (default = 0).
   *  - \a length.maximal: (real) \c the length of the link
   *    (default = inifinity).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT link_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(link_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    link_creator();

    bool set_real_field( const std::string& name, double value );
    bool set_item_field( const std::string& name, engine::base_item* value );
    bool is_valid() const;

    void build();

  private:
    /** \brief The strength of the elastic. */
    double m_strength;

    /** \brief The minimal length of the elastic. */
    universe::coordinate_type m_minimal_length;

    /** \brief The maximal length of the link. */
    universe::coordinate_type m_maximal_length;

    /** \brief The item at the first end of the elastic. */
    engine::base_item* m_first_item;

    /** \brief The item at the second end of the elastic. */
    engine::base_item* m_second_item;

  }; // class link_creator
} // namespace bear

#endif // __BEAR_LINK_CREATOR_HPP__
