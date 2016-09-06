/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::animation class.
 * \author Julien Jorge
 */
#include "bf/animation.hpp"

#include "bf/compiled_file.hpp"
#include "bf/version.hpp"

#include <claw/assert.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Default constructor.
 */
bf::animation::animation()
  : m_loops(0), m_loop_back(false), m_first_index(0), m_last_index(0)
{

} // animation::animation()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two animations are the same.
 * \param that The other animation.
 */
bool bf::animation::operator==(const animation& that) const
{
  return (m_loops == that.m_loops)
    && (m_loop_back == that.m_loop_back)
    && (m_first_index == that.m_first_index)
    && (m_last_index == that.m_last_index)
    && bitmap_rendering_attributes::operator==(that)
    && ( static_cast< std::list<animation_frame> >(*this)
         == static_cast< std::list<animation_frame> >(that) );
} // animation::operator==()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if two animations are different.
 * \param that The other animation.
 */
bool bf::animation::operator!=(const animation& that) const
{
  return !(*this == that);
} // animation::operator!=()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare with an other animation in lexicographic order of their
 *        attributes.
 * \param that The other animation.
 */
bool bf::animation::operator<(const animation& that) const
{
  if ( m_loops != that.m_loops )
    return m_loops < that.m_loops;

  if ( m_loop_back != that.m_loop_back )
    return m_loop_back < that.m_loop_back;

  if ( m_first_index != that.m_first_index )
    return m_first_index < that.m_first_index;

  if ( m_last_index != that.m_last_index )
    return m_last_index < that.m_last_index;

  if ( bitmap_rendering_attributes::operator!=(that) )
    return bitmap_rendering_attributes::operator<(that);

  return
    std::lexicographical_compare( begin(), end(), that.begin(), that.end() );
} // animation::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the frames of the animation.
 * \param frames The new frames.
 */
void bf::animation::set_frames( const frame_list& frames )
{
  clear();
  insert(begin(), frames.begin(), frames.end());
  set_size( get_max_size() );
} // animation::set_frames()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the number of times the animation will be played.
 * \param loops The new value.
 */
void bf::animation::set_loops( unsigned int loops )
{
  m_loops = loops;
} // animation::set_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the "play reverse at end" attribute of the animation.
 * \param b The new value.
 */
void bf::animation::set_loop_back( bool b )
{
  m_loop_back = b;
} // animation::set_loop_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the index of the first frame of the loops.
 * \param index The new index of first sprite.
 */
void bf::animation::set_first_index( unsigned int index )
{
  m_first_index = index;
} // animation::set_first_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the index of the last frame of the loops.
 * \param index The new index of tlast sprite.
 */
void bf::animation::set_last_index( unsigned int index )
{
  m_last_index = index;
} // animation::set_last_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the loops value.
 */
unsigned int bf::animation::get_loops() const
{
  return m_loops;
} // animation::get_loops()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the loop_back value.
 */
bool bf::animation::get_loop_back() const
{
  return m_loop_back;
} // animation::get_loop_back()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the index of the first frame for loops.
 */
unsigned int bf::animation::get_first_index() const
{
  return m_first_index;
} // animation::get_first_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the index of the last frame for loops.
 */
unsigned int bf::animation::get_last_index() const
{
  return m_last_index;
} // animation::get_last_index()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a frame at the end of the animation.
 */
bf::animation_frame& bf::animation::add_frame()
{
  push_back( animation_frame() );
  return back();
} // animation::add_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a frame given its index.
 * \param index The index of the frame to get.
 */
bf::animation_frame& bf::animation::get_frame(unsigned int index)
{
  CLAW_PRECOND(index < size() );

  iterator it = begin();
  std::advance(it, index);

  return *it;
} // animation::get_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a frame given its index.
 * \param index The index of the frame to get.
 */
const bf::animation_frame& bf::animation::get_frame(unsigned int index) const
{
  CLAW_PRECOND(index < size() );

  const_iterator it = begin();
  std::advance(it, index);

  return *it;
} // animation::get_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Delete a frame.
 * \param index The index of the frame to remove.
 */
