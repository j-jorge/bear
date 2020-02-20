/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for the callbacks of the controls.
 * \author Julien Jorge
 */
#ifndef __GUI_BASE_CALLBACK_HPP__
#define __GUI_BASE_CALLBACK_HPP__

#include "bear/gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief Base class for the callbacks of the controls.
     * \author Julien Jorge
     */
    class GUI_EXPORT base_callback
    {
    public:
      virtual ~base_callback() {}

      virtual base_callback* clone() const = 0;

      /**
       * \brief Execute the callback.
       */
      virtual void execute() = 0;

    }; // class base_callback
  } // namespace gui
} // namespace bear

#endif // __GUI_BASE_CALLBACK_HPP__
