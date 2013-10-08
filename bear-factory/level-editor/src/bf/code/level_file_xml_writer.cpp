/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::level_file_xml_writer class.
 * \author Julien Jorge
 */
#include "bf/level_file_xml_writer.hpp"

#include "bf/item_class_pool.hpp"
#include "bf/item_comparator.hpp"
#include "bf/level.hpp"
#include "bf/workspace_environment.hpp"
#include "bf/xml/item_instance_node.hpp"
#include "bf/xml/util.hpp"

#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a level.
 * \param os The stream in which we write.
 * \param lvl The level to save.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_writer::save
( std::ostream& os, const level& lvl, workspace_environment* env) const
{
  os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
     << "<level name='" << lvl.get_name() << "' width='" << lvl.get_width()
     << "' height='" << lvl.get_height()
     << "' music='" << lvl.get_music() << "'>\n";

  for (unsigned int i=0; i!=lvl.layers_count(); ++i)
    save_layer( os, lvl.get_layer(i), env );

  os << "</level>\n";
} // level_file_xml_writer::save()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save a layer.
 * \param os The stream in which we write.
 * \param the_layer The layer to save.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_writer::save_layer
( std::ostream& os, const layer& the_layer, workspace_environment* env) const
{
  os << "  <layer class_name='" << the_layer.get_class_name()
     << "' name='"
     << xml::util::replace_special_characters( the_layer.get_name() )
     << "' width='"
     << the_layer.get_width() << "' height='" << the_layer.get_height()
     << "' fit_level='" << the_layer.fits_level()
     << "' tag='" << the_layer.get_tag() << "'>\n\n";

  save_items(os, the_layer, env);
  save_priority(os, the_layer);

  os << "  </layer><!-- " << the_layer.get_class_name() << " -->\n\n";
} // level_file_xml_writer::save_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the items of a layer.
 * \param os The stream in which we write.
 * \param the_layer The layer to save.
 * \param env The workspace environment used.
 */
void bf::level_file_xml_writer::save_items
( std::ostream& os, const layer& the_layer, workspace_environment* env) const
{
  os << "<items>\n";

  std::list<item_instance> items
    ( the_layer.item_begin_no_filter(), the_layer.item_end_no_filter() );

  items.sort( item_comparator::by_place() );

  for ( ; !items.empty(); items.pop_front() )
    {
      const item_rendering_parameters& r
        ( items.front().get_rendering_parameters() );

      if ( (r.get_left() > the_layer.get_width())
           || (r.get_bottom() > the_layer.get_height())
           || (r.get_right() < 0)
           || (r.get_top() < 0) )
        claw::logger << claw::log_warning << "Item '"
                     << items.front().get_class().get_class_name()
                     << "' at position "
                     << r.get_left() << ' ' << r.get_bottom() << ' '
                     << r.get_right() << ' ' << r.get_top()
                     << " is outside layer '" << the_layer.get_class_name()
                     << "' and will not be saved." << std::endl;
      else
        save_item( os, items.front(), env );
    }

  os << "</items>\n\n";
} // level_file_xml_writer::save_items()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save the priority of the items.
 * \param os The stream in which we write.
 * \param the_layer The layer to save.
 */
void bf::level_file_xml_writer::save_priority
( std::ostream& os, const layer& the_layer ) const
{
  os << "<priority>\n";

  std::vector<item_instance*> items = the_layer.get_prioritized_items();

  for ( std::size_t i=0; i!=items.size(); ++i )
    {
      if ( !items[i]->get_id().empty() )
        os << "<ref id=\""
           << xml::util::replace_special_characters( items[i]->get_id() )
           << "\"/>\n";
      else
        claw::logger << claw::log_warning << "Item '"
                     << items[i]->get_class().get_class_name()
                     << "' has no id. His priority is not saved." << std::endl;
    }

  os << "</priority>\n\n";
} // level_file_xml_writer::save_priority()

/*----------------------------------------------------------------------------*/
/**
 * \brief Save an item.
 * \param os The stream in which we write.
 * \param item The item to save.
 * \param env The workspace environment used. 
 */
void bf::level_file_xml_writer::save_item
( std::ostream& os, const item_instance& item, 
  workspace_environment* env ) const
{
  xml::item_instance_node item_node(env);
  item_node.write(item, os);
} // level_file_xml_writer::save_item()
