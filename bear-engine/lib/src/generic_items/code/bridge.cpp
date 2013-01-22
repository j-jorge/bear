/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::bridge class.
 * \author Sebastie Angibaud
 */
#include "generic_items/bridge.hpp"
#include "visual/scene_element_sequence.hpp"

#include "engine/layer/layer.hpp"
#include "engine/world.hpp"
#include "engine/level_globals.hpp"
#include "engine/item_brick/loader/z_shift_loader.hpp"

#include "universe/collision_info.hpp"
#include "universe/collision_repair.hpp"
#include "universe/forced_movement/forced_tracking.hpp"

BASE_ITEM_EXPORT( bridge, bear )

const bear::universe::coordinate_type bear::bridge::s_line_width = 10;

/*----------------------------------------------------------------------------*/
/**
 * \brief Construtor.
 */
bear::bridge::item_on_bridge::item_on_bridge()
  : m_item(NULL), m_reference_item(NULL)
{
} // item_on_bridge() 


/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::bridge::item_on_bridge::~item_on_bridge()
{
  delete m_reference_item;
  m_reference_item = NULL;
} // item_on_bridge::~item_on_bridge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set item on the bridge.
 * \param The item on the bridge.
 */
void bear::bridge::item_on_bridge::set_item_on_bridge(engine::base_item* item)
{
  CLAW_ASSERT
    ( item != NULL,
      "An item_on_bridge is created with NULL item" );
  
  m_item = item_handle(item);
  
  m_reference_item = new bear::reference_item;
  m_reference_item->set_center_of_mass
    ( item->get_bottom_middle() + 
      universe::position_type(0,-s_line_width/2) );
} // item_on_bridge() 

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the reference item under the item on the bridge.
 */
bear::reference_item*
bear::bridge::item_on_bridge::get_reference_item()
{
  return m_reference_item;
} // item_on_bridge::get_reference_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the reference item under the item on the bridge.
 */
const bear::reference_item*
bear::bridge::item_on_bridge::get_reference_item() const
{
  return m_reference_item;
} // item_on_bridge::get_reference_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the item on the bridge.
 */
const bear::bridge::item_on_bridge::item_handle&
bear::bridge::item_on_bridge::get_item() const
{
  return m_item;
} // item_on_bridge::get_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compare two item on bridge.
 * \return reference_item.hrizontal_middle < 
 *         that.reference_item.hrizontal_middle
 */
bool bear::bridge::item_on_bridge::operator<
  (  const item_on_bridge& that ) const
{
  if ( m_reference_item == NULL )
    return false;
  else if ( that.m_reference_item == NULL )
    return true;
  else
    return m_reference_item->get_horizontal_middle() < 
      that.m_reference_item->get_horizontal_middle();
} // item_on_bridge::operator<()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::bridge::bridge()
  : m_max_fall(0), m_top_left_ref(NULL), m_top_right_ref(NULL),
    m_top_left_reference(NULL), m_top_right_reference(NULL)
{
} // bridge::bridge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::bridge::set_item_field
( const std::string& name, base_item* value )
{
  bool ok = true;

  if (name == "bridge.left_item")
    m_top_left_reference = value;
  else if (name == "bridge.right_item")
    m_top_right_reference = value;
  else
    ok = super::set_item_field(name, value);

  return ok;
} // bridge::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "real".
 * \param name The name of the field.
 * \param value The value of the field.
 */
bool bear::bridge::set_real_field
( const std::string& name, double value )
{
  bool result = true;

  if ( name == "bridge.max_fall" )
    m_max_fall = value;
  else
    result = super::set_real_field(name, value);

  return result;
} // bridge::set_real_field()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 */
void bear::bridge::progress( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);

  // update reference_item's positions
  items_list_iterator it;
  for ( it = m_items.begin(); it != m_items.end(); ++it )
    {
      if ( it->get_item().get() != NULL )
	{
	  universe::position_type pos;
	  pos.x = it->get_item()->get_horizontal_middle();
	  pos.y = it->get_item()->get_bottom() - s_line_width/2;

	  it->get_reference_item()->set_center_of_mass(pos);
	}
      else
	it->get_reference_item()->set_bottom(get_top() + 1);
    }

  update_items_list(); 
} // bridge::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
void bear::bridge::on_enters_layer()
{
  super::on_enters_layer();

  create_extremities(m_top_left_ref, m_top_left_reference);
  create_extremities(m_top_right_ref, m_top_right_reference);
} // bridge::on_enters_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visuals of this item.
 * \param visuals (out) The visuals.
 */
