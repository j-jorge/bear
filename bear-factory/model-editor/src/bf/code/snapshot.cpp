/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::snapshot class.
 * \author Julien Jorge
 */
#include "bf/snapshot.hpp"

#include "bf/compiled_file.hpp"
#include "bf/mark.hpp"

#include <claw/assert.hpp>
#include <limits>

/*----------------------------------------------------------------------------*/
const std::string bf::snapshot::horizontal_alignment::align_left = "align_left";
const std::string
bf::snapshot::horizontal_alignment::align_center = "align_center";
const std::string
bf::snapshot::horizontal_alignment::align_right = "align_right";

const std::string
bf::snapshot::vertical_alignment::align_bottom = "align_bottom";
const std::string
bf::snapshot::vertical_alignment::align_center = "align_center";
const std::string bf::snapshot::vertical_alignment::align_top = "align_top";




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bf::snapshot::snapshot()
  : m_date(0), m_width(10), m_height(10), m_x_alignment("align_left"),
    m_x_alignment_value(0), m_y_alignment("align_bottom"),
    m_y_alignment_value(0)
{

} // snapshot::get_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 * \param marks The copies of the marks of that to use in the copy of the
 *        snapshot.
 */
bf::snapshot::snapshot
( const snapshot& that, const std::map<const mark*, const mark*>& marks )
  : m_date(that.m_date), m_function(that.m_function), m_sound(that.m_sound),
    m_width(that.m_width), m_height(that.m_height),
    m_x_alignment(that.m_x_alignment),
    m_x_alignment_value(that.m_x_alignment_value),
    m_y_alignment(that.m_y_alignment),
    m_y_alignment_value(that.m_y_alignment_value)
{
  placement_list::const_iterator it;

  for ( it=that.m_placement.begin(); it!=that.m_placement.end(); ++it )
    {
      m_placement.push_back
        ( mark_placement(marks.find(it->get_mark())->second) );
      m_placement.back().copy(*it);
    }
} // snapshot::snapshot()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the date.
 */
double bf::snapshot::get_date() const
{
  return m_date;
} // snapshot::get_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the date.
 * \param date The new date.
 */
void bf::snapshot::set_date( double date )
{
  m_date = date;
} // snapshot::set_date()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a given mark in the snapshot.
 * \param m The mark to search.
 */
bool bf::snapshot::has_mark( const mark* m ) const
{
  return get_const_iterator(m) != m_placement.end();
} // snapshot::has_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if there is a given mark in the snapshot.
 * \param mark_label The label of the mark to search.
 */
bool bf::snapshot::has_mark( const std::string& mark_label ) const
{
  return get_const_iterator(mark_label) != m_placement.end();
} // snapshot::has_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the placement of a given mark.
 * \param m The mark for which we want the placement.
 * \pre There is such a mark \a m in the snapshot.
 */
bf::mark_placement& bf::snapshot::get_placement( const mark* m )
{
  CLAW_PRECOND( has_mark(m) );

  return *get_iterator(m);
} // snapshot::get_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the placement of a given mark.
 * \param mark_label The label of the mark for which we want the placement.
 * \pre There is a mark labeled \a mark_label in the snapshot.
 */
bf::mark_placement& bf::snapshot::get_placement( const std::string& mark_label )
{
  CLAW_PRECOND( has_mark(mark_label) );

  placement_list::iterator it = m_placement.begin();
  bool found(false);

  while ( !found )
    if (it->get_mark()->get_label() == mark_label)
      found = true;
    else
      ++it;

  return *it;
} // snapshot::get_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the placement of a given mark.
 * \param m The mark for which we want the placement.
 * \pre There is such a mark \a m in the snapshot.
 */
const bf::mark_placement& bf::snapshot::get_placement( const mark* m ) const
{
  CLAW_PRECOND( has_mark(m) );

  return *get_const_iterator(m);
} // snapshot::get_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the function executed when passing on this snapshot.
 * \param c The name of the function.
 */
void bf::snapshot::set_function( const std::string& c )
{
  m_function = c;
} // snapshot::set_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the name of the function executed when passing on this snapshot.
 */
const std::string& bf::snapshot::get_function() const
{
  return m_function;
} // snapshot::get_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the sound played when passing on this snapshot.
 * \param s The sound played when passing on this snapshot.
 */
void bf::snapshot::set_sound( const sound_description& s )
{
  m_sound = s;
} // snapshot::set_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sound played when passing on this snapshot.
 */
bf::sound_description& bf::snapshot::get_sound()
{
  return m_sound;
} // snapshot::get_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the sound played when passing on this snapshot.
 */
const bf::sound_description& bf::snapshot::get_sound() const
{
  return m_sound;
} // snapshot::get_sound()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a mark in the snapshot.
 * \param m The identifier of the mark concerned by the placement.
 * \pre has_mark(m) == false
 */
void bf::snapshot::add_mark( mark* m )
{
  CLAW_PRECOND( !has_mark(m) );

  m_placement.push_front( mark_placement(m) );
} // snapshot::add_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove a mark from the snapshot.
 * \param m The identifief of the mark to remove.
 * \pre has_mark(m) == true
 */
void bf::snapshot::remove_mark( mark* m )
{
  CLAW_PRECOND( has_mark(m) );

  m_placement.erase( get_iterator(m) );
} // snapshot::remove_mark()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visual bounds of this snapshot.
 * \param min_x The minimum x-coordinate with whether an animation or a mark.
 * \param min_y The minimum y-coordinate with whether an animation or a mark.
 * \param max_x The maximum x-coordinate with whether an animation or a mark.
 * \param max_y The maximum y-coordinate with whether an animation or a mark.
 */
