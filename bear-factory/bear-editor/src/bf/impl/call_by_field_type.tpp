/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::call_by_field_type class.
 * \author Julien Jorge
 */
#include "bf/any_animation.hpp"
#include "bf/color.hpp"
#include "bf/custom_type.hpp"
#include "bf/font.hpp"
#include "bf/sample.hpp"
#include "bf/sprite.hpp"
#include "bf/type_field.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the method F<> with the correct type and the given arguments.
 * \param f The field from which we take the type.
 * \param a1 The single argument passed to F<type>::operator().
 */
template<template<typename T> class F, typename R>
template<typename A1>
R bf::call_by_field_type<F, R>::operator()( const type_field& f, A1& a1 ) const
{
  if ( f.is_list() )
    switch ( f.get_field_type() )
      {
      case type_field::integer_field_type:
        return do_call< std::list<integer_type> >(a1);
      case type_field::u_integer_field_type:
        return do_call< std::list<u_integer_type> >(a1);
      case type_field::real_field_type:
        return do_call< std::list<real_type> >(a1);
      case type_field::boolean_field_type:
        return do_call< std::list<bool_type> >(a1);
      case type_field::string_field_type:
        return do_call< std::list<string_type> >(a1);
      case type_field::sprite_field_type:
        return do_call< std::list<sprite> >(a1);
      case type_field::animation_field_type:
        return do_call< std::list<any_animation> >(a1);
      case type_field::item_reference_field_type:
        return do_call< std::list<item_reference_type> >(a1);
      case type_field::font_field_type:
        return do_call< std::list<font> >(a1);
      case type_field::sample_field_type:
        return do_call< std::list<sample> >(a1);
      case type_field::color_field_type:
        return do_call< std::list<color> >(a1);
      }
  else
    switch ( f.get_field_type() )
      {
      case type_field::integer_field_type:
        return do_call<integer_type>(a1);
      case type_field::u_integer_field_type:
        return do_call<u_integer_type>(a1);
      case type_field::real_field_type:
        return do_call<real_type>(a1);
      case type_field::boolean_field_type:
        return do_call<bool_type>(a1);
      case type_field::string_field_type:
        return do_call<string_type>(a1);
      case type_field::sprite_field_type:
        return do_call<sprite>(a1);
      case type_field::animation_field_type:
        return do_call<any_animation>(a1);
      case type_field::item_reference_field_type:
        return do_call<item_reference_type>(a1);
      case type_field::font_field_type:
        return do_call<font>(a1);
      case type_field::sample_field_type:
        return do_call<sample>(a1);
      case type_field::color_field_type:
        return do_call<color>(a1);
      }
} // call_by_type::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the method F<> with the correct type and the given arguments.
 * \param f The field from which we take the type.
 * \param a1 The first argument passed to F<type>::operator().
 * \param a2 The second argument passed to F<type>::operator().
 */
template<template<typename T> class F, typename R>
template<typename A1, typename A2>
R bf::call_by_field_type<F, R>::operator()
  ( const type_field& f, A1& a1, A2& a2 ) const
{
  if ( f.is_list() )
    switch ( f.get_field_type() )
      {
      case type_field::integer_field_type:
        return do_call< std::list<integer_type> >(a1, a2);
      case type_field::u_integer_field_type:
        return do_call< std::list<u_integer_type> >(a1, a2);
      case type_field::real_field_type:
        return do_call< std::list<real_type> >(a1, a2);
      case type_field::boolean_field_type:
        return do_call< std::list<bool_type> >(a1, a2);
      case type_field::string_field_type:
        return do_call< std::list<string_type> >(a1, a2);
      case type_field::sprite_field_type:
        return do_call< std::list<sprite> >(a1, a2);
      case type_field::animation_field_type:
        return do_call< std::list<any_animation> >(a1, a2);
      case type_field::item_reference_field_type:
        return do_call< std::list<item_reference_type> >(a1, a2);
      case type_field::font_field_type:
        return do_call< std::list<font> >(a1, a2);
      case type_field::sample_field_type:
        return do_call< std::list<sample> >(a1, a2);
      case type_field::color_field_type:
        return do_call< std::list<color> >(a1, a2);
      
      default:
        {
          CLAW_FAIL( "The field is not in the switch." );
          return R();
        }
      }
  else
    switch ( f.get_field_type() )
      {
      case type_field::integer_field_type:
        return do_call<integer_type>(a1, a2);
      case type_field::u_integer_field_type:
        return do_call<u_integer_type>(a1, a2);
      case type_field::real_field_type:
        return do_call<real_type>(a1, a2);
      case type_field::boolean_field_type:
        return do_call<bool_type>(a1, a2);
      case type_field::string_field_type:
        return do_call<string_type>(a1, a2);
      case type_field::sprite_field_type:
        return do_call<sprite>(a1, a2);
      case type_field::animation_field_type:
        return do_call<any_animation>(a1, a2);
      case type_field::item_reference_field_type:
        return do_call<item_reference_type>(a1, a2);
      case type_field::font_field_type:
        return do_call<font>(a1, a2);
      case type_field::sample_field_type:
        return do_call<sample>(a1, a2);
      case type_field::color_field_type:
        return do_call<color>(a1, a2);
      
      default:
        {
          CLAW_FAIL( "The field is not in the switch." );
          return R();
        }
      }
} // type_field::call_by_type()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the method F<> with the type passed as a template parameter and
 *        with the given arguments.
 * \param a1 The single argument passed to F<type>::operator().
 */
template<template<typename T> class F, typename R>
template<typename Type, typename A1>
R bf::call_by_field_type<F, R>::do_call( A1& a1 ) const
{
  F<Type> f;
  return f(a1);
} // call_by_field_type::do_call()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the method F<> with the type passed as a template parameter and
 *        with the given arguments.
 * \param a1 The first argument passed to F<type>::operator().
 * \param a2 The second argument passed to F<type>::operator().
 */
template<template<typename T> class F, typename R>
template<typename Type, typename A1, typename A2>
R bf::call_by_field_type<F, R>::do_call( A1& a1, A2& a2 ) const
{
  F<Type> f;
  return f(a1, a2);
} // call_by_field_type::do_call()
