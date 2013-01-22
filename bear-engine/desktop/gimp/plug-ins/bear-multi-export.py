#!/usr/bin/env python
# _*_ coding: utf_8 _*_
#
# Bear Engine's multi-export 0.1.1 is a script to use with TheGimp
# Copyright (C) 2011 by Ferry Jérémie <ferryjeremie@mothsart.com>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.


'''
Bear Engine's multi-export 0.1 is a script to use with TheGimp
for the free game projet "Plee the Bear" : http://plee-the-bear.sourceforge.net/

Install to /usr/lib/gimp/2.0/plug-ins or ~/gimp-2.6/plug-ins
(no testing on older and newest than the 2.6)

need actually:
 * python-yaml (Ubuntu : sudo apt-get install python-yaml)

Suggestion if manipulate openRaster (.ora) files :
 * the plugin "file-ora.py" on http://gimp.foebud.org/files/file-ora.py
'''

import os

import gimp, gimpplugin, math
from gimpenums import *
pdb = gimp.pdb
import gtk, gimpui, gimpcolor
from gimpshelf import shelf

# i18n
import gettext
locale_directory = gimp.locale_directory
t = gettext.translation('gimp20-python', gimp.locale_directory, fallback=True)
_ = t.ugettext

class multi_export:
    def __init__(self, runmode, img, drawable):
        self.origMsgHandler = pdb.gimp_message_get_handler()
        pdb.gimp_message_set_handler(ERROR_CONSOLE)
        self.img = img
        self.drawable = drawable
        if runmode == RUN_INTERACTIVE:
            self.showDialog()

    def showDialog(self):
        self.dialog = gimpui.Dialog("Multi export", "multi_export_dialog")

        self.table = gtk.Table(2, 4, True)
        self.table.set_homogeneous(True)
        self.table.set_row_spacings(5)
        self.table.set_col_spacings(5)
        self.table.show()

        #Config File
        label = gtk.Label('   '+_('Config File :')+'   ')
        label.set_use_underline(True)
        label.show()
        self.table.attach(label, 0, 1, 0, 1)

        config_chooser_dialog = gtk.FileChooserDialog(title=_("Open a Yaml config file"),
            action=gtk.FILE_CHOOSER_ACTION_OPEN,
            buttons=(gtk.STOCK_CANCEL,gtk.RESPONSE_CANCEL,gtk.STOCK_OPEN,gtk.RESPONSE_OK))
        config_chooser_dialog.set_default_response(gtk.RESPONSE_OK)

        filtre = gtk.FileFilter()
        filtre.set_name(_('Yaml config file'))
        #TODO : find the real(s) mime-type format
        filtre.add_mime_type('text/yaml')
        filtre.add_pattern("*.yaml")
        filtre.add_pattern("*.yml")
        config_chooser_dialog.add_filter(filtre)

        self.config_chooser = gtk.FileChooserButton(config_chooser_dialog)
        self.yaml_file = os.path.splitext(self.img.filename)[0]+'.yaml'
        #select yaml file on the same image directory
        if os.path.exists(self.yaml_file):
            self.config_chooser.set_filename(self.yaml_file)
        #else choose the path on image directory
        else:
            self.config_chooser.set_filename(os.path.dirname(self.img.filename)+'/*')

        self.config_chooser.show()
        self.table.attach(self.config_chooser, 1, 4, 0, 1)
        #Save Directory
        label = gtk.Label('   '+_('Save Directory :')+'   ')
        label.set_use_underline(True)
        label.show()
        self.table.attach(label, 0, 1, 1, 2)

        chooser_dialog = gtk.FileChooserDialog(title=_('Open the destination directory'),
            action=gtk.FILE_CHOOSER_ACTION_SELECT_FOLDER,
            buttons=(gtk.STOCK_CANCEL,gtk.RESPONSE_CANCEL,gtk.STOCK_OPEN,gtk.RESPONSE_OK))
        chooser_dialog.set_default_response(gtk.RESPONSE_OK)

        self.chooser = gtk.FileChooserButton(chooser_dialog)
        self.chooser.set_current_folder(os.path.dirname(self.img.filename))

        self.chooser.show()
        self.table.attach(self.chooser, 1, 4, 1, 2)

        self.bar_progress = gimpui.ProgressBar()
        self.bar_progress.set_text(_('Processing ...'))

        self.dialog.vbox.hbox1 = gtk.HBox(False, 5)
        self.dialog.vbox.hbox1.show()
        self.dialog.vbox.pack_start(self.dialog.vbox.hbox1, False, False, 5)
        self.dialog.vbox.hbox1.pack_start(self.table, True, True, 5)
        self.dialog.vbox.hbox2 = gtk.HBox(False, 5)
        self.dialog.vbox.hbox2.show()
        self.dialog.vbox.pack_end(self.dialog.vbox.hbox2, False, False, 5)
        self.dialog.vbox.hbox2.pack_end(self.bar_progress, True, True, 5)
        self.bar_progress.show()

        cancel_button = self.dialog.add_button(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL)
        ok_button = self.dialog.add_button(gtk.STOCK_OK, gtk.RESPONSE_OK)
        cancel_button.connect("clicked", self.cancel)
        ok_button.connect("clicked", self.validate)

        #progress
        self.process = gimp.progress_install(self.start_progress, self.end_progress,\
            self.text_progress, self.value_progress)

        self.dialog.show()

        gtk.main()

    def cancel(self, widget):
        gtk.main_quit()

    def validate(self, widget):
        #self.bar_progress.set_fraction(0.2)
        #gtk.main_iteration()
        #TODO : préciser dépendance python-yaml
        import yaml
        if os.path.exists(self.yaml_file):
            try:
                serialize = yaml.load(open(self.config_chooser.get_filename()))
            except:
                gimp.pdb.gimp_message(_('Your Yaml file is corrrupt. Check it carefully!'))
                gtk.main_quit()
            #increment for the progress bar
            progress_inc = 1/float(len(serialize))
            self.progress = 0
            self.inc = 0
            self.sprite_name = ''
            for instruction in serialize:
                #advance the barre progression
                self.inc = self.inc + 1
                self.progress = self.inc*progress_inc
                self.sprite_name = instruction

                extension = os.path.splitext(instruction)[1]
                attr = serialize[instruction]

                instruction = self.chooser.get_current_folder()+'/'+instruction
                new_img = pdb.gimp_image_duplicate(self.img)

                #picture become an composite file
                if type(attr['layers']) == dict:
                    self._composite_picture(attr, new_img, instruction)
                    continue
                #remove layers not selected
                for layer in new_img.layers:
                    if layer.name in attr['layers']:
                        layer.visible = True
                    else:
                        new_img.remove_layer(layer)

                #merge when the picture is not .xcf or .ora file
                if extension not in ['.xcf', '.ora']:
                    new_img.merge_visible_layers(CLIP_TO_IMAGE)
                    new_img.active_layer.resize_to_image_size()

                new_img.crop(attr['crop_width'], attr['crop_height'], attr['x'], attr['y'])
                new_img.scale(attr['width'] , attr['height'])

                pdb.gimp_file_save(new_img, new_img.active_layer, instruction, instruction)
                gimp.delete(new_img)
            pdb.gimp_progress_uninstall(self.process)

        else:
            #TODO : Create good Error Message
            #gimp.pdb.gimp_message("error message")
            #gimp.pdb.gimp_message_set_handler( ERROR_CONSOLE )
            print 'null'
        gtk.main_quit()

    def _composite_picture(self, attr, new_img, instruction):
        extension = os.path.splitext(instruction)[1]
        for prop in attr['layers']:
            new_layers = []
            for layer in reversed(new_img.layers):

                if layer.name in attr['layers'][prop]:
                    new_layer = layer.copy()
                    new_layers.append(new_layer.name)
                    new_img.add_layer(new_layer)
            #select layers for merging
            for layer in new_img.layers:
                if layer.name in new_layers:
                    layer.visible = True
                else:
                    layer.visible = False
            sprite = new_img.merge_visible_layers(CLIP_TO_IMAGE)
            #remove duplicate layer with the same name
            for layer in new_img.layers:
                if layer.name == prop:
                    new_img.remove_layer(layer)
            sprite.name = prop

        #remove layers not selected
        for layer in new_img.layers:
            if layer.name in attr['layers']:
                #TODO : layer.linked = False ?
                layer.visible = True
                #pdb.gimp_image_set_active_layer(new_img, layer)
                #pdb.plug_in_autocrop_layer(new_img, layer)
            else:
                new_img.remove_layer(layer)

        new_img.crop(attr['crop_width'], attr['crop_height'], attr['x'], attr['y'])
        new_img.scale(attr['width'] , attr['height'])

        if extension not in ['.xcf', '.ora']:
            # resize layers
            for layer in new_img.layers:
                layer.resize_to_image_size()

            #algorithm to determine the good width and height of the picture
            #and place all the sprites
            #the generated picture must be a power of 2 pixel size

            #first, calculate the max width and height size
            w = new_img.width
            h = new_img.height
            max_w = w*len(new_img.layers)
            max_h = h*len(new_img.layers)

            pow2_w = pow2_h = max_pow2_w = max_pow2_h = 2
            while pow2_w < w:
                pow2_w = pow2_w*2
            while pow2_h < h:
                pow2_h = pow2_h*2
            while max_pow2_w < max_w:
                max_pow2_w = max_pow2_w*2
            while max_pow2_h < max_h:
                max_pow2_h = max_pow2_h*2

            #stock all posibilities
            possibilities = []
            while pow2_h < max_pow2_h or max_pow2_w > pow2_w:
                possibilities.append([max_pow2_w, pow2_h])
                max_pow2_w = max_pow2_w/2
                pow2_h = pow2_h*2
            #keep the best
            best_p = possibilities[int(math.floor(len(possibilities)/2))]
            #resize the final picture
            new_img.resize(best_p[0], best_p[1])

            # place all layers
            spritepos = ''
            x_index = y_index = 0

            for layer in new_img.layers:
                index = new_img.layers.index(layer)
                spritepos += layer.name+': '+str(x_index*w)+' '\
                    +str(y_index*h)+' '\
                    +str(layer.width)+' '+str(layer.height)+'\n'

                new_img.layers[index].translate(x_index*w, y_index*h)
                if (x_index+2)*pow2_w > best_p[0]:
                    x_index = 0
                    y_index = y_index + 1
                else:
                    x_index = x_index + 1

            new_img.merge_visible_layers(CLIP_TO_IMAGE)
            new_img.active_layer.resize_to_image_size()

            create_spritepos = True
            if 'spritepos' in attr:
                if attr['spritepos'] != True:
                    create_spritepos = False
            if create_spritepos:
                head = "# The format of the lines is\n# picture: x y width height\n\n"
                spritepos_file = open(os.path.splitext(instruction)[0]+'.spritepos', 'w')
                spritepos_file.write(head+spritepos[:len(spritepos)-2:])

        pdb.gimp_file_save(new_img, new_img.active_layer, instruction, instruction)
        gimp.delete(new_img)

    def start_progress(self, message, cancelable):
        self.bar_progress.set_text(_('Processing sprite %s  n° %i') % (self.sprite_name, self.inc))
        self.bar_progress.set_fraction(self.progress)
        while gtk.events_pending():
            gtk.main_iteration()
    def end_progress(self):
        pass
    def text_progress(self, message):
        pass
    def value_progress(self, value):
        pass

