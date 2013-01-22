/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the engine::item_that_speaks class.
 * \author Sebastien Angibaud
 */

#include "engine/level_globals.hpp"

#include "engine/comic/loader/speaker_item_loader.hpp"
#include "engine/comic/message/balloon_layer_add_message.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param balloon_layer_name The name of the layer in which the balloon of the
 *        speaker item is displayed.
 */
template<class Base>
bear::engine::item_that_speaks<Base>::item_that_speaks
( std::string balloon_layer_name )
  : m_registered_in_layer( false ), m_balloon_layer_name( balloon_layer_name )
{

} // item_that_speaks::item_that_speaks()

/*----------------------------------------------------------------------------*/
/**
 * \brief Copy constructor.
 * \param that The instance to copy from.
 */
template<class Base>
bear::engine::item_that_speaks<Base>::item_that_speaks
( const item_that_speaks<Base>& that )
  : super( that ), speaker_item( that ),
    m_registered_in_layer( false ),
    m_balloon_layer_name( that.m_balloon_layer_name )
{

} // item_that_speaks::item_that_speaks()

/*---------------------------------------------------------------------------*/
/**
 * \brief Do one iteration in the progression of the item.
 * \param elapsed_time Elapsed time since the last call.
 */
template<class Base>
void bear::engine::item_that_speaks<Base>::progress
( universe::time_type elapsed_time )
{
  super::progress(elapsed_time);
  speaker_item::progress(elapsed_time);

  if ( !m_registered_in_layer )
    {
      balloon_layer_add_message msg_speaker;
      msg_speaker.set_speaker(this);

      m_registered_in_layer =
        this->get_level_globals().send_message
        ( m_balloon_layer_name, msg_speaker );
    }
} // item_that_speaks::progress()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adds the loaders of this item class into a given loader map.
 * \param m The map in which the loaders are inserted.
 */
template<class Base>
void
bear::engine::item_that_speaks<Base>::populate_loader_map( item_loader_map& m )
{
  super::populate_loader_map( m );

  m.insert( speaker_item_loader("item_that_speaks", *this) );
} // item_that_speaks::populate_loader_map()

/*----------------------------------------------------------------------------*/
/**
 * \brief Export the methods of the class.
 */
template<class Base>
void bear::engine::item_that_speaks<Base>::init_exported_methods()
{
  TEXT_INTERFACE_CONNECT_PARENT_METHOD_1
    ( item_that_speaks<Base>, speaker_item, speak, void,
      const std::vector<std::string>& );
} // item_that_speaks::init_exported_methods()

/*----------------------------------------------------------------------------*/
TEXT_INTERFACE_IMPLEMENT_TEMPLATE_METHOD_LIST_1( bear::engine::item_that_speaks,
                                                 typename, Base )
