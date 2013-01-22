/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::node_parser_sprite_declaration class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser_sprite_declaration.hpp"

#include "grammar.hpp"

#include <claw/string_algorithm.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Parses node of type sprite_declaration.
 * \param xcf The description of the content of the xcf.
 * \param desc The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_declaration::parse_node
( const xcf_map& xcf, spritedesc& desc, const tree_node& node ) const
{
  // node 0 is the name
  // node 1 is the size
  // node 2 is the source image
  // node 3 is the layer list
  // node 4 is the mask (optional)

  spritedesc::sprite s;
  get_sprite_name( s, node.children[0] );

  if ( desc.get_sprite_by_name(s.name) != desc.sprite_end() )
    std::cerr << "warning: duplicate sprite entry '" << s.name << "'."
              << std::endl;

  get_image_id( s, node.children[2] );

  xcf_info image;
  get_xcf_from_id( xcf, desc, image, s.xcf_id );

  get_layers_and_size( image, s, node.children[1], node.children[3] );

  if ( node.children.size() == 5 )
    s.mask = get_layers( image, node.children[4] );

  desc.add_sprite( s );
} // node_parser_sprite_declaration::parse_node()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes the node representing the name of the sprite.
 * \param s The sprite that receives the name.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_declaration::get_sprite_name
( spritedesc::sprite& s, const tree_node& node ) const
{
  const std::string name( node.value.begin(), node.value.end() );

  claw::text::c_escape
    ( name.begin(), name.end(), std::inserter( s.name, s.name.end() ) );
} // node_parser_sprite_declaration::get_sprite_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes the node representing the identifier of the image to use for
 *        the sprite.
 * \param s The sprite that receives the id.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_declaration::get_image_id
( spritedesc::sprite& s, const tree_node& node ) const
{
  s.xcf_id = std::string( node.value.begin(), node.value.end() );
} // node_parser_sprite_declaration::get_image_id()

/*----------------------------------------------------------------------------*/
/**
 * \brief Fill the layers of a sprite and set its size.
 * \param image The image where the sprites are taken.
 * \param s The sprite that receives the layers and the size.
 * \param size_node The node containing the informations about the size.
 * \param layer_list_node The node containing the list of the layers.
 */
void sdc::node_parser_sprite_declaration::get_layers_and_size
( const xcf_info& image, spritedesc::sprite& s, const tree_node& size_node,
  const tree_node& layer_list_node ) const
{
  tree_node ratio;

  if ( size_node.value.id() == grammar::id_autosize )
    {
      s.layers = get_layers( image, layer_list_node );
      compute_source_box( s );
      ratio = size_node;
    }
  else
    {
      if ( size_node.value.id() == grammar::id_image_size )
        {
          s.source_box.set( 0, 0, image.width, image.height );
          ratio = size_node;
        }
      else // id == grammar::id_layer_size
        {
          layer_info layer;
          get_layer_info( image, layer, size_node.children[0] );

          s.source_box = layer.box;
          ratio = size_node.children[1];
        }

      s.layers = get_layers( image, layer_list_node );
    }

  crop_sprite_to_image_bounds( image, s );
  apply_result_box_ratio( s, ratio );
} // node_parser_sprite_declaration::get_layers_and_size()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes a node describing a list of layers.
 * \param image The image from which the layers are taken.
 * \param layer_list_node The node listing the layers.
 */
std::list<sdc::layer_info> sdc::node_parser_sprite_declaration::get_layers
( const xcf_info& image, const tree_node& layer_list_node ) const
{
  std::list<layer_info> result;

  if ( (layer_list_node.value.id() == grammar::id_string)
       || (layer_list_node.value.id() == grammar::id_glob) )
    add_layers( image, result, layer_list_node );
  else
    for ( std::size_t i(0); i!=layer_list_node.children.size(); ++i )
      add_layers( image, result, layer_list_node.children[i] );

  return result;
} // node_parser_sprite_declaration::get_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes a node describing a layer in a layer list.
 * \param image The image from which the layers are taken.
 * \param result The list at the end of which the layers are inserted.
 * \param node The node describing the layers.
 */
