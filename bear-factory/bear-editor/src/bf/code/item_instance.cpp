/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::item_instance class.
 * \author Julien Jorge
 */
#include "bf/item_instance.hpp"

#include "bf/call_by_field_type.hpp"
#include "bf/compilation_context.hpp"
#include "bf/item_check_result.hpp"
#include "bf/item_class.hpp"

#include <claw/assert.hpp>
#include <claw/exception.hpp>

#define SPECIALISE_FIELD_TYPE( type, name )                             \
  template<>                                                            \
  struct item_instance::field_map_by_type<type>                         \
  {                                                                     \
    static std::map<std::string, type>& get( item_instance& item )      \
    {                                                                   \
      return item.m_ ## name;                                           \
    }                                                                   \
                                                                        \
    static const std::map<std::string, type>&                           \
    get( const item_instance& item )                                    \
    {                                                                   \
      return item.m_ ## name;                                           \
    }                                                                   \
  };                                                                    \
                                                                        \
  template<>                                                            \
  struct item_instance::field_map_by_type< std::list<type> >            \
  {                                                                     \
    static std::map< std::string, std::list<type> >&                    \
    get( item_instance& item )                                          \
    {                                                                   \
      return item.m_ ## name ## _list;                                  \
    }                                                                   \
                                                                        \
    static const std::map< std::string, std::list<type> >&              \
    get( const item_instance& item )                                    \
    {                                                                   \
      return item.m_ ## name ## _list;                                  \
    }                                                                   \
  }

namespace bf
{
  SPECIALISE_FIELD_TYPE( integer_type, int );
  SPECIALISE_FIELD_TYPE( u_integer_type, u_int );
  SPECIALISE_FIELD_TYPE( real_type, real );
  SPECIALISE_FIELD_TYPE( bool_type, bool );
  SPECIALISE_FIELD_TYPE( string_type, string );
  SPECIALISE_FIELD_TYPE( sprite, sprite );
  SPECIALISE_FIELD_TYPE( any_animation, animation );
  SPECIALISE_FIELD_TYPE( item_reference_type, item_reference );
  SPECIALISE_FIELD_TYPE( font, font );
  SPECIALISE_FIELD_TYPE( sample, sample ); 
  SPECIALISE_FIELD_TYPE( color, color );
} // namespace bf




/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a value has been given to a field.
 * \param item The item instance in which the field is searched.
 * \param field_name The name of the searched field.
 */
template<typename T>
bool bf::item_instance::field_has_value<T>::operator()
  ( const item_instance& item, const std::string& field_name ) const
{
  return field_map_by_type<T>::get(item).find(field_name)
    != field_map_by_type<T>::get(item).end();
} // item_instance::field_has_value::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Erase the value of a field.
 * \param item The item instance in which the field is erased.
 * \param field_name The name of the erased field.
 */
template<typename T>
void bf::item_instance::erase_field_value<T>::operator()
  ( item_instance& item, const std::string& field_name ) const
{
  field_map_by_type<T>::get(item).erase(field_name);
} // item_instance::erase_field_value::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param c The class I am an instance of.
 */
bf::item_instance::item_instance( item_class const* c )
  : m_class( c ), m_fixed( m_class->get_fixable() ),
    m_rendering_parameters(*this)
{
  m_rendering_parameters.init();
} // item_instance::item_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bf::item_instance::item_instance( const item_instance& that )
  : m_class(that.m_class), m_fixed( that.get_fixed() ),
    m_id(that.get_id()),
    m_rendering_parameters(*this)
{
  assign(that);
} // item_instance::item_instance()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bf::item_instance& bf::item_instance::operator=( const item_instance& that )
{
  if ( this != &that )
    assign(that);

  return *this;
} // item_instance::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the values of the fields of an other instance.
 * \param that The instance to copy from.
 */
void bf::item_instance::copy_fields_from( const item_instance& that )
{
  copy_fields_from_no_invalid_values( that );
  remove_invalid_values();

  m_rendering_parameters.reset();
} // item_instance::()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering parameters of the item.
 */
const bf::item_rendering_parameters&
bf::item_instance::get_rendering_parameters() const
{
  return m_rendering_parameters;
} // item_instance::get_rendering_parameters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the rendering parameters of the item.
 */
