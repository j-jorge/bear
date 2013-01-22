/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The context in which the scripts on items are executed.
 * \author Julien Jorge
 */
#ifndef __ENGINE_SCRIPT_CONTEXT_HPP__
#define __ENGINE_SCRIPT_CONTEXT_HPP__

#include "engine/class_export.hpp"
#include "text_interface/argument_converter.hpp"
#include "text_interface/base_exportable.hpp"
#include "universe/derived_item_handle.hpp"
#include "universe/physical_item.hpp"

#include <map>
#include <string>

namespace bear
{
  namespace engine
  {
    class base_item;

    /**
     * \brief The context in which the scripts on items are executed.
     * \author Julien Jorge
     */
    class ENGINE_EXPORT script_context:
      public text_interface::argument_converter
    {
    private:
      /** \brief Handle on the actor. */
      typedef
      universe::derived_item_handle
      <text_interface::base_exportable, base_item> handle_type;

    public:
      /** \brief The type of the container in which we store the actors
          inheriting from base_item. */
      typedef std::map<std::string, handle_type> actor_item_map_type;

      /** \brief The type of iterator on actor map. */
      typedef actor_item_map_type::iterator actor_item_map_iterator_type;

       /** \brief The type of const iterator on actor map. */
      typedef actor_item_map_type::const_iterator
      actor_item_map_const_iterator_type;

      /** \brief The type of the container in which we store the actors. */
      typedef
      std::map
      <std::string, text_interface::base_exportable*> actor_map_type;

      /** \brief The type of iterator on actor map. */
      typedef actor_map_type::iterator actor_map_iterator_type;

      /** \brief The type of const iterator on actor map. */
      typedef actor_map_type::const_iterator actor_map_const_iterator_type;

    public:
      void set_actor_item( const std::string& name, base_item* item );
      base_item* get_actor_item( const std::string& name ) const;

      void set_actor
      ( const std::string& name, text_interface::base_exportable* item );

      text_interface::base_exportable*
        get_actor( const std::string& name ) const;

      actor_item_map_iterator_type get_actors_item_begin();
      actor_item_map_iterator_type get_actors_item_end();
      actor_map_iterator_type get_actors_begin();
      actor_map_iterator_type get_actors_end();

      actor_item_map_const_iterator_type get_actors_item_begin() const;
      actor_item_map_const_iterator_type get_actors_item_end() const;
      actor_map_const_iterator_type get_actors_begin() const;
      actor_map_const_iterator_type get_actors_end() const;

    private:
      virtual text_interface::converted_argument
        do_convert_argument
        ( const std::string& arg, const std::type_info& type ) const;

    private:
      /** \brief The actor used in the script that inherits of base_item. */
      actor_item_map_type m_actor_item;

      /** \brief The items used in the script that inherits of
          with_text_interface. */
      actor_map_type m_actor;

    }; // class script_context

  } // namespace engine
} // namespace bear

#endif // __ENGINE_SCRIPT_CONTEXT_HPP__
