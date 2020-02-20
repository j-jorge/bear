/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::balloon_placement class.
 * \author Julien Jorge
 */
#include "bear/engine/comic/layer/balloon_placement/balloon_placement.hpp"

#include "bear/engine/comic/item/speaker_item.hpp"
#include "bear/universe/zone.hpp"

/**
 * Define this macro to disable the placement of the balloons. The balloons can
 * then overlap and be offscreen.
 */
// #define BALLOON_PLACEMENT_DISABLED

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param i The speaking item.
 * \param r The bounding box of the speaking item.
 * \param v Tell if the item is visible on the screen.
 */
bear::engine::balloon_placement::scene_character::scene_character
( speaker_item& i, const universe::rectangle_type& r, bool v )
  : box(r), item(i), visible(v)
{
  // nothing to do
} // balloon_placement::scene_character::scene_character()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the balloon for this speaker.
 */
bear::universe::size_box_type
bear::engine::balloon_placement::scene_character::get_balloon_size() const
{
  return item.get_balloon().get_final_size();
} // balloon_placement::scene_character::get_balloon_size()




/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two candidates in decreasing order of quality.
 * \param a The left operand.
 * \param b The right operand.
 */
bool
bear::engine::balloon_placement::candidate::increasing_conflicts::operator()
  ( const candidate* a, const candidate* b ) const
{
  return a->eval() > b->eval();
} // balloon_placement::candidate::increasing_conflicts::operator()()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param r The rectangle where the balloon would be.
 * \param s The character owning this balloon.
 * \param score The score of this candidate.
 */
bear::engine::balloon_placement::candidate::candidate
( const universe::rectangle_type& r, const scene_character& s, int score )
  : rect(r), speaker(s), m_conflicts_count(0), m_is_valid(true), m_score(score),
    m_covered_area(0)
{
  // nothing to do
} // balloon_placement::candidate::candidate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicate a covered area in the balloon.
 * \param percent Percent of covered area between zero and one.
 */
void bear::engine::balloon_placement::candidate::add_covered_area
( double percent )
{
  m_covered_area = std::min( 1.0, m_covered_area + percent );
} // balloon_placement::candidate::add_covered_area()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicate a conflict with an other candidate.
 * \param c The other candidate.
 */
void bear::engine::balloon_placement::candidate::set_in_conflict_with
( candidate* c )
{
  CLAW_PRECOND(c != this);
  CLAW_PRECOND( std::find( m_conflicts.begin(), m_conflicts.end(), c )
                == m_conflicts.end() );

  ++m_conflicts_count;
  m_conflicts.push_back(c);
} // balloon_placement::candidate::set_in_conflict_with()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the number of conflicts.
 */
std::size_t
bear::engine::balloon_placement::candidate::get_conclicts_count() const
{
  return m_conflicts_count;
} // balloon_placement::candidate::get_conclicts_count()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this is still a valid candidate.
 */
bool bear::engine::balloon_placement::candidate::is_valid() const
{
  return m_is_valid;
} // balloon_placement::candidate::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicate that this candidate should not be used anymore.
 */
void bear::engine::balloon_placement::candidate::invalidate()
{
  m_is_valid = false;
} // balloon_placement::candidate::invalidate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an evaluation of this candidate.
 */
int bear::engine::balloon_placement::candidate::eval() const
{
  return m_score - m_conflicts_count - (int)(m_covered_area * 100 + 0.5);
} // balloon_placement::candidate::eval()

/*----------------------------------------------------------------------------*/
/**
 * \brief Prints the candidate details in a stream.
 * \param os The stream in which we write.
 */
void bear::engine::balloon_placement::candidate::print_formatted
( std::ostream& os, bool top, bool right ) const
{
  os << "Candidate left=" << rect.left() << " right=" << rect.right()
     << " bottom=" << rect.bottom() << " top=" << rect.top() << '\n'
     << "placed "
     << (top ? "top" : "bottom") << '-' << (right ? "right" : "left") << '\n'
     << "Evaluated at " << eval() << " with\n"
     << "score:\t" << m_score << '\n'
     << "conflicts:\t" << m_conflicts_count << '\n'
     << "covered area:\t" << (int)(m_covered_area * 100 + 0.5)
     << std::endl;
} // balloon_placement::candidate::print_formatted()




