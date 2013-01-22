/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A field taking its value from a set.
 * \author Julien Jorge
 */
#ifndef __BF_TYPE_FIELD_SET_HPP__
#define __BF_TYPE_FIELD_SET_HPP__

#include "bf/type_field.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief A field taking its value from a set.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT type_field_set:
    public type_field
  {
  public:
    type_field_set( const std::string& name, field_type ft,
                    const std::list<std::string>& values );

    virtual type_field* clone() const;
    virtual range_type get_range_type() const;
    virtual void get_set( std::list<std::string>& values ) const;

  private:
    /** \brief The values this field can take. */
    std::list<std::string> m_values;

  }; // class type_field_set
} // namespace bf

#endif // __BF_TYPE_FIELD_SET_HPP__
