/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::make_toggle_on_from_class class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief This method is called when the toggles switches on.
 * \param activator The item that activates the toggle, if any.
 */
template<typename Base, void (Base::*Method)()>
void bear::engine::make_toggle_on_from_class<Base, Method>::on_toggle_on
( base_item* activator )
{
  (this->*Method)();
} // make_toggle_on_from_class::on_toggle_on()




/*----------------------------------------------------------------------------*/
/**
 * \brief This method is called when the toggles switches on.
 * \param activator The item that activates the toggle, if any.
 */
template<typename Base, void (Base::*Method)() const>
void bear::engine::make_toggle_on_from_class_const<Base, Method>::on_toggle_on
( base_item* activator )
{
  (this->*Method)();
} // make_toggle_on_from_class_const::on_toggle_on()