/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two candidate groups by decreasing order of the quality of
 *        their first candidate.
 * \param a The left operand.
 * \param b The right operand.
 */
bool bear::engine::balloon_placement::group_ordering::operator()
  ( const candidate_group& a, const candidate_group& b ) const
{
  CLAW_PRECOND( !a.empty() );
  CLAW_PRECOND( !b.empty() );

  return a.front()->eval() > b.front()->eval();
} // balloon_placement::group_ordering::operator()()




/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param w The width of the area where the speakers are.
 * \param h The height of the area where the speakers are.
 */
bear::engine::balloon_placement::balloon_placement
( universe::size_type w, universe::size_type h )
  : m_view(0, 0, w, h)
{

} // balloon_placement::balloon_placement()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a speaker.
 * \param c The speaker to add.
 * \param rect The bounding box of the speaker in the area.
 */
void bear::engine::balloon_placement::add_speaker
( speaker_item& c, const universe::rectangle_type& rect )
{
  bool is_visible(false);

  if ( rect.intersects(m_view) )
    is_visible = !rect.intersection(m_view).empty() || rect.empty();

  m_characters.push_back(scene_character(c, rect, is_visible));
} // balloon_placement::add_speaker()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the position of the balloons for all speakers previously added
 *        by add_speaker.
 */
void bear::engine::balloon_placement::place_balloons() const
{
  candidate_group_list clist;
  create_candidates(clist);
  sort_candidates(clist);

  candidate_group_list::iterator it;
  candidate_group remaining;

  for (it=clist.begin(); it!=clist.end(); ++it)
    {
      candidate_group::iterator git;
      candidate* c(NULL);

      for (git=it->begin(); (c==NULL) && (git!=it->end()); ++git)
        if ( (*git)->is_valid() )
          c = *git;

      if (c == NULL)
        c = it->front();
      else
        c->invalidate();

      c->speaker.item.get_balloon().set_position
        ( c->rect.bottom_left(), check_on_top(*c), check_on_right(*c) );

      candidate_group::const_iterator cit;
      for (cit=c->get_conflicts().begin(); cit!=c->get_conflicts().end(); ++cit)
        (*cit)->invalidate();
    }

  for (it=clist.begin(); it!=clist.end(); ++it)
    {
      candidate_group::iterator git;

      for (git=it->begin(); git!=it->end(); ++git)
        delete *git;
    }
} // balloon_placement::place_balloons()

/*----------------------------------------------------------------------------*/
/**
 * \brief Sort the candidate groups such that the most interestings become
 *        first.
 * \param c The groups.
 */
void bear::engine::balloon_placement::sort_candidates
( candidate_group_list& c ) const
{
  candidate_group_list::iterator it;

  for ( it=c.begin(); it!=c.end(); ++it )
    it->sort( candidate::increasing_conflicts() );

  c.sort( group_ordering() );
} // balloon_placement::sort_candidates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the candidates for all characters.
 * \param c (out) The candidates.
 */
void bear::engine::balloon_placement::create_candidates
( candidate_group_list& c ) const
{
  character_list_type::const_iterator it;

  for ( it=m_characters.begin(); it!=m_characters.end(); ++it )
    {
      candidate_group result;

      if ( it->item.get_balloon().is_finished() )
        result.push_back
          ( new candidate( it->box, *it, -1000 ) );
      else if ( it->visible )
        create_candidate_visible(*it, result);
      else
        {
#ifdef BALLOON_PLACEMENT_DISABLED
            create_candidate_visible(*it, result);
#else
          if ( it->item.get_persistent_balloon() )
            create_candidate_not_visible(*it, result);
          else
            create_candidate_visible(*it, result);
#endif
        }

      if ( result.empty() )
        result.push_back
          ( new candidate
            ( universe::rectangle_type
              ( it->box.top_left(),
                it->box.top_left() + it->get_balloon_size() ),
              *it, -1000 ) );

#ifndef BALLOON_PLACEMENT_DISABLED
      check_conflicts(result, c);
#endif

      c.push_back(result);
    }
} // balloon_placement::create_candidates()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the conflicts of a group with the other groups.
 * \param g The group of new candidates.
 * \param result (in/out) The candidates created before the ones in \a g.
 * \pre All candidates in \a g concern the same item.
 */
