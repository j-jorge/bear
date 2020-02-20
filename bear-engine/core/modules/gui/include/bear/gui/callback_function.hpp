/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A callback that calls a given function.
 * \author Julien Jorge
 */
#ifndef __GUI_CALLBACK_FUNCTION_HPP__
#define __GUI_CALLBACK_FUNCTION_HPP__

#include "bear/gui/base_callback.hpp"

namespace bear
{
  namespace gui
  {
    /**
     * \brief A callback that calls a given function.
     * \author Julien Jorge
     */
    template<typename F>
    class callback_function:
      public base_callback
    {
    public:
      callback_function( const F& f );

      callback_function<F>* clone() const;

      void execute();

    private:
      /** \brief The function called by this callback. */
      F m_function;

    }; // class callback_function

    /**
     * \brief A function to ease the creation of a callback_function.
     * \param f The function called by the callback.
     */
    template<typename F>
    callback_function<F> callback_function_maker( const F& f )
    {
      return callback_function<F>(f);
    } // callback_function_maker()

  } // namespace gui
} // namespace bear

#include "bear/gui/impl/callback_function.tpp"

#endif // __GUI_CALLBACK_FUNCTION_HPP__
