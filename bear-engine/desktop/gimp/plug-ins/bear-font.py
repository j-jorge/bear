#!/usr/bin/env python
# _*_ coding: utf_8 _*_
#
# Bear Engine's font script for use with The Gimp

# gimp proc
from gimpfu import *

# i18n
import gettext
locale_directory = gimp.locale_directory
gettext.install( "gimp20-template" , locale_directory, unicode=True )

# usefull import
import os

MAX_IMAGE_SIZE = 512

class FontBuilder:

    def __init__(self, image, drawable, directory=os.getcwd() ):
        self.image = image
        self.drawable = drawable
        self.save_directory = directory
        self.font_image = gimp.Image(image.width, image.height, RGB_IMAGE)
        self.font_image.disable_undo()
        # remove all layer (ie. the default background layer)
        for layer in self.font_image.layers:
            self.font_image.remove_layer(layer)

        self.font_layer_list = list()
        self.complete_glyph_dict = dict()
        self.fnt_content = ""
        self.glyph_width = image.width
        self.glyph_height = image.height

        self.glyph_per_row = 0
        self.glyph_per_column = 0

        self.font_image_ajustment = 100

    # add glyph layer to font layer list
    def createFontLayerList(self):
        for layer in self.image.layers:
            if len(unicode(layer.name)) == 1:
                if layer.name == " " or self.hasVisiblePixel(layer) is True:
                    tmp_layer = pdb.gimp_layer_new_from_drawable(layer, 
                        self.font_image)
                    tmp_layer.name = layer.name
                    tmp_layer.visible = True
                    self.font_layer_list.append(tmp_layer)


    # complete glyph list
    def completeGlyphList(self, uppercase=False, lowercase=False):
        glyph_list = list()
        for layer in self.font_image.layers:
            glyph_list.append(layer.name)
        for layer in self.font_image.layers:
            self.complete_glyph_dict[layer.name] = list()
            if uppercase is True:
                upper = unicode(layer.name).upper()
                if upper not in glyph_list:
                    self.complete_glyph_dict[layer.name].append(upper)
            if lowercase is True:
                lower = unicode(layer.name).lower()
                if lower not in glyph_list:
                    self.complete_glyph_dict[layer.name].append(lower)


    def calculateImageSizes(self, font_image_ajustment=100):
        glyph_width = int(round(self.font_image.width * (font_image_ajustment / 100)))
        glyph_heigth = int(round(self.font_image.height * (font_image_ajustment / 100)))
        total_glyph = len(self.font_layer_list)
        if total_glyph > 0:
            glyph_counter = 0
            self.glyph_per_row = 1
            self.glyph_per_column = 1
            while glyph_width * (self.glyph_per_row + 1) < MAX_IMAGE_SIZE and glyph_counter < total_glyph:
                self.glyph_per_row = self.glyph_per_row + 1
                glyph_counter = glyph_counter + 1
            while glyph_counter < total_glyph:
                self.glyph_per_column = self.glyph_per_column + 1
                glyph_counter = glyph_counter * 2


    # resize the font image (width and height are power of two)
    def resizeFontImage(self, font_image_ajustment=100):
        self.font_image_ajustment = font_image_ajustment
        self.font_image.scale(int(
          round(self.font_image.width * (font_image_ajustment / 100))),
          int(round(self.font_image.height * (font_image_ajustment / 100))))
        self.glyph_width = self.font_image.width
        self.glyph_height = self.font_image.height
        total_glyph = len(self.font_layer_list)
        if total_glyph > 0:
            glyph_counter = 0
            self.glyph_per_row = 1
            self.glyph_per_column = 1
            while self.glyph_width * (self.glyph_per_row + 1) < MAX_IMAGE_SIZE and glyph_counter < total_glyph:
                self.glyph_per_row = self.glyph_per_row + 1
                glyph_counter = glyph_counter + 1
            while glyph_counter < total_glyph:
                self.glyph_per_column = self.glyph_per_column + 1
                glyph_counter = glyph_counter * 2

            # calculate size image (width and height are power of two)
            # x size 
            x = 1
            while ( self.glyph_width * self.glyph_per_row ) > 2**x:
                x = x + 1
            x_offset = 2**x - ( self.glyph_width * self.glyph_per_row )
            image_width = (self.glyph_width * self.glyph_per_row) + x_offset

            # y size
            y = 1
            while ( self.glyph_height * self.glyph_per_column ) > 2**y:
                y = y + 1
            y_offset = 2**y - (self.glyph_height * self.glyph_per_column)
            image_height = (self.glyph_height * self.glyph_per_column) + y_offset

            # resize font image
            self.font_image.resize(image_width, image_height, 0, 0)


    # add glyph layers to the font image
    def addFontLayersToFontImage(self):
        # add all font layer
        x = 0
        y = 0
        for layer in self.font_layer_list:
            if x + 1 > self.glyph_per_row:
                x = 0
                y = y + 1
            pdb.gimp_image_add_layer(self.font_image, layer, 0)
            layer.set_offsets(self.glyph_width * x, self.glyph_height * y)
            x = x + 1


    def getFontImageBaseName(self):
        filename = "default"
        pos = self.image.name.find(".")
        if pos > 0:
            filename=self.image.name[:pos]
        return filename 


    #  create the fnt file content
    def createFntContent(self):
        ressouce_name = self.getFontImageBaseName() + ".png"
        self.fnt_content = "%d %d\n"%(self.glyph_width,
            self.glyph_height)
        self.fnt_content += "1\n"
        self.fnt_content += "font/%s\n"%(ressouce_name)
        for layer in self.font_image.layers:
            if layer.name == " ":
                # add sp add nbsp
                self.fnt_content += "%s %d %d 0\n"%(unichr(160),
                    layer.offsets[0], layer.offsets[1])
                self.fnt_content += "%s %d %d 0\n"%(unichr(32),
                    layer.offsets[0], layer.offsets[1])
            else:
                self.fnt_content += "%s %d %d 0\n"%(layer.name, layer.offsets[0],
                    layer.offsets[1])
                if self.complete_glyph_dict.has_key(layer.name):
                    for complete_glyph in self.complete_glyph_dict[layer.name]:
                        self.fnt_content += "%s %d %d 0\n"%(complete_glyph, 
                            layer.offsets[0], layer.offsets[1])
                

    # save fnt file
    def saveFntFile(self):
        filename = self.getFontImageBaseName() + ".fnt"
        fullpath = os.path.join(self.save_directory, filename)
        file = open(fullpath, "w")
        file.write(self.fnt_content.encode("iso-8859-1", "ignore"))
        file.close()


    # save font image as png file in save directory
    def saveFontImage(self):
        # merge all visibles layers
        self.font_image.merge_visible_layers(EXPAND_AS_NECESSARY)
        # rename last layer
        index = 0
        for layer in self.font_image.layers:
            if layer.visible is True:
                break
            else:
                index = index + 1
        self.font_image.layers[index].name = self.getFontImageBaseName()
        self.font_image.layers[index].resize(self.font_image.width,
            self.font_image.height, 0, 0)
        # save png
        filename = self.getFontImageBaseName() + ".png"
        fullpath = os.path.join(self.save_directory, filename)
        self.font_image.layers[index].height
        pdb.file_png_save2( self.font_image, self.font_image.layers[index], 
            fullpath, filename, 0, 9, 1, 1, 1, 1, 1, 0, 1)


    # display font image
    def displayFontImage(self):
        self.font_image.enable_undo()
        gimp.Display(self.font_image)
        gimp.displays_flush()



    # add Glyph layer to font image (ie. space glyph)
    def addGlyphLayerToFontImage(self, glyph_name):
        add = True
        for layer in self.image.layers:
            if unicode(layer.name) == glyph_name:
                add = False
                break
        if add is True:
            layer = gimp.Layer(self.font_image, glyph_name, self.glyph_width,
            self.glyph_height, RGBA_IMAGE, 0, NORMAL_MODE)
            layer.add_alpha()
            layer.fill(TRANSPARENT_FILL)
            layer.visible = True
            self.font_layer_list.append(layer)


    # add Glyph layer to image
    def addGlyphLayerToImage(self, glyph_name):
        layer = gimp.Layer(self.image, glyph_name, self.glyph_width,
            self.glyph_height, RGBA_IMAGE, 0, NORMAL_MODE)
        layer.add_alpha()
        layer.fill(TRANSPARENT_FILL)
        layer.visible = True
        self.image.add_layer(layer, 0)
    

    # Tell if drawable (image or layer) has visible pixel
    def hasVisiblePixel(self, drawable):
        # TODO OPTIMIZE, very slow
        for y in range(0,drawable.height):
            for x in range(0,drawable.width):
                (channels,pixel) = pdb.gimp_drawable_get_pixel(drawable,x,y)
                if pixel != (0, 0, 0, 0):
                    return True
        return False

    
    # get iso-8859-15 printable glyph
    # don't use "string.printable"
    def getPrintableGlyphsList(self, ignore_lowercase=False,
        ignore_uppercase=False):
        listing = list()
        iso_glyph_codes_tmp = range(161, 256, 1)
        iso_glyph_codes = range(33, 127, 1)
        for i in iso_glyph_codes_tmp:
            iso_glyph_codes.append(i)

        for glyph in iso_glyph_codes:
            glyph_str = unichr(glyph)
            if ignore_lowercase is True and glyph_str.islower() is True:
                continue
            if ignore_uppercase is True and glyph_str.isupper() is True:
                continue
            listing.append(glyph_str)
        listing.sort()
        return listing


