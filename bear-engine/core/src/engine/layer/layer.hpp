/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A layer represent a part of the world, but with an orhtogonal view.
 *        Each layer is a little environment with its own items.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LAYER_HPP__
#define __ENGINE_LAYER_HPP__

#include "concept/region.hpp"
#include "engine/base_item.hpp"
#include "visual/shader_program.hpp"

#include <claw/math.hpp>
#include <set>

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class level;
    class world;

    /**
     * \brief A layer represent a part of the world, but with an orhtogonal
     *        view. Each layer is a little environment with its own items.
     */
    class ENGINE_EXPORT layer:
      virtual public level_object
    {
    public:
      /** \brief The type of the active area passed to the progress() method. */
      typedef concept::region<universe::rectangle_type> region_type;

    public:
      layer( const universe::size_box_type& size );
      virtual ~layer();

      const universe::size_box_type& get_size() const;
      void set_tag(const std::string& tag);
      std::string get_tag() const;

      virtual void start();

      void update
        ( const region_type& active_area, universe::time_type elapsed_time  );

      void get_visual
      ( std::list<scene_visual>& visuals,
        const universe::rectangle_type& visible_area ) const;

      void add_item( base_item& item );
      void remove_item( base_item& item );
      void drop_item( base_item& item );

      void set_always_displayed( base_item& item );
      void unset_always_displayed( base_item& item );

      bool has_world() const;
      world& get_world();
      const world& get_world() const;

      void set_visible( bool v );
      bool is_visible() const;

      void set_active( bool v );
      bool is_active() const;

      void set_shader( visual::shader_program s );

    private:
      virtual void progress
      ( const region_type& active_area, universe::time_type elapsed_time  ) = 0;

      virtual void do_add_item( base_item& item );
      virtual void do_remove_item( base_item& item );
      virtual void do_drop_item( base_item& item );

      virtual void do_get_visual
      ( std::list<scene_visual>& visuals,
        const universe::rectangle_type& visible_area ) const = 0;

      virtual world* do_get_world();
      virtual const world* do_get_world() const;

    protected:
      /** \brief Size of the layer. */
      const universe::size_box_type m_size;

    private:
      /** \brief Always displayed items. */
      std::set<base_item*> m_always_displayed;

      /** \brief The tag of the layer. */
      std::string m_tag;

      /** \brief Tells if the layer is visible. */
      bool m_visible;

      /** \brief Tells if the layer is active. */
      bool m_active;

      /** \brief The shader to apply to the items in this layer. */
      visual::shader_program m_shader;

    }; // class layer
  } // namespace engine
} // namespace bear

#endif // __ENGINE_LAYER_HPP__
