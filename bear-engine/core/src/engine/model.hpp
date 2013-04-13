/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class manage some basic informations on items: actions with
 *        their sounds, animations and bounding boxes.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_HPP__
#define __ENGINE_MODEL_HPP__

#include "audio/sample.hpp"
#include "engine/item_brick/basic_renderable_item.hpp"
#include "engine/model/model_action.hpp"
#include "engine/model/model_actor.hpp"
#include "engine/model/model_snapshot_tweener.hpp"

#include "text_interface/base_exportable.hpp"

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief This class manage some basic informations on items: actions with
     *        their sounds, animations and bounding boxes.
     *
     * \b Template \b arguments:
     *  - \a Base the base class of the model.
     *
     * \remark This class assume that animations are oriented to the right by
     *         default.
     *
     * \author Julien Jorge
     */
    template<class Base>
    class model:
      public basic_renderable_item<Base>
    {
    public:
      /** \brief The type of the parent class. */
      typedef basic_renderable_item<Base> super;

    private:
      TEXT_INTERFACE_DECLARE_METHOD_LIST(super, init_exported_methods)

    public:
      model();
      model( const model<Base>& that );
      ~model();

      model<Base>& operator=( const model<Base>& that );

      void progress( universe::time_type elapsed_time );
      void get_visual( std::list<scene_visual>& visuals ) const;

      void set_model_actor(const model_actor& actor);
      void start_model_action(const std::string& name);
      void switch_to_model_action(const std::string& name);
      const std::string& get_current_action_name() const;
      model_action const* get_action( const std::string& name ) const;
      model_action const* get_current_action() const;
      const model_action::const_snapshot_iterator get_current_snapshot() const;

      universe::position_type get_mark_world_position( std::size_t i ) const;
      universe::position_type
      get_mark_world_position( const std::string& mark_name ) const;

      bool get_mark_placement
      ( const std::string& mark_name, model_mark_placement& m ) const;
      bool get_current_local_mark_placement
      ( const std::string& mark_name, model_mark_placement& m ) const;

      model_mark_item* get_model_mark_item( const std::string& name ) const;

      void set_mark_visibility_in_action( const std::string& name, bool b );

    protected:
      void progress_model( universe::time_type elapsed_time );

      void set_global_substitute
      ( const std::string& mark_name, const model_animation& anim );
      void remove_global_substitute( const std::string& mark_name );

      void set_mark_angle_in_action
      ( const std::string& mark_name, double angle );
      void set_mark_position_in_action
      ( const std::string& mark_name, universe::position_type );

      std::size_t get_mark_id( const model_mark& mark ) const;
      std::size_t get_mark_id( const std::string& mark_name ) const;
      bool get_mark_placement( std::size_t i, model_mark_placement& m ) const;
      bool get_mark_placement( model_mark_placement& m ) const;

      bool get_current_local_mark_placement
      ( std::size_t i, model_mark_placement& m ) const;

      void to_string( std::string& str ) const;

      void on_leaves_layer();

    private:
      void progress_to_snapshot
      ( universe::time_type initial_time, universe::time_type elapsed_time,
        const model_action::const_snapshot_iterator& eit );

      void execute_function( const std::string& name );

      scene_visual get_mark_visual
      ( const model_mark& mark, const model_mark_placement& p ) const;

      bool get_oriented_mark_placement( model_mark_placement& m ) const;

      void start_action( universe::time_type d );
      void stop_action();
      void reset_action( universe::time_type d );

      void update_sound_position();
      void progress_animations
      ( universe::time_type initial_time, universe::time_type elapsed_time,
        bool only_finite = false );
      void execute_snapshot();

      void update_mark_items();
      void update_mark_items_positions();

      void update_bounding_box();
      void update_bounding_box_width();
      void update_bounding_box_height();

      void clear();

      void remove_all_mark_items_from_layer() const;

      void add_mark_item_in_layer( std::size_t i );

      void create_tweeners();
      void create_tweeners_to_snapshot( const model_snapshot& s );
      void create_tweeners_to_action( const model_action& a );

      static void init_exported_methods();

    private:
      /** \brief The model_actor (the actions) of this model. */
      model_actor m_actor;

      /** \brief The action currently done by the model. */
      model_action* m_action;

      /** \brief The name of the current action done by the model. */
      std::string m_action_name;

      /** \brief The current snapshot in the current action. */
      model_action::const_snapshot_iterator m_snapshot;

      /** \brief The time elapsed on the current action. */
      universe::time_type m_date;

      /** \brief The sound played during the action. */
      audio::sample* m_sample;

      /** \brief The tweeners for the current snapshot. */
      model_snapshot_tweener* m_current_snapshot;

    }; // class model
  } // namespace engine
} // namespace bear

#include "engine/impl/model.tpp"

#endif // __ENGINE_MODEL_HPP__