class myplugin(gimpplugin.plugin):
    def start(self):
        gimp.main(self.init, self.quit, self.query, self._run)
    def init(self):
        pass
    def quit(self):
        pass
    def query(self):
        authorname = "Jérémie Ferry alias <mothsART>"
        copyrightname = "Jérémie Ferry alias <mothsART>"
        imgmenupath = "<Image>/Tools/Bear Engine/export/"
        date = "october 2011"

        script_sprite_create_help = _("""Build sprite files from the current image""")
        script_sprite_create_description = _("""Build multi-pictures files from a source (.xcf or .ora) file with
        a config file (yaml).
        The generated pictures can be resized and reported as an composite file. spritepos correspondent.""")
        script_sprite_create_label = _("""Build sprite files""")

        params = [
            (PDB_INT32, "run_mode", "Run mode"),
            (PDB_IMAGE, "image", "Input image"),
            (PDB_DRAWABLE, "drawable", "Input drawable"),
        ]

        gimp.install_procedure("python_multi_export",
            script_sprite_create_description,
                script_sprite_create_help,
                authorname,
                copyrightname,
                date,
                "%s_multi-export..." % (imgmenupath),
                "RGB*, GRAY*",
                PLUGIN,
                params,
                [])

    def python_multi_export(self, runmode, img, drawable):
        multi_export(runmode, img, drawable)

if __name__ == "__main__":
  myplugin().start()