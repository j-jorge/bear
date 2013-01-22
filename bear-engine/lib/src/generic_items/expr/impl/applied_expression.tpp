/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::applied_expression class.
 * \author Julien Jorge
 */

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
template<typename Expression>
bool bear::applied_expression<Expression>::set_item_field
( const std::string& name, engine::base_item* value )
{
  bool result = true;

  if ( name == "applied_expression.expression" )
    {
      creation_class_type* e( dynamic_cast<creation_class_type*>(value) );

      if ( e != NULL )
        m_expression = e->get_expression();
      else
        claw::logger << claw::log_error << name << ": item is not of a type "
                     << "'with_expression_creation'." << std::endl;
    }
  else
    result = super::set_item_field(name, value);

  return result;
} // applied_expression::set_item_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set a field of type "list of item".
 * \param name The name of the field.
 * \param value The value of the field.
 */
template<typename Expression>
bool bear::applied_expression<Expression>::set_item_list_field
( const std::string& name, const std::vector<engine::base_item*>& value )
{
  bool result = true;

  if ( name == "applied_expression.receiver" )
    for ( std::size_t i=0; i!=value.size(); ++i )
      {
        assignment_class_type* a
          ( dynamic_cast<assignment_class_type*>(value[i]) );

        if ( a != NULL )
          m_items.push_back(a);
        else
          claw::logger << claw::log_error << name
                       << ": item #" << i
                       << " is not of type 'with_expression_assignment'."
                       << std::endl;
      }
  else
    result = super::set_item_list_field(name, value);

  return result;
} // applied_expression::set_item_list_field()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if all required fields are initialized.
 */
template<typename Expression>
bool bear::applied_expression<Expression>::is_valid() const
{
  return !m_items.empty();
} // applied_expression::is_valid()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the item.
 */
template<typename Expression>
void bear::applied_expression<Expression>::build()
{
  for (unsigned int i=0; i!=m_items.size(); ++i)
    m_items[i]->set_expression(m_expression);

  kill();
} // applied_expression::build()
