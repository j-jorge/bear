/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::messageable_item class.
 * \author Julien Jorge.
 */

#include "communication/post_office.hpp"

#include "engine/level_globals.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<class Base>
bear::engine::messageable_item<Base>::messageable_item()
{

} // messageable_item::messageable_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the item.
 */
template<class Base>
bear::engine::messageable_item<Base>::messageable_item
( const std::string& name )
  : communication::messageable(name)
{

} // messageable_item::messageable_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
template<class Base>
void bear::engine::messageable_item<Base>::on_enters_layer()
{
  super::on_enters_layer();

  if ( get_name() != communication::post_office::no_name )
    this->get_level_globals().register_item(*this);
} // messageable_item::on_enters_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroy the item.
 */
template<class Base>
void bear::engine::messageable_item<Base>::destroy()
{
  this->get_level_globals().release_item(*this);
  super::destroy();
} // messageable_item::destroy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "string".
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
template<class Base>
bool bear::engine::messageable_item<Base>::set_string_field
( const std::string& name, const std::string& value )
{
  bool result = true;

  if (name == "messageable_item.name")
    communication::messageable::set_name(value);
  else
    result = super::set_string_field(name, value);

  return result;
} // messageable_item::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
template<class Base>
bool bear::engine::messageable_item<Base>::is_valid() const
{
  return (get_name() != communication::post_office::no_name)
    && super::is_valid();
} // messageable_item::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a string representation of the item.
 * \param str (out) The result of the convertion.
 */
template<class Base>
void bear::engine::messageable_item<Base>::to_string( std::string& str ) const
{
  std::string s;
  super::to_string(s);

  str = "name: " + get_name() + "\n" + s;
} // messageable_item::to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set/change the name of the item. The item is first released of the
 *        post_office if it was previously registered.
 * \param name The new name of the item.
 * \remark Do not call this method from the constructor of items appearing in a
 *         level file or created in the constructor of such an item.
 */
template<class Base>
bool bear::engine::messageable_item<Base>::set_name( const std::string& name )
{
  bool result = false;

  if ( name != communication::post_office::no_name )
    {
      if ( get_name() != communication::post_office::no_name )
        this->get_level_globals().release_item(*this);

      communication::messageable::set_name(name);

      result = true;
    }

  return result;
} // messageable_item::set_name()

