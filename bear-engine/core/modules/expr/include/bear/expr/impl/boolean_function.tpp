/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::boolean_function class.
 * \author Julien Jorge.
 */

#include <typeinfo>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename FunctionType, typename PointerType>
bear::expr::boolean_function<FunctionType, PointerType>::boolean_function()
{

} // boolean_function::boolean_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy _constructor.
 */
template<typename FunctionType, typename PointerType>
bear::expr::boolean_function<FunctionType, PointerType>::boolean_function
(const boolean_function<FunctionType, PointerType>& that)
  : m_value(that.m_value), m_function(that.m_function)
{

} // boolean_function::boolean_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param inst The instance on which we call the function.
 * \param m The function to call with \a *inst as the only parameter.
 */
template<typename FunctionType, typename PointerType>
bear::expr::boolean_function<FunctionType, PointerType>::boolean_function
( const PointerType& inst, FunctionType m )
  : m_value(inst), m_function(m)
{

} // boolean_function::boolean_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
template<typename FunctionType, typename PointerType>
bear::expr::base_boolean_expression*
bear::expr::boolean_function<FunctionType, PointerType>::clone() const
{
  return new boolean_function<FunctionType, PointerType>(*this);
} // boolean_function::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
template<typename FunctionType, typename PointerType>
bool bear::expr::boolean_function<FunctionType, PointerType>::evaluate() const
{
  return m_function(*m_value);
} // boolean_function::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
template<typename FunctionType, typename PointerType>
std::string
bear::expr::boolean_function<FunctionType, PointerType>::formatted_string()
  const
{
  return std::string( typeid(m_function).name() )
    + "( " + typeid(*m_value).name() + " )";
} // boolean_function::formatted_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the fonction to call.
 * \param m The function to call.
 */
template<typename FunctionType, typename PointerType>
void bear::expr::boolean_function<FunctionType, PointerType>::set_function
( FunctionType m )
{
  m_function(m);
} // boolean_function::set_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
template<typename FunctionType>
bear::expr::boolean_function<FunctionType, void>::boolean_function()
{

} // boolean_function::boolean_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param m The function to call with \a *inst as the only parameter.
 */
template<typename FunctionType>
bear::expr::boolean_function<FunctionType, void>::boolean_function
( FunctionType m )
  : m_function(m)
{

} // boolean_function::boolean_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
template<typename FunctionType>
bear::expr::base_boolean_expression*
bear::expr::boolean_function<FunctionType, void>::clone() const
{
  return new boolean_function<FunctionType, void>(*this);
} // boolean_function::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
template<typename FunctionType>
bool bear::expr::boolean_function<FunctionType, void>::evaluate() const
{
  return m_function();
} // boolean_function::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Gets a formatted and human readable representation of this expression.
 */
template<typename FunctionType>
std::string
bear::expr::boolean_function<FunctionType, void>::formatted_string() const
{
  return std::string( typeid(m_function).name() ) + "()";
} // boolean_function::formatted_string()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the fonction to call.
 * \param m The function to call.
 */
template<typename FunctionType>
void bear::expr::boolean_function<FunctionType, void>::set_function
( FunctionType m )
{
  m_function = m;
} // boolean_function::set_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Construct a boolean_function.
 * \param inst The instance on which we call the function.
 * \param m The function to call with \a *inst as the only parameter.
 *
 * This function is an easier way to create a boolean function than using the
 * constructor. Compare those two solutions, both creating a boolean function
 * that calls a member function of bear::expr::base_item:
 *
 * \code
 * typedef std::const_mem_fun_ref_t<double, base_item> function_type;
 * boolean_expression f
 *   ( boolean_function<base_item*, function_type>
 *     ( item, function_type(&base_item::get_left()) ) );
 * \endcode
 * and
 * \code
 * boolean_expression f
 *   ( boolean_expression_maker
 *     ( item, std::mem_fun_ref(&base_item::get_left()) ) );
 * \endcode
 */
template<typename FunctionType, typename PointerType>
bear::expr::boolean_function<FunctionType, PointerType>
bear::expr::boolean_function_maker(const PointerType& inst, FunctionType m)
{
  return boolean_function<FunctionType, PointerType>(inst, m);
} // boolean_function_maker()

/*----------------------------------------------------------------------------*/
/**
 * \brief Construct a boolean_function.
 * \param m The function to call.
 *
 * This function is an easier way to create a boolean function than using the
 * constructor. Compare those two solutions, both creating a boolean function
 * that calls boolean function:
 *
 * \code
 * typedef bool (*some_function)() function_type;
 * boolean_expression f
 *   ( boolean_function<function_type, void>( &my_function ) );
 * \endcode
 * and
 * \code
 * boolean_expression f( boolean_expression_maker( &my_function ) );
 * \endcode
 */
template<typename FunctionType>
bear::expr::boolean_function<FunctionType, void>
bear::expr::boolean_function_maker(FunctionType m)
{
  return boolean_function<FunctionType, void>();
} // boolean_function_maker()
