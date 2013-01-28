/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the sdc::node_parser_sprite_description_base class.
 * \author Julien Jorge
 */
#include "node_parser/node_parser_sprite_description_base.hpp"

#include "xcf_map.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Parse a node describing the ratio of the size of a sprite according to
 *        the source box.
 * \param s The structure receiving the result of the parsing.
 * \param node Node to parse.
 */
void sdc::node_parser_sprite_description_base::apply_result_box_ratio
( spritedesc::sprite& s, const tree_node& node ) const
{ 
  const std::string input( node.value.begin(), node.value.end() );
  std::istringstream iss( input );
  double ratio;

  if ( iss >> ratio )
    {
      s.result_box.width = s.source_box.width * ratio;
      s.result_box.height = s.source_box.height * ratio;
    }
  else
    std::cerr << "invalid ratio '" << input
              << "' for sprite '" << s.name << "'" << std::endl;
} // node_parser_sprite_description_base::apply_result_box_ratio()

/*----------------------------------------------------------------------------*/
/**
 * \brief Adjusts the bounds of the source box of a sprite in order to stay in
 *        the image.
 * \param image The image from which the sprite is built.
 * \param s The sprite to adjust.
 */
void sdc::node_parser_sprite_description_base::crop_sprite_to_image_bounds
( const xcf_info& image, spritedesc::sprite& s ) const
{
  if ( s.source_box.position.x < 0 )
    {
      s.source_box.width += s.source_box.position.x;
      s.source_box.position.x = 0;
    }

  if ( s.source_box.position.y < 0 )
    {
      s.source_box.height += s.source_box.position.y;
      s.source_box.position.y = 0;
    }

  if ( s.source_box.position.x + s.source_box.width > (int)image.width )
    s.source_box.width = image.width - s.source_box.position.x;

  if ( s.source_box.position.y + s.source_box.height > (int)image.height )
    s.source_box.height = image.height - s.source_box.position.y;
} // node_parser_sprite_description_base::crop_sprite_to_image_bounds()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the xcf info corresponding to a given identifier.
 * \param xcf The description of the content of the xcf.
 * \param desc The The structure where the identifier of the xcf can be solved.
 * \param info The structure receiving the result of the parsing.
 * \param id The identifier of the xcf.
 */
void sdc::node_parser_sprite_description_base::get_xcf_from_id
( const xcf_map& xcf, const spritedesc& desc, xcf_info& info,
  const std::string& xcf_id ) const
{
  // solve the xcf name from the identifier.
  const std::map<std::string, std::string>::const_iterator it =
    desc.xcf.find( xcf_id );

  if ( it == desc.xcf.end() )
    std::cerr << "Unknown xcf identifier '" << xcf_id
              << "'" << std::endl;
  else
    {
      const std::string xcf_name = it->second;

      if ( !xcf.has_info( xcf_name ) )
        std::cerr << "Unknown xcf file '" << xcf_name << "'" << std::endl;
      else
        info = xcf.get_info( xcf_name );
    }
} // node_parser_sprite_description_base::get_xcf_from_id()