void sdc::node_parser_sprite_declaration::add_layers
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  if ( node.value.id() == grammar::id_glob )
    add_layers_glob( image, result, node );
  else if ( node.value.id() == grammar::id_exclude )
    exclude_layers( image, result, node );
  else // node.value.id() == grammar::id_string
    add_single_layer( image, result, node );
} // node_parser_sprite_declaration::add_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes a node describing a layer in a layer list with a globalized
 *        expression.
 * \param image The image from which the layers are taken.
 * \param result The list at the end of which the layers are inserted.
 * \param node The node describing the layers.
 */
void sdc::node_parser_sprite_declaration::add_layers_glob
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_glob );
  CLAW_PRECOND( node.children.size() == 2 );

  const std::string pattern
    ( node.children[1].value.begin(), node.children[1].value.end() );

  xcf_info::layer_map::const_iterator it;
  bool found=false;

  for ( it=image.layers.begin(); it!=image.layers.end(); ++it )
    if ( claw::text::glob_match( pattern, it->first ) )
      {
        result.push_back( it->second );
        found = true;
      }

  if ( !found )
    std::cerr << "Unmatched pattern '" << pattern << "'" << std::endl;
} // node_parser_sprite_declaration::add_layers_glob()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes a node describing the exclusion of some layers from a layer
 *        list.
 * \param image The image from which the layers are taken.
 * \param result The list at the end of which the layers are inserted.
 * \param node The node describing the layers.
 */
void sdc::node_parser_sprite_declaration::exclude_layers
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  CLAW_PRECOND( node.value.id() == grammar::id_exclude );
  CLAW_PRECOND( node.children.size() == 2 );

  std::list<layer_info> excluded;
  add_layers( image, excluded, node.children[1] );

  for ( std::list<layer_info>::iterator it( excluded.begin() );
        it != excluded.end(); ++it )
    {
      std::list<layer_info>::iterator candidate( result.begin() );
      bool found(false);

      while ( !found && (candidate != result.end()) )
        if ( candidate->index == it->index )
          {
            found = true;
            result.erase( candidate );
          }
        else
          ++candidate;
    }
} // node_parser_sprite_declaration::exclude_layers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Processes a node describing a layer by a string.
 * \param image The image from which the layers are taken.
 * \param result The list at the end of which the layers are inserted.
 * \param node The node describing the layer.
 */
void sdc::node_parser_sprite_declaration::add_single_layer
( const xcf_info& image, std::list<layer_info>& result,
  const tree_node& node ) const
{
  layer_info info;
        
  get_layer_info( image, info, node );
  result.push_back( info );
} // node_parser_sprite_declaration::add_single_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Computes the bounding box in the source image of the layers of which a
 *        sprite is made.
 * \param s The sprite whose source_box has to be computed.
 */
void sdc::node_parser_sprite_declaration::compute_source_box
( spritedesc::sprite& s ) const
{
  // Compute the size of the sprite in the source image.
  if ( s.layers.empty() )
    std::cerr << "missing layers for sprite '"
              << s.name << "'" << std::endl;
  else
    {
      s.source_box = s.layers.front().box;
      
      for ( std::list<layer_info>::const_iterator it = s.layers.begin();
            it != s.layers.end(); ++it )
        s.source_box = s.source_box.join( it->box );
    }
} // node_parser_sprite_declaration::compute_source_box()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the layer info associated to a given layer name in a given xcf.
 * \param xcf The description of the content of the xcf.
 * \param layer The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_declaration::get_layer_info
( const xcf_info& xcf, layer_info& layer, const tree_node& node ) const
{
  std::string raw_name( node.value.begin(), node.value.end() );
  std::string layer_name;
  claw::text::c_escape
    ( raw_name.begin(), raw_name.end(),
      std::inserter( layer_name, layer_name.end() ) );

  const xcf_info::layer_map::const_iterator it_layer =
    xcf.layers.find( layer_name );

  if ( it_layer == xcf.layers.end() )
    std::cerr << "Unknown layer '" << layer_name << "'" << std::endl;
  else
    layer = it_layer->second;
} // node_parser_sprite_declaration::get_layer_info()
