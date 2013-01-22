/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief The action of resizing a level.
 * \author Julien Jorge
 */
#ifndef __BF_ACTION_RESIZE_LEVEL_HPP__
#define __BF_ACTION_RESIZE_LEVEL_HPP__

#include "bf/history/level_action.hpp"

#include <string>

namespace bf
{
  /**
   * \brief The action of resizing a level.
   * \author Julien Jorge
   */
  class action_resize_level:
    public level_action
  {
  public:
    /**
     * \brief Constructor.
     * \param name The new name of the level.
     * \param width The new width of the level.
     * \param height The new height of the level.
     * \param music The new music.
     */
    action_resize_level
    ( const std::string& name, unsigned int width, unsigned int height,
      const std::string& music );

    /**
     * \remark Calling execute() two times will restore the initial size.
     */
    void execute( gui_level& lvl );
    void undo( gui_level& lvl );

    bool is_identity( const gui_level& gui ) const;
    wxString get_description() const;

  private:
    /** \brief The new name of the level. */
    std::string m_name;

    /** \brief The new width of the level. */
    unsigned int m_width;

    /** \brief The new height of the level. */
    unsigned int m_height;

    /** \brief The new music. */
    std::string m_music;

  }; // class action_resize_level
} // namespace bf

#endif // __BF_ACTION_RESIZE_LEVEL_HPP__
