/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief Implementation of the template methods of bear::visual::text_layout.
 * \author Julien Jorge
 */

/*----------------------------------------------------------------------------*/
/**
 * \brief A function that arrange a text in a given box. A user-defined function
 * is called each time a word should be displayed.
 *
 * \b Template \b parameters:
 * - \a Func A function called each time a word should be displayed.
 *
 * \param func The function called when a word should be displayed.
 *
 * The signature of \a func be <tt> void func( position_type p,
 * std::size_t first, std::size_t last ) </tt>. The return
 * type can be different but will be ignored. The parameters of this function
 * are
 * - \a p The position in the box where the word starts. The y-coordinate is the
 *   baseline of the glyphs.
 * - \a first The first character of the word to display, in the initial string.
 * - \a last The character just past the last character to display.
 */
template<typename Func>
void bear::visual::text_layout::arrange_text( Func func ) const
{
  std::size_t i=0;

  // The position on the Y-axis is computed such that the text starts on the
  // top of the box.
  cursor_type cursor
    ( compute_line_left(0), m_size.y - compute_line_height_above_baseline(0) );

  // We allow to write outside the box if it is on less than one screen unit.
  while ( (cursor.y > -1) && (i!=m_text.size()) )
    if ( m_text[i] == '\n' )
      {
        ++i;
        cursor.y -= m_font.get_line_spacing();
        cursor.x = compute_line_left(i);
      }
    else
      arrange_next_word<Func>( func, cursor, i );
} // text_layout::arrange_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the next word and display it.
 *
 * \b Template \b parameters:
 * - \a Func A function called each time a word should be displayed.
 *
 * \param func The function called when a word should be displayed.
 * \param cursor (in/out) The position of the cursor in the component.
 * \param i (in/out) Index of the first character of the word. (out) Index of
 *        the next character to print.
 *
 * The signature of \a func be <tt> void func( position_type p,
 * std::size_t first, std::size_t last ) </tt>. The return
 * type can be different but will be ignored. The parameters of this function
 * are
 * - \a p The position in the box where the word starts. The y-coordinate is the
 *   baseline of the glyphs.
 * - \a first The first character of the word to display, in the initial string.
 * - \a last The character just past the last character to display.
 *
 * \sa arrange_text
 */
template<typename Func>
void bear::visual::text_layout::arrange_next_word
( Func func, cursor_type& cursor, std::size_t& i ) const
{
  // find the first word
  std::size_t word = m_text.find_first_not_of(' ', i);

  if (word == std::string::npos)
    i = m_text.size();
  else if (m_text[word] == '\n')
    i = word;
  else
    {
      // the end of the word
      std::size_t space = m_text.find_first_of( " \n", word );

      if (space == std::string::npos)
        space = m_text.size();

      coordinate_type p( cursor.x );
      std::size_t j( i );
      bool fit_on_line(true);

      while ( fit_on_line && (j != space) )
        {
          const size_type w
            ( m_font.get_metrics( m_text[ j ] ).get_advance().x );

          if ( p + w <= m_size.x )
            {
              p += w;
              ++j;
            }
          else
            fit_on_line = false;
        }

      // the word fits on the line
      if ( fit_on_line )
        {
          const std::size_t word_length = space - i;
          arrange_word<Func>( func, cursor, i, word_length );
          cursor.x = p;
        }
      else
        {
          if ( cursor.x == 0 )
            {
              // the word doesn't fit on the full line
              const std::size_t line_length( j - word );
              arrange_word<Func>( func, cursor, i, line_length );
            }
          else
            {
              // We will retry at the begining of the line in the next
              // loop. We remove the spaces at the begining of the line.
              i = word;
            }

          cursor.y -= m_font.get_line_spacing();
          cursor.x = compute_line_left(i);
        }
    }
} // text_layout::arrange_next_word()

/*----------------------------------------------------------------------------*/
/**
 * \brief Find the next word and display it.
 *
 * \b Template \b parameters:
 * - \a Func A function called each time a word should be displayed.
 *
 * \param func The function called when a word should be displayed.
 * \param cursor (in/out) The position of the cursor in the component.
 * \param i (in/out) Index of the first character of the word. (out) Index of
 *        the next character to print.
 * \param n Number of characters to print.
 *
 * The signature of \a func be <tt> void func( position_type p,
 * std::size_t first, std::size_t last ) </tt>. The return
 * type can be different but will be ignored. The parameters of this function
 * are
 * - \a p The position in the box where the word starts. The y-coordinate is the
 *   baseline of the glyphs.
 * - \a first The first character of the word to display, in the initial string.
 * - \a last The character just past the last character to display.
 *
 * \sa arrange_text
 *
 */
template<typename Func>
void bear::visual::text_layout::arrange_word
( Func func, cursor_type& cursor, std::size_t& i, const std::size_t n ) const
{
  func( cursor, i, i+n );
  i += n;
} // text_layout::arrange_word()
