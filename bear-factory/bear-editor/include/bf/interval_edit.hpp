/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A control for editing a field that takes its value in an interval.
 * \author Julien Jorge
 */
#ifndef __BF_INTERVAL_EDIT_HPP__
#define __BF_INTERVAL_EDIT_HPP__

#include "bf/simple_edit.hpp"
#include "bf/spin_ctrl.hpp"
#include "bf/default_value.hpp"

namespace bf
{
  /**
   * \brief A control for editing a field that can takes ites value in an
   *        interval.
   * \author Julien Jorge
   */
  template<typename Type>
  class interval_edit:
    public simple_edit<Type>,
    public spin_ctrl<typename Type::value_type>
  {
  public:
    /** The type of the current class. */
    typedef interval_edit<Type> self_type;

    /** \brief The type of the value of the edited field. */
    typedef typename simple_edit<Type>::value_type value_type;

    /** \brief The type of the parent edit. */
    typedef simple_edit<Type> super_edit;

    /** \brief The type of the parent control. */
    typedef spin_ctrl<typename Type::value_type> super_ctrl;

  public:
    interval_edit
    ( wxWindow& parent, const value_type& min, const value_type& max,
      const value_type& v = default_value<Type>::get() );

    bool validate();

  private:
    void value_updated();

  }; // class interval_edit
} // namespace bf

#include "bf/impl/interval_edit.tpp"

#endif // __BF_INTERVAL_EDIT_HPP__
