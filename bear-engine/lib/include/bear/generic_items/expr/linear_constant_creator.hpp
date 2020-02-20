/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Class to create an item that is a linear constant.
 * \author Sébastien Angibaud
 */
#ifndef __BEAR_LINEAR_CONSTANT_CREATOR_HPP__
#define __BEAR_LINEAR_CONSTANT_CREATOR_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/expr/linear_constant.hpp"
#include "bear/engine/item_brick/with_linear_expression_creation.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class is a linear expression of type "linear_constant".
   *
   * This item is a parameter for other items. He kills himself at the end of
   * his build() method.
   *
   * The fields of this item are
   *  - \a operand: (double) \c the value of the constant.
   *
   * \author Sébastien Angibaud
   */
  class GENERIC_ITEMS_EXPORT linear_constant_creator:
    public engine::base_item,
    public engine::with_linear_expression_creation
  {
    DECLARE_BASE_ITEM(linear_constant_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    linear_constant_creator();

    bool set_real_field( const std::string& name, double value );

    void build();

  private:
    virtual expr::linear_expression do_get_expression() const;

  private:
    /** \brief The expression created by this item. */
    expr::linear_constant m_expr;

  }; // class linear_constant_creator

} // namespace bear

#endif // __BEAR_LINEAR_CONSTANT_CREATOR_HPP__