bf::item_rendering_parameters& bf::item_instance::get_rendering_parameters()
{
  return m_rendering_parameters;
} // item_instance::get_rendering_parameters()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the class I am an instance of.
 */
const bf::item_class& bf::item_instance::get_class() const
{
  return *m_class;
} // item_instance::get_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer to the class I am an instance of.
 */
bf::item_class const* bf::item_instance::get_class_ptr() const
{
  return m_class;
} // item_instance::get_class_ptr()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the class of the item.
 * \param c The new class of the item.
 * \pre c!=NULL
 */
void bf::item_instance::set_class( item_class const* c )
{
  CLAW_PRECOND( c!=NULL );

  m_class = c;
  m_fixed = m_class->get_fixable() && m_fixed;

  remove_invalid_values();

  m_rendering_parameters.reset();
} // item_instance::set_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the the identifier of this item.
 */
const std::string& bf::item_instance::get_id() const
{
  return m_id;
} // item_instance::get_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the the identifier of this item.
 * \param id The new identifier of this item.
 */
void bf::item_instance::set_id( const std::string& id )
{
  m_id = id;
} // item_instance::set_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Rename the identifiers in fields of type item_reference.
 * \param map_id The map of idendifier's modification.
 */
void bf::item_instance::rename_item_reference_fields
( const std::map<std::string, std::string>& map_id)
{
  std::list<std::string> fields;
  std::list<std::string>::const_iterator it;

  m_class->get_field_names_in_hierarchy(fields);

  for ( it=fields.begin(); it!=fields.end(); ++it )
    {
      const type_field& f = m_class->get_field(*it);

      if( f.get_field_type() == type_field::item_reference_field_type )
        {
          if( has_value(f) )
            {
              if( f.is_list() )
                {
                  std::list<item_reference_type>::iterator bit =
                    m_item_reference_list.find(f.get_name())->second.begin();
                  const std::list<item_reference_type>::iterator eit =
                    m_item_reference_list.find(f.get_name())->second.end();

                  for ( ; bit!=eit; ++bit)
                    if ( map_id.find(bit->get_value()) != map_id.end() )
                      bit->set_value(map_id.find(bit->get_value())->second);
                }
              else
                {
                  std::string id;
                  id = m_item_reference.find(f.get_name())->second.get_value();

                  if ( map_id.find(id) != map_id.end() )
                    m_item_reference.find
                      (f.get_name())->second.set_value(map_id.find(id)->second);
                }
            }
        }
    }
} // item_instance::rename_item_reference_fields()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a field has a value.
 * \param field_name The name of the field to check.
 */
bool bf::item_instance::has_value( const std::string& field_name ) const
{
  return has_value( get_class().get_field(field_name) );
} // item_instance::has_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if a field has a value.
 * \param f The field to check.
 */
bool bf::item_instance::has_value( const type_field& f ) const
{
  call_by_field_type<field_has_value, bool> call;
  return call(f, *this, f.get_name());
} // item_instance::has_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete the value of a field.
 * \param f The field to delete.
 */
void bf::item_instance::delete_value( const type_field& f )
{
  call_by_field_type<erase_field_value, void> call;
  call(f, *this, f.get_name());
  m_rendering_parameters.field_changed( f.get_name() );
} // item_instance::delete_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item instance has a reference to a given other instance.
 * \param id The identifier of the other instance.
 */
bool bf::item_instance::has_a_reference_to( const std::string& id ) const
{
  item_reference_type_map::const_iterator it;

  for ( it=m_item_reference.begin(); it!=m_item_reference.end(); ++it )
    if ( it->second.get_value() == id )
      return true;

  item_reference_type_list_map::const_iterator it_list;

  for ( it_list=m_item_reference_list.begin();
      it_list!=m_item_reference_list.end(); ++it_list )
    {
      std::list<item_reference_type>::const_iterator iti;

      for ( iti=it_list->second.begin(); iti!=it_list->second.end(); ++iti )
        if ( iti->get_value() == id )
          return true;
    }

  return false;
} // item_instance::has_a_reference_to()


/*----------------------------------------------------------------------------*/
/**
 * \brief Get names of reference item fields.
 * \param item_reference_fields List of item_reference fields (out).
 * \param item_reference_list_fields List of item_reference list fields (out).
 */