void bf::snapshot::get_bounds
( double& min_x, double& min_y, double& max_x, double& max_y ) const
{
  const_mark_placement_iterator it;
  const const_mark_placement_iterator eit( mark_placement_end() );

  min_x = 0;
  min_y = 0;
  max_x = m_width;
  max_y = m_height;

  for (it=mark_placement_begin(); it!=eit; ++it)
    {
      const double x( it->get_x_position() );
      const double y( it->get_y_position() );
      double w(0);
      double h(0);

      if ( it->get_mark()->has_animation() )
        {
          w = it->get_mark()->get_animation_data().get_max_size().x;
          h = it->get_mark()->get_animation_data().get_max_size().y;
        }

      min_x = std::min( min_x, x - w / 2 );
      min_y = std::min( min_y, y - h / 2 );
      max_x = std::max( max_x, x + w / 2 );
      max_y = std::max( max_y, y + h / 2 );
    }
} // snapshot::get_bounds()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at the beginning of placements.
 */
bf::snapshot::const_mark_placement_iterator
bf::snapshot::mark_placement_begin() const
{
  return m_placement.begin();
} // snapshot::mark_placement_begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator at the end of placements.
 */
bf::snapshot::const_mark_placement_iterator
bf::snapshot::mark_placement_end() const
{
  return m_placement.end();
} // snapshot::mark_placement_end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the size of this snapshot.
 * \param width The width of the box.
 * \param height The height of the box.
 */
void bf::snapshot::set_size( double width, double height )
{
  m_width = width;
  m_height = height;
} // snapshot::set_bounding_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the width of this snapshot.
 */
double bf::snapshot::get_width() const
{
  return m_width;
} // snapshot::get_width()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the height of this snapshot.
 */
double bf::snapshot::get_height() const
{
  return m_height;
} // snapshot::get_height()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the alignment type on x-coordinate.
 */
void bf::snapshot::set_x_alignment(const std::string& a)
{
  m_x_alignment = a;
} // snapshot::set_x_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the alignment type on x-coordinate.
 */
const std::string& bf::snapshot::get_x_alignment() const
{
  return m_x_alignment;
} // snapshot::get_x_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value alignment on x-coordinate.
 */
void bf::snapshot::set_x_alignment_value(double value)
{
  m_x_alignment_value = value;
} // snapshot::set_x_alignment_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get value of the alignment on x-coordinate.
 */
double bf::snapshot::get_x_alignment_value() const
{
  return m_x_alignment_value;
} // snapshot::get_x_alignment_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the alignment type on y-coordinate.
 */
void bf::snapshot::set_y_alignment(const std::string& a)
{
  m_y_alignment = a;
} // snapshot::set_y_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the alignment type on y-coordinate.
 */
const std::string& bf::snapshot::get_y_alignment() const
{
  return m_y_alignment;
} // snapshot::get_y_alignment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the value alignment on y-coordinate.
 */
void bf::snapshot::set_y_alignment_value(double value)
{
  m_y_alignment_value = value;
} // snapshot::set_y_alignment_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get value of the alignment on y-coordinate.
 */
double bf::snapshot::get_y_alignment_value() const
{
  return m_y_alignment_value;
} // snapshot::get_y_alignment_value()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile the snapshot.
 * \param f The file in which we compile.
 * \param label_to_int The index associated to each mark label.
 */
void bf::snapshot::compile
( compiled_file& f,
  const std::map<std::string, unsigned int>& label_to_int ) const
{
  f << m_date << m_function << m_width << m_height << m_x_alignment
    << m_y_alignment << m_x_alignment_value << m_y_alignment_value;

  m_sound.compile(f);

  const_mark_placement_iterator it;
  for (it=mark_placement_begin(); it!=mark_placement_end(); ++it)
    {
      f << label_to_int.find(it->get_mark()->get_label())->second;
      it->compile(f);
    }
} // snapshot::compile()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the placement of a given mark.
 * \param m The mark for which we want the iterator.
 */
bf::snapshot::placement_list::iterator
bf::snapshot::get_iterator( const mark* m )
{
  return get_iterator(m->get_label());
} // snapshot::get_iterator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator on the placement of a given mark.
 * \param m The mark for which we want the iterator.
 */
bf::snapshot::placement_list::const_iterator
bf::snapshot::get_const_iterator( const mark* m ) const
{
  return get_const_iterator(m->get_label());
} // snapshot::get_const_iterator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the placement of a given mark.
 * \param mark_label The label of the mark for which we want the iterator.
 */
bf::snapshot::placement_list::iterator bf::snapshot::get_iterator
( const std::string& mark_label )
{
  placement_list::iterator it( m_placement.begin() );
  bool found(false);

  while ( !found && (it!=m_placement.end()) )
    if (it->get_mark()->get_label() == mark_label)
      found = true;
    else
      ++it;

  return it;
} // snapshot::get_iterator()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a constant iterator on the placement of a given mark.
 * \param mark_label The label of the mark for which we want the iterator.
 */
bf::snapshot::placement_list::const_iterator
bf::snapshot::get_const_iterator( const std::string& mark_label ) const
{
  placement_list::const_iterator it( m_placement.begin() );
  bool found(false);

  while ( !found && (it!=m_placement.end()) )
    if (it->get_mark()->get_label() == mark_label)
      found = true;
    else
      ++it;

  return it;
} // snapshot::get_const_iterator()
