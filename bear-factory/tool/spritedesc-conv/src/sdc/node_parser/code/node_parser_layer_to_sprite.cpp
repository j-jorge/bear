/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::node_parser_layer_to_sprite class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser_layer_to_sprite.hpp"

#include "grammar.hpp"

#include <set>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node of type layer_to_sprite.
 * \param xcf The description of the content of the xcf.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_layer_to_sprite::parse_node
( const xcf_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  node_children_iterator node_it=node.children.begin();
  const node_children_iterator size_node_it(node_it);

  ++node_it;

  const std::string xcf_id( node_it->value.begin(), node_it->value.end() );
  ++node_it;

  std::set<std::string> exclude;

  for ( ; node_it!=node.children.end(); ++node_it )
    exclude.insert
      ( std::string( node_it->value.begin(), node_it->value.end() ) );

  xcf_info image;

  get_xcf_from_id( xcf, desc, image, xcf_id );

  for ( xcf_info::layer_map::const_iterator it=image.layers.begin();
        it!=image.layers.end(); ++it )
    if ( exclude.find( it->first ) == exclude.end() )
      {
        spritedesc::sprite s;
        s.xcf_id = xcf_id;
        s.name = it->first;
        s.layers.push_back( it->second );

        if ( size_node_it->value.id() == grammar::id_autosize )
          {
            s.source_box = it->second.box;
            crop_sprite_to_image_bounds( image, s );
          }
        else // id == grammar::id_image_size
          s.source_box.set( 0, 0, image.width, image.height );
        
        apply_result_box_ratio( s, *size_node_it );
        desc.add_sprite( s );
      }      
} // node_parser_layer_to_sprite::parse_node()