void bear::bridge::get_visual
( std::list<engine::scene_visual>& visuals ) const
{
  universe::coordinate_type length = 0;
  const visual::sprite s(get_sprite());
  
  const std::size_t n(get_bridge_length()/s.width());
  const universe::coordinate_type unity = get_bridge_length()/n;
  
  const base_item* start = m_top_left_ref;
  const base_item* end;
  items_list_const_iterator it;
  universe::position_type pos = m_top_left_ref->get_center_of_mass();
  
  for ( it = m_items.begin(); it != m_items.end(); ++it ) 
    {
      end = it->get_reference_item();
      
      add_bridge_visuals(visuals, start, end, unity, length , pos );

      start = end;
    }
  
  end = m_top_right_ref;
  add_bridge_visuals(visuals, start, end, unity, length , pos );

  if ( (unity - length) < 1 )
    add_bridge_visual(visuals, pos, end->get_center_of_mass() );
} // bridge::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::bridge::is_valid() const
{
  return (m_top_left_reference != NULL) && (m_top_right_reference != NULL) && 
    super::is_valid();
} // bridge::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::bridge::populate_loader_map( engine::item_loader_map& m )
{
  super::populate_loader_map(m);

  m.insert( engine::z_shift_loader(m_z_shift) );
} // bridge::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add bridge visuals on a line.
 * \param visuals (out) The visuals.
 * \param start The reference item at the start.
 * \param end The reference item at the end.
 * \param unity The distance between two visuals.
 * \param length The remaining distance for the first visual. 
 * \param previous_pos Th position of the last visual.
 */
void bear::bridge::add_bridge_visuals
( std::list<engine::scene_visual>& visuals,
  const base_item* start, const base_item* end,
  const universe::coordinate_type& unity,
  universe::coordinate_type& length, 
  universe::position_type& previous_pos ) const
{
  universe::vector_type dir = 
    end->get_center_of_mass() - start->get_center_of_mass();
  
  dir.normalize();

  universe::position_type origin = start->get_center_of_mass();
  
  while ( origin.x < end->get_horizontal_middle() )
    {
      universe::coordinate_type dist = unity - length;
      origin += (dir * dist); 
      
      if ( origin.x < end->get_horizontal_middle() ) 
	{
	  length = 0;	  
	  add_bridge_visual(visuals, previous_pos, origin);
	  previous_pos = origin; 
	}
      else
	length = previous_pos.distance(end->get_center_of_mass());
    }
} // bridge::add_bridge_visuals()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add bridge visual on a line.
 * \param visuals (out) The visuals.
 * \param left_pos Left position.
 * \param right_pos Right position.
 */
void bear::bridge::add_bridge_visual
( std::list<engine::scene_visual>& visuals,
  const universe::position_type& left_pos,
  const universe::position_type& right_pos ) const
{
  visual::sprite s(get_sprite());
      
  visual::position_type p
    ( ( left_pos + right_pos - s.get_size() ) / 2 + get_gap() );
  
  claw::math::line_2d<universe::coordinate_type> line
    ( left_pos, right_pos - left_pos );
  
  double angle = std::atan(line.direction.y / line.direction.x);
  s.set_angle(angle);
  
  visuals.push_front
    ( engine::scene_visual( p, s, get_z_position() ) );
} // bridge::add_bridge_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check collision on bridge.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::bridge::collision_as_bridge
( engine::base_item& that, universe::collision_info& info )
{  
  if ( ( ! that.is_phantom() ) && 
       satisfy_collision_condition(that) && 
       ( that.get_horizontal_middle() >= get_left() ) &&
       ( that.get_horizontal_middle() <= get_right() ) ) 
    {
	if ( align_on_bridge(that, info) )
	  {
	    that.set_contact_friction(get_top_friction());
	    m_z_shift.do_z_shift(that, *this);
	    search_or_create_item(that);
	  }
    }  
} // bridge::collision_as_bridge()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call collision_as_bridge().
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::bridge::collision
( engine::base_item& that, universe::collision_info& info )
{
  collision_as_bridge(that, info);
} // bridge::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Align \a that on the bridge.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
bool bear::bridge::align_on_bridge
( engine::base_item& that, universe::collision_info& info )
{
  bool result = false;
  
  universe::position_type previous_pos;
  universe::position_type next_pos;
  
  compute_neighboor
    (info.other_previous_state().get_bottom_middle(),previous_pos,next_pos);
  if ( check_item
       ( info.other_previous_state().get_bottom_middle(),
	 previous_pos, next_pos, 0 ) )
    {
      compute_neighboor(that.get_bottom_middle(),previous_pos,next_pos);

      if ( ! check_item( that.get_bottom_middle(), previous_pos, next_pos, 
			 s_line_width ) ) 
	{
	  universe::position_type pos = 
	    compute_align_position(that,previous_pos, next_pos);

	  apply_angle_to(that,info);
	  
	  return collision_align_top(info, pos);	  
	}
    }
  return result;
} // bridge::align_on_bridge()
	  
