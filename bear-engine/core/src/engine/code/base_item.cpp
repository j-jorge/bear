/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::base_item class.
 * \author Julien Jorge.
 */
#include "engine/base_item.hpp"

#include <algorithm>
#include <claw/logger.hpp>

#include "engine/layer/layer.hpp"
#include "engine/loader/base_item_loader.hpp"
#include "engine/loader/item_loader_fallback.hpp"
#include "engine/level.hpp"
#include "universe/collision_info.hpp"

#include "visual/scene_element_sequence.hpp"

#include "debug/scoped_time_measure.hpp"

/*----------------------------------------------------------------------------*/
bear::engine::base_item::id_type bear::engine::base_item::s_next_id = 1;

#ifndef NDEBUG
std::list<bear::engine::base_item*> bear::engine::base_item::s_allocated;

struct item_count
{
public:
  struct decreasing_max
  {
    bool operator()( const item_count& a, const item_count& b ) const
    {
      if ( a.get_max() != b.get_max() )
        return a.get_max() > b.get_max();

      if ( a.get_max_size() != b.get_max_size() )
        return a.get_max_size() > b.get_max_size();

      return a.get_size() > b.get_size();
    }
  }; // struct decreasing_max()

public:
  item_count()
    : m_count(0), m_max(0), m_size(0)
  { }

  item_count( const bear::engine::base_item& item )
    : m_count(1), m_max(1), m_size(item.size_of())
  { }

  void add()
  {
    ++m_count;
    m_max = std::max(m_count, m_max);
  }

  void remove()
  {
    --m_count;
  }

  std::size_t get_max() const { return m_max; }
  std::size_t get_max_size() const { return m_max * m_size; }
  std::size_t get_size() const { return m_size; }

private:
  std::size_t m_count;
  std::size_t m_max;
  std::size_t m_size;
};

struct item_counter
{
private:
  typedef std::map<std::string, item_count> counter_map;

public:
  void count( const bear::engine::base_item& item )
  {
    if ( m_counter.find( item.get_class_name() ) == m_counter.end() )
      m_counter[item.get_class_name()] = item_count(item);
    else
      m_counter[item.get_class_name()].add();
  }

  void uncount( const bear::engine::base_item& item )
  {
    m_counter[item.get_class_name()].remove();
  }

  ~item_counter()
  {
    typedef
      std::multimap
      < item_count, std::string, item_count::decreasing_max > max_map_type;
    max_map_type by_max;
    
    for ( counter_map::const_iterator it=m_counter.begin();
          it!=m_counter.end(); ++it )
      by_max.insert( max_map_type::value_type( it->second, it->first ) );

    std::size_t c(0), s(0);
    for ( max_map_type::const_iterator it=by_max.begin();
          it!=by_max.end(); ++it )
      {
        c += it->first.get_max();
        s += it->first.get_max_size();

        std::cout << it->second << ": " << it->first.get_max() << " instances "
                  << it->first.get_max_size() << " bytes ("
                  << it->first.get_size() << " each)." << std::endl;
      }

    std::cout << "sum: " << c << " instances " << s << " bytes." << std::endl;
  }
private:
  std::map<std::string, item_count> m_counter;

}; // struct

static item_counter s_item_counter;
#endif

/*----------------------------------------------------------------------------*/
/**
 * \brief Print the address of the items that were not deleted.
 */
void bear::engine::base_item::print_allocated()
{
#ifdef BEAR_DEBUG_ALLOCATED_ITEMS
  unsigned int s = s_allocated.size();

  if ( s == 0 )
    claw::logger << claw::log_verbose << "All base_item have been deleted."
                 << std::endl;
  else
    {
      claw::logger << claw::log_verbose << s
                   << " base_item have NOT been deleted." << std::endl;

      std::list<base_item*>::const_iterator it;

      for (it=s_allocated.begin(); it!=s_allocated.end(); ++it)
        {
          std::string str;
          (*it)->to_string( str );

          claw::logger << claw::log_verbose << "-- Item\n" << str
                       << std::endl;
        }
    }
#endif
} // base_item::print_allocated()

/*----------------------------------------------------------------------------*/
/**
 * \brief Contructor.
 */
bear::engine::base_item::base_item()
  : m_id( s_next_id ), m_layer(NULL), m_z_position(0),
    m_flags(item_flag_z_fixed), m_dying(false), m_world(NULL)
{
  ++s_next_id;
#ifndef NDEBUG
  s_allocated.push_front(this);
#endif
} // base_item::base_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy contructor.
 */
