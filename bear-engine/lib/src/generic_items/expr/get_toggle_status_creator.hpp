/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class is a boolean expression of type "get_toggle_status".
 * \author Julien Jorge
 */
#ifndef __BEAR_GET_TOGGLE_STATUS_CREATOR_HPP__
#define __BEAR_GET_TOGGLE_STATUS_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/expr/get_toggle_status.hpp"
#include "engine/item_brick/with_boolean_expression_creation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class is a boolean expression of type "get_toggle_status".
   *
   * This item is a parameter for the items of type applied_boolean_expression.
   * He kills himself at the end of his build() method.
   *
   * The fields of this item are
   *  - \a toggle: (item) \b [required] \c the toggle from which the status is
   *    taken.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT get_toggle_status_creator:
    public engine::base_item,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(get_toggle_status_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool set_item_field( const std::string& name, engine::base_item* value );

    void build();

    bool is_valid() const;

  private:
    virtual expr::boolean_expression do_get_expression() const;

  private:
    /** \brief The expression created by this item. */
    engine::get_toggle_status m_expr;

  }; // class get_toggle_status_creator
} // namespace bear

#endif // __BEAR_GET_TOGGLE_STATUS_CREATOR_HPP__
