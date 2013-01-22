/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class that stores the type of the fields of an item class.
 * \author Julien Jorge
 */
#ifndef __BF_ITEM_CLASS_HPP__
#define __BF_ITEM_CLASS_HPP__

#include <string>
#include <map>
#include <list>
#include <claw/functional.hpp>
#include <claw/iterator.hpp>

#include "bf/type_field.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief A class that stores the type of the fields of an item class.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT item_class
  {
  public:
    /** \brief The list of parent classes. */
    typedef std::list<item_class const*> super_class_list;

    /** \brief The association of a field's name with a description of his
        name. */
    typedef std::map<std::string, const type_field*> field_map_type;

    /** \brief Iterator on the fields of the class. */
    typedef claw::wrapped_iterator
    < const type_field,
      field_map_type::const_iterator,
      claw::unary_compose
      < claw::const_dereference<type_field>,
        claw::const_pair_second<field_map_type::value_type> > >
    ::iterator_type field_iterator;

    /** \brief Iterator on the parent classes. */
    typedef claw::wrapped_iterator
    < const item_class,
      super_class_list::const_iterator,
      claw::const_dereference<item_class> >
    ::iterator_type const_super_class_iterator;

  private:
    typedef std::map<std::string, std::string> string_map_type;

  public:
    item_class();
    item_class( const item_class& that );
    ~item_class();

    item_class& operator=(const item_class& that);

    field_iterator field_begin() const;
    field_iterator field_end() const;
    const_super_class_iterator super_class_begin() const;
    const_super_class_iterator super_class_end() const;

    void add_field( const std::string& name, const type_field& field );

    void set_description( const std::string& desc );
    void set_url( const std::string& url );
    void set_class_name( const std::string& class_name );
    void set_category( const std::string& category );
    void set_color( const std::string& color );
    void set_fixable( bool fixable );

    bool inherits_from( const std::string& super_class ) const;
    bool has_super_class( const std::string& super_class ) const;
    void add_super_class( item_class const* super_class );
    void remove_super_class( const std::string& super_class );

    void new_default_value( const std::string& f, const std::string& v );
    std::string get_default_value( const std::string& f ) const;

    void add_removed_field( const std::string& f );
    bool is_removed_field( const std::string& f ) const;

    void get_field_names_in_hierarchy( std::list<std::string>& f ) const;

    const type_field& get_field( const std::string& field_name ) const;
    const std::string& get_description() const;
    const std::string& get_url() const;
    const std::string& get_class_name() const;
    const std::string& get_category() const;
    const std::string& get_color() const;
    bool get_fixable() const;
    bool get_need_a_name() const;
    void find_hierarchy( std::list<item_class const*>& hierarchy ) const;

    bool has_field( const std::string& name ) const;
    bool has_field( const std::string& name, type_field::field_type t ) const;

    bool field_unicity_test(std::string& error_msg) const;

  private:
    void clear();
    void copy( const item_class& that );

    const type_field* search_field( const std::string& field_name ) const;

    void
    get_removed_fields_names_in_hierarchy( std::list<std::string>& f ) const;
    void get_all_field_names_in_hierarchy( std::list<std::string>& f ) const;

  private:
    /** \brief The name of the class. */
    std::string m_class_name;

    /** \brief A short description of this class. */
    std::string m_description;

    /** \brief An URL where this item is documented. */
    std::string m_url;

    /** \brief The category in which this class fits. */
    std::string m_category;

    /** \brief The color used for displaying the item. */
    std::string m_color;

    /** \brief Tell if the item can be fixed. */
    bool m_fixable;

    /** \brief The classes from which this one inherit. */
    super_class_list m_super_classes;

    /** \brief The fields of this class. */
    field_map_type m_field;

    /** \brief The default values for the fields of the class. */
    string_map_type m_default_value;

    /** \brief The fields of the parent classes that will be set by this
        class. */
    std::list<std::string> m_removed_fields;

  }; // class item_class
} // namespace bf

#endif // __BF_ITEM_CLASS_HPP__
