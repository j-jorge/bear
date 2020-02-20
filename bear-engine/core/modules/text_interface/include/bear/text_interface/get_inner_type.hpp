/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Get the most inner type in a declaration of a constant and/or a
 *        reference.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_GET_INNER_TYPE_HPP__
#define __TEXT_INTERFACE_GET_INNER_TYPE_HPP__

namespace bear
{
  namespace text_interface
  {
    /**
     * \brief Get the most inner type in a declaration of a constant and/or a
     *        reference.
     * \author Julien Jorge.
     */
    template<typename T>
    class get_inner_type
    {
    public:
      /** \brief The inner type. */
      typedef T type;
    }; // class get_inner_type

    /**
     * \brief Get the most inner type in a declaration of a constant.
     * \author Julien Jorge.
     */
    template<typename T>
    class get_inner_type<const T>:
      public get_inner_type<T>
    { }; // class get_inner_type

    /**
     * \brief Get the most inner type in a declaration of a constant reference.
     * \author Julien Jorge.
     */
    template<typename T>
    class get_inner_type<const T&>:
      public get_inner_type<T>
    { }; // class get_inner_type

  } // namespace text_interface
} // namespace bear

#endif // __TEXT_INTERFACE_GET_INNER_TYPE_HPP__
