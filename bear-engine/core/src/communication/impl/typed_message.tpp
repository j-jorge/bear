/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::communication::typed_message class.
 * \author Julien Jorge
 */
#include <cstdlib>

/*----------------------------------------------------------------------------*/
/**
 * \brief Apply the message to something.
 * \param that The thing to apply the message to.
 */
template<typename T>
bool bear::communication::typed_message<T>::apply_to(messageable& that)
{
  T* p = (T*)&that;

  if (p != NULL)
    return apply_to(*p);
  else
    return false;
} // typed_message::apply_to()