# Documentation
script_glyph_layer_update_help = _("""Add iso-8858-15 glyph layers""")
script_glyph_layer_update_description = _("""Add iso-8858-15 glyph layers to the current image""")
script_glyph_layer_update_label = _("""Add glyph layers""")

def script_glyph_layer_update(image, drawable, ignore_lowercase,
    ignore_uppercase):
    pdb.gimp_progress_set_text("Add glyph layers")
    pdb.gimp_progress_update(0.1)
    builder = FontBuilder(image=image, drawable=drawable)
    pdb.gimp_progress_update(0.2)
    pdb.gimp_progress_set_text("Find glyph layers")
    listGlyph = builder.getPrintableGlyphsList(
        ignore_lowercase=ignore_lowercase, ignore_uppercase=ignore_uppercase)
    up = 0.8 / len(listGlyph)
    i = 0
    for char in listGlyph:
        add = True
        for layer in image.layers:
            if len(unicode(layer.name)) == 1:
                if unicode(layer.name) == char:
                    add = False
        i = i + 1
        if add:
            pdb.gimp_progress_set_text("Add glyph layer '%s'"%(char))
            pdb.gimp_progress_update(0.2 + up * i )
            builder.addGlyphLayerToImage(char)
    pdb.gimp_progress_end()




# Documentation
script_build_font_file_help = _("""Create and build font image from layers""")
script_build_font_file_description = _("""Create and build font image from layers.
The font file (.fnt) can also be built (optional)
and a scale factor can also be set (optional)""")
script_build_font_file_label = _("""Build font files""")