bear::engine::base_item::base_item( const base_item& that )
  : level_object(that), physical_item(that), m_id( s_next_id ), m_layer(NULL),
    m_z_position(that.get_z_position()), m_flags(item_flag_z_fixed),
    m_dying(false)
{
  m_flags |= that.m_flags & item_flag_built;

  ++s_next_id;
#ifndef NDEBUG
  s_allocated.push_front(this);
#endif
} // base_item::base_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destructor.
 */
bear::engine::base_item::~base_item()
{
#ifndef NDEBUG
  s_item_counter.uncount(*this);
  s_allocated.erase( std::find(s_allocated.begin(), s_allocated.end(), this) );
#endif
} // base_item::~base_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Instanciate a copy of this item.
 */
bear::engine::base_item* bear::engine::base_item::clone() const
{
  return new base_item(*this);
} // base_item::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the item if it has never been built.
 */
void bear::engine::base_item::build_item()
{
  if ( (m_flags & item_flag_built) == item_flag_none )
    {
#ifndef NDEBUG
      s_item_counter.count(*this);
#endif
      m_flags |= item_flag_built;
      build();
    }
} // base_item::build_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the loaders to use to initialize this item from a level file.
 */
bear::engine::item_loader_map bear::engine::base_item::get_loaders()
{
  item_loader_map result( item_loader_fallback(*this) );

  populate_loader_map( result );

  return result;
} // base_item::get_loaders()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the item that it is in a layer now.
 */
void bear::engine::base_item::enters_layer()
{
  on_enters_layer();
} // base_item::enters_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Pre-cache the resources you need. This method is called automatically
 *        by the owner.
 */
void bear::engine::base_item::pre_cache()
{
  // nothing to do
} // base_item::pre_cache()

/*----------------------------------------------------------------------------*/
/**
 * \brief Build the item. This method is called automatically by the owner.
 */
void bear::engine::base_item::build()
{
  // nothing to do
} // base_item::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the item it is in a layer now.
 */
void bear::engine::base_item::on_enters_layer()
{
  // nothing to do
} // base_item::on_enters_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Destroy the item. This method is called automatically when the item
 *        is killed.
 * \pre The item has been added in a layer.
 */
void bear::engine::base_item::destroy()
{
  // nothing to do
} // base_item::destroy()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::base_item::progress( universe::time_type elapsed_time )
{
  // nothing to do
} // base_item::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Insert the visual of the item at the end of a given list.
 * \param visuals The list in which the visual is added.
 *
 * This method uses get_visual() to get the visuals and make a
 * visual::scene_element_sequence of them.
 */
void
bear::engine::base_item::insert_visual( std::list<scene_visual>& visuals ) const
{
  BEAR_CREATE_SCOPED_TIMELOG
    ( std::string("insert_visual ") + get_class_name() );

  std::list<scene_visual> v;
  get_visual( v );

  if ( v.size() > 1 )
    {
      v.sort( scene_visual::z_position_compare() );
      visual::scene_element_sequence e;

      for ( ; !v.empty() ; v.pop_front() )
        {
#ifndef NDEBUG
          const visual::scene_element& elem( v.front().scene_element );

          if ( !elem.always_displayed()
               && elem.get_bounding_box().empty() )
            claw::logger << claw::log_warning
                         << "Empty visual::scene_element is inserted in a "
                         << "visual::scene_element_sequence by '"
                         << get_class_name() << "'. This should be avoided."
                         << std::endl;
#endif // NDEBUG

          e.push_back(v.front().scene_element);
        }

      visuals.push_back( scene_visual(e, get_z_position()) );
    }
  else if (v.size() == 1)
    {
      visuals.push_back( v.front() );
      visuals.back().z_position = get_z_position();
    }
} // base_item::insert_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the visuals representing the item.
 * \param visuals (out) The sprites of the item, and their positions.
 */
