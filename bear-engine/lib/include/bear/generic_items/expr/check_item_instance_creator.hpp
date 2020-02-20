/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class is a boolean expression of type "check_item_instance".
 * \author Julien Jorge
 */
#ifndef __BEAR_CHECK_ITEM_INSTANCE_CREATOR_HPP__
#define __BEAR_CHECK_ITEM_INSTANCE_CREATOR_HPP__

#include "bear/engine/base_item.hpp"
#include "bear/engine/item_brick/with_boolean_expression_creation.hpp"

#include "bear/engine/export.hpp"
#include "bear/generic_items/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class with_trigger;
  } // namespace engine

  /**
   * \brief This class is a boolean expression of type "check_item_instance".
   *
   * This item is a parameter for the items of type applied_boolean_expression.
   * He kills himself at the end of his build() method.
   *
   * The fields of this item are
   *  - \a collision_data: (item) \b [required] \c the item from which we take
   *    the collision data. Must inherit from engine::with_trigger,
   *  - \a instance: (list of item) \b [required] \c the instances that validate
   *    the expression.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT check_item_instance_creator:
    public engine::base_item,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(check_item_instance_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

    /**
     * \brief The item loader for the level settings.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;

    public:
      explicit loader( check_item_instance_creator& creator );

      loader* clone() const;

      bool set_field( const std::string& name, engine::base_item* value );
      bool set_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

    private:
      /** \brief The instance through which we set the parameters. */
      check_item_instance_creator& m_creator;

    }; // class loader

  private:
    friend class loader;

  public:
    bool is_valid() const;

    void build();

  private:
    virtual expr::boolean_expression do_get_expression() const;
    virtual void populate_loader_map( engine::item_loader_map& m );

  private:
    /** \brief The list of the instances that validate the expression. */
    std::vector<engine::base_item*> m_instance;

    /** \brief the item from which we take the collision data. */
    engine::with_trigger* m_collision_data;

  }; // class check_item_instance_creator
} // namespace bear

#endif // __BEAR_CHECK_ITEM_INSTANCE_CREATOR_HPP__