/*----------------------------------------------------------------------------*/
/**
 * \brief Compute neighboor of a position.
 * \param that The other item of the collision.
 * \param previous_pos Position of previous item.
 * \param next_pos Position of next item.
 */
bear::universe::position_type bear::bridge::compute_align_position
( engine::base_item& that, const universe::position_type& previous_pos,
  const universe::position_type& next_pos) const
{ 
  claw::math::line_2d<universe::coordinate_type> line
    ( previous_pos, next_pos - previous_pos );
  
  universe::position_type pos
    ( that.get_left(),
      std::max( std::min( compute_giving_way(that),
			  line.y_value(that.get_horizontal_middle()) )
		+ s_line_width/2,
		that.get_bottom() - s_line_width/2 ) );

  return pos;
} // bridge::compute_align_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute neighboor of a position.
 * \param pos The considered position.
 * \param previous_pos Position of previous item.
 * \param next_pos Position of next item.
 */
void bear::bridge::compute_neighboor
( const universe::position_type& pos, universe::position_type& previous_pos,
  universe::position_type& next_pos) const
{ 
  items_list_const_iterator it; 

  previous_pos = m_top_left_ref->get_center_of_mass();
  it = m_items.begin();
  
  if ( it == m_items.end() )
    next_pos = m_top_right_ref->get_center_of_mass();
  else
    {
      bool ok = true;

      for ( ; (it != m_items.end()) && ok; ++it )
	if ( it->get_reference_item() != NULL )
	  {
	    if ( it->get_reference_item()->get_horizontal_middle() < pos.x )
	      previous_pos = 
		it->get_reference_item()->get_center_of_mass();
	    else 
	      {
		ok = false;
		next_pos = it->get_reference_item()->get_center_of_mass();
	      }
	  }
      
      if ( ok )
	next_pos = m_top_right_ref->get_center_of_mass();
    }
} // bridge::compute_neighboor()

/*----------------------------------------------------------------------------*/
/**
 * \brief Search an item in the items lists. Item is created if not found.
 * \param that The item to search.
 */
void bear::bridge::search_or_create_item(engine::base_item& that)
{
  items_list_iterator it;
  
  bool found(false);

  for ( it = m_items.begin(); ( it != m_items.end() ) && ( ! found ); ++it )
    if ( it->get_item() == that )
      found = true;
  
  if ( ! found )
    {
      m_items.push_front(item_on_bridge());
      m_items.front().set_item_on_bridge(&that);
      update_items_list();
    }
} // bridge::search_or_create_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Update list of items.
 */
void bear::bridge::update_items_list()
{
  if ( ! m_items.empty() ) 
    {
      m_items.sort();
      bool changement;
      
      do
	{
	  changement = false;
	  items_list_iterator next_it, current_it; 
	  universe::position_type previous_pos = 
	    m_top_left_ref->get_center_of_mass();
	  universe::position_type next_pos;
	  
	  current_it = m_items.begin();
	  next_it = m_items.begin();
	  next_it++;
	  if ( next_it != m_items.end() )
	    next_pos = next_it->get_reference_item()->get_center_of_mass();
	  else
	    next_pos = m_top_right_ref->get_center_of_mass();
	  
	  for ( ; current_it != m_items.end(); )
	    {
	      if ( check_erase_item(current_it,previous_pos,next_pos) )
		{
		  changement = true;
		  previous_pos = 
		    current_it->get_reference_item()->get_center_of_mass();
		  m_items.erase(current_it);
		}
	      else
		previous_pos = 
		  current_it->get_reference_item()->get_center_of_mass();
	      
	      current_it = next_it;
	      if ( next_it != m_items.end() )
		next_it++;
	      
	      if ( next_it != m_items.end() ) 
		next_pos = next_it->get_reference_item()->get_center_of_mass();
	      else
		next_pos = 
		  m_top_right_ref->get_center_of_mass();
	    }
	}
      while ( changement );
    }
} // bridge::update_items_list()

/*----------------------------------------------------------------------------*/
/**
 * \brief Check if a given item on bridge must be erase.
 * \param it Iterator on item to consider.
 * \param previous_pos Position of previous item.
 * \param next_pos Position of next item.
 */
  bool bear::bridge::check_erase_item
  (items_list_iterator it, const universe::position_type& previous_pos,
   const universe::position_type& next_pos) const
{
  return 
    check_item
    ( it->get_reference_item()->get_center_of_mass(), 
      previous_pos, next_pos, 0 ) ||
    ( it->get_item().get() == NULL ) ||
    ( it->get_item()->get_bottom() > get_top() ) ||
    ( it->get_item()->get_horizontal_middle() < get_left() )|| 
    ( it->get_item()->get_horizontal_middle() > get_right() );
} // bridge::check_erase_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the y coordinate of a given item.
 * \param item The considered item. 
 */