void
bear::engine::base_item::get_visual( std::list<scene_visual>& visuals ) const
{
  // nothing to do
} // base_item::get_visual()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c unsigned \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_u_integer_field
( const std::string& name, unsigned int value )
{
  return false;
} // base_item::set_u_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::base_item::set_integer_field( const std::string& name, int value )
{
  return false;
} // base_item::set_integer_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::base_item::set_real_field( const std::string& name, double value )
{
  return false;
} // base_item::set_real_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::base_item::set_bool_field( const std::string& name, bool value )
{
  return false;
} // base_item::set_bool_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_string_field
( const std::string& name, const std::string& value )
{
  return false;
} // base_item::set_string_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c base_item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_item_field
( const std::string& name, base_item* value )
{
  return false;
} // base_item::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::sprite.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_sprite_field
( const std::string& name, const visual::sprite& value )
{
  return false;
} // base_item::set_sprite_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_animation_field
( const std::string& name, const visual::animation& value )
{
  return false;
} // base_item::set_animation_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c audio::sample.
 * \param name The name of the field.
 * \param value The new value of the field. The item receiving this value must
 *        release de memory with a delete at some time.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_sample_field
( const std::string& name, audio::sample* value )
{
  return false;
} // base_item::set_sample_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::font.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_font_field
( const std::string& name, visual::font value )
{
  return false;
} // base_item::set_font_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c visual::color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_color_field
( const std::string& name, visual::color value )
{
  return false;
} // base_item::set_color_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of unsigned integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_u_integer_list_field
( const std::string& name, const std::vector<unsigned int>& value )
{
  return false;
} // base_item::set_u_integer_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of integer.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::base_item::set_integer_list_field
( const std::string& name, const std::vector<int>& value )
{
  return false;
} // base_item::set_integer_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of real.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::base_item::set_real_list_field
( const std::string& name, const std::vector<double>& value )
{
  return false;
} // base_item::set_real_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of bool.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool
bear::engine::base_item::set_bool_list_field
( const std::string& name, const std::vector<bool>& value )
{
  return false;
} // base_item::set_bool_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of string.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_string_list_field
( const std::string& name, const std::vector<std::string>& value )
{
  return false;
} // base_item::set_string_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of item.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_item_list_field
( const std::string& name, const std::vector<base_item*>& value )
{
  return false;
} // base_item::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::sprite.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_sprite_list_field
( const std::string& name, const std::vector<visual::sprite>& value )
{
  return false;
} // base_item::set_sprite_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::animation.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_animation_list_field
( const std::string& name, const std::vector<visual::animation>& value )
{
  return false;
} // base_item::set_animation_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of audio::sample.
 * \param name The name of the field.
 * \param value The new value of the field. The item receiving this value must
 *        release de memory with a delete at some time.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_sample_list_field
( const std::string& name, const std::vector<audio::sample*>& value )
{
  return false;
} // base_item::set_sample_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::font.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_font_list_field
( const std::string& name, const std::vector<visual::font>& value )
{
  return false;
} // base_item::set_font_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type \c list of visual::color.
 * \param name The name of the field.
 * \param value The new value of the field.
 * \return false if the field "name" is unknow, true otherwise.
 */
bool bear::engine::base_item::set_color_list_field
( const std::string& name, const std::vector<visual::color>& value )
{
  return false;
} // base_item::set_color_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is correctly initialized.
 */
bool bear::engine::base_item::is_valid() const
{
  return true;
} // base_item::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get item's identifier.
 */
bear::engine::base_item::id_type bear::engine::base_item::get_id() const
{
  return m_id;
} // base_item::get_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get item's class name.
 */
const char* bear::engine::base_item::get_class_name() const
{
  return "bear::engine::base_item";
} // base_item::get_class_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the size of the derived class.
 */
std::size_t bear::engine::base_item::size_of() const
{
  return sizeof(*this);
} // base_item::size_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Kill the item.
 * \pre m_owner != NULL
 */
void bear::engine::base_item::kill()
{
  claw::logger << claw::log_verbose
               << "Killing id #" << m_id << std::endl;

  CLAW_PRECOND( m_layer != NULL );
  CLAW_PRECOND( !get_insert_as_static() );

  if (!m_dying)
    {
      m_dying = true;
      destroy();
      m_layer->remove_item(*this);
      m_world = NULL;

      for ( life_chain_list::iterator it = m_life_chain.begin();
            it != m_life_chain.end(); ++it )
        if ( *it != (base_item*)NULL )
          (*it)->kill();
      
      m_life_chain.clear();
    }
} // base_item::kill()

/*----------------------------------------------------------------------------*/
/**
 * \brief Indicates if the fonction "kill" has been called.
 */
