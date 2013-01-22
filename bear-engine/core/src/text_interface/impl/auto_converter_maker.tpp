/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The base class for all classes for which we want to be able to call
 *        methods from a text_interface file.
 * \author Julien Jorge.
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an auto_converter containing one value.
 * \param arg0 The value.
 */
template<typename A0>
bear::text_interface::auto_converter
bear::text_interface::auto_converter_maker( A0 arg0 )
{
  auto_converter result;
  result.push(arg0);

  return result;
} // auto_converter_maker()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an auto_converter containing 2 values.
 * \param arg0 The first value.
 * \param arg1 The second value.
 */
template<typename A0, typename A1>
bear::text_interface::auto_converter
bear::text_interface::auto_converter_maker( A0 arg0, A1 arg1 )
{
  auto_converter result( auto_converter_maker(arg0) );
  result.push(arg1);
  return result;
} // auto_converter_maker()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an auto_converter containing 3 values.
 * \param arg0 The first value.
 * \param arg1 The second value.
 * \param arg2 The third value.
 */
template<typename A0, typename A1, typename A2>
bear::text_interface::auto_converter
bear::text_interface::auto_converter_maker( A0 arg0, A1 arg1, A2 arg2 )
{
  auto_converter result( auto_converter_maker( arg0, arg1 ) );
  result.push(arg2);
  return result;
} // auto_converter_maker()
