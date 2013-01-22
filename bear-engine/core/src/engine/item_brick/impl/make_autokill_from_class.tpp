/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::make_autokill_from_class class.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
template<typename Base, void (Base::*Method)()>
void bear::engine::make_autokill_from_class<Base, Method>::build()
{
  Base::build();
  (this->*Method)();
  this->kill();
} // make_autokill_from_class::on_toggle_on()




/*----------------------------------------------------------------------------*/
/**
 * \brief Initialise the item.
 */
template<typename Base, void (Base::*Method)() const>
void bear::engine::make_autokill_from_class_const<Base, Method>::build()
{
  Base::build();
  (this->*Method)();
  this->kill();
} // make_autokill_from_class_const::on_toggle_on()
