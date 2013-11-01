/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the bear::input::keyboard class.
 * \author Julien Jorge
 */
#include "input/keyboard.hpp"

#include "bear_gettext.hpp"

#include <SDL2/SDL.h>
#include <algorithm>
#include <claw/assert.hpp>
#include <claw/logger.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a key_code to a human-readable string.
 * \param k The key to convert.
 */
std::string bear::input::keyboard::get_name_of( key_code k )
{
  switch(k)
    {
    case kc_backspace: return "backspace"; break;
    case kc_tab: return "tab"; break;
    case kc_new_line: return "new line"; break;
    case kc_pause: return "pause"; break;

    case kc_escape: return "escape"; break;
    case kc_space: return "space"; break;
    case kc_exclamation_mark: return "!"; break;
    case kc_double_quotes: return "\""; break;
    case kc_hash: return "#"; break;
    case kc_dollar: return "$"; break;
    case kc_percent: return "%"; break;
    case kc_ampersand: return "&"; break;
    case kc_quote: return "'"; break;
    case kc_left_parenthesis: return "("; break;
    case kc_right_parenthesis: return ")"; break;
    case kc_asterisk: return "*"; break;
    case kc_plus: return "+"; break;
    case kc_comma: return ", "; break;
    case kc_minus: return "-"; break;
    case kc_period: return "."; break;
    case kc_slash: return "/"; break;

    case kc_0: return "0"; break;
    case kc_1: return "1"; break;
    case kc_2: return "2"; break;
    case kc_3: return "3"; break;
    case kc_4: return "4"; break;
    case kc_5: return "5"; break;
    case kc_6: return "6"; break;
    case kc_7: return "7"; break;
    case kc_8: return "8"; break;
    case kc_9: return "9"; break;

    case kc_colon: return ":"; break;
    case kc_semicolon: return "; break;"; break;
    case kc_less: return "<"; break;
    case kc_equal: return "="; break;
    case kc_greater: return ">"; break;
    case kc_question: return "?"; break;
    case kc_at: return "@"; break;

    case kc_A: return "A"; break;
    case kc_B: return "B"; break;
    case kc_C: return "C"; break;
    case kc_D: return "D"; break;
    case kc_E: return "E"; break;
    case kc_F: return "F"; break;
    case kc_G: return "G"; break;
    case kc_H: return "H"; break;
    case kc_I: return "I"; break;
    case kc_J: return "J"; break;
    case kc_K: return "K"; break;
    case kc_L: return "L"; break;
    case kc_M: return "M"; break;
    case kc_N: return "N"; break;
    case kc_O: return "O"; break;
    case kc_P: return "P"; break;
    case kc_Q: return "Q"; break;
    case kc_R: return "R"; break;
    case kc_S: return "S"; break;
    case kc_T: return "T"; break;
    case kc_U: return "U"; break;
    case kc_V: return "V"; break;
    case kc_W: return "W"; break;
    case kc_X: return "X"; break;
    case kc_Y: return "Y"; break;
    case kc_Z: return "Z"; break;

    case kc_left_bracket: return "["; break;
    case kc_backslash: return "\\"; break;
    case kc_right_bracket: return "]"; break;
    case kc_caret: return "^"; break;
    case kc_underscore: return "_"; break;
    case kc_backquote: return "`"; break;

    case kc_a: return "a"; break;
    case kc_b: return "b"; break;
    case kc_c: return "c"; break;
    case kc_d: return "d"; break;
    case kc_e: return "e"; break;
    case kc_f: return "f"; break;
    case kc_g: return "g"; break;
    case kc_h: return "h"; break;
    case kc_i: return "i"; break;
    case kc_j: return "j"; break;
    case kc_k: return "k"; break;
    case kc_l: return "l"; break;
    case kc_m: return "m"; break;
    case kc_n: return "n"; break;
    case kc_o: return "o"; break;
    case kc_p: return "p"; break;
    case kc_q: return "q"; break;
    case kc_r: return "r"; break;
    case kc_s: return "s"; break;
    case kc_t: return "t"; break;
    case kc_u: return "u"; break;
    case kc_v: return "v"; break;
    case kc_w: return "w"; break;
    case kc_x: return "x"; break;
    case kc_y: return "y"; break;
    case kc_z: return "z"; break;

    case kc_left_brace: return "{"; break;
    case kc_vertical_line: return "|"; break;
    case kc_right_brace: return "}"; break;
    case kc_tilde: return "~"; break;
    case kc_delete: return "delete"; break;

    case kc_keypad_0: return "keypad 0"; break;
    case kc_keypad_1: return "keypad 1"; break;
    case kc_keypad_2: return "keypad 2"; break;
    case kc_keypad_3: return "keypad 3"; break;
    case kc_keypad_4: return "keypad 4"; break;
    case kc_keypad_5: return "keypad 5"; break;
    case kc_keypad_6: return "keypad 6"; break;
    case kc_keypad_7: return "keypad 7"; break;
    case kc_keypad_8: return "keypad 8"; break;
    case kc_keypad_9: return "keypad 9"; break;

    case kc_keypad_period: return "keypad ."; break;
    case kc_keypad_divide: return "keypad /"; break;
    case kc_keypad_multiply: return "keypad *"; break;
    case kc_keypad_minus: return "keypad -"; break;
    case kc_keypad_plus: return "keypad +"; break;
    case kc_keypad_enter: return "keypad enter"; break;
    case kc_keypad_equals: return "keypad ="; break;

    case kc_up: return "up"; break;
    case kc_down: return "down"; break;
    case kc_right: return "right"; break;
    case kc_left: return "left"; break;

    case kc_insert: return "insert"; break;
    case kc_home: return "home"; break;
    case kc_end: return "end"; break;
    case kc_page_up: return "page up"; break;
    case kc_page_down: return "page down"; break;

    case kc_F1: return "F1"; break;
    case kc_F2: return "F2"; break;
    case kc_F3: return "F3"; break;
    case kc_F4: return "F4"; break;
    case kc_F5: return "F5"; break;
    case kc_F6: return "F6"; break;
    case kc_F7: return "F7"; break;
    case kc_F8: return "F8"; break;
    case kc_F9: return "F9"; break;
    case kc_F10: return "F10"; break;
    case kc_F11: return "F11"; break;
    case kc_F12: return "F12"; break;
    case kc_F13: return "F13"; break;
    case kc_F14: return "F14"; break;
    case kc_F15: return "F15"; break;

    case kc_num_lock: return "numlock"; break;
    case kc_caps_lock: return "capslock"; break;
    case kc_scroll_lock: return "scrolllock"; break;
    case kc_right_shift: return "right shift"; break;
    case kc_left_shift: return "left shift"; break;
    case kc_right_control: return "right control"; break;
    case kc_left_control: return "left control"; break;
    case kc_right_alt: return "right alt"; break;
    case kc_left_alt: return "left alt"; break;
    case kc_right_super: return "right super"; break;
    case kc_left_super: return "left super"; break;
    case kc_print_screen: return "print screen"; break;
    case kc_system: return "system"; break;
    case kc_break: return "break"; break;
    case kc_menu: return "menu"; break;
    case kc_back: return "back"; break;
    default:
      return "Unknown key";
    }
} // keyboard::get_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a key_code to a human-readable string translated with gettext.
 * \param k The key to convert.
 */
std::string bear::input::keyboard::get_translated_name_of( key_code k )
{
  return bear_gettext( get_name_of(k).c_str() );
} // keyboard::get_translated_name_of()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get a key_code from its human-readable name.
 * \param n The name of the code.
 */
bear::input::key_code
bear::input::keyboard::get_key_named( const std::string& n )
{
  for (key_code i=kc_range_min; i<=kc_range_max; ++i)
    if ( get_name_of(i) == n )
      return i;

  return kc_not_a_key;
} // keyboard::get_key_named()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator on the first pressed key.
 */
bear::input::keyboard::const_iterator bear::input::keyboard::begin() const
{
  return m_pressed_keys.begin();
} // keyboard::begin()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get an iterator after the last pressed key.
 */
bear::input::keyboard::const_iterator bear::input::keyboard::end() const
{
  return m_pressed_keys.end();
} // keyboard::end()

/*----------------------------------------------------------------------------*/
/**
 * \brief Returns the last events.
 */
const bear::input::keyboard::event_list&
bear::input::keyboard::get_events() const
{
  return m_key_events;
} // keyboard::get_events()

/*----------------------------------------------------------------------------*/
/**
 * \brief Tell if no keys are pressed.
 */
bool bear::input::keyboard::empty() const
{
  return m_pressed_keys.empty();
} // keyboard::empty()

