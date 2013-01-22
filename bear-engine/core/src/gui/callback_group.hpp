/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A callback that calls several callbacks.
 * \author Julien Jorge
 */
#ifndef __GUI_CALLBACK_GROUP_HPP__
#define __GUI_CALLBACK_GROUP_HPP__

#include "gui/base_callback.hpp"
#include "gui/callback.hpp"

#include <vector>

namespace bear
{
  namespace gui
  {
    class callback;

    /**
     * \brief A callback that calls several callbacks.
     * \author Julien Jorge
     */
    class GUI_EXPORT callback_group:
      public base_callback
    {
    public:
      callback_group* clone() const;

      void execute();

      void add( const callback& c );
      bool empty() const;

    private:
      /** \brief The group called by this callback. */
      std::vector<callback> m_group;

    }; // class callback_group

  } // namespace gui
} // namespace bear

#endif // __GUI_CALLBACK_GROUP_HPP__
