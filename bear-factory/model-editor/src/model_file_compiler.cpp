/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bf::model_file_compiler class.
 * \author Julien Jorge
 */
#include "bf/model_file_compiler.hpp"

#include "bf/compilation_context.hpp"
#include "bf/compiled_file.hpp"
#include "bf/model.hpp"

#include <fstream>
#include <limits>

/*----------------------------------------------------------------------------*/
/**
 * \brief Compile a .mdl file into a .cm file.
 * \param mdl The model to compile.
 * \param path The path to the .mdl file.
 * \param env The workspace environment to use.
 */
bool bf::model_file_compiler::compile
( const model& mdl, const std::string& path, workspace_environment& env ) const
{
  std::string cm_path(path);

  std::size_t pos = cm_path.rfind(".mdl");
  
  if ( pos != std::string::npos )
    cm_path = cm_path.substr(0, pos);

  cm_path += ".cm";

  std::ofstream f( cm_path.c_str() );
  bool result = false;

  if (f)
    {
      compiled_file cf(f);
      compilation_context context
        ( std::numeric_limits<unsigned int>::max(), env );
      mdl.compile(cf, context);
      
      result = true;
    }

  return result;
} // model_file_compiler::compile()
