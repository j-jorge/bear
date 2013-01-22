/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Exception thrown when a class is required but not found.
 * \author Julien Jorge
 */
#ifndef __BF_CLASS_NOT_FOUND_HPP__
#define __BF_CLASS_NOT_FOUND_HPP__

#include "bf/libeditor_export.hpp"

#include <exception>
#include <string>

namespace bf
{
  /**
   * \brief Exception thrown when a class is required but not found.
   * \author Julien Jorge
   */
  class BEAR_EDITOR_EXPORT class_not_found:
    public std::exception
  {
  public:
    class_not_found( const std::string& class_name );
    virtual ~class_not_found() throw();

    virtual const char* what() const throw();
    const std::string& class_name() const throw();

  private:
    /** \brief A short explanation of the problem. */
    const std::string m_msg;

    /** \brief The name of the class. */
    const std::string m_class_name;

  }; // class class_not_found
} // namespace bf

#endif // __BF_CLASS_NOT_FOUND_HPP__
