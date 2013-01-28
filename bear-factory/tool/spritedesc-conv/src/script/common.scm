;
;  Plee the Bear -- gfx script tools
;
;  Copyright (C) 2005-2010 Julien Jorge, Sebastien Angibaud
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
;  Please add the tag [PTB] in the subject of your mails.
;
; --
;
; This script is intended to be included by the others scripts.

; function telling if a value is in a list
(define in-list
  (lambda (l e)
    (if (null? l)
        #f
        (if (= (car l) e)
            #t
            (in-list (cdr l) e)
            )
        )
    )
  )

; function to modify the position of a layer
(define set-layer-position
  (lambda (img layer x y)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let ( (layer_i (aref (cadr all_layers) layer)) )
        (gimp-layer-set-offsets layer_i x y)
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; function showing a set of layer, and hide the others
(define show-layers
  (lambda (img layers)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (let ( (layer_i (aref (cadr all_layers) i)) )
                  (if (in-list layers i)
                      (gimp-drawable-set-visible layer_i TRUE)
                      (gimp-drawable-set-visible layer_i FALSE))
                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; function showing all layers
(define show-all-layers
  (lambda (img)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (let ( (layer_i (aref (cadr all_layers) i)) )
                  (gimp-drawable-set-visible layer_i TRUE)
                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; function to shift the position of a layer and loop its content
(define priv-paste-shift-layer
  (lambda (layer x y w h)
    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (gimp-layer-set-offsets layer_copy x y)
      (gimp-floating-sel-anchor layer_copy)
      ); let

    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (if (> x 0)
          (gimp-layer-set-offsets layer_copy (- x w) y)
          (gimp-layer-set-offsets layer_copy (+ w x) y)
          ) ; if
      (gimp-floating-sel-anchor layer_copy)
      ); let

    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (if (> y 0)
          (gimp-layer-set-offsets layer_copy x (- y h))
          (gimp-layer-set-offsets layer_copy x (+ h y))
          ) ; if
      (gimp-floating-sel-anchor layer_copy)
      ); let

    (let ( (layer_copy (car (gimp-edit-paste layer 1)) ) )
      (if (> x 0)
          (if (> y 0)
              (gimp-layer-set-offsets layer_copy (- x w) (- y h))
              (gimp-layer-set-offsets layer_copy (- x w) (+ h y))
              ) ; if
          (if (> y 0)
              (gimp-layer-set-offsets layer_copy (+ w x) (- y h))
              (gimp-layer-set-offsets layer_copy (+ w x) (+ h y))
              ) ; if
          ) ; if
      (gimp-floating-sel-anchor layer_copy)
      ) ; let
    ) ; lambda
  ) ; define priv-paste-shift-layer

; function to shift the position of a list of layers and loop their content
(define shift-layers
  (lambda (img layers x y)
    (show-layers img layers)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (if (in-list layers i)
                    (let ( (layer_i (aref (cadr all_layers) i)) )
                      (let ( (w (car (gimp-drawable-width layer_i)))
                             (h (car (gimp-drawable-height layer_i)))
                             )
                        (gimp-edit-copy layer_i)
                        (gimp-edit-clear layer_i)
                        
                        (priv-paste-shift-layer layer_i x y w h)
                        ) ; let
                      ) ; let
                    ) ; if
                (loop (+ i 1))
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define shift-layers

; use a layer of the source image as a mask for a layer of the destination
(define set-layer-mask
  (lambda (src mask dest)
    (if (null? mask)
        ()
        (begin
          (show-layers src mask)
          (gimp-edit-copy-visible src)

          (let ( (the_layer (car (gimp-image-get-active-layer dest))) )
            ( gimp-layer-add-mask
              the_layer
              (car (gimp-layer-create-mask the_layer 0)) )
            (gimp-layer-set-apply-mask the_layer TRUE)
            (gimp-layer-set-edit-mask the_layer TRUE)
            (gimp-edit-paste (car (gimp-layer-get-mask the_layer)) 1)
            (gimp-floating-sel-anchor (car (gimp-image-get-active-layer dest)))
            (gimp-layer-set-edit-mask the_layer FALSE)
            (gimp-layer-set-show-mask the_layer FALSE)
            ) ; let
          ) ; begin
        ) ; if
    ) ; lambda
  ) ; define

; change the opacity of a layer
(define set-layer-opacity
  (lambda (img layers opacity)
    (let ( (all_layers (gimp-image-get-layers img)) )
      (let loop ((i 0))
        (unless (= i (car all_layers))
                (let ( (layer_i (aref (cadr all_layers) i)) )
                  (if (in-list layers i)
                      (gimp-layer-set-opacity layer_i opacity))
                  (loop (+ i 1))
                  ) ; let
                ) ; unless
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; create the scaled frame of an animation with the current visible layers
(define create-layer-from-image
  (lambda (img x y w h the_image)
    (let ( (frame (car (begin
                         (gimp-edit-copy-visible img)
                         (gimp-edit-paste-as-new)
                         ) ; begin
                       ) ; car
                  ) ) ; frame
                                        ; create the resulting merged layer
      (let ( (the_layer (car
                         (gimp-layer-new-from-drawable
                          (car (gimp-image-get-active-drawable frame))
                          the_image
                          ) ; gimp-layer-new-from-drawable
                         ) ; car
                        ) ) ; the_layer
                                        ; resize the layer
        (gimp-image-add-layer the_image the_layer -1)
        (gimp-layer-scale-full the_layer w h TRUE INTERPOLATION-CUBIC)
        (gimp-layer-set-offsets the_layer x y)

	the_layer
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; create the scaled frame of an animation
(define create-layer
  (lambda (img layers x y w h the_image)
    ; select the visible layers
    (show-layers img layers)
    (create-layer-from-image img x y w h the_image)
    ) ; lambda
  ) ; define

; create the scaled sprite of an item
(define create-layer-crop
  (lambda (img layers sx sy sw sh x y w h the_image mask)
    ; select the visible layers
    (show-layers img layers)
    (create-layer-crop-current img sx sy sw sh x y w h the_image mask)
    ) ; lambda
  ) ; define

; create the scaled sprite of an item with the currently visible layers
(define create-layer-crop-current
  (lambda (img sx sy sw sh x y w h the_image mask)

    (let ( (frame (car (begin
                         (gimp-edit-copy-visible img)
                         (gimp-edit-paste-as-new)
                         ) ; begin
                       ) ; car
                  ) ) ; frame

      ; turn the mask on
      (set-layer-mask img mask frame)

      ; get the sub part of the image
      (gimp-image-crop frame sw sh sx sy)
                                        ; create the resulting merged layer
      (let ( (the_layer (car
                         (gimp-layer-new-from-drawable
                          (car (gimp-image-get-active-drawable frame))
                          the_image
                          ) ; gimp-layer-new-from-drawable
                         ) ; car
                        ) ) ; the_layer

        (gimp-image-add-layer the_image the_layer -1)

                                        ; resize the layer
        (gimp-layer-scale-full the_layer w h TRUE INTERPOLATION-CUBIC)
        (gimp-layer-set-offsets the_layer x y)

	the_layer
        ) ; let
      ) ; let
    ) ; lambda
  ) ; define

; Create a new image
(define new-image
  (lambda (width height)
    (let ( (dest_image (car (gimp-image-new width height RGB)))
           )

      (let ( (bk (car (gimp-layer-new dest_image width height
                                      1 "background" 100 0)))
             )
        (gimp-image-add-layer dest_image bk 0)
        )

      dest_image
      ) ; let
    ) ; lambda
  ) ; new-image

; Create a new image
(define new-image-max
  (lambda (width height)
    (let ( (dest_image (car (gimp-image-new width height RGB)))
           )
      dest_image
      ) ; let
    ) ; lambda
  ) ; new-image-max

; Save the resulting image
(define save-frames
  (lambda (name img)
    (gimp-image-merge-visible-layers img 1)
    (gimp-file-save 1 img (car (gimp-image-get-active-drawable img)) name name)
    ) ; lambda
  ) ; define

; Save the resulting image then exit the Gimp
(define save-frames-and-exit
  (lambda (name img)
    (save-frames name img)
    (gimp-quit 1)
    ) ; lambda
  ) ; define

