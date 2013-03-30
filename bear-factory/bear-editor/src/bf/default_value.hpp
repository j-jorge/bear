/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Default values for the types used in a level file.
 * \author Julien Jorge
 */
#ifndef __BF_DEFAULT_VALUE_HPP__
#define __BF_DEFAULT_VALUE_HPP__

#include "bf/custom_type.hpp"
#include "bf/libeditor_export.hpp"

namespace bf
{
  /**
   * \brief Default values for the types used in a level file.
   * \author Julien Jorge
   */
  template<typename Type>
  class default_value
  {
  public:
    static Type get() { return Type(); }
  }; // class default_value

  /**
   * \brief Default values for the types used in a level file. Specialisation
   *        for string_type.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT default_value<string_type>
  {
  public:
    static string_type get() { return string_type(); }
  }; // class default_value [string_type]

  /**
   * \brief Default values for the types used in a level file. Specialisation
   *        for easing_type.
   * \author Julien Jorge
   */
  template<>
  class BEAR_EDITOR_EXPORT default_value<easing_type>
  {
  public:
    static easing_type get() { return easing_type(); }
  }; // class default_value [easing_type]

  /**
   * \brief Default values for the types used in a level file. Specialisation
   *        others custom types.
   * \author Julien Jorge
   */
  template<typename T>
  class default_value< custom_type<T> >
  {
  public:
    static custom_type<T> get() { return custom_type<T>(0); }
  }; // class default_value [custom_type]

} // namespace bf

#endif // __BF_DEFAULT_VALUE_HPP__
