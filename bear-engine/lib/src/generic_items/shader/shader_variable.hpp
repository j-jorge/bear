/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A variable in a shader program and how its value is updated.
 * \author Julien Jorge
 */
#ifndef __BEAR_SHADER_VARIABLE_HPP__
#define __BEAR_SHADER_VARIABLE_HPP__

#include "engine/base_item.hpp"

#include "generic_items/class_export.hpp"

#include "engine/export.hpp"

namespace bear
{
  /**
   * \brief A variable in a shader program and how its value is updated.
   *
   * The fields of this item are
   *  - \a name: (string) \c The name of the variable,
   *  - \a tweener.easing: (easing_function) \c The easing function applied to
   *    vary the value of the variable (default = none:in_out),
   *  - \a origin: (real) The initial value of the variable (default = 0),
   *  - \a target: (real) The final value of the variable (default = 1),
   *  - \a duration: (real) The duration of the tweener, in seconds
   *    (default = 1).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT shader_variable:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(shader_variable);
    
    /** \brief The type of the parent class. */
    typedef engine::base_item super;
    
  private:
    /**
     * \brief The item loader for the shader variable.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;
      
    public:
      explicit loader( shader_variable& item );

      loader* clone() const;

      bool set_field( const std::string& name, const std::string& value );

    private:
      /** \brief The instance loaded by this loader. */
      shader_variable& m_item;

    }; // class loader

  public:
    shader_variable();

    void progress( universe::time_type elapsed_time );

    std::string get_name() const;
    void set_name( std::string n );

    void set_tweener( claw::tween::single_tweener t );

    double get_value() const;

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    /** \brief The name of the variable. */
    std::string m_name;

    /** \brief The tweener applied to change the value of the variable. */
    claw::tween::single_tweener m_tweener;

  }; // class shader_variable
} // namespace bear

#endif // __BEAR_SHADER_VARIABLE_HPP__