/*----------------------------------------------------------------------------*/
/**
 * \brief Re-read the status of all keys.
 * \pre The caller is an instance of bear::input::system.
 */
void bear::input::keyboard::refresh()
{
  refresh_events();
  refresh_keys();
} // keyboard::refresh()

/*----------------------------------------------------------------------------*/
/**
 * \brief Get all keyboard events.
 */
void bear::input::keyboard::refresh_events()
{
  SDL_Event e;
  m_key_events.clear();

  // The range of events to process.
  const SDL_EventType event_min( SDL_TEXTINPUT );
  const SDL_EventType event_max( event_min );
  
  while ( SDL_PeepEvents(&e, 1, SDL_GETEVENT, event_min, event_max ) == 1 )
    {
      const SDL_TextInputEvent* const evt
        ( reinterpret_cast<SDL_TextInputEvent*>(&e) );
      
      for ( const char* c( evt->text ); *c != '\0'; ++c )
        m_key_events.push_back
          ( key_event( key_event::key_event_character, key_info( *c ) ) );
    }

} // keyboard::refresh_events()

/*----------------------------------------------------------------------------*/
/**
 * \brief Refresh the state of the keys.
 */
void bear::input::keyboard::refresh_keys()
{
  int num_codes;
  const Uint8* scan_codes( SDL_GetKeyboardState( &num_codes ) );

  m_pressed_keys.clear();

  for (unsigned int i=0; i!=(unsigned int)num_codes; ++i)
    if ( scan_codes[i] )
      {
        const SDL_Keymod mod( SDL_GetModState() );
        const key_code k
          ( sdl_key_to_local
            ( SDL_SCANCODE_TO_KEYCODE(i), mod & KMOD_SHIFT, mod & KMOD_ALT) );

        if ( (k != kc_not_a_key) &&
             (k != kc_num_lock) &&
             (k != kc_caps_lock) &&
             (k != kc_scroll_lock) )
          m_pressed_keys.push_back( k );
      }
} // keyboard::refresh_keys()

/*----------------------------------------------------------------------------*/
/**
 * \brief Convert a SDLK_* value to the corresponding key_code.
 * \param sdl_val The SDL value to convert.
 * \param shift Tell if a shift button is considered pressed.
 * \param alt Tell if an alt button is considered pressed.
 */