bool bear::engine::base_item::is_dead() const
{
  return m_dying;
} // base_item::is_dead()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove the data about the environment of the item.
 * \post m_layer == NULL
 *
 * This method is called automatically when the item is removed from a layer.
 */
void bear::engine::base_item::clear_environment()
{
  clear_level();

  m_layer = NULL;
} // base_item::clear_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell the environment in which the item is.
 * \param the_level The level in which we are.
 * \pre m_layer == NULL
 *
 * This method is called automatically when the item is added in a layer.
 */
void bear::engine::base_item::set_environment( layer& the_layer )
{
  CLAW_PRECOND( m_layer == NULL );

  set_level( the_layer.get_level() );

  m_layer = &the_layer;
} // base_item::set_environment()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tells if the item is in any layer.
 */
bool bear::engine::base_item::is_in_layer() const
{
  return m_layer != NULL;
} // base_item::is_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if the item is in a given layer.
 * \param the_layer The layer where the item could be.
 */
bool bear::engine::base_item::is_in_layer
( const bear::engine::layer& the_layer ) const
{
  return m_layer == &the_layer;
} // base_item::is_in_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the position of the item on the Z axis.
 * \param z The new position.
 */
void bear::engine::base_item::set_z_position( int z )
{
  m_z_position = z;
} // base_item::set_z_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the position of the item on the Z axis.
 */
int bear::engine::base_item::get_z_position() const
{
  return m_z_position;
} // base_item::get_z_position()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the z fixed statut.
 * \param fixed The new statut.
 */
void bear::engine::base_item::set_z_fixed( bool fixed )
{
  if (fixed)
    m_flags |= item_flag_z_fixed;
  else
    m_flags &= ~item_flag_z_fixed;
} // base_item::set_z_fixed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the z fixed statut.
 */
bool bear::engine::base_item::is_z_fixed() const
{
  return m_flags & item_flag_z_fixed;
} // base_item::is_z_fixed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Call this method before inserting the item in a level to ensure it
 *        will be static in the world.
 */
void bear::engine::base_item::set_insert_as_static()
{
  m_flags |= item_flag_insert_as_static;
} // base_item::set_insert_as_static()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item has to be inserted as a static item in the level.
 */
bool bear::engine::base_item::get_insert_as_static() const
{
  return m_flags & item_flag_insert_as_static;
} // base_item::get_insert_as_static()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if this item is in a world.
 */
bool bear::engine::base_item::has_world() const
{
  return m_world != NULL;
} // base_item::has_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the world in which this item is.
 */
const bear::engine::world& bear::engine::base_item::get_world() const
{
  CLAW_PRECOND( has_world() );
  return *m_world;
} // base_item::get_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the world in which this item is.
 * \param w The world.
 */
void bear::engine::base_item::set_world( const world& w )
{
  m_world = &w;
} // base_item::set_world()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the layer in which the item is.
 * \pre m_layer != NULL
 */
bear::engine::layer& bear::engine::base_item::get_layer() const
{
  CLAW_PRECOND( m_layer != NULL );

  return *m_layer;
} // base_item::get_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add an item in the same environment of \a this.
 * \pre m_layer != NULL
 */
void bear::engine::base_item::new_item( base_item& item ) const
{
  CLAW_PRECOND( m_layer != NULL );

  m_layer->add_item(item);
} // base_item::new_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds an item that must be killed when this item dies.
 * \param item The item to kill when this one dies.
 */
void bear::engine::base_item::kill_when_dying( base_item& item )
{
  clean_life_chain();
  m_life_chain.push_back( item );
} // base_item::kill_when_dying()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets the items that must be updated when this one is updated.
 * \param d (out) The items to update with this one.
 */
void bear::engine::base_item::get_dependent_items
( std::list<universe::physical_item*>& d ) const
{
  super::get_dependent_items(d);

  for ( life_chain_list::const_iterator it = m_life_chain.begin();
        it != m_life_chain.end(); ++it )
    if ( *it != (base_item*)NULL )
      d.push_back( it->get_item() );
} // base_item::get_dependent_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell that this item is always displayed.
 * \param b Always displayed or not.
 */