void bear::engine::balloon_placement::check_conflicts
( candidate_group& g, candidate_group_list& result ) const
{
  candidate_group_list::iterator it;
  candidate_group::iterator sit, git;

  for ( it=result.begin(); it!=result.end(); ++it )
    for ( sit=it->begin(); sit!=it->end(); ++sit )
      for ( git=g.begin(); git!=g.end(); ++git )
        {
          bool conflict(false);

          if ( (*sit)->rect.intersects( (*git)->rect ) )
            conflict = !(*sit)->rect.intersection( (*git)->rect ).empty();

          if (!conflict)
            if ( (*sit)->rect.intersects( (*git)->speaker.box ) )
              conflict =
                !(*sit)->rect.intersection( (*git)->speaker.box ).empty();

          if (!conflict)
            if ( (*git)->rect.intersects( (*sit)->speaker.box ) )
              conflict =
                !(*git)->rect.intersection( (*sit)->speaker.box ).empty();

          if (conflict)
            {
              (*sit)->set_in_conflict_with( *git );
              (*git)->set_in_conflict_with( *sit );
            }
        }
} // balloon_placement::check_conflicts()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add a new candidate for a given character.
 * \param c The caracter for which we create the candidate.
 * \param result (out) The group where is added the candidate.
 * \param left The left position of the balloon.
 * \param bottom The bottom position of the balloon.
 * \param score The score assigned to this candidate, by default. The final
 *        score given to the candidate can be different.
 */
void bear::engine::balloon_placement::new_candidate
( const scene_character& c, candidate_group& result,
  universe::coordinate_type left,
  universe::coordinate_type bottom, int score ) const
{
  const universe::rectangle_type r
    ( left, bottom, left + c.get_balloon_size().x,
      bottom + c.get_balloon_size().y );

  double covered_area = 0;

  if ( c.visible )
    {
#ifndef BALLOON_PLACEMENT_DISABLED
      if ( m_view.includes(r) )
        score += 10;
      else if ( c.item.get_persistent_balloon() )
        return;
      else if ( !m_view.intersects(r) )
        score = -1;
      else
        {
          score = -1;
          covered_area = 1.0 - m_view.intersection(r).area() / r.area();
        }
#endif
    }
  else if ( !m_view.intersects(r) )
    score = -1;


  candidate* cd = new candidate(r, c, score);
  cd->add_covered_area( covered_area );
  result.push_back( cd );
} // balloon_placement::new_candidate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the candidates for an item visible in the area.
 * \param c The item.
 * \param result (out) The candidates for \a c.
 */
void bear::engine::balloon_placement::create_candidate_visible
( const scene_character& c, candidate_group& result ) const
{
  const int on_finished_preference =
    c.item.get_balloon().is_finished() ? 1 : 0;

  const int score_left =
    !c.item.get_balloon().is_on_right() ? (1 - on_finished_preference) : 0;
  const int score_right =
    c.item.get_balloon().is_on_right() ? 1 : on_finished_preference;
  const int score_bottom =
    !c.item.get_balloon().is_on_top() ? (1 - on_finished_preference) : 0;
  const int score_top =
    c.item.get_balloon().is_on_top() ? 1 : on_finished_preference;

  const int top_factor( 2 );
#ifdef BALLOON_PLACEMENT_DISABLED
  const int right_factor( 10 );
#else
  const int right_factor( 2 );
#endif
  const int left_factor( 2 );

  new_candidate
    ( c, result,  c.box.right(), c.box.top(),
      top_factor * right_factor * (score_right + score_top) );
  new_candidate
    ( c, result,  c.box.left() - c.get_balloon_size().x, c.box.top(),
      top_factor * left_factor * (score_left + score_top) );
  new_candidate
    ( c, result,  c.box.right(), c.box.bottom() - c.get_balloon_size().y,
      right_factor * (score_right + score_bottom) );
  new_candidate
    ( c, result,  c.box.left() - c.get_balloon_size().x,
      c.box.bottom() - c.get_balloon_size().y,
      left_factor * (score_left + score_bottom) );
} // balloon_placement::create_candidate_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create the candidates for an off-screen item.
 * \param c The item.
 * \param result (out) The candidates for \a c.
 */
