/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief A class to represent a boolean that can be calculated by random.
 * \author Sebastien Angibaud
 */
#ifndef __BF_TRINARY_LOGIC_HPP__
#define __BF_TRINARY_LOGIC_HPP__

#include <string>

namespace bf
{
  /**
   * \brief A class to represent a boolean that can be calculated by random.
   * \author Sebastien Angibaud
   */
  struct trinary_logic
  {
  public:
    /** \brief The type of the codes for the actions. */
    typedef unsigned int value_type;

  public:
    static std::string to_string(value_type value);
    static value_type from_string(const std::string& str);
    static bool to_bool(value_type value);
    static value_type from_bool(bool value);
    static bool to_random_bool(value_type value);

  public:
    /** \brief Boolean true. */
    static const value_type boolean_false = 0;

    /** \brief Boolean false. */
    static const value_type boolean_true = boolean_false + 1;

    /** \brief Boolean random. */
    static const value_type boolean_random = boolean_true + 1;
  }; // struct trinary_logic

} // namespace bf

#endif // __BF_TRINARY_LOGIC_HPP__