void bear::engine::base_item::set_always_displayed(bool b)
{
  if (m_layer != NULL)
    {
      if (b)
        m_layer->set_always_displayed(*this);
      else
        m_layer->unset_always_displayed(*this);
    }
} // base_item::set_always_displayed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Add the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
void bear::engine::base_item::populate_loader_map( item_loader_map& m )
{
  m.insert( base_item_loader(*this) );
} // base_item::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief A collision with an other item has occured.
 * \param that The other item of the collision.
 * \param info Some informations about the collision.
 */
void bear::engine::base_item::collision
( base_item& that, universe::collision_info& info )
{

} // base_item::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Give a string representation of the item.
 * \param str (out) The result of the convertion.
 */
void bear::engine::base_item::to_string( std::string& str ) const
{
  std::ostringstream oss;
  oss << "id/class: " << m_id << '/' << get_class_name() << "\n";
  oss << "pos_z: " << m_z_position << " (fixed =" << is_z_fixed() << ")\n";

  universe::physical_item::to_string(str);

  str = oss.str() + str;
} // base_item::to_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
void bear::engine::base_item::init_exported_methods()
{
  TEXT_INTERFACE_CONNECT_METHOD_1
    ( base_item, set_z_position, void, int );

  TEXT_INTERFACE_CONNECT_METHOD_0
    ( base_item, kill, void );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_0
    ( base_item, universe::physical_item_state,
      remove_position_constraint_x, void );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_0
    ( base_item, universe::physical_item_state,
      remove_position_constraint_y, void );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_0
    ( base_item, universe::physical_item_state,
      add_position_constraint_x, void );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_0
    ( base_item, universe::physical_item_state,
      add_position_constraint_y, void );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_speed, void, double, double );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_center_of_mass, void,
      const universe::coordinate_type&, const universe::coordinate_type& );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_top_left, void,
      const universe::coordinate_type&, const universe::coordinate_type& );
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_top_middle, void,
      const universe::coordinate_type&, const universe::coordinate_type& );
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_top_right, void,
      const universe::coordinate_type&, const universe::coordinate_type& );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_bottom_left, void,
      const universe::coordinate_type&, const universe::coordinate_type& );
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_bottom_middle, void,
      const universe::coordinate_type&, const universe::coordinate_type& );
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_bottom_right, void,
      const universe::coordinate_type&, const universe::coordinate_type& );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_left_middle, void,
      const universe::coordinate_type&, const universe::coordinate_type& );
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_2
    ( base_item, universe::physical_item_state,
      set_right_middle, void,
      const universe::coordinate_type&, const universe::coordinate_type& );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_1
    ( base_item, universe::physical_item_state,
      set_horizontal_middle, void, universe::coordinate_type );
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_1
    ( base_item, universe::physical_item_state,
      set_vertical_middle, void, universe::coordinate_type );

  TEXT_INTERFACE_CONNECT_PARENT_METHOD_1
    ( base_item, universe::physical_item_state, set_center_on, void,
      const universe::physical_item_state& );
} // god::init_exported_methods()

/*----------------------------------------------------------------------------*/
/**
 * \brief Cast the other item and call collision(base_item, collision_info).
 * \param info Some informations about the collision.
 */
void bear::engine::base_item::collision( universe::collision_info& info )
{
  base_item* o = dynamic_cast<base_item*>(&info.other_item());

  if (o)
    collision(*o, info);
  else
    claw::logger << claw::log_error
     << "bear::engine::base_item::collision(): Collision with an "
     << "item of type different of bear::engine::base_item."
     << std::endl;
} // base_item::collision()

/*----------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
void bear::engine::base_item::time_step( universe::time_type elapsed_time )
{
  BEAR_CREATE_SCOPED_TIMELOG( std::string("progress ") + get_class_name() );

  if (!m_dying)
    progress(elapsed_time);
} // base_item::time_step()

/*----------------------------------------------------------------------------*/
/**
 * \brief Removes the items that are already dead in m_life_chain.
 */
void bear::engine::base_item::clean_life_chain()
{
  life_chain_list::iterator it = m_life_chain.begin();

  while ( it != m_life_chain.end() )
    if ( *it == (universe::physical_item*)NULL )
      {
        life_chain_list::iterator tmp(it);
        ++it;
        m_life_chain.erase( tmp );
      }
    else
      ++it;
} // base_item::clean_life_chain()

/*----------------------------------------------------------------------------*/
TEXT_INTERFACE_IMPLEMENT_METHOD_LIST( bear::engine::base_item )