void bear::engine::balloon_placement::create_candidate_not_visible
( const scene_character& c, candidate_group& result ) const
{
  switch( universe::zone::find( c.box, m_view ) )
    {
    case universe::zone::top_left_zone:
      repeat_candidate_horizontally
        ( c, result, m_view.left(), m_view.width() / 2,
          m_view.top() - c.get_balloon_size().y );
      repeat_candidate_vertically
        ( c, result, m_view.top() - c.get_balloon_size().y,
          m_view.top() - m_view.height() / 2, m_view.left() );
      break;
    case universe::zone::top_zone:
      repeat_candidate_placed_horizontally
        ( c, result, m_view.top() - c.get_balloon_size().y );
      break;
    case universe::zone::top_right_zone:
      repeat_candidate_horizontally
        ( c, result, m_view.right(), m_view.right() - m_view.width() / 2,
          m_view.top() - c.get_balloon_size().y );
      repeat_candidate_vertically
        ( c, result, m_view.top() - c.get_balloon_size().y,
          m_view.top() - m_view.height() / 2,
          m_view.right() - c.get_balloon_size().x );
      break;
    case universe::zone::middle_left_zone:
      repeat_candidate_placed_vertically( c, result, m_view.left() );
      break;
    case universe::zone::middle_right_zone:
      repeat_candidate_placed_vertically
        ( c, result, m_view.right() - c.get_balloon_size().x );
      break;
    case universe::zone::bottom_left_zone:
      repeat_candidate_horizontally
        ( c, result, m_view.left(), m_view.width() / 2, m_view.bottom() );
      repeat_candidate_vertically
        ( c, result, m_view.bottom() + c.get_balloon_size().y,
          m_view.bottom() + m_view.height() / 2, m_view.left() );
      break;
    case universe::zone::bottom_zone:
      repeat_candidate_placed_horizontally( c, result, m_view.bottom() );
      break;
    case universe::zone::bottom_right_zone:
      repeat_candidate_horizontally
        ( c, result, m_view.right(), m_view.right() - m_view.width() / 2,
          m_view.bottom() );
      repeat_candidate_vertically
        ( c, result, m_view.bottom() + c.get_balloon_size().y,
          m_view.bottom() + m_view.height() / 2,
          m_view.right() - c.get_balloon_size().x );
      break;
    default:
      {
        CLAW_FAIL("Invalid zone for an invisible candidate.");
      }
    }
} // balloon_placement::create_candidate_not_visible()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an horizontal line of candidates for an off-screen item.
 * \param c The item.
 * \param result (out) The candidates for \a c.
 * \param first X-position of the first candidate.
 * \param last X-position of the last candidate.
 * \param y Y-position for the candidates.
 */
void bear::engine::balloon_placement::repeat_candidate_horizontally
( const scene_character& c, candidate_group& result,
  universe::coordinate_type first, universe::coordinate_type last,
  universe::coordinate_type y ) const
{
  const unsigned int count =
    (unsigned int)(std::abs(last - first) / c.get_balloon_size().x);
  universe::coordinate_type x;

  if ( first < last )
    x = first;
  else
    x = first - count * c.get_balloon_size().x;

  for (unsigned int i=0; i!=count; ++i, x+=c.get_balloon_size().x)
    new_candidate(c, result, x, y, 1);
} // balloon_placement::repeat_candidate_horizontally()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a vertical line of candidates for an off-screen item.
 * \param c The item.
 * \param result (out) The candidates for \a c.
 * \param first Y-position of the first candidate.
 * \param last Y-position of the last candidate.
 * \param x X-position for the candidates.
 */
