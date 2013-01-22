/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Codes describing various values for the easing functions.
 * \author Julien Jorge
 */
#ifndef __BEAR_EASING_HPP__
#define __BEAR_EASING_HPP__

#include "claw/tween/single_tweener.hpp"
#include "claw/tween/easing/easing_back.hpp"
#include "claw/tween/easing/easing_bounce.hpp"
#include "claw/tween/easing/easing_circ.hpp"
#include "claw/tween/easing/easing_cubic.hpp"
#include "claw/tween/easing/easing_elastic.hpp"
#include "claw/tween/easing/easing_expo.hpp"
#include "claw/tween/easing/easing_linear.hpp"
#include "claw/tween/easing/easing_none.hpp"
#include "claw/tween/easing/easing_quad.hpp"
#include "claw/tween/easing/easing_quart.hpp"
#include "claw/tween/easing/easing_quint.hpp"
#include "claw/tween/easing/easing_sine.hpp"

namespace bear
{
  /**
   * \brief Codes describing the easing functions.
   * \param Julien Jorge
   */
  class easing
  {
  public:
    /** \brief The easing functions. */
    class function
    {
    public:
      typedef unsigned int function_type;

    public:
      static const function_type not_an_easing_function = 0;
      static const function_type back = 1;
      static const function_type bounce = 2;
      static const function_type circ = 3;
      static const function_type cubic = 4;
      static const function_type elastic = 5;
      static const function_type expo = 6;
      static const function_type linear = 7;
      static const function_type none = 8;
      static const function_type quad = 9;
      static const function_type quart = 10;
      static const function_type quint = 11;
      static const function_type sine = 12;

      /**
       * \brief Convert a function type into a string.
       * \param f The function type.
       */
      static std::string to_string( function_type f )
      {
        switch ( f )
          {
          case back: return "back";
          case bounce: return "bounce";
          case circ: return "circ";
          case cubic: return "cubic";
          case elastic: return "elastic";
          case expo: return "expo";
          case linear: return "linear";
          case none: return "none";
          case quad: return "quad";
          case quart: return "quart";
          case quint: return "quint";
          case sine: return "sine";
          default: return "not_an_easing_function";
          }
      } // to_string()

      /**
       * \brief Get a fonction type given a string containing its name.
       * \param f The function.
       */
      static function_type from_string( const std::string& f )
      {
        if ( f == "back" )
          return back;
        else if ( f == "bounce" )
          return bounce;
        else if ( f == "circ" )
          return circ;
        else if ( f == "cubic" )
          return cubic;
        else if ( f == "elastic" )
          return elastic;
        else if ( f == "expo" )
          return expo;
        else if ( f == "linear" )
          return linear;
        else if ( f == "none" )
          return none;
        else if ( f == "quad" )
          return quad;
        else if ( f == "quart" )
          return quart;
        else if ( f == "quint" )
          return quint;
        else if ( f == "sine" )
          return sine;
        else
          return not_an_easing_function;
      } // from_string()
    }; // class function

    /** \brief The easing directions. */
    class direction
    {
    public:
      typedef unsigned int direction_type;

    public:
      static const direction_type not_an_easing_direction = 0;
      static const direction_type in = 1;
      static const direction_type out = 2;
      static const direction_type in_out = 3;

      /**
       * \brief Convert a direction type into a string.
       * \param d The direction type.
       */
      static std::string to_string( direction_type d )
      {
        switch ( d )
          {
          case in: return "in";
          case out: return "out";
          case in_out: return "in_out";
          default: return "not_an_easing_direction";
          }
      } // to_string()

      /**
       * \brief Get a fonction type given a string containing its name.
       * \param d The direction.
       */
      static direction_type from_string( const std::string& d )
      {
        if ( d == "in" )
          return in;
        else if ( d == "out" )
          return out;
        else if ( d == "in_out" )
          return in_out;
        else
          return not_an_easing_direction;
      } // from_string()

    }; // direction_type

  public:
    /**
     * \brief Constructor.
     */
    easing()
      : m_function(function::none), m_direction(direction::in)
    { }

    /**
     * \brief Constructor.
     * \param f The function.
     * \param d The direction.
     */
    easing( function::function_type f, direction::direction_type d )
      : m_function(f), m_direction(d)
    { }

    /**
     * \brief Check if this easing is the same than an other one.
     * \param that The instance to compare to.
     */
    bool operator==( const easing& that ) const
    {
      return
        (m_function == that.m_function) && (m_direction == that.m_direction);
    }

    /** \brief Get the function of this easing. */
    function::function_type get_function() const { return m_function; }

    /**
     * \brief Set the function of this easing.
     * \param f The function.
     */
    void set_function( function::function_type f ) { m_function = f; }

    /** \brief Get the direction of this easing. */
    direction::direction_type get_direction() const { return m_direction; }

    /**
     * \brief Set the direction of this easing.
     * \param d The direction.
     */
    void set_direction( direction::direction_type d ) { m_direction = d; }

    /**
     * \brief Get a string made of the function name, a colon and the direction
     *        name.
     */
    std::string to_string() const
    {
      return function::to_string(m_function) + ':'
        + direction::to_string(m_direction);
    } // to_string()

    /**
     * \brief Initialize the instance from a string made of the function name, a
     *        colon and the direction name.
     * \param s The string to initialize from.
     */
    void from_string( const std::string& s )
    {
      const std::size_t pos = s.find_last_of(':');

      if ( (pos == std::string::npos) || (pos == 0) )
        {
          m_function = function::not_an_easing_function;
          m_direction = direction::not_an_easing_direction;
        }
      else
        {
          m_function = function::from_string( s.substr(0, pos) );
          m_direction = direction::from_string( s.substr(pos + 1) );
        }
    } // from_string()

    /**
     * \brief Get the easing from claw function corresponding to this instance.
     */
    claw::tween::single_tweener::easing_function to_claw_easing_function() const
    {
      switch (m_function)
        {
        case function::back:
          return get_claw_easing_direction<claw::tween::easing_back>();
        case function::bounce:
          return get_claw_easing_direction<claw::tween::easing_bounce>();
        case function::circ:
          return get_claw_easing_direction<claw::tween::easing_circ>();
        case function::cubic:
          return get_claw_easing_direction<claw::tween::easing_cubic>();
        case function::elastic:
          return get_claw_easing_direction<claw::tween::easing_elastic>();
        case function::expo:
          return get_claw_easing_direction<claw::tween::easing_expo>();
        case function::linear:
          return get_claw_easing_direction<claw::tween::easing_linear>();
        case function::quad:
          return get_claw_easing_direction<claw::tween::easing_quad>();
        case function::quart:
          return get_claw_easing_direction<claw::tween::easing_quart>();
        case function::quint:
          return get_claw_easing_direction<claw::tween::easing_quint>();
        case function::sine:
          return get_claw_easing_direction<claw::tween::easing_sine>();
        default:
          return get_claw_easing_direction<claw::tween::easing_none>();
        }
    } // to_claw_easing_function()

  private:

    /**
     * \brief Get the easing function corresponding to the direction.
     */
    template<typename Easing>
    claw::tween::single_tweener::easing_function
    get_claw_easing_direction() const
    {
      switch( m_direction )
        {
        case direction::in: return &Easing::ease_in;
        case direction::out: return &Easing::ease_out;
        default: return &Easing::ease_in_out;
        }
    } // get_claw_easing_direction()

  private:
    /** \brief The function of the easing. */
    function::function_type m_function;

    /** \brief The direction of the easing. */
    direction::direction_type m_direction;

  }; // class easing
} // namespace bear

#endif // __BEAR_EASING_HPP__