void bf::item_instance::get_item_reference_field_names
( std::set<std::string> & item_reference_fields, 
  std::set<std::string> & item_reference_list_fields ) const
{
  std::list<std::string> all_fields;
  get_class().get_field_names_in_hierarchy( all_fields );

  std::list<std::string>::const_iterator it;
  for ( it = all_fields.begin(); it != all_fields.end(); ++it )
    {
      type_field field = get_class().get_field(*it);
      if ( field.get_field_type() == type_field::item_reference_field_type )
        {
          if ( field.is_list() )
            item_reference_list_fields.insert( field.get_name() );
          else
            item_reference_fields.insert( field.get_name() );
        }
    }
} // item_instance::get_item_reference_field_names()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is fixed.
 */
bool bf::item_instance::get_fixed() const
{
  return m_fixed && m_class->get_fixable();
} // item_instance::get_fixed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that this item is fixed.
 * \param b Fixed or not ?
 */
void bf::item_instance::set_fixed( bool b )
{
  m_fixed = b;
} // item_instance::set_fixed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the fields of the item.
 * \param f The file in which we compile.
 * \param c The context in which the compilation is done.
 */
void bf::item_instance::compile
( compiled_file& f, compilation_context& c ) const
{
  f << get_fixed();

  std::list<std::string> fields;

  for( sort_fields(fields); !fields.empty(); fields.pop_front() )
    compile_field( f, m_class->get_field(fields.front()), c );
} // item_instance::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the item is well defined.
 * \param result (out) The result of the check.
 * \param map_id The set of all item identifiers.
 */
void bf::item_instance::check
( item_check_result& result, const std::set<std::string>& map_id ) const
{
  check_required_fields( result );
  check_mass_for_fixed_item( result );
  check_id_required(result, map_id);
} // item_instance::check()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the fixed item has an infinite mass.
 * \param result (out) The result of the check.
 */
void bf::item_instance::check_mass_for_fixed_item
( item_check_result& result ) const
{
  if ( m_fixed )
    {
      const type_field field("base_item.mass", type_field::real_field_type);

      if ( has_value(field) )
        result.add( check_error("Fixed item must have an infinite mass.") );
    }
} // item_instance::check_mass_for_fixed_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if the referenced items exist.
 * \param result (out) The result of the check.
 * \param map_id The set of all item identifiers.
 */
void bf::item_instance::check_id_required
( item_check_result& result, const std::set<std::string>& map_id ) const
{
  std::list<std::string> fields;
  std::list<std::string>::const_iterator it;

  m_class->get_field_names_in_hierarchy(fields);

  for ( it=fields.begin(); it!=fields.end(); ++it )
    {
      const type_field& f = m_class->get_field(*it);
      if( (f.get_field_type() == type_field::item_reference_field_type)
          && has_value(f) )
        {
          if( f.is_list() )
            {
              std::list<item_reference_type>::const_iterator bit =
                m_item_reference_list.find(f.get_name())->second.begin();
              const std::list<item_reference_type>::const_iterator eit =
                m_item_reference_list.find(f.get_name())->second.end();

              for ( ; bit!=eit; ++bit)
                if ( map_id.find(bit->get_value()) == map_id.end() )
                  result.add
                    ( check_error
                      ( bit->get_value(),
                        "There is no item with such identifier." ) );
            }
          else
            {
              std::string id =
                m_item_reference.find(f.get_name())->second.get_value();

              if ( map_id.find(id) == map_id.end() )
                result.add
                  ( check_error
                    ( id, "There is no item with such identifier." ) );
            }
        }
    }
} // item_instance::check_id_required()

/*----------------------------------------------------------------------------*/
/**
 * \brief Test if all required fields are set.
 * \param result (out) The result of the check.
 */
void bf::item_instance::check_required_fields( item_check_result& result ) const
{
  std::list<std::string> fields;
  std::list<std::string>::const_iterator it;

  m_class->get_field_names_in_hierarchy(fields);

  for ( it=fields.begin(); it!=fields.end(); ++it )
    {
      const type_field& f = m_class->get_field(*it);
      if( f.get_required() && !has_value(f) )
        result.add( check_error( *it, "Field value is required." ) );
    }
} // item_instance::check_required_fields()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the fields, ordered so that fields depending on other
 *        fields are after them in the list.
 * \param fields (out) The name of the fields.
 */
