/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The model_file_compiler class compiles a .mdl file into a .cm file.
 * \author Julien Jorge
 */
#ifndef __BF_MODEL_FILE_COMPILER_HPP__
#define __BF_MODEL_FILE_COMPILER_HPP__

#include <string>

namespace bf
{
  class model;

  /**
   * \brief The model_file_compiler class compiles a .mdl file into a .cm file.
   * \author Julien Jorge
   */
  class model_file_compiler
  {
  public:
    bool compile( const model& mdl, const std::string& model_file ) const;

  }; // class model_file_compiler
} // namespace bf

#endif // __BF_MODEL_FILE_COMPILER_HPP__
