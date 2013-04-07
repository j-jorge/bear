/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item that sets the shader applied to the items of its layer.
 * \author Julien Jorge
 */
#ifndef __BEAR_LAYER_SHADER_HPP__
#define __BEAR_LAYER_SHADER_HPP__

#include "engine/base_item.hpp"

#include "generic_items/shader/shader_variable.hpp"

#include "visual/shader_program.hpp"

#include "generic_items/class_export.hpp"

namespace bear
{
  /**
   * \brief An item that sets the shader applied to the items of its layer.
   *
   * The fields of this item are
   *  - \a shader: (string) \c The shader file,
   *  - \a variables: (list of shader_variable) \c The items that compute the
   *       values of the variables in the shader. (default = none:in_out),
   *  - \a kill_delay: (real) Tells the item to kill itself after this number of
   *       seconds (default = infinity),
   *  - \a duration: (real) The duration of the tweener, in seconds
   *    (default = 1).
   *
   * \author Julien Jorge
   */
  class GENERIC_ITEMS_EXPORT layer_shader:
    public engine::base_item
  {
    DECLARE_BASE_ITEM(layer_shader);
    
  private:
    /** \brief The type of the parent class. */
    typedef engine::base_item super;
    
    /** \brief The type of the pointers on the variables of the shader. */
    typedef
      universe::derived_item_handle<shader_variable> shader_variable_pointer;

  private:
    /**
     * \brief The item loader for the layer shader.
     */
    class loader:
      public engine::item_loader_base
    {
    private:
      /** \brief The type of the parent class. */
      typedef engine::item_loader_base super;
      
    public:
      explicit loader( layer_shader& item );

      loader* clone() const;

      bool set_field( const std::string& name, const std::string& value );
      bool set_field( const std::string& name, double value );
      bool set_field
      ( const std::string& name, const std::vector<engine::base_item*>& value );

    private:
      /** \brief The instance loaded by this loader. */
      layer_shader& m_item;

    }; // class loader

  public:
    layer_shader();
    layer_shader( const layer_shader& that );

    void progress( universe::time_type elapsed_time );

    void set_kill_delay( universe::time_type t );

    void set_shader_file( std::string n );

    void add_variable( const shader_variable& v );

  protected:
    void populate_loader_map( engine::item_loader_map& m );

  private:
    /** \brief The shader to pass to the layer. */
    visual::shader_program m_shader;

    /** \brief The delay after which the item must kill itself. */
    universe::time_type m_kill_delay;

    /** \brief The items that compute the values of the variables in the
        shader. */
    std::vector<shader_variable_pointer> m_variables;

  }; // class layer_shader
} // namespace bear

#endif // __BEAR_LAYER_SHADER_HPP__
