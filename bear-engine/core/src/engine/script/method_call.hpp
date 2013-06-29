/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The data needed to call a method on an item when executing a script.
 * \author Julien Jorge
 */
#ifndef __ENGINE_METHOD_CALL_HPP__
#define __ENGINE_METHOD_CALL_HPP__

#include "engine/class_export.hpp"

#include <vector>
#include <string>

namespace bear
{
  namespace engine
  {
    /**
     * \brief The context in which the scripts on items are executed.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT method_call
    {
    public:
      void set_actor_name( const std::string& name );
      const std::string& get_actor_name() const;

      void set_method_name( const std::string& name );
      const std::string& get_method_name() const;

      void set_arguments( const std::vector<std::string>& val );
      const std::vector<std::string>& get_arguments() const;

    private:
      /** \brief The name of the actor. */
      std::string m_actor_name;

      /** \brief The name of the method. */
      std::string m_method_name;

      /** \brief The arguments passed to the method. */
      std::vector<std::string> m_arguments;

    }; // class method_call

  } // namespace engine
} // namespace bear

#endif // __ENGINE_METHOD_CALL_HPP__
