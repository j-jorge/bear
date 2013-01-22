/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class defines a toggle calling a method of an existing class
 *        when toggled on.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MAKE_TOGGLE_ON_FROM_CLASS_HPP__
#define __ENGINE_MAKE_TOGGLE_ON_FROM_CLASS_HPP__

#include "engine/item_brick/item_with_toggle.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief This class defines a toggle calling a method of an existing class
     *        when toggled on.
     *
     * \b Template \b parameters:
     * - \a Base The base class, in which we call the method.
     * - \a Method The method to call in Base.
     *
     * \author Julien Jorge
     */
    template<typename Base, void (Base::*Method)()>
    class make_toggle_on_from_class:
      public item_with_toggle<Base>
    {
    private:
      void on_toggle_on( base_item* activator );

    }; // class make_toggle_on_from_class

    /**
     * \brief This class defines a toggle calling a constant method of an
     *        existing class when toggled on.
     *
     * \b Template \b parameters:
     * - \a Base The base class, in which we call the method.
     * - \a Method The method to call in Base.
     *
     * \author Julien Jorge
     */
    template<typename Base, void (Base::*Method)() const>
    class make_toggle_on_from_class_const:
      public item_with_toggle<Base>
    {
    private:
      void on_toggle_on( base_item* activator );

    }; // class make_toggle_on_from_class_const

  } // namespace engine
} // namespace bear

#include "engine/item_brick/impl/make_toggle_on_from_class.tpp"

#endif // __ENGINE_MAKE_TOGGLE_ON_FROM_CLASS_HPP__
