/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief The description of a sound and how to play it.
 * \author Julien Jorge
 */
#ifndef __BF_SOUND_DESCRIPTION_HPP__
#define __BF_SOUND_DESCRIPTION_HPP__

#include <string>
#include <set>

namespace bf
{
  class compiled_file;

  /**
   * \brief The description of a sound and how to play it.
   * \author Julien Jorge
   */
  class sound_description
  {
  public:
    /** \brief The type of the set containing the names of the files to play. */
    typedef std::set<std::string> sound_set;

  public:
    sound_description();

    void insert_file( std::string file );
    void erase_file( std::string file );
    const sound_set& get_files() const;

    void set_played_globally( bool b );
    bool is_played_globally() const;

    void compile( compiled_file& f ) const;

    bool operator==( const sound_description& that ) const;
    bool operator!=( const sound_description& that ) const;

  private:
    /** \brief The path to the sound files. */
    sound_set m_files;

    /** \brief Tell if the sound is played globally (default is "false"). */
    bool m_played_globally;

  }; // class sound_description
} // namespace bf

#endif // __BF_SOUND_DESCRIPTION_HPP__