bear::universe::coordinate_type 
bear::bridge::compute_giving_way(const base_item& item) const
{
  universe::coordinate_type coord = 
    item.get_mass() * 
    ( ( get_width() / 2 ) -
      std::abs(item.get_horizontal_middle() - get_horizontal_middle()) ) * 
      m_max_fall / ( get_width() * 50 );

  if ( coord > m_max_fall ) 
    coord = m_max_fall;

  claw::math::line_2d<universe::coordinate_type> line
    ( m_top_left_ref->get_center_of_mass(), 
      m_top_right_ref->get_center_of_mass() - 
      m_top_left_ref->get_center_of_mass());
  
  return line.y_value( item.get_horizontal_middle() ) - coord;
} // bridge::compute_giving_way()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute if an item is over a line created by two contiguous item.
 * \param pos Position of current item.
 * \param previous_pos Position of previous item.
 * \param next_pos Position of next item.
 */
bool bear::bridge::check_item
( const universe::position_type& pos,
  const universe::position_type& previous_pos,
  const universe::position_type& next_pos,
  universe::coordinate_type margin ) const
{
  claw::math::line_2d<universe::coordinate_type> line
    ( previous_pos + universe::position_type(0,margin), 
      next_pos - previous_pos );
  
  return pos.y > line.y_value( pos.x );
} // bridge::check_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the items concerned by a progress/move of this one.
 * \param d (out) A list to which are added such items.
 */
void bear::bridge::get_dependent_items( std::list<physical_item*>& d ) const
{
  items_list_const_iterator it;
  
  for( it=m_items.begin(); it!=m_items.end(); ++it )
    if ( it->get_item().get() != NULL ) 
      d.push_front( it->get_item().get() );

  d.push_front(m_top_left_ref);
  d.push_front(m_top_right_ref);
} // bridge::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the angle of the slope to a colliding item.
 * \param that The other item in the collision.
 * \param info Informations on the collision.
 */
void bear::bridge::apply_angle_to
( engine::base_item& that, const universe::collision_info& info) const
{
  universe::position_type left_pos(that.get_bottom_left());
  universe::position_type right_pos(that.get_bottom_right());
  universe::position_type previous_pos;
  universe::position_type next_pos;
  
  compute_neighboor
    (that.get_bottom_left(),previous_pos,next_pos);
  claw::math::line_2d<universe::coordinate_type> line1
    ( previous_pos, next_pos - previous_pos );
  left_pos.y = line1.y_value(that.get_left());

  compute_neighboor
    (that.get_bottom_right(),previous_pos,next_pos);
  claw::math::line_2d<universe::coordinate_type> line2
    ( previous_pos, next_pos - previous_pos );
  right_pos.y = line2.y_value(that.get_right());
  
  claw::math::line_2d<universe::coordinate_type> line
    ( left_pos, right_pos - left_pos );
  double angle = std::atan(line.direction.y / line.direction.x);
  
  that.set_contact_angle(angle);

  info.get_collision_repair().set_contact_normal
    (that, that.get_x_axis().get_orthonormal_anticlockwise());
} // bridge::apply_angle_to()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get bridge length.
 */
bear::universe::coordinate_type bear::bridge::get_bridge_length() const
{
  items_list_const_iterator it;
  const base_item* previous = m_top_left_ref;
  
  universe::coordinate_type length(0);
  
  for( it=m_items.begin(); it!=m_items.end(); ++it )
    {
      length += 
	it->get_reference_item()->get_center_of_mass().distance
	(previous->get_center_of_mass());
      previous = it->get_reference_item();
    }

  length += previous->get_center_of_mass().distance
    ( m_top_right_ref->get_center_of_mass() );

  return length;
} // bridge::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create an extremities.
 */
void bear::bridge::create_extremities
( base_item* & item, base_item* reference)
{  
  item = new bear::reference_item;

  CLAW_ASSERT
    (item->is_valid(),
     "The reference_item created by bridge isn't correctly initialized" );

  item->set_center_of_mass
    ( reference->get_center_of_mass() + 
      universe::position_type(0,-s_line_width/2));
  
  new_item( *item );

  bear::universe::forced_tracking mvt;
  mvt.set_reference_point_on_center(*reference);
  
  item->set_forced_movement( mvt );
} // bridge::create_extremities()
