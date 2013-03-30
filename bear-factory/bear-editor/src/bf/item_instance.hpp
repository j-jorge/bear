/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An instance of an item class.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_INSTANCE_HPP__
#define __BF_ITEM_INSTANCE_HPP__

#include <map>
#include <string>
#include <set>
#include <list>
#include <vector>

#include <claw/assert.hpp>

#include "bf/any_animation.hpp"
#include "bf/color.hpp"
#include "bf/custom_type.hpp"
#include "bf/font.hpp"
#include "bf/item_rendering_parameters.hpp"
#include "bf/sample.hpp"
#include "bf/type_field.hpp"
#include "bf/libeditor_export.hpp"

#include "level_code_value.hpp"

/**
 * \brief Declare the member variables and accessors for a field of a given
 *        type.
 * \param type The type of the field.
 * \param name Root name of the variables created.
 *
 * This macro declares two private variables named m_(name) and m_(name)_list,
 * of type std::map<std::string, (type)> and std::map<std::string,
 * std::list<(type)> >.
 *
 * Methods to access these variables are also created:
 *
 *  void set_value( const std::string&, const (type)& )
 *  void set_value( const std::string&, const std::list<(type)>& )
 *  void get_value( const std::string&, (type)& ) const
 *  void get_value( const std::string&, std::list<(type)>& ) const
 *  void get_value( const type_field&, (type)& ) const
 *  void get_value( const type_field&, std::list<(type)>& ) const
 */
