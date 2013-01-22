/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A field taking its value in an interval.
 * \author Julien Jorge
 */
#ifndef __BF_TYPE_FIELD_INTERVAL_HPP__
#define __BF_TYPE_FIELD_INTERVAL_HPP__

#include "bf/type_field.hpp"

namespace bf
{
  /**
   * \brief A field taking its value in an interval.
   * \author Julien Jorge
   */
  template<typename T>
  class type_field_interval:
    public type_field
  {
  public:
    type_field_interval( const std::string& name, T min, T max );

    virtual type_field* clone() const;
    virtual range_type get_range_type() const;
    virtual void get_interval( T& min, T& max ) const;

  private:
    /** \brief The minimum valid value. */
    const T m_min;

    /** \brief The maximum valid value. */
    const T m_max;

  }; // class type_field_interval
} // namespace bf

#include "bf/impl/type_field_interval.tpp"

#endif // __BF_TYPE_FIELD_INTERVAL_HPP__
