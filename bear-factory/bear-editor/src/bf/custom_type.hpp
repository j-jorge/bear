/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Base class for simple types (integer, real, bool and string).
 * \author Julien Jorge
 */
#ifndef __BF_CUSTOM_TYPE__
#define __BF_CUSTOM_TYPE__

#include <string>
#include "color.hpp"

namespace bf
{
  class compiled_file;

  /**
   * \brief Base class for simple types (integer, real, bool and string).
   * \author Julien Jorge
   */
  template<typename Type>
  class custom_type
  {
  public:
    typedef Type value_type;

  public:
    custom_type( const value_type& value = value_type() );

    void set_value( const value_type& value );
    const value_type& get_value() const;

    void compile( compiled_file& f ) const;

    bool operator==( const custom_type<value_type>& that ) const;
    bool operator!=( const custom_type<value_type>& that ) const;

  private:
    /** \brief The current value of the variable. */
    value_type m_value;

  }; // class custom_type

  /** \brief Integer type. */
  typedef custom_type<int> integer_type;

  /** \brief Unsigned integer type. */
  typedef custom_type<unsigned int> u_integer_type;

  /** \brief Real number type. */
  typedef custom_type<double> real_type;

  /** \brief String type. */
  typedef custom_type<std::string> string_type;

  /** \brief Boolean type. */
  typedef custom_type<bool> bool_type;

  /** \brief Base type for the files. */
  class base_file_type:
    public custom_type<std::string>
  {
  public:
    void compile( compiled_file& f ) const;
  };

  /**
   * \brief Type of a reference on an item.
   *
   * The type must be clearly different of string_type.
   */
  class item_reference_type:
    public custom_type<std::string>
  {
  public:
    bool operator==( const item_reference_type& that ) const;
    bool operator!=( const item_reference_type& that ) const;
  };

} // namespace bf

template<typename Type>
std::ostream& operator<<( std::ostream& is, const bf::custom_type<Type>& v );

#include "impl/custom_type.tpp"

#endif // __BF_CUSTOM_TYPE__
