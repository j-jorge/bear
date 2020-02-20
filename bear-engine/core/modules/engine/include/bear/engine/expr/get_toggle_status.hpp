/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief An expression that returns the status of a toggle.
 * \author Julien Jorge
 */
#ifndef __ENGINE_GET_TOGGLE_STATUS_HPP__
#define __ENGINE_GET_TOGGLE_STATUS_HPP__

#include "bear/expr/base_boolean_expression.hpp"
#include "bear/universe/derived_item_handle.hpp"

#include "bear/engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;
    class with_toggle;

    /**
     * \brief An expression that returns the status of a toggle.
     *
     * \author Julien Jorge
     */
    class ENGINE_EXPORT get_toggle_status:
      public expr::base_boolean_expression
    {
    private:
      /** \brief The type of the handles on the toggles. */
      typedef
        universe::const_derived_item_handle<const with_toggle, const base_item>
        toggle_handle;

    public:
      get_toggle_status();
      explicit get_toggle_status( const base_item& item );

      void set_toggle( const base_item& item );
      const base_item* get_toggle() const;

      expr::base_boolean_expression* clone() const;
      bool evaluate() const;

      std::string formatted_string() const;

    private:
      /** \brief The toggle to check. */
      toggle_handle m_toggle;

    }; // class get_toggle_status

  } // namespace engine
} // namespace bear

#endif // __ENGINE_GET_TOGGLE_STATUS_HPP__
