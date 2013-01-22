/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief An item in a level.
 * \author Julien Jorge
 */
#ifndef __ENGINE_LEVEL_OBJECT_HPP__
#define __ENGINE_LEVEL_OBJECT_HPP__

#include "engine/class_export.hpp"

namespace bear
{
  namespace engine
  {
    class level;
    class level_globals;

    /**
     * \brief An item in a level.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT level_object
    {
    public:
      level_object();
      level_object( const level_object& that );
      virtual ~level_object();

      void clear_level();
      void set_level( level& lvl );
      level& get_level() const;

      level_globals& get_level_globals() const;

    private:
      /** \brief The level in which I am. */
      level* m_level;

    }; // class level_object

  } // namespace engine
} // namespace bear

#endif // __ENGINE_LEVEL_OBJECT_HPP__
