/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::speaker_item_loader class.
 * \author Julien Jorge.
 */
#include "engine/comic/loader/speaker_item_loader.hpp"

#include "engine/comic/item/speaker_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param prefix The prefix of the fields that can be set through this loader.
 * \param item The item loaded by this loader.
 */
bear::engine::speaker_item_loader::speaker_item_loader
( std::string prefix, speaker_item& item )
  : item_loader_base( prefix ), m_item(item)
{

} // speaker_item_loader::speaker_item_loader()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this object.
 */
bear::engine::speaker_item_loader*
bear::engine::speaker_item_loader::clone() const
{
  return new speaker_item_loader(*this);
} // speaker_item_loader::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type liste of strings.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::speaker_item_loader::set_field
( const std::string& name,  const std::vector<std::string>& value )
{
  bool ok(true);

  if (name == "speeches")
    m_item.speak( value );
  else
    ok = false;

  return ok;
} // speaker_item_loader::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::speaker_item_loader::set_field
( const std::string& name, bool value )
{
  bool ok(true);

  if (name == "persistent_balloon")
    m_item.set_persistent_balloon( value );
  else
    ok = false;

  return ok;
} // speaker_item_loader::set_field()
