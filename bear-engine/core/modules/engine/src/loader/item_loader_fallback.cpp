/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_loader_fallback class.
 * \author Julien Jorge.
 */
#include "bear/engine/loader/item_loader_fallback.hpp"

#include "bear/engine/base_item.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param item The item to which the fields' values are passed.
 */
bear::engine::item_loader_fallback::item_loader_fallback( base_item& item )
  : item_loader_base("- fallback- "), m_item( item )
{

} // item_loader_fallback::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this instance.
 */
bear::engine::item_loader_fallback*
bear::engine::item_loader_fallback::clone() const
{
  return new item_loader_fallback(*this);
} // item_loader_fallback::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c unsigned \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, unsigned int value )
{
  return m_item.set_u_integer_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, int value )
{
  return m_item.set_integer_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, double value )
{
  return m_item.set_real_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, bool value )
{
  return m_item.set_bool_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::string& value )
{
  return m_item.set_string_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, base_item* value )
{
  return m_item.set_item_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::sprite.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const visual::sprite& value )
{
  return m_item.set_sprite_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const visual::animation& value )
{
  return m_item.set_animation_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c audio::sample.
 * \param name The name of the field.
 * \param value The new value of the field. The item receiving this value must
 *        release de memory with a delete at some time.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, audio::sample* value )
{
  return m_item.set_sample_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::font.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, visual::font value )
{
  return m_item.set_font_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, visual::color value )
{
  return m_item.set_color_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of unsigned integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<unsigned int>& value )
{
  return m_item.set_u_integer_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<int>& value )
{
  return m_item.set_integer_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<double>& value )
{
  return m_item.set_real_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<bool>& value )
{
  return m_item.set_bool_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<std::string>& value )
{
  return m_item.set_string_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<base_item*>& value )
{
  return m_item.set_item_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::sprite.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<visual::sprite>& value )
{
  return m_item.set_sprite_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<visual::animation>& value )
{
  return m_item.set_animation_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of audio::sample.
 * \param name The name of the field.
 * \param value The new value of the field. The item receiving this value must
 *        release de memory with a delete at some time.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<audio::sample*>& value )
{
  return m_item.set_sample_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::font.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<visual::font>& value )
{
  return m_item.set_font_list_field( name, value );
} // item_loader_fallback::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_fallback::set_field
( const std::string& name, const std::vector<visual::color>& value )
{
  return m_item.set_color_list_field( name, value );
} // item_loader_fallback::set_field()