bear::input::key_code bear::input::keyboard::sdl_key_to_local
( unsigned int sdl_val, bool shift, bool alt ) const
{
  switch(sdl_val)
    {
    case SDLK_BACKSPACE : return kc_backspace; break;
    case SDLK_TAB       : return kc_tab;       break;
    case SDLK_CLEAR     : return kc_new_page;  break;
    case SDLK_RETURN    : return kc_new_line;  break;
    case SDLK_PAUSE     : return kc_pause;     break;
    case SDLK_ESCAPE    : return kc_escape;    break;

    case SDLK_SPACE      : return kc_space;             break;
    case SDLK_EXCLAIM    : return kc_exclamation_mark;  break;
    case SDLK_QUOTEDBL   : return kc_double_quotes;     break;
    case SDLK_HASH       : return kc_hash;              break;
    case SDLK_DOLLAR     : return kc_dollar;            break;
    case SDLK_AMPERSAND  : return kc_ampersand;         break;
    case SDLK_QUOTE      : return kc_quote;             break;
    case SDLK_LEFTPAREN  : return kc_left_parenthesis;  break;
    case SDLK_RIGHTPAREN : return kc_right_parenthesis; break;
    case SDLK_ASTERISK   : return kc_asterisk;          break;
    case SDLK_PLUS       : return kc_plus;              break;
    case SDLK_COMMA      : return kc_comma;             break;
    case SDLK_MINUS      : return kc_minus;             break;
    case SDLK_PERIOD     : return kc_period;            break;
    case SDLK_SLASH      : return kc_slash;             break;

    case SDLK_0 : return kc_0; break;
    case SDLK_1 : return kc_1; break;
    case SDLK_2 : return kc_2; break;
    case SDLK_3 : return kc_3; break;
    case SDLK_4 : return kc_4; break;
    case SDLK_5 : return kc_5; break;
    case SDLK_6 : return kc_6; break;
    case SDLK_7 : return kc_7; break;
    case SDLK_8 : return kc_8; break;
    case SDLK_9 : return kc_9; break;

    case SDLK_COLON        : return kc_colon;         break;
    case SDLK_SEMICOLON    : return kc_semicolon;     break;
    case SDLK_LESS         : return kc_less;          break;
    case SDLK_EQUALS       : return kc_equal;         break;
    case SDLK_GREATER      : return kc_greater;       break;
    case SDLK_QUESTION     : return kc_question;      break;
    case SDLK_AT           : return kc_at;            break;
    case SDLK_LEFTBRACKET  : return kc_left_bracket;  break;
    case SDLK_BACKSLASH    : return kc_backslash;     break;
    case SDLK_RIGHTBRACKET : return kc_right_bracket; break;
    case SDLK_CARET        : return kc_caret;         break;
    case SDLK_UNDERSCORE   : return kc_underscore;    break;
    case SDLK_BACKQUOTE    : return kc_backquote;     break;

    case SDLK_a : if (shift) return kc_A; else return kc_a; break;
    case SDLK_b : if (shift) return kc_B; else return kc_b; break;
    case SDLK_c : if (shift) return kc_C; else return kc_c; break;
    case SDLK_d : if (shift) return kc_D; else return kc_d; break;
    case SDLK_e : if (shift) return kc_E; else return kc_e; break;
    case SDLK_f : if (shift) return kc_F; else return kc_f; break;
    case SDLK_g : if (shift) return kc_G; else return kc_g; break;
    case SDLK_h : if (shift) return kc_H; else return kc_h; break;
    case SDLK_i : if (shift) return kc_I; else return kc_i; break;
    case SDLK_j : if (shift) return kc_J; else return kc_j; break;
    case SDLK_k : if (shift) return kc_K; else return kc_k; break;
    case SDLK_l : if (shift) return kc_L; else return kc_l; break;
    case SDLK_m : if (shift) return kc_M; else return kc_m; break;
    case SDLK_n : if (shift) return kc_N; else return kc_n; break;
    case SDLK_o : if (shift) return kc_O; else return kc_o; break;
    case SDLK_p : if (shift) return kc_P; else return kc_p; break;
    case SDLK_q : if (shift) return kc_Q; else return kc_q; break;
    case SDLK_r : if (shift) return kc_R; else return kc_r; break;
    case SDLK_s : if (shift) return kc_S; else return kc_s; break;
    case SDLK_t : if (shift) return kc_T; else return kc_t; break;
    case SDLK_u : if (shift) return kc_U; else return kc_u; break;
    case SDLK_v : if (shift) return kc_V; else return kc_v; break;
    case SDLK_w : if (shift) return kc_W; else return kc_w; break;
    case SDLK_x : if (shift) return kc_X; else return kc_x; break;
    case SDLK_y : if (shift) return kc_Y; else return kc_y; break;
    case SDLK_z : if (shift) return kc_Z; else return kc_z; break;

    case SDLK_DELETE : return kc_delete; break;

    case SDLK_KP_0 : return kc_keypad_0; break;
    case SDLK_KP_1 : return kc_keypad_1; break;
    case SDLK_KP_2 : return kc_keypad_2; break;
    case SDLK_KP_3 : return kc_keypad_3; break;
    case SDLK_KP_4 : return kc_keypad_4; break;
    case SDLK_KP_5 : return kc_keypad_5; break;
    case SDLK_KP_6 : return kc_keypad_6; break;
    case SDLK_KP_7 : return kc_keypad_7; break;
    case SDLK_KP_8 : return kc_keypad_8; break;
    case SDLK_KP_9 : return kc_keypad_9; break;
    case SDLK_KP_PERIOD   : return kc_keypad_period;   break;
    case SDLK_KP_DIVIDE   : return kc_keypad_divide;   break;
    case SDLK_KP_MULTIPLY : return kc_keypad_multiply; break;
    case SDLK_KP_MINUS    : return kc_keypad_minus;    break;
    case SDLK_KP_PLUS     : return kc_keypad_plus;     break;
    case SDLK_KP_ENTER    : return kc_keypad_enter;    break;
    case SDLK_KP_EQUALS   : return kc_keypad_equals;   break;

    case SDLK_UP    : return kc_up;    break;
    case SDLK_DOWN  : return kc_down;  break;
    case SDLK_RIGHT : return kc_right; break;
    case SDLK_LEFT  : return kc_left;  break;

    case SDLK_INSERT   : return kc_insert;    break;
    case SDLK_HOME     : return kc_home;      break;
    case SDLK_END      : return kc_end;       break;
    case SDLK_PAGEUP   : return kc_page_up;   break;
    case SDLK_PAGEDOWN : return kc_page_down; break;

    case SDLK_F1  : return kc_F1;  break;
    case SDLK_F2  : return kc_F2;  break;
    case SDLK_F3  : return kc_F3;  break;
    case SDLK_F4  : return kc_F4;  break;
    case SDLK_F5  : return kc_F5;  break;
    case SDLK_F6  : return kc_F6;  break;
    case SDLK_F7  : return kc_F7;  break;
    case SDLK_F8  : return kc_F8;  break;
    case SDLK_F9  : return kc_F9;  break;
    case SDLK_F10 : return kc_F10; break;
    case SDLK_F11 : return kc_F11; break;
    case SDLK_F12 : return kc_F12; break;
    case SDLK_F13 : return kc_F13; break;
    case SDLK_F14 : return kc_F14; break;
    case SDLK_F15 : return kc_F15; break;

    case SDLK_NUMLOCKCLEAR : return kc_num_lock;    break;
    case SDLK_CAPSLOCK     : return kc_caps_lock;   break;
    case SDLK_SCROLLLOCK   : return kc_scroll_lock; break;

    case SDLK_RSHIFT : return kc_right_shift;   break;
    case SDLK_LSHIFT : return kc_left_shift;    break;
    case SDLK_RCTRL  : return kc_right_control; break;
    case SDLK_LCTRL  : return kc_left_control;  break;
    case SDLK_RALT   : return kc_right_alt;     break;
    case SDLK_LALT   : return kc_left_alt;      break;
    case SDLK_LGUI   : return kc_left_super;    break;
    case SDLK_RGUI   : return kc_right_super;   break;

    case SDLK_PRINTSCREEN : return kc_print_screen; break;
    case SDLK_SYSREQ      : return kc_system;       break;
    case SDLK_MENU        : return kc_menu;         break;

    case SDLK_AC_BACK : return kc_back; break;

    default: return kc_not_a_key;
    }

} // keyboard::sdl_key_to_local()
