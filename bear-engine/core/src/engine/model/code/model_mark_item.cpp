/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::engine::model_mark_item class.
 * \author Julien Jorge
 */
#include "engine/model/model_mark_item.hpp"

#include "text_interface/auto_converter_maker.hpp"
#include "universe/collision_info.hpp"

BASE_ITEM_IMPLEMENT(model_mark_item, bear::engine)

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 */
bear::engine::model_mark_item::model_mark_item()
: base_item(), m_model_item(NULL)
{
  set_phantom(true);
} // model_mark_item::model_mark_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 * \remark The model to which \a that item belongs will not kept in the copy.
 */
bear::engine::model_mark_item::model_mark_item( const model_mark_item& that )
  : level_object(that), base_item(that), m_model_item(NULL),
    m_collision_function(that.m_collision_function)
{

} // model_mark_item::model_mark_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the model item to which the collisions are reported.
 * \param item The item to which the collisions are reported.
 */
void bear::engine::model_mark_item::set_model_item( base_item& item )
{
  m_model_item = &item;
} // model_mark_item::set_model_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the model item to which the collisions are reported.
 */
bear::engine::base_item& bear::engine::model_mark_item::get_model_item()
{
  return *m_model_item;
} // model_mark_item::get_model_item()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the name of the function called when a collision occurs.
 * \param f The name of the collision function.
 */
void
bear::engine::model_mark_item::set_collision_function( const std::string& f )
{
  m_collision_function = f;
} // model_mark_item::set_collision_function()

/*----------------------------------------------------------------------------*/
/**
 * \brief Process a collision with an other item.
 * \param that The other item.
 * \param info Some informations about the collision.
 */
void bear::engine::model_mark_item::collision
( base_item& that, universe::collision_info& info )
{
  if ( (&that != m_model_item) && (m_model_item != (base_item*)NULL) )
    {
      const model_mark_item* m = dynamic_cast<model_mark_item*>(&that);
      if ( (m == NULL) || (m->m_model_item != m_model_item) )
        m_model_item->execute
          ( m_collision_function,
            text_interface::auto_converter_maker(this, &that, &info) );
    }
} // model_mark_item::collision()
