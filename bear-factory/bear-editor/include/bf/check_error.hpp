/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief This class carries a description of a problem found during the
 *        check of the level.
 * \author Julien Jorge
 */
#ifndef __BF_CHECK_ERROR_HPP__
#define __BF_CHECK_ERROR_HPP__

#include <string>

namespace bf
{
  /**
   * \brief This class carries a description of a problem found during the
   *        check of the level.
   * \author Julien Jorge
   */
  class check_error
  {
  public:
    explicit check_error( const std::string& msg );
    check_error( const std::string& prefix, const std::string& msg );

    const std::string& get_cause() const;
    const std::string& get_message() const;

  private:
    /** \brief The cause of the message describes the element on which the
        error occured. */
    std::string m_cause;

    /** \brief The message describes the problem. */
    std::string m_message;

  }; // class check_error
} // namespace bf

#endif // __BF_CHECK_ERROR_HPP__
