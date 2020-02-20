/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for the fields of an item class.
 * \author Julien Jorge
 */
#ifndef __BF_TYPE_FIELD_HPP__
#define __BF_TYPE_FIELD_HPP__

#include "bf/libeditor_export.hpp"

#include <list>
#include <string>
#include <set>

namespace bf
{
  /**
   * \brief Base class for the fields of an item class.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT type_field
  {
  public:
    /** \brief The type of the field. */
    enum field_type
      {
        integer_field_type,
        u_integer_field_type,
        real_field_type,
        string_field_type,
        boolean_field_type,
        sprite_field_type,
        animation_field_type,
        item_reference_field_type,
        font_field_type,
        sample_field_type,
        color_field_type,
        easing_field_type
      }; // enum field_type

    /** \brief The type of the range of valid values. */
    enum range_type
      {
        field_range_free,
        field_range_set,
        field_range_interval
      }; // range_type

  public:
    type_field( const std::string& name, field_type ft );
    virtual ~type_field();

    virtual type_field* clone() const;

    const std::string& get_name() const;
    field_type get_field_type() const;
    virtual range_type get_range_type() const;

    virtual void get_set( std::list<std::string>& values ) const;
    virtual void get_interval( int& min, int& max ) const;
    virtual void get_interval( unsigned int& min, unsigned int& max ) const;
    virtual void get_interval( double& min, double& max ) const;

    const std::set<std::string>& get_preceding() const;
    void set_preceding( const std::list<std::string>& prec );
    void set_required( bool r );
    bool get_required() const;

    void set_description( const std::string& str );
    const std::string& get_description() const;

    void set_default_value( const std::string& str );
    const std::string& get_default_value() const;

    void set_is_list( bool v );
    bool is_list() const;

  private:
    /** \brief The name of the field. */
    const std::string m_name;

    /** \brief The type of the field. */
    const field_type m_field_type;

    /** \brief The names of the fields that must be defined before this one. */
    std::set<std::string> m_preceding;

    /** \brief Tell if this field must be filled. */
    bool m_required;

    /** \brief Tell if the field is a list of values. */
    bool m_is_list;

    /** \brief A description of the field. */
    std::string m_description;

    /** \brief A string representation of the default value of the field. */
    std::string m_default_value;

  }; // class type_field
} // namespace bf

#endif // __BF_TYPE_FIELD_HPP__
