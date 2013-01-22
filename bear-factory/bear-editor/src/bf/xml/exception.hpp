/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Various exceptions thrown when parsing an XML file.
 */
#ifndef __BF_XML_EXCEPTION_HPP__
#define __BF_XML_EXCEPTION_HPP__

#include "bf/libeditor_export.hpp"

#include <exception>
#include <string>

namespace bf
{
  namespace xml
  {
    /**
     * \brief Exception thrown when the parser find an unexpected node.
     * \author Julien Jorge
     */
    class BEAR_EDITOR_EXPORT bad_node:
      public std::exception
    {
    public:
      bad_node( const std::string& node_name );
      virtual ~bad_node() throw();

      virtual const char* what() const throw();

    private:
      /** \brief A short explanation of the problem. */
      const std::string m_msg;

    }; // class bad_node

    /**
     * \brief Exception thrown when the parser does not find an needed node.
     * \author Julien Jorge
     */
    class BEAR_EDITOR_EXPORT missing_node:
      public std::exception
    {
    public:
      missing_node( const std::string& property_name );
      virtual ~missing_node() throw();

      virtual const char* what() const throw();

    private:
      /** \brief A short explanation of the problem. */
      const std::string m_msg;

    }; // class missing_node

    /**
     * \brief Exception thrown when the parser does not find an needed property.
     * \author Julien Jorge
     */
    class BEAR_EDITOR_EXPORT missing_property:
      public std::exception
    {
    public:
      missing_property( const std::string& property_name );
      virtual ~missing_property() throw();

      virtual const char* what() const throw();

    private:
      /** \brief A short explanation of the problem. */
      const std::string m_msg;

    }; // class missing_property

    /**
     * \brief Exception thrown when the parser find a not supported value.
     * \author Julien Jorge
     */
    class BEAR_EDITOR_EXPORT bad_value:
      public std::exception
    {
    public:
      bad_value( const std::string& value );
      bad_value( const std::string& type, const std::string& value );
      virtual ~bad_value() throw();

      virtual const char* what() const throw();

    private:
      /** \brief A short explanation of the problem. */
      const std::string m_msg;

    }; // class bad_value
  } // namespace xml
} // namespace bf

#endif // __BF_XML_EXCEPTION_HPP__

