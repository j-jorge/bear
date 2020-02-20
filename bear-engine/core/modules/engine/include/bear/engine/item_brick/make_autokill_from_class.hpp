/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class calls a method of an existing class in its build, then
 *        commit suicide.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MAKE_AUTOKILL_FROM_CLASS_HPP__
#define __ENGINE_MAKE_AUTOKILL_FROM_CLASS_HPP__

namespace bear
{
  namespace engine
  {
    /**
     * \brief This class calls a method of an existing class in its build, then
     *        commit suicide.
     *
     * \b Template \b parameters:
     * - \a Base The base class, in which we call the method.
     * - \a Method The method to call in Base.
     *
     * \author Julien Jorge
     */
    template<typename Base, void (Base::*Method)()>
    class make_autokill_from_class:
      public Base
    {
    private:
      void build();

    }; // class make_autokill_from_class

    /**
     * \brief This class calls a const method of an existing class in its build,
     *        then commit suicide.
     *
     * \b Template \b parameters:
     * - \a Base The base class, in which we call the method.
     * - \a Method The method to call in Base.
     *
     * \author Julien Jorge
     */
    template<typename Base, void (Base::*Method)() const>
    class make_autokill_from_class_const:
      public Base
    {
    private:
      void build();

    }; // class make_autokill_from_class_const

  } // namespace engine
} // namespace bear

#include "bear/engine/item_brick/impl/make_autokill_from_class.tpp"

#endif // __ENGINE_MAKE_AUTOKILL_FROM_CLASS_HPP__
