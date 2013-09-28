/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class loads a model_actor.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MODEL_LOADER_HPP__
#define __ENGINE_MODEL_LOADER_HPP__

#include "engine/compiled_file.hpp"
#include "engine/model/model_animation.hpp"

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class level_globals;
    class model_action;
    class model_actor;
    class model_snapshot;

    /**
     * \brief This class loads a model_actor.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT model_loader
    {
    private:
      /** \brief The type of the map containing all the animations of the
          model. */
      typedef std::vector<model_animation> anim_map_type;

    public:
      model_loader( std::istream& f, level_globals& glob );

      model_actor* run();

    private:
      bool model_version_greater_or_equal
        ( unsigned int major, unsigned int minor, unsigned int release ) const;

      void load_actions( model_actor& m, const anim_map_type& anim_map );
      void load_action( model_actor& m, const anim_map_type& anim_map );
      void load_sound( std::vector<std::string>& sound_name, bool& glob );
      void load_marks( model_action& a, const anim_map_type& anim_map );
      void load_snapshots( model_action& a );
      void load_snapshot( model_action& a );
      void load_mark_placements( model_snapshot& s );
      void load_mark_placement( model_snapshot& s );

      void load_animations( anim_map_type& anim_map );

    private:
      /** \brief The file from which we load the model. */
      compiled_file m_file;

      /** \brief The level_globals in which we load the resources. */
      level_globals& m_level_globals;

      /** \brief The major version number in the model file. */
      unsigned int m_major_version;

      /** \brief The minor version number in the model file. */
      unsigned int m_minor_version;

      /** \brief The release version number in the model file. */
      unsigned int m_release_version;

    }; // class model_loader
  } // namespace engine
} // namespace bear

#endif // __ENGINE_MODEL_LOADER_HPP__
