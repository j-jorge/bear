/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class is a boolean expression of type "check_item_class".
 * \author Julien Jorge
 */
#ifndef __BEAR_CHECK_ITEM_CLASS_CREATOR_HPP__
#define __BEAR_CHECK_ITEM_CLASS_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/with_boolean_expression_creation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class with_trigger;
  } // namespace engine

  /**
   * \brief This class is a boolean expression of type "check_item_class".
   *
   * This item is a parameter for the items of type applied_boolean_expression.
   * He kills himself at the end of his build() method.
   *
   * The fields of this item are
   *  - \a collision_data: (item) \b [required] \c the item from which we take
   *    the collision data. Must inherit from engine::with_trigger,
   *  - \a class_name: (string) \b [required] \c the name of the class to pass
   *    to check_item_class.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT check_item_class_creator:
    public engine::base_item,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(check_item_class_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  public:
    bool set_string_list_field
      ( const std::string& name, const std::vector<std::string>& value );
    bool set_item_field( const std::string& name, engine::base_item* value );
    bool is_valid() const;

    void build();

  private:
    virtual expr::boolean_expression do_get_expression() const;

  private:
    /** \brief The list of the class names to check. */
    std::vector<std::string> m_class_name;

    /** \brief the item from which we take the collision data. */
    engine::with_trigger* m_collision_data;

  }; // class check_item_class_creator
} // namespace bear

#endif // __BEAR_CHECK_ITEM_CLASS_CREATOR_HPP__
