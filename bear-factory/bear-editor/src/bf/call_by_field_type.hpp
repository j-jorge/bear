/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A function object that calls an other function object according to the
 *        type of a field.
 * \author Julien Jorge
 */
#ifndef __BF_CALL_BY_FIELD_TYPE_HPP__
#define __BF_CALL_BY_FIELD_TYPE_HPP__

namespace bf
{
  class type_field;

  /**
   * \brief A function object that calls an other function object according to
   *        the type of a field.
   *
   * \b Template \b parameters:
   * - \param F A template function object taking a field type as its argument.
   *
   * \author Julien Jorge
   */
  template<template<typename T> class F, typename R>
  class call_by_field_type
  {
  public:
    template<typename A1>
    R operator()( const type_field& f, A1& a1 ) const;

    template<typename A1, typename A2>
    R operator()( const type_field& f, A1& a1, A2& a2 ) const;

  private:
    template<typename Type, typename A1>
    R do_call( A1& a1 ) const;

    template<typename Type, typename A1, typename A2>
    R do_call( A1& a1, A2& a2 ) const;

  }; // class call_by_field_type
} // namespace bf

#include "bf/impl/call_by_field_type.tpp"

#endif // __BF_CALL_BY_FIELD_TYPE_HPP__
