/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Search a given directory for files with specific extensions.
 * \author Julien Jorge
 */
#ifndef __BF_SCAN_DIR_HPP__
#define __BF_SCAN_DIR_HPP__

namespace bf
{
  /**
   * \brief Search a given directory for files with specific extensions.
   * \param Func The type of a copyable function object used to process the
   *        files.
   *
   * This class scan a given directory and call Func( file_name ) on each
   * file_name found in the directory, ending with one of the given extensions.
   *
   * \author Julien Jorge
   */
  template<typename Func>
  class scan_dir
  {
  public:
    template<typename Iterator>
    void operator()
    ( const std::string& dir, Func& f, Iterator first_ext, Iterator last_ext );

  private:
    template<typename Iterator>
    bool supported_extension
    ( const std::string& dir, Iterator first_ext, Iterator last_ext );

  }; // class scan_dir
} // namespace bf

#include "bf/impl/scan_dir.tpp"

#endif // __BF_SCAN_DIR_HPP__
