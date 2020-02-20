/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Wrapper for base_callback.
 * \author Julien Jorge
 */
#ifndef __GUI_CALLBACK_HPP__
#define __GUI_CALLBACK_HPP__

#include "bear/gui/class_export.hpp"

namespace bear
{
  namespace gui
  {
    class base_callback;

    /**
     * \brief Wrapper for base_callback.
     * \author Julien Jorge
     */
    class GUI_EXPORT callback
    {
    public:
      callback();
      callback( const base_callback& c );
      callback( const callback& that );
      ~callback();

      callback& operator=( const callback& that );

      void execute();

      bool operator==( const callback& that ) const;
      bool operator!=( const callback& that ) const;

    private:
      /** \brief The callback executed by this wrapper. */
      base_callback* m_callback;

    }; // class callback
  } // namespace gui
} // namespace bear

#endif // __GUI_CALLBACK_HPP__