void bf::item_instance::sort_fields( std::list<std::string>& fields ) const
{
  std::set<std::string> all_fields;

  copy_field_names( m_int, all_fields );
  copy_field_names( m_u_int, all_fields );
  copy_field_names( m_real, all_fields );
  copy_field_names( m_bool, all_fields );
  copy_field_names( m_string, all_fields );
  copy_field_names( m_sprite, all_fields );
  copy_field_names( m_animation, all_fields );
  copy_field_names( m_item_reference, all_fields );
  copy_field_names( m_font, all_fields );
  copy_field_names( m_sample, all_fields );
  copy_field_names( m_color, all_fields );
  copy_field_names( m_int_list, all_fields );
  copy_field_names( m_u_int_list, all_fields );
  copy_field_names( m_real_list, all_fields );
  copy_field_names( m_bool_list, all_fields );
  copy_field_names( m_string_list, all_fields );
  copy_field_names( m_sprite_list, all_fields );
  copy_field_names( m_animation_list, all_fields );
  copy_field_names( m_item_reference_list, all_fields );
  copy_field_names( m_font_list, all_fields );
  copy_field_names( m_sample_list, all_fields );
  copy_field_names( m_color_list, all_fields );

  while ( !all_fields.empty() )
    {
      const std::string field_name( *all_fields.begin() );
      insert_field( field_name, fields, all_fields );
    }
} // item_instance::sort_fields()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the fields, ordered so that fields depending on other
 *        fields are after them in the list.
 * \param field_name The name of the field to insert.
 * \param fields (out) The name of the fields.
 * \param all_fields (in/out) All the remaining fields to insert.
 */
void bf::item_instance::insert_field
( const std::string& field_name, std::list<std::string>& fields,
  std::set<std::string>& all_fields ) const
{
  if ( all_fields.find(field_name) != all_fields.end() )
    {
      all_fields.erase(field_name);

      std::set<std::string>::const_iterator it;

      for ( it=m_class->get_field(field_name).get_preceding().begin();
            it!=m_class->get_field(field_name).get_preceding().end(); ++it )
        insert_field(*it, fields, all_fields);

      fields.push_back( field_name );
    }
} // item_instance::insert_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a field.
 * \param f The file in which we compile.
 * \param field The field to save.
 * \param c The context in which the compilation is done.
 */
void bf::item_instance::compile_field
( compiled_file& f, const type_field& field, compilation_context& c ) const
{
  CLAW_PRECOND( has_value(field) );

  if ( field.is_list() )
    f << bear::level_code_value::field_list;

  f << get_code_value(field) << field.get_name();

  if ( field.is_list() )
    compile_field_list(f, field, c);
  else
    compile_field_single(f, field, c);
} // item_instance::compile_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a field made of a single values.
 * \param f The file in which we compile.
 * \param field The field to save.
 * \param c The context in which the compilation is done.
 */
void bf::item_instance::compile_field_single
( compiled_file& f, const type_field& field, compilation_context& c ) const
{
  CLAW_PRECOND( has_value(field) );
  const std::string& field_name = field.get_name();

  switch ( field.get_field_type() )
    {
    case type_field::integer_field_type:
      m_int.find(field_name)->second.compile(f);
      break;
    case type_field::u_integer_field_type:
      m_u_int.find(field_name)->second.compile(f);
      break;
    case type_field::real_field_type:
      m_real.find(field_name)->second.compile(f);
      break;
    case type_field::boolean_field_type:
      m_bool.find(field_name)->second.compile(f);
      break;
    case type_field::string_field_type:
      m_string.find(field_name)->second.compile(f);
      break;
    case type_field::sprite_field_type:
      m_sprite.find(field_name)->second.compile(f, c);
      break;
    case type_field::animation_field_type:
      m_animation.find(field_name)->second.compile(f, c);
      break;
    case type_field::item_reference_field_type:
      f << c.get_compiled_identifier
        ( m_item_reference.find(field_name)->second.get_value() );
      break;
    case type_field::font_field_type:
      m_font.find(field_name)->second.compile(f);
      break;
    case type_field::sample_field_type:
      m_sample.find(field_name)->second.compile(f);
      break; 
    case type_field::color_field_type:
      m_color.find(field_name)->second.compile(f);
      break;
    }
} // item_instance::compile_field_single()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a field made of a list of values.
 * \param f The file in which we compile.
 * \param field The field to save.
 * \param c The context in which the compilation is done.
 */