void bear::engine::balloon_placement::repeat_candidate_vertically
( const scene_character& c, candidate_group& result,
  universe::coordinate_type first, universe::coordinate_type last,
  universe::coordinate_type x ) const
{
  const unsigned int count =
    (unsigned int)(std::abs(last - first) / c.get_balloon_size().y);
  universe::coordinate_type y;

  if ( first < last )
    y = first;
  else
    y = first - count * c.get_balloon_size().y;

  for (unsigned int i=0; i!=count; ++i, y+=c.get_balloon_size().y)
    new_candidate(c, result, x, y, 1);
} // balloon_placement::repeat_candidate_vertically()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an horizontal line of candidates for an off-screen item, with a
 *        balloon placed near the item.
 * \param c The item.
 * \param result (out) The candidates for \a c.
 * \param y Y-position for the candidates.
 * \pre ( c.box.left() <= m_view.right() )
 *      && ( c.box.right() >= m_view.left() )
 */
void bear::engine::balloon_placement::repeat_candidate_placed_horizontally
( const scene_character& c, candidate_group& result,
  universe::coordinate_type y ) const
{
  CLAW_PRECOND( c.box.left() <= m_view.right() );
  CLAW_PRECOND( c.box.right() >= m_view.left() );

  universe::coordinate_type origin =
    c.box.left() - c.get_balloon_size().x;

  if ( origin < m_view.left() )
    {
      origin = c.box.right();

      if ( origin > m_view.right() )
        origin = m_view.left() + m_view.width() / 2;
    }

  universe::coordinate_type x = origin;
  while ( x >= m_view.left() )
    {
      new_candidate(c, result, x, y, 1);
      x -= c.get_balloon_size().x;
    }

  x = origin + c.get_balloon_size().x;
  while ( x + c.get_balloon_size().x <= m_view.right() )
    {
      new_candidate(c, result, x, y, 1);
      x += c.get_balloon_size().x;
    }
} // balloon_placement::repeat_candidate_placed_horizontally()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a vertical line of candidates for an off-screen item, with a
 *        balloon placed near the item.
 * \param c The item.
 * \param result (out) The candidates for \a c.
 * \param x X-position for the candidates.
 * \pre ( c.box.bottom() <= m_view.top() )
 *      && ( c.box.top() >= m_view.bottom() )
 */
void bear::engine::balloon_placement::repeat_candidate_placed_vertically
( const scene_character& c, candidate_group& result,
  universe::coordinate_type x ) const
{
  CLAW_PRECOND( c.box.bottom() <= m_view.top() );
  CLAW_PRECOND( c.box.top() >= m_view.bottom() );

  universe::coordinate_type origin = c.box.top();

  if ( origin + c.get_balloon_size().y > m_view.top() )
    {
      origin = c.box.bottom() - c.get_balloon_size().y;

      if ( origin < m_view.bottom() )
        origin = m_view.bottom() + m_view.height() / 2;
    }

  universe::coordinate_type y = origin;
  while ( y >= m_view.bottom() )
    {
      new_candidate(c, result, x, y, 1);
      y -= c.get_balloon_size().y;
    }

  y = origin + c.get_balloon_size().y;
  while ( y + c.get_balloon_size().y <= m_view.top() )
    {
      new_candidate(c, result, x, y, 1);
      y += c.get_balloon_size().y;
    }
} // balloon_placement::repeat_candidate_placed_vertically()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the balloon of a candidate is on the top of the item.
 * \param c The candidate.
 */
bool bear::engine::balloon_placement::check_on_top( const candidate& c ) const
{
  return c.rect.bottom() >= c.speaker.box.bottom() + c.speaker.box.height() / 2;
} // balloon_placement::check_on_top()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the balloon of a candidate is on the right of the item.
 * \param c The candidate.
 */
bool bear::engine::balloon_placement::check_on_right( const candidate& c ) const
{
  return c.rect.left() >= c.speaker.box.left() + c.speaker.box.width() / 2;
} // balloon_placement::check_on_right()
