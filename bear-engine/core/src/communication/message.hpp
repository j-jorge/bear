/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Base class for message events.
 * \author Julien Jorge
 */
#ifndef __COMMUNICATION_MESSAGE_HPP__
#define __COMMUNICATION_MESSAGE_HPP__

#include "communication/class_export.hpp"

namespace bear
{
  namespace communication
  {
    class messageable;

    /**
     * \brief Base class for message events.
     * \author Julien Jorge
     */
    class COMMUNICATION_EXPORT message
    {
    public:
      virtual ~message();

      virtual bool apply_to( messageable& that ) = 0;

    }; // class message

  } // namespace communication
} // namespace bear

#endif // __COMMUNICATION_MESSAGE_HPP__