void bf::item_instance::compile_field_list
( compiled_file& f, const type_field& field, compilation_context& c ) const
{
  CLAW_PRECOND( has_value(field) );
  const std::string& field_name = field.get_name();

  switch ( field.get_field_type() )
    {
    case type_field::integer_field_type:
      compile_list( f, m_int_list.find(field_name)->second );
      break;
    case type_field::u_integer_field_type:
      compile_list( f, m_u_int_list.find(field_name)->second );
      break;
    case type_field::real_field_type:
      compile_list( f, m_real_list.find(field_name)->second );
      break;
    case type_field::boolean_field_type:
      compile_list( f, m_bool_list.find(field_name)->second );
      break;
    case type_field::string_field_type:
      compile_list( f, m_string_list.find(field_name)->second );
      break;
    case type_field::sprite_field_type:
      compile_list( f, c, m_sprite_list.find(field_name)->second );
      break;
    case type_field::animation_field_type:
      compile_list( f, c, m_animation_list.find(field_name)->second );
      break;
    case type_field::item_reference_field_type:
      {
        f << m_item_reference_list.find(field_name)->second.size();

        std::list<item_reference_type>::const_iterator it =
          m_item_reference_list.find(field_name)->second.begin();
        const std::list<item_reference_type>::const_iterator eit =
          m_item_reference_list.find(field_name)->second.end();

        for ( ; it!=eit; ++it)
          f << c.get_compiled_identifier(it->get_value());

        break;
      }
    case type_field::font_field_type:
      compile_list( f, m_font_list.find(field_name)->second );
      break;
    case type_field::sample_field_type:
      compile_list( f, m_sample_list.find(field_name)->second );
      break;
    case type_field::color_field_type:
      compile_list( f, m_color_list.find(field_name)->second );
      break;
    }
} // item_instance::compile_field_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the compiler code value for type of field.
 * \param field The field forwhich we want the value.
 */
bear::level_code_value::value_type
bf::item_instance::get_code_value( const type_field& field ) const
{
  bear::level_code_value::value_type result;

  switch ( field.get_field_type() )
    {
    case type_field::integer_field_type:
      result = bear::level_code_value::field_int;
      break;
    case type_field::u_integer_field_type:
      result = bear::level_code_value::field_u_int;
      break;
    case type_field::real_field_type:
      result = bear::level_code_value::field_real;
      break;
    case type_field::boolean_field_type:
      result = bear::level_code_value::field_bool;
      break;
    case type_field::string_field_type:
      result = bear::level_code_value::field_string;
      break;
    case type_field::sprite_field_type:
      result = bear::level_code_value::field_sprite;
      break;
    case type_field::animation_field_type:
      result = bear::level_code_value::field_animation;
      break;
    case type_field::item_reference_field_type:
      result = bear::level_code_value::field_item;
      break;
    case type_field::font_field_type:
      result = bear::level_code_value::field_font;
      break;
    case type_field::sample_field_type:
      result = bear::level_code_value::field_sample;
      break;
    case type_field::color_field_type:
      result = bear::level_code_value::field_color;
      break;
    
    default:
      {
        throw new claw::exception( "Invalid field type." );
      }
    }

  return result;
} // item_instance::get_code_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assign the value from an other item.
 * \param that The instance to copy from.
 */
void bf::item_instance::assign( const item_instance& that )
{
  m_class = that.m_class;
  m_fixed = that.get_fixed();
  m_id = that.get_id();

  copy_fields_from_no_invalid_values( that );

  m_rendering_parameters.reset();
} // item_instance::assign()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy the values of the fields of an other instance without removing
 *        the invalid values.
 * \param that The instance to copy the fields from.
 */
void bf::item_instance::copy_fields_from_no_invalid_values
( const item_instance& that )
{
  m_int = that.m_int;
  m_int_list = that.m_int_list;
  m_u_int = that.m_u_int;
  m_u_int_list = that.m_u_int_list;
  m_real = that.m_real;
  m_real_list = that.m_real_list;
  m_bool = that.m_bool;
  m_bool_list = that.m_bool_list;
  m_string = that.m_string;
  m_string_list = that.m_string_list;
  m_sprite = that.m_sprite;
  m_sprite_list = that.m_sprite_list;
  m_animation = that.m_animation;
  m_animation_list = that.m_animation_list;
  m_item_reference = that.m_item_reference;
  m_item_reference_list = that.m_item_reference_list;
  m_font = that.m_font;
  m_font_list = that.m_font_list;
  m_sample = that.m_sample;
  m_sample_list = that.m_sample_list;
  m_color = that.m_color;
  m_color_list = that.m_color_list;
} // item_instance::copy_fields_from_no_invalid_values()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the values set for some fields not present in the item class.
 */
