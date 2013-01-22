/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This class represent a level variable or a game variable.
 * \author Julien Jorge
 */
#ifndef __ENGINE_BASE_VARIABLE_HPP__
#define __ENGINE_BASE_VARIABLE_HPP__

#include "engine/class_export.hpp"
#include "engine/variable/var_map.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief The base class for a level variable or a game variable.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT base_variable
    {
    public:
      explicit base_variable( const std::string& name );

      const std::string& get_name() const;

      virtual void assign_value_to( var_map& m ) const = 0;
      virtual void get_value_from( const var_map& m ) = 0;
      virtual bool exists( const var_map& m ) const = 0;

    private:
      /** \brief The name of the variable. */
      const std::string m_name;

    }; // class base_variable;

  } // namespace engine
} // namespace bear

#endif // __ENGINE_BASE_VARIABLE_HPP__