void bf::animation::delete_frame(unsigned int index)
{
  CLAW_PRECOND(index < size() );

  iterator it = begin();
  std::advance(it, index);

  erase(it);
} // animation::delete_frame()

/*----------------------------------------------------------------------------*/
/**
 * \brief Reverse some frames.
 * \param indexes The indexes of the frame to reverse.
 */
void bf::animation::reverse_frames(const std::list<unsigned int>& indexes)
{
  std::list<unsigned int> index_list(indexes);

  while ( index_list.size() > 1 )
    {
      iterator it_first = begin();
      std::advance( it_first, index_list.front() );

      iterator it_end = begin();
      std::advance( it_end, index_list.back() );

      std::swap(*it_first, *it_end);
      
      index_list.pop_front();
      index_list.pop_back();
    }
} // animation::reverse_frames()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move a frame one position backward.
 * \param index The index of the frame to move.
 */
void bf::animation::move_backward(unsigned int index)
{
  CLAW_PRECOND(index < size() );

  if (index > 0)
    {
      iterator it = begin();
      std::advance(it, index);
      iterator prec(it);
      --prec;

      std::swap(*prec, *it);
    }
} // animation::move_backward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Move a frame one position backward.
 * \param index The index of the frame to move.
 */
void bf::animation::move_forward(unsigned int index)
{
  CLAW_PRECOND(index < size() );

  if (index + 1 < size() )
    {
      iterator it = begin();
      std::advance(it, index);
      iterator next(it);
      ++next;

      std::swap(*it, *next);
    }
} // animation::move_forward()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a sprite given its index. The sprite has the attributes of the
 *        frame combined with those of the animation.
 * \param index The index of the sprite to get.
 */
bf::sprite bf::animation::get_sprite(unsigned int index) const
{
  CLAW_PRECOND(index < size() );

  sprite result( get_frame(index).get_sprite() );
  result.combine(*this);

  const double w
    ( get_max_size().x == 0 ? 0 : result.width() * width() / get_max_size().x );
  const double h
    ( get_max_size().y == 0
      ? 0
      : result.height() * height() / get_max_size().y );

  result.set_size( w, h );

  return result;
} // animation::get_sprite()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the biggest size of the frames.
 */
claw::math::coordinate_2d<unsigned int> bf::animation::get_max_size() const
{
  claw::math::coordinate_2d<unsigned int> result(0, 0);
  const_iterator it;

  for (it=begin(); it!=end(); ++it)
    {
      if ( it->get_sprite().width() > result.x )
        result.x = it->get_sprite().width();

      if ( it->get_sprite().height() > result.y )
        result.y = it->get_sprite().height();
    }

  return result;
} // animation::get_max_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the number of frames.
 */
std::size_t bf::animation::frames_count() const
{
  return size();
} // animation::frames_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the total duration of the animation.
 */
double bf::animation::get_duration() const
{
  if ( empty() || (m_last_index >= frames_count())
       || (m_first_index > m_last_index) )
    return 0;

  double loop_duration( 0 );

  for ( std::size_t i(m_first_index); i <= m_last_index; ++i )
    loop_duration += get_frame( i ).get_duration();

  if ( m_loop_back && (m_last_index - m_first_index > 1) )
    loop_duration +=
      loop_duration
      - get_frame( m_first_index ).get_duration()
      - get_frame( m_last_index ).get_duration();

  double result( loop_duration );

  if ( m_loops != 0 )
    result *= m_loops;

  for ( std::size_t i(0); i != m_first_index; ++i )
    result += get_frame( i ).get_duration();

  for ( std::size_t i( m_last_index + 1 ); i != frames_count(); ++i )
    result += get_frame( i ).get_duration();

  return result;
} // animation::get_duration()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the animation.
 * \param f The stream in which we write the compiled animation.
 * \param c The context in which the compilation is done.
 */
void bf::animation::compile( compiled_file& f, compilation_context& c ) const
{
  const_iterator it;

  f << size();

  for ( it=begin(); it!=end(); ++it)
    {
      f << it->get_duration();
      it->get_sprite().compile(f, c);
    }

  f << m_loops << m_loop_back << m_first_index << m_last_index;

  bitmap_rendering_attributes::compile(f);
} // animation::compile()
