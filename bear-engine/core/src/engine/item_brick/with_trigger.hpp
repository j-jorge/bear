/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class describes a trigger that can activate a toggle when a given
 *        condition is verified.
 * \author Julien Jorge
 */
#ifndef __ENGINE_WITH_TRIGGER_HPP__
#define __ENGINE_WITH_TRIGGER_HPP__

#include "engine/expr/collision_in_expression.hpp"
#include "engine/item_brick/with_boolean_expression_assignment.hpp"
#include "expr/boolean_expression.hpp"
#include "universe/derived_item_handle.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief This class describes a trigger that can activate a toggle when a
     *        given condition is verified.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT with_trigger:
      public with_boolean_expression_assignment
    {
    public:
      with_trigger();
      with_trigger( const with_trigger& that );

      with_trigger operator=( const with_trigger& that );

      void set_condition( const expr::boolean_expression& e );
      const expr::boolean_expression& get_condition() const;

      collision_in_expression get_collision_in_expression() const;

    protected:
      void set_collision_data
      ( base_item& that, universe::collision_info& info );
      void clear_collision_data();

      void check_condition( base_item* activator );

    private:
      virtual void on_trigger_on( base_item* activator );
      virtual void on_trigger_off( base_item* activator );

      void do_set_expression( const expr::boolean_expression& e );

    private:
      /** \brief The condition to verify to turn on the toggles. */
      expr::boolean_expression m_condition;

      /** \brief The item in collision, when checking in collisions. */
      const base_item* m_colliding_item;

      /** \brief The informations about the collision, when checking in
          collisions. */
      const universe::collision_info* m_collision_info;

    }; // class with_trigger
  } // namespace engine
} // namespace bear

#endif // __ENGINE_WITH_TRIGGER_HPP__