#define FIELD_TYPE( type, name )                                        \
  public:                                                               \
  void set_value( const std::string& field_name, const type& v )        \
  {                                                                     \
    m_ ## name[field_name] = v;                                         \
    m_rendering_parameters.field_changed(field_name);                   \
  }                                                                     \
                                                                        \
  void                                                                  \
  set_value( const std::string& field_name, const std::list<type>& v )  \
  {                                                                     \
    m_ ## name ## _list[field_name] = v;                                \
    m_rendering_parameters.field_changed(field_name);                   \
  }                                                                     \
                                                                        \
  void get_value( const std::string& field_name, type& v ) const        \
  {                                                                     \
    CLAW_PRECOND( m_ ## name.find(field_name) != m_ ## name.end() );    \
    v = m_ ## name.find(field_name)->second;                            \
  }                                                                     \
                                                                        \
  void                                                                  \
  get_value( const std::string& field_name, std::list<type>& v ) const  \
  {                                                                     \
    CLAW_PRECOND( m_ ## name ## _list.find(field_name)                  \
                  != m_ ## name ## _list.end() );                       \
    v = m_ ## name ## _list.find(field_name)->second;                   \
  }                                                                     \
                                                                        \
  void get_value( const type_field& f, type& v ) const                  \
  {                                                                     \
    get_value( f.get_name(), v );                                       \
  }                                                                     \
                                                                        \
  void get_value( const type_field& f, std::list<type>& v ) const       \
  {                                                                     \
    get_value( f.get_name(), v );                                       \
  }                                                                     \
                                                                        \
  private:                                                              \
  typedef std::map<std::string, type> type ## _map;                     \
  typedef std::map< std::string, std::list<type> > type ## _list_map;   \
                                                                        \
  private:                                                              \
  type ## _map m_ ## name;                                              \
  type ## _list_map m_ ## name ## _list;                                \
                                                                        \
  friend struct field_map_by_type<type>;                                \
  friend struct field_map_by_type< std::list<type> >

namespace bf
{
  class item_check_result;
  class item_class;

  /**
   * \brief An instance of an item class.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT item_instance
  {
  private:
    template<typename T> struct field_map_by_type;

    /**
     * \brief Function object to tell if a value has been given to a field.
     *
     * \b Template \b parameters:
     * - \param T The type of the field data.
     */
    template<typename T>
    struct field_has_value
    {
      bool operator()
        ( const item_instance& item, const std::string& field_name ) const;
    }; // field_has_value

    /**
     * \brief Function object to erase the value of a field.
     *
     * \b Template \b parameters:
     * - \param T The type of the field data.
     */
    template<typename T>
    struct erase_field_value
    {
      void operator()
        ( item_instance& item, const std::string& field_name ) const;
    }; // erase_field_value

  public:
    item_instance( item_class const* c );
    item_instance( const item_instance& that );

    item_instance& operator=( const item_instance& that );

    void copy_fields_from( const item_instance& that );

    const item_rendering_parameters& get_rendering_parameters() const;
    item_rendering_parameters& get_rendering_parameters();

    const item_class& get_class() const;
    item_class const* get_class_ptr() const;
    void set_class( item_class const* c );

    const std::string& get_id() const;
    void set_id( const std::string& id );

    bool has_value( const std::string& field_name ) const;
    bool has_value( const type_field& f ) const;
    void delete_value( const type_field& f );

    bool has_a_reference_to( const std::string& id ) const;
    void get_item_reference_field_names
      ( std::set<std::string> & item_reference_fields, 
        std::set<std::string> & item_reference_list_fields ) const;

    bool get_fixed() const;
    void set_fixed( bool b );
    void rename_item_reference_fields
    ( const std::map<std::string, std::string>& map_id );

    void compile( compiled_file& f, compilation_context& c ) const;

    void check
      ( item_check_result& result, const std::set<std::string>& map_id ) const;

  private:
    void check_mass_for_fixed_item( item_check_result& result ) const;
    void check_id_required
      ( item_check_result& result, const std::set<std::string>& map_id ) const;
    void check_required_fields( item_check_result& result ) const;

    void sort_fields( std::list<std::string>& fields ) const;
    void insert_field
    ( const std::string& field_name, std::list<std::string>& fields,
      std::set<std::string>& all_fields ) const;

    void compile_field
    ( compiled_file& f, const type_field& field, compilation_context& c ) const;
    void compile_field_single
    ( compiled_file& f, const type_field& field, compilation_context& c ) const;
    void compile_field_list
    ( compiled_file& f, const type_field& field, compilation_context& c ) const;

    bear::level_code_value::value_type
    get_code_value( const type_field& field ) const;

    void assign( const item_instance& that );
    void copy_fields_from_no_invalid_values( const item_instance& that );
    void remove_invalid_values();

    template<typename MapType>
    void
    copy_field_names( const MapType& m, std::set<std::string>& fields ) const;

    template<typename T>
    void compile_list( compiled_file& f, const std::list<T>& v ) const;
    template<typename T>
    void compile_list
      ( compiled_file& f, compilation_context& c, const std::list<T>& v ) const;

    template<typename T>
    void remove_invalid_values
    ( std::map<std::string, T>& m, std::map< std::string, std::list<T> >& ml,
      type_field::field_type t ) const;

  private:
    /** \brief The item class I am an instance of. */
    item_class const* m_class;

    /** \brief Tell if the item is fixed. */
    bool m_fixed;

    /** \brief The identifier of this item, for references with other items. */
    std::string m_id;

    /** \brief Rendering parameters of the instance. */
    item_rendering_parameters m_rendering_parameters;

    FIELD_TYPE( integer_type, int );
    FIELD_TYPE( u_integer_type, u_int );
    FIELD_TYPE( real_type, real );
    FIELD_TYPE( bool_type, bool );
    FIELD_TYPE( string_type, string );
    FIELD_TYPE( sprite, sprite );
    FIELD_TYPE( any_animation, animation );
    FIELD_TYPE( item_reference_type, item_reference );
    FIELD_TYPE( font, font );
    FIELD_TYPE( sample, sample );
    FIELD_TYPE( color, color );
    FIELD_TYPE( easing_type, easing );

  }; // class item_instance
} // namespace bf

#endif // __BF_ITEM_INSTANCE_HPP__