def script_build_font_file(image, drawable, directory, font_image_ajustment, add_space, 
    create_fnt, add_uppercase, add_lowercase):
    pdb.gimp_progress_set_text("Build font")
    pdb.gimp_progress_update(0.1)
    builder = FontBuilder(image=image, drawable=drawable, directory=directory)
    pdb.gimp_progress_set_text("Create layer list")
    pdb.gimp_progress_update(0.2)
    builder.createFontLayerList()
    if add_space is True:
        pdb.gimp_progress_set_text("Add space layer to font image")
        builder.addGlyphLayerToFontImage(" ")
        pdb.gimp_progress_update(0.35)
    pdb.gimp_progress_set_text("Calculate font image sizes")
    pdb.gimp_progress_update(0.4)
    builder.calculateImageSizes(font_image_ajustment=font_image_ajustment)
    pdb.gimp_progress_set_text("Add layer to font image")
    pdb.gimp_progress_update(0.5)
    builder.addFontLayersToFontImage()
    pdb.gimp_progress_set_text("Resize font image")
    pdb.gimp_progress_update(0.6)
    builder.resizeFontImage(font_image_ajustment=font_image_ajustment)
    if create_fnt is True:
        if add_uppercase is True or add_lowercase is True:
            pdb.gimp_progress_set_text("Complete fnt file")
            builder.completeGlyphList(uppercase=add_uppercase, lowercase=add_lowercase)
            pdb.gimp_progress_update(0.65)
        pdb.gimp_progress_set_text("Create fnt file")
        pdb.gimp_progress_update(0.7)
        builder.createFntContent()
        builder.saveFntFile()
    pdb.gimp_progress_set_text("Save font image")
    pdb.gimp_progress_update(0.8)
    builder.saveFontImage()
    pdb.gimp_progress_set_text("Display font image")
    pdb.gimp_progress_update(0.9)
    builder.displayFontImage()
    pdb.gimp_progress_end()




# Register script_glyph_layer_update script
register(
    proc_name=("bear_script_glyph_layer_update"),
    blurb=(script_glyph_layer_update_description),
    help=(script_glyph_layer_update_help),
    author=("Sébastien Debrard"),
    copyright=("GPLv2"),
    date=("2010"),
    label=(script_glyph_layer_update_label),
    imagetypes=("*"),
    params=[
        (PF_IMAGE, "image", "Input image", None),
        (PF_DRAWABLE, "drawable", "Input drawable", None),
        (PF_BOOL, "ignore_lowercase", _("Ignore lower case glyphs"),
          False),
        (PF_BOOL, "ignore_uppercase", _("Ignore upper case glyphs"),
          False),
    ],
    results=[],
    function=(script_glyph_layer_update),
    menu=("<Image>/Tools/Bear Engine/Font/"),
    domain=("gimp20-python", gimp.locale_directory)
)

# Register script_build_font_file script
register(
    proc_name=("bear_script_build_font_file"),
    blurb=(script_build_font_file_description),
    help=(script_build_font_file_help),
    author=("Sébastien Debrard"),
    copyright=("GPLv2"),
    date=("2010"),
    label=(script_build_font_file_label),
    imagetypes=("*"),
    params=[
        (PF_IMAGE, "image", "Input image", None),
        (PF_DRAWABLE, "drawable", "Input drawable", None),
        (PF_DIRNAME, "directory", _("Save directory"), os.getcwd()),
        (PF_ADJUSTMENT, "font_image_ajustment",  _("Scale ratio (percent)"), 100,
          (10, 1000, 10,
          10, 10, 10)),
        (PF_BOOL, "create_fnt", _("Create .fnt file"), True),
        (PF_BOOL, "add_space_glyph", _("Add space glyph if it doesn't exist"), True),
        (PF_BOOL, "add_uppercase", _("Complete .fnt file with upper case glyphs"),
          False),
        (PF_BOOL, "add_uppercase", _("Complete .fnt file with lower case glyphs"),
          False),
    ],
    results=[],
    function=(script_build_font_file),
    menu=("<Image>/Tools/Bear Engine/Font/"),
    domain=("gimp20-python", gimp.locale_directory)
)


if __name__ == "__main__":
    main()
