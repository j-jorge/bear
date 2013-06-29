/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
#include "visual/writing.hpp"

#include "visual/bitmap_writing.hpp"
#include "visual/scene_writing.hpp"
#include "visual/text_metric.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::visual::writing::writing()
  : m_writing( new writing_type ), m_counter(new std::size_t(0))
{

} // writing::writing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to render the text.
 * \param str The text to render.
 * \param h The horizontal alignment of the text.
 * \param v The vertical alignment of the text.
 */
bear::visual::writing::writing
( const font& f, const std::string& str, text_align::horizontal_align h,
  text_align::vertical_align v )
  : m_writing( new writing_type ), m_counter(new std::size_t(0))
{
  create( f, str, h, v );
} // writing::writing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The font used to render the text.
 * \param str The text to render.
 * \param s The maximum size of the writing.
 * \param h The horizontal alignment of the text.
 * \param v The vertical alignment of the text.
 */
bear::visual::writing::writing
( const font& f, const std::string& str, const size_box_type& s,
  text_align::horizontal_align h, text_align::vertical_align v )
  : m_writing( new writing_type ), m_counter(new std::size_t(0))
{
  create( f, str, s, h, v );
} // writing::writing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
bear::visual::writing::writing( const writing& that )
  : m_writing(that.m_writing), m_counter(that.m_counter)
{
  ++*m_counter;
} // writing::writing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::visual::writing::~writing()
{
  if ( *m_counter > 0 )
    --*m_counter;
  else
    {
      delete m_counter;
      delete m_writing;
    }
} // writing::writing()

/*----------------------------------------------------------------------------*/
/**
 * \brief Assignment.
 * \param that The instance to copy from.
 */
bear::visual::writing& bear::visual::writing::operator=( const writing& that )
{
  writing tmp(that);
  swap(tmp);
  return *this;
} // writing::operator=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two instances.
 * \param that The instance to swap with.
 */
void bear::visual::writing::swap( writing& that )
{
  std::swap( m_writing, that.m_writing );
  std::swap( m_counter, that.m_counter );
} // writing::swap()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the effect to apply to the sprites of the text.
 * \param e The effect.
 */
void bear::visual::writing::set_effect( sequence_effect e )
{
  m_writing->set_effect(e);
} // writing::set_effect()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update the rendering of the writing and its effects.
 * \param t The elapsed type since the last call.
 */
void bear::visual::writing::update( double t )
{
  m_writing->update(t);
} // writing::update()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of the writing.
 */
bear::visual::coordinate_type bear::visual::writing::get_width() const
{
  return m_writing->get_size().x;
} // writing::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of the writing.
 */
bear::visual::coordinate_type bear::visual::writing::get_height() const
{
  return m_writing->get_size().y;
} // writing::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the writing.
 */
const bear::visual::size_box_type& bear::visual::writing::get_size() const
{
  return m_writing->get_size();
} // writing::get_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the implemented writing.
 */
bear::visual::writing::writing_reference bear::visual::writing::operator*()
{
  return *m_writing;
} // writing::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the implemented writing.
 */
bear::visual::writing::const_writing_reference
bear::visual::writing::operator*() const
{
  return *m_writing;
} // writing::operator*()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer on the implemented writing.
 */
bear::visual::writing::writing_pointer bear::visual::writing::operator->()
{
  return m_writing;
} // writing::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer on the implemented writing.
 */
bear::visual::writing::const_writing_pointer
bear::visual::writing::operator->() const
{
  return m_writing;
} // writing::operator->()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the pointer on the implemented writing.
 */
bear::visual::writing::const_writing_pointer
bear::visual::writing::get_impl() const
{
  return m_writing;
} // writing::get_impl()

/*----------------------------------------------------------------------------*/
/**
 * \brief Place the characters of the text.
 * \param f The font used to render the text.
 * \param str The text to render.
 * \param h The horizontal alignment of the text.
 * \param v The vertical alignment of the text.
 */
void bear::visual::writing::create
( const font& f, const std::string& str, text_align::horizontal_align h,
  text_align::vertical_align v )
{
  text_metric tm( str, f );
  create( f, str, size_box_type(tm.width(), tm.height()), h, v );
} // writing::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Place the characters of the text.
 * \param f The font used to render the text.
 * \param str The text to render.
 * \param s The maximum size of the writing.
 * \param h The horizontal alignment of the text.
 * \param v The vertical alignment of the text.
 */
void bear::visual::writing::create
( const font& f, const std::string& str, const size_box_type& s,
  text_align::horizontal_align h, text_align::vertical_align v )
{
  if ( *m_counter > 0 )
    {
      --*m_counter;
      m_writing = new writing_type(*m_writing);
      m_counter = new std::size_t(0);
    }

  m_writing->create( f, str, s, h, v );
} // writing::create()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call the render method of a scene_writing (double dispach) .
 * \param s The scene_writing on which we call the method.
 * \param scr The screen to pass to \a s.
 */
void bear::visual::writing::call_render
( const scene_writing& s, base_screen& scr ) const
{
  m_writing->call_render(s, scr);
} // writing::call_render()

/*----------------------------------------------------------------------------*/
/**
 * \brief Swap two writings.
 * \param a The first writing.
 * \param b The second writing.
 */
void std::swap( bear::visual::writing& a, bear::visual::writing& b )
{
  a.swap(b);
} // swap()
