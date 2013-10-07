/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::compilation_context class.
 * \author Julien Jorge
 */
#include "bf/compilation_context.hpp"

#include <wx/image.h>

#include "bf/opaque_rectangle/ler_problem.hpp"
#include "bf/opaque_rectangle/ler_solver.hpp"

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param optimization_level The optimisation level during the compilation. If
 *        the value is less than 1 then the opaque boxes are not computed.
 * \param env The compilation environemtn to use.
 */
 bf::compilation_context::compilation_context
 ( unsigned int optimization_level, workspace_environment* env )
   : m_image_cache(env), m_optimization_level(optimization_level), 
     m_workspace(env)
{
} // compilation_context::compilation_context()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the opaque rectangle of a given sprite.
 * \param s The sprite for which we want the opaque rectangle.
 * \param image_name The image name for which we want the opaque rectangle.
 */
bf::compilation_context::rectangle
bf::compilation_context::get_opaque_rectangle
( const sprite& s, const std::string& image_name )
{
  sprite key;
  key.set_image_name( image_name );
  key.set_clip_rectangle( s.get_clip_rectangle() );

  opaque_rectangle_map::iterator it = m_opaque_rectangle.find(key);

  if ( it == m_opaque_rectangle.end() )
    it =
      m_opaque_rectangle.insert
      ( std::make_pair( key, compute_opaque_rectangle(key) ) ).first;

  return it->second;
} // compilation_context::get_opaque_rectangle()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get the identifier associated to a given item_instance in the compiled
 *        files.
 * \param item The identifier of the item for which we want the compiled
 *        identifier.
 */
unsigned int
bf::compilation_context::get_compiled_identifier
( const std::string& item ) const
{
  CLAW_PRECOND( m_identifier.find(item) != m_identifier.end() );

  return m_identifier.find(item)->second;
} // compilation_context::get_compiled_identifier()

/*----------------------------------------------------------------------------*/
/**
 * \brief Set the identifier associated to a given item_instance in the compiled
 *        files.
 * \param item The identifier of the item for which we set the compiled
 *        identifier.
 * \param id The compiled identifier.
 */
void bf::compilation_context::set_compiled_identifier
( const std::string& item, unsigned int id )
{
  CLAW_PRECOND( m_identifier.find(item) == m_identifier.end() );
  
  m_identifier[item] = id;
} // compilation_context::set_compiled_identifier()

/*----------------------------------------------------------------------------*/
/**
 * \brief Remove all the identifiers associated to the item_instance in the
 *        compiled files.
 */
void bf::compilation_context::clear_compiled_identifiers()
{
  m_identifier.clear();
} // compilation_context::clear_compiled_identifiers()

/*----------------------------------------------------------------------------*/
/**
 * \brief Return the workspace name to use.
 */
std::string bf::compilation_context::get_workspace_name() const
{
  return m_workspace->name;
} // compilation_context::get_workspace_name()

/*----------------------------------------------------------------------------*/
/**
 * \brief Compute the opaque rectangle of a given sprite.
 * \param s The sprite for which we compute the opaque rectangle.
 */
bf::compilation_context::rectangle
bf::compilation_context::compute_opaque_rectangle( const sprite& s )
{
  if ( (m_optimization_level < 1)
       || (s.get_clip_width() == 0) || (s.get_clip_height() == 0) )
    return rectangle(0, 0, 0, 0);

  const std::pair<wxBitmap, wxPoint> source = m_image_cache.get_image(s);
  wxImage image( source.first.ConvertToImage().Mirror(false) );

  ler_problem pb;
  ler_base_problem::rectangle r
    ( ler_base_problem::point(0, 0),
      ler_base_problem::point
      (s.get_clip_width() - 1, s.get_clip_height() - 1) );
  
  pb.set_bounding_rectangle(r);

  if ( !image.HasAlpha() && image.HasMask() )
    image.InitAlpha();

  if ( image.HasAlpha() )
    {
      // ler_problem expects the points to be inserted column per column.
      for ( unsigned int i = 0; i != s.get_clip_width(); ++i )
        for ( unsigned int j = 0; j != s.get_clip_height(); ++j )
          if ( image.GetAlpha(i, j) != 255 )
            pb.add_forbidden_point( ler_base_problem::point(i, j) );
    }

  ler_solver solver(pb);
  solver.resolve();

  if ( solver.is_solved() )
    r = solver.get_solution();
  else
    r = ler_base_problem::rectangle(0, 0, 0, 0);

  return r;
} // compilation_context::compute_opaque_rectangle()
