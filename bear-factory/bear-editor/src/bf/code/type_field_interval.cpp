/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the specialized methods of the
 *        bf::type_field_interval class.
 * \author Julien Jorge
 */
#include "bf/type_field_interval.hpp"

namespace bf
{
  /*--------------------------------------------------------------------------*/
  /**
   * \brief Constructor.
   * \param name The name of the field.
   * \param min The minimum valid value of the interval.
   * \param max The maximum valid value of the interval.
   */
  template<>
  type_field_interval<int>::type_field_interval
  ( const std::string& name, int min, int max )
    : type_field( name, type_field::integer_field_type ), m_min(min), m_max(max)
  {

  } // type_field_interval::type_field_interval()
}

namespace bf
{
  /*--------------------------------------------------------------------------*/
  /**
   * \brief Constructor.
   * \param name The name of the field.
   * \param min The minimum valid value of the interval.
   * \param max The maximum valid value of the interval.
   */
  template<>
  type_field_interval<unsigned int>::type_field_interval
  ( const std::string& name, unsigned int min, unsigned int max )
    : type_field( name, type_field::u_integer_field_type ), m_min(min),
      m_max(max)
  {

  } // type_field_interval::type_field_interval()
}

namespace bf
{
  /*--------------------------------------------------------------------------*/
  /**
   * \brief Constructor.
   * \param name The name of the field.
   * \param min The minimum valid value of the interval.
   * \param max The maximum valid value of the interval.
   */
  template<>
  type_field_interval<double>::type_field_interval
  ( const std::string& name, double min, double max )
    : type_field(name, type_field::real_field_type), m_min(min), m_max(max)
  {

  } // type_field_interval::type_field_interval()
}