void bf::item_instance::remove_invalid_values()
{
  remove_invalid_values( m_int, m_int_list, type_field::integer_field_type );
  remove_invalid_values
    ( m_u_int, m_u_int_list, type_field::u_integer_field_type );
  remove_invalid_values( m_real, m_real_list, type_field::real_field_type );
  remove_invalid_values( m_bool, m_bool_list, type_field::boolean_field_type );
  remove_invalid_values
    ( m_string, m_string_list, type_field::string_field_type );
  remove_invalid_values
    ( m_sprite, m_sprite_list, type_field::sprite_field_type );
  remove_invalid_values
    ( m_animation, m_animation_list, type_field::animation_field_type );
  remove_invalid_values
    ( m_item_reference, m_item_reference_list,
      type_field::item_reference_field_type );
  remove_invalid_values
    ( m_font, m_font_list, type_field::font_field_type );
  remove_invalid_values
    ( m_sample, m_sample_list, type_field::sample_field_type );
  remove_invalid_values
    ( m_color, m_color_list, type_field::color_field_type );
} // item_instance::set_class()

/*----------------------------------------------------------------------------*/
/**
 * \brief Extract the names of defined fields.
 * \param m The map in which we take the fields names.
 * \param fields The set in which the fields are saved.
 */
template<typename MapType>
void bf::item_instance::copy_field_names
( const MapType& m, std::set<std::string>& fields ) const
{
  typename MapType::const_iterator it;

  for (it=m.begin(); it!=m.end(); ++it)
    fields.insert(it->first);
} // item_instance::copy_field_names()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a list of values.
 * \param f The file in which we compile.
 * \param v The values to save.
 */
template<typename T>
void bf::item_instance::compile_list
( compiled_file& f, const std::list<T>& v ) const
{
  f << v.size();

  typename std::list<T>::const_iterator it = v.begin();
  const typename std::list<T>::const_iterator eit = v.end();

  for ( ; it!=eit; ++it)
    it->compile(f);
} // item_instance::compile_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a list of values requiring a compilation context.
 * \param f The file in which we compile.
 * \param v The values to save.
 * \param c The context in which the compilation is done.
 */
template<typename T>
void bf::item_instance::compile_list
( compiled_file& f, compilation_context& c, const std::list<T>& v ) const
{
  f << v.size();

  typename std::list<T>::const_iterator it = v.begin();
  const typename std::list<T>::const_iterator eit = v.end();

  for ( ; it!=eit; ++it)
    it->compile(f, c);
} // item_instance::compile_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the field values that are not available for the class of this
 *        item.
 * \param m The map of values.
 * \param ml The map of list of values.
 * \param t The type of the fields explored.
 */
template<typename T>
void bf::item_instance::remove_invalid_values
( std::map<std::string, T>& m, std::map< std::string, std::list<T> >& ml,
  type_field::field_type t ) const
{
  std::list<std::string> to_remove;
  typename std::map<std::string, T>::const_iterator it;

  for (it=m.begin(); it!=m.end(); ++it)
    if ( !m_class->has_field( it->first, t ) )
      to_remove.push_front(it->first);
    else
      {
        const type_field& f = m_class->get_field(it->first);
        if ( f.is_list() )
          to_remove.push_front(it->first);
      }

  for ( ; !to_remove.empty(); to_remove.pop_front() )
    m.erase( to_remove.front() );

  typename std::map< std::string, std::list<T> >::const_iterator itl;

  for (itl=ml.begin(); itl!=ml.end(); ++itl)
    if ( !m_class->has_field( itl->first, t ) )
      to_remove.push_front(itl->first);
    else
      {
        const type_field& f = m_class->get_field(itl->first);
        if ( !f.is_list() )
          to_remove.push_front(itl->first);
      }

  for ( ; !to_remove.empty(); to_remove.pop_front() )
    ml.erase( to_remove.front() );
} // item_instance::remove_invalid_values()
