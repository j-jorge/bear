/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::item_loader_base class.
 * \author Julien Jorge.
 */
#include "bear/engine/loader/item_loader_base.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of this loader.
 */
bear::engine::item_loader_base::item_loader_base( const std::string& name )
  : m_name(name)
{
  // nothing to do
} // item_loader_base::item_loader_base()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::item_loader_base::~item_loader_base()
{
  // nothing to do
} // item_loader_base::~item_loader_base()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name assigned to this loader.
 */
const std::string& bear::engine::item_loader_base::get_name() const
{
  return m_name;
} // item_loader_base::get_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c unsigned \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, unsigned int value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, int value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, double value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, bool value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::string& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, base_item* value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::sprite.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const visual::sprite& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const visual::animation& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c audio::sample.
 * \param name The name of the field.
 * \param value The new value of the field. The item receiving this value must
 *        release de memory with a delete at some time.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, audio::sample* value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::font.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, visual::font value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, visual::color value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c easing function.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, easing_function value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of unsigned integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<unsigned int>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<int>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<double>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<bool>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<std::string>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<base_item*>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::sprite.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<visual::sprite>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<visual::animation>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of audio::sample.
 * \param name The name of the field.
 * \param value The new value of the field. The item receiving this value must
 *        release de memory with a delete at some time.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<audio::sample*>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::font.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<visual::font>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<visual::color>& value )
{
  return false;
} // item_loader_base::set_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of easing functions.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::item_loader_base::set_field
( const std::string& name, const std::vector<easing_function>& value )
{
  return false;
} // item_loader_base::set_field()
