/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Convert a string to the equivalent value to pass as an argument for a
 *        character method.
 * \author Julien Jorge.
 */
#ifndef __TEXT_INTERFACE_CONVERTED_ARGUMENT_HPP__
#define __TEXT_INTERFACE_CONVERTED_ARGUMENT_HPP__

namespace bear
{
  namespace text_interface
  {
    /**
     * \brief Helper class to decide whether an argument is converted using the
     *        default procedure or delegated to the implementation of the
     *        subclass.
     */
    class converted_argument
    {
    public:
      converted_argument();
      converted_argument( const converted_argument& that );

      template<typename T>
      converted_argument( T* v );

      template<typename T>
      converted_argument( T& v );

      template<typename T>
      T cast_to() const;

    private:
      /** \brief The converted value. */
      void* m_holder;

    }; // class converted_argument()

  } // namespace text_interface
} // namespace bear

#include "text_interface/impl/converted_argument.tpp"

#endif // __TEXT_INTERFACE_CONVERTED_ARGUMENT_HPP__
