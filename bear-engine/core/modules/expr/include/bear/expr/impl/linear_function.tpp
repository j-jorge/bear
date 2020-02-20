/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::expr::linear_function class.
 * \author Julien Jorge.
 */

#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param inst The instance on which we call the function.
 * \param m The function to call with \a *inst as the only parameter.
 */
template<typename FunctionType, typename PointerType>
bear::expr::linear_function<FunctionType, PointerType>::linear_function
( const PointerType& inst, FunctionType m )
  : m_value(inst), m_function(m)
{

} // linear_function::linear_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this expression.
 */
template<typename FunctionType, typename PointerType>
bear::expr::base_linear_expression*
bear::expr::linear_function<FunctionType, PointerType>::clone() const
{
  return new linear_function<FunctionType, PointerType>(*this);
} // linear_function::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Evaluate the expression.
 */
template<typename FunctionType, typename PointerType>
double
bear::expr::linear_function<FunctionType, PointerType>::evaluate() const
{
  if ( m_value == static_cast<PointerType>(NULL) )
    return std::numeric_limits<double>::quiet_NaN();
  else
    return (double)m_function(*m_value);
} // linear_function::evaluate()

/*----------------------------------------------------------------------------*/
/**
 * \brief Construct a linear_function.
 * \param inst The instance on which we call the function.
 * \param m The function to call with \a *inst as the only parameter.
 *
 * This function is an easier way to create a linear function than using the
 * constructor. Compare those two solutions, both creating a linear function
 * that calls a member function of bear::expr::base_item:
 *
 * \code
 * typedef std::const_mem_fun_ref_t<double, base_item> function_type;
 * linear_expression f
 *   ( linear_function<base_item*, function_type>
 *     ( item, function_type(&base_item::get_left) ) );
 * \endcode
 * and
 * \code
 * linear_expression f
 *   ( linear_expression_maker
 *     ( item, std::mem_fun_ref(&base_item::get_left) ) );
 * \endcode
 */
template<typename FunctionType, typename PointerType>
bear::expr::linear_function<FunctionType, PointerType>
bear::expr::linear_function_maker(const PointerType& inst, FunctionType m)
{
  return linear_function<FunctionType, PointerType>(inst, m);
} // linear_function_maker()
