;
;  Bear Engine
;
;  Copyright (C) 2005-2012 Julien Jorge, Sebastien Angibaud
;
;  This program is free software; you can redistribute it and/or modify it
;  under the terms of the GNU General Public License as published by the
;  Free Software Foundation; either version 2 of the License, or (at your
;  option) any later version.
;
;  This program is distributed in the hope that it will be useful, but WITHOUT
;  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
;  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
;  more details.
;
;  You should have received a copy of the GNU General Public License along
;  with this program; if not, write to the Free Software Foundation, Inc.,
;  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
;
;  contact: plee-the-bear@gamned.org
;

;-------------------------------------------------------------------------------
; \brief Turn all semi-transparent pixels of a drawable into opaque pixels of a
;        given color.
; \param img The image to modify.
; \param drawable The drawable to modify.
; \param color The color to assign to the semi-transparent pixels.
(define (script-fu-sharp-alpha img drawable color)
  (let* (
         (w (car (gimp-drawable-width drawable)))
         (h (car (gimp-drawable-height drawable)))
         )

    (gimp-image-undo-disable img)

    (let loop_y ((y 0))
      (unless (= y h)
              (gimp-progress-update (/ y h))

              (let loop_x ((x 0))
                (unless (= x w)
                        (let* (
                               (p (gimp-drawable-get-pixel drawable x y))
                               (pixel-color (cadr p))
                               (alpha (aref pixel-color 3))
                               )
                          (if (and (> alpha 0) (< alpha 255))
                              (let ()
                                (aset pixel-color 0 (car color))
                                (aset pixel-color 1 (cadr color))
                                (aset pixel-color 2 (caddr color))
                                (aset pixel-color 3 255)

                                (gimp-drawable-set-pixel drawable x y (car p)
                                                         pixel-color)
                                )
                              )
                          )
                        (loop_x (+ x 1))
                        )
                ) ; loop_x
              (loop_y (+ y 1))
              )
      ) ; loop_y

    (gimp-image-undo-enable img)
    (gimp-displays-flush)
    ) ; let*
  ) ; define

(script-fu-register "script-fu-sharp-alpha"
  _"Sharp alpha..."
  _"Replace semi-transparent pixels with opaque pixels of a given color"
  "Julien Jorge"
  "Julien Jorge"
  "2012"
  ""
  SF-IMAGE       "Image to filter"    0
  SF-DRAWABLE    "Drawable to filter" 0
  SF-COLOR      _"Pixel color"        '(255 0 0)
)

(script-fu-menu-register "script-fu-sharp-alpha"
                         "<Image>/Tools/Bear Engine")
