/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class is a boolean expression of type "check_item_class".
 * \author Julien Jorge
 */
#ifndef __BEAR_CHECK_SYSTEM_NAME_CREATOR_HPP__
#define __BEAR_CHECK_SYSTEM_NAME_CREATOR_HPP__

#include "engine/base_item.hpp"
#include "engine/item_brick/with_boolean_expression_creation.hpp"

#include "engine/export.hpp"
#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief This class is a boolean expression telling if the game is running on
   *        a specific system.
   *
   * The fields of this item are
   *  - \a system_name: (string) \b [required] \c the name of the system to
   *    test.
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT check_system_name_creator:
    public engine::base_item,
    public engine::with_boolean_expression_creation
  {
    DECLARE_BASE_ITEM(check_system_name_creator);

  public:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;

  private:
    /**
     * The item loader associated with check_system_name_creator.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;

    public:
      explicit loader( check_system_name_creator& item );

      loader* clone() const;

      bool set_field( const std::string& name, const std::string& value );

    private:
      /** \brief The item we are loading. */
      check_system_name_creator& m_item;

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
    /** \brief The name of the system for which the expression is true. */
    std::string m_system_name;

  }; // class check_system_name_creator
} // namespace bear

#endif // __BEAR_CHECK_SYSTEM_NAME_CREATOR_HPP__
