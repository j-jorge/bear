/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A base class owning the edited value for the controls that edit an
 *        item property.
 * \author Julien Jorge
 */
#ifndef __BF_BASE_EDIT_HPP__
#define __BF_BASE_EDIT_HPP__

namespace bf
{
  /**
   * \brief A base class owning the edited value for the controls that edit an
   *        item property.
   * \author Julien Jorge
   */
  template<typename Type>
  class base_edit
  {
  public:
    /** \brief The type of the value of the edited field. */
    typedef Type value_type;

  public:
    base_edit( const value_type& v );
    virtual ~base_edit();

    /** \brief Check if the displayed value is correct and, if it is, set the
        value according to the display. */
    virtual bool validate() = 0;

    const value_type& get_value() const;
    void set_value( const value_type& v );

  protected:
    /** \brief Method called after changing the value by set_value(). */
    virtual void value_updated() = 0;

  private:
    /** \brief The value. */
    value_type m_value;

  }; // class base_edit
} // namespace bf

#include "bf/impl/base_edit.tpp"

#endif // __BF_BASE_EDIT_HPP__
