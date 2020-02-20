/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A physical item that can receive messages.
 * \author Julien Jorge
 */
#ifndef __ENGINE_MESSAGEABLE_ITEM_HPP__
#define __ENGINE_MESSAGEABLE_ITEM_HPP__

#include "bear/communication/messageable.hpp"

namespace bear
{
  namespace engine
  {
    /**
     * \brief A physical item that can receive messages.
     * \author Julien Jorge
     *
     * \b Template \b arguments:
     * - \a Base: any hierarchy ending by base_item.
     */
    template<class Base>
    class messageable_item :
      public Base,
      public communication::messageable
    {
    public:
      /** \brief The type of the parent class. */
      typedef Base super;

    public:
      messageable_item();
      messageable_item( const std::string& name );

      void on_enters_layer();
      void destroy();

      bool
      set_string_field( const std::string& name, const std::string& value );

      bool is_valid() const;

    protected:
      void to_string( std::string& str ) const;

      bool set_name( const std::string& name );

    }; // class messageable_item
  } // namespace engine
} // namespace bear

#include "bear/engine/impl/messageable_item.tpp"

#endif // __ENGINE_MESSAGEABLE_ITEM_HPP__
