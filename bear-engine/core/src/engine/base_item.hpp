/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The basic physical item of our game.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_ITEM_HPP__
#define __ENGINE_BASE_ITEM_HPP__

#include <string>

#include "engine/class_export.hpp"

#include "universe/derived_item_handle.hpp"
#include "universe/physical_item.hpp"
#include "engine/item_flag_type.hpp"
#include "engine/level_object.hpp"
#include "engine/scene_visual.hpp"
#include "engine/loader/item_loader_map.hpp"
#include "text_interface/base_exportable.hpp"
#include "audio/sample.hpp"

#include "visual/animation.hpp"
#include "visual/color.hpp"
#include "visual/sprite.hpp"
#include "visual/font/font.hpp"

namespace bear
{
  namespace engine
  {
    class layer;
    class level;
    class world;

    /**
     * \brief The basic physical item of our game.
     * \author Julien Jorge
     *  - \a position.left: \c The x-coordinate of the left edge,
     *  - \a position.bottom: \c The y-coordinate of the bottom egde,
     *  - \a position.depth: \c The z-coordinate,
     *  - \a size.height: \c The height of the box,
     *  - \a size.width: \c The width of the box,
     *  - \a mass: \c Mass of the item,
     *  - \a density: \c Density of the item,
     *  - \a elasticty: \c Elasticity of the item,
     *  - \a hardness: \c Hardness of the item,
     *  - \a system_angle: \c Angle of the item,
     *  - \a speed.x: \c The initial speed on the x-axis,
     *  - \a speed.y: \c The initial speed on the y-axis,
     *  - \a angular_speed: \c The initial angular speed,
     *  - \a artificial: \c Tell if the item is artificial,
     *  - \a can_move_items: \c Tell if the item can move other items,
     *  - \a phantom: \c Tell if the item is a phantom,
     *  - \a global: \c Tell if the item is global,
     *  - \a fixed.x: \c Tell if the item is fixed on x-coordinate,
     *  - \a fixed.y: \c Tell if the item is fixed on y-coordinate,
     *  - \a free_system: \c Tell if the system angle of the item is not
     *    modified by the collisions.
     */
    class ENGINE_EXPORT base_item:
      public universe::physical_item,
      public text_interface::base_exportable,
      virtual public level_object
    {
    public:
      /** \brief type of the unique identifier of items. */
      typedef unsigned int id_type;

      /** \brief The type of the class that stores our fields. */
      typedef physical_item_state state_type;

      /** \brief The type of the parent class. */
      typedef universe::physical_item super;

      TEXT_INTERFACE_DECLARE_METHOD_LIST
      (text_interface::base_exportable, init_exported_methods);

    private:

      /** \brief The type of the handles on a base_item. */
      typedef universe::derived_item_handle<base_item> item_handle;

      /** \brief The type of the list of the items to kill with this item. */
      typedef std::list<item_handle> life_chain_list;

    public:
      static void print_allocated();

      base_item();
      base_item( const base_item& that );
      ~base_item();

      virtual base_item* clone() const;

      void build_item();
      bool is_built() const;

      void enters_layer();
      void leaves_layer();

      item_loader_map get_loaders();

      virtual void pre_cache();
      virtual void progress( universe::time_type elapsed_time );

      void insert_visual( std::list<scene_visual>& visuals ) const;
      virtual void get_visual( std::list<scene_visual>& visuals ) const;

      virtual bool set_u_integer_field
      ( const std::string& name, unsigned int value );
      virtual bool set_integer_field( const std::string& name, int value );
      virtual bool set_real_field( const std::string& name, double value );
      virtual bool set_bool_field( const std::string& name, bool value );
      virtual bool set_string_field
      ( const std::string& name, const std::string& value );
      virtual bool set_item_field
      ( const std::string& name, base_item* value );
      virtual bool set_sprite_field
      ( const std::string& name, const visual::sprite& value );
      virtual bool set_animation_field
      ( const std::string& name, const visual::animation& value );
      virtual bool set_sample_field
      ( const std::string& name, audio::sample* value );
      virtual bool set_font_field
      ( const std::string& name, visual::font value );
      virtual bool set_color_field
      ( const std::string& name, visual::color value );

      virtual bool set_u_integer_list_field
      ( const std::string& name, const std::vector<unsigned int>& value );
      virtual bool set_integer_list_field
      ( const std::string& name, const std::vector<int>& value );
      virtual bool set_real_list_field
      ( const std::string& name, const std::vector<double>& value );
      virtual bool set_bool_list_field
      ( const std::string& name, const std::vector<bool>& value );
      virtual bool set_string_list_field
      ( const std::string& name, const std::vector<std::string>& value );
      virtual bool set_item_list_field
      ( const std::string& name, const std::vector<base_item*>& value );
      virtual bool set_sprite_list_field
      ( const std::string& name, const std::vector<visual::sprite>& value );
      virtual bool set_animation_list_field
      ( const std::string& name, const std::vector<visual::animation>& value );
      virtual bool set_sample_list_field
      ( const std::string& name, const std::vector<audio::sample*>& value );
      virtual bool set_font_list_field
      ( const std::string& name, const std::vector<visual::font>& value );
      virtual bool set_color_list_field
      ( const std::string& name, const std::vector<visual::color>& value );

      virtual bool is_valid() const;

      id_type get_id() const;

      virtual const char* get_class_name() const;
      virtual std::size_t size_of() const;

      void kill();
      bool is_dead() const;

      void clear_environment();
      void set_environment( layer& owner );
      bool is_in_layer() const;
      bool is_in_layer( const layer& the_layer ) const;

      void set_z_position( int z );
      int get_z_position() const;

      void set_z_fixed( bool fixed );
      bool is_z_fixed() const;

      void set_insert_as_static();
      bool get_insert_as_static() const;

      bool has_world() const;
      const world& get_world() const;
      /* public only for engine::world. */
      void set_world( const world& w );

      layer& get_layer() const;
      void new_item( base_item& item ) const;

      void kill_when_dying( base_item& item );
      virtual void get_dependent_items
        ( std::list<universe::physical_item*>& d ) const;
      
    protected:
      void set_always_displayed(bool b);

      virtual void populate_loader_map( item_loader_map& m );

      virtual void collision
      ( base_item& that, universe::collision_info& info );

      virtual void build();
      virtual void on_enters_layer();
      virtual void on_leaves_layer();
      virtual void destroy();

      void to_string( std::string& str ) const;

      static void init_exported_methods();

    private:
      void collision( universe::collision_info& info );
      void time_step( universe::time_type elapsed_time );
      
      void clean_life_chain();

    private:
      /**
       * \brief Unique identifier.
       *
       * Valid identifiers are strictly greater than zero.
       */
      const id_type m_id;

      /** \brief The layer in which I am. */
      layer* m_layer;

      /** \brief The z position of the item in the layer. */
      int m_z_position;

      /** \brief Flags applyed to the item. */
      item_flag_type m_flags;

      /** \brief Tell if the kill() method has been called. */
      bool m_dying;

      /** \brief The world in which the item is. */
      world const* m_world;

      /** \brief The items to kill with this item. */
      life_chain_list m_life_chain;

      /** \brief Next item identifier. */
      static id_type s_next_id;

#ifndef NDEBUG
      /** \brief A list in which we store the reference of the items, so we can
          detect memory leaks. */
      static std::list<base_item*> s_allocated;
#endif

    }; // class base_item

  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_ITEM_HPP__
