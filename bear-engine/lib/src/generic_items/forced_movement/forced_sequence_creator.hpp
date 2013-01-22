/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class creates a forced movement of type "forced_sequence",
 *        applies it to itself then die.
 * \author Julien Jorge
 */
#ifndef __BEAR_FORCED_SEQUENCE_CREATOR_HPP__
#define __BEAR_FORCED_SEQUENCE_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "universe/forced_movement/forced_sequence.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class creates a forced movement of type "forced_sequence",
   *        applies it to itself then die.
   *
   * The fields of this item are
   *  - \a movements: (item list) \b [required] \c the movements of the
   *    sequence, must be not empty,
   *  - \a loops: (unsigned int) \c how many times the sequence will be
   *    done (default = no limits = 0).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT forced_sequence_creator:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(forced_sequence_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool set_u_integer_field( const std::string& name, unsigned int value );
    bool set_item_list_field
    ( const std::string& name, const std::vector<engine::base_item*>& value );

    void build();

  private:
    /** \brief The movement finally applied. */
    universe::forced_sequence m_movement;

  }; // class forced_sequence_creator
} // namespace bear

#endif // __BEAR_FORCED_SEQUENCE_CREATOR_HPP__
