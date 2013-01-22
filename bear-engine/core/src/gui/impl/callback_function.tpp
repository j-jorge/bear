/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::gui::callback_function class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param f The function called by this callback.
 */
template<typename F>
bear::gui::callback_function<F>::callback_function( const F& f )
  : m_function(f)
{

} // callback_function::callback_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Create a copy of this callback.
 */
template<typename F>
bear::gui::callback_function<F>* bear::gui::callback_function<F>::clone() const
{
  return new callback_function<F>(*this);
} // callback_function::clone()

/*----------------------------------------------------------------------------*/
/**
 * \brief Execute the callback.
 */
template<typename F>
void bear::gui::callback_function<F>::execute()
{
  m_function();
} // callback_function::execute()
