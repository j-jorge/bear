/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief A message that can be applied only to a given type.
 * \author Julien Jorge
 */
#ifndef __COMMUNICATION_TYPED_MESSAGE_HPP__
#define __COMMUNICATION_TYPED_MESSAGE_HPP__

#include "bear/communication/message.hpp"

namespace bear
{
  namespace communication
  {
    /**
     * \brief A message that can be applied only to a given type.
     * \author Julien Jorge
     */
    template<typename T>
    class typed_message:
      public message
    {
    protected:
      virtual bool apply_to( T& that ) = 0;

    private:
      bool apply_to( messageable& that );

    }; // class message

  } // namespace communication
} // namespace bear

#include "bear/communication/impl/typed_message.tpp"

#endif // __COMMUNICATION_MESSAGE_HPP__
