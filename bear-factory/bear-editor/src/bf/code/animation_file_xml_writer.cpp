/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the bf::animation_file_xml_writer class.
 * \author Julien Jorge
 */
#include "bf/animation_file_xml_writer.hpp"

#include "bf/xml/value_to_xml.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a animation.
 * \param os The stream in which we write.
 * \param anim The animation to save.
 */
void bf::animation_file_xml_writer::save
( std::ostream& os, const animation& anim ) const
{
  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  xml::value_to_xml<animation>::write(os, anim);
} // animation_file_xml_writer::save()
