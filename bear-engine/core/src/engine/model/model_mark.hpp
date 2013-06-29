/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A mark in an action.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_MARK_HPP__
#define __ENGINE_MODEL_MARK_HPP__

#include "universe/types.hpp"
#include "engine/model/model_animation.hpp"
#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class layer;
    class model_mark_item;

    /**
     * \brief A placement for a mark in a snapshot.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_mark
    {
    public:
      model_mark();
      model_mark
      ( const std::string& label, const model_animation& anim,
        bool apply_angle, bool pause_hidden );
      model_mark( const model_mark& that );
      ~model_mark();

      model_mark& operator=( model_mark that );
      void swap( model_mark& that ) throw();

      const std::string& get_label() const;
      bool has_animation() const;
      model_animation& get_animation();
      const model_animation& get_animation() const;

      const model_animation& get_main_animation() const;
      void set_main_animation( const model_animation& anim );

      bool apply_angle_to_animation() const;
      bool pause_when_hidden() const;

      const model_animation& get_substitute() const;
      void set_substitute( const model_animation& anim );
      void remove_substitute();

      model_mark_item& get_box_item() const;

      void add_box_item_in_layer( layer& the_layer );
      void drop_box_item_from_layer();

    private:
      /** \brief The label of the mark. */
      std::string m_label;

      /** \brief The animation displayed on the mark. */
      model_animation m_animation;

      /** \brief A substitution to the main animation. */
      model_animation m_substitute;

      /** \brief Tell if the angle is applied to the animation. */
      bool m_apply_angle_to_animation;

      /** \brief Tell if the animation must be paused when hidden. */
      bool m_pause_when_hidden;

      /** \brief The item centered on this mark. */
      model_mark_item* m_box_item;

      /** \brief Tells if m_box_item has been inserted in a layer. */
      bool m_item_in_layer;

    }; // class model_mark

    void swap( engine::model_mark& a, engine::model_mark& b ) throw();

  } // namespace engine
} // namespace bear

namespace std
{
  template<>
  void swap( bear::engine::model_mark& a, bear::engine::model_mark& b );
} // namespace std

#endif // __ENGINE_MODEL_MARK_HPP__
