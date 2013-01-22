#!/usr/bin/env python
# _*_ coding: utf_8 _*_
#
# Bear Engine's sprite script to use with TheGimp

# gimp proc
from gimpfu import *

# i18n
import gettext
locale_directory = gimp.locale_directory
gettext.install( "gimp20-template" , locale_directory, unicode=True )

# usefull import
import os

class SpriteBuilder:

    def __init__(self, image, drawable, directory=os.getcwd(), adjustment = 50 ):
        self.image = image
        self.drawable = drawable
        self.save_directory = directory
        self.sprite_image = pdb.gimp_image_duplicate(self.image)
        self.sprite_image.disable_undo()
        
        self.sprite_image.scale(int(round(self.sprite_image.width * (adjustment / 100))), 
int(round(self.sprite_image.height * (adjustment / 100))))
        self.spritepos_content = ""

    # create the spritepos file content
    def createSpriteposContent(self):
        self.spritepos_content += "# The format of the lines is\n"
        self.spritepos_content += "# picture: x y width height\n"
        self.spritepos_content += "\n"
	for layer in self.sprite_image.layers:
            self.spritepos_content += "%s: %d %d %d %d\n"%(layer.name,
                layer.offsets[0], layer.offsets[1], layer.width, layer.height)
        self.spritepos_content += "\n"

    # save spritepos file
    def saveSpriteposFile(self):
        filename = self.getSpriteImageBaseName() + ".spritepos"
        fullpath = os.path.join(self.save_directory, filename)
        file = open(fullpath, "w")
        file.write(self.spritepos_content.encode("iso-8859-1", "ignore"))
        file.close()

    # get the srpiet image base name
    def getSpriteImageBaseName(self):
        filename = "default"
        pos = self.image.name.find(".")
        if pos > 0:
            filename=self.image.name[:pos]
        return filename

    # save sprite image as png file in save directory
    def saveSpriteImage(self):
        # merge all visibles layers
        self.sprite_image.merge_visible_layers(EXPAND_AS_NECESSARY)
        # rename last layer
        index = 0
        for layer in self.sprite_image.layers:
            if layer.visible is True:
                break
            else:
                index = index + 1
        self.sprite_image.layers[index].name = self.getSpriteImageBaseName()
        self.sprite_image.layers[index].resize(self.sprite_image.width, 
             self.sprite_image.height, 0, 0)
        # save png
        filename = self.getSpriteImageBaseName() + ".png"
        fullpath = os.path.join(self.save_directory, filename)
        pdb.file_png_save2( self.sprite_image, self.sprite_image.layers[index],
            fullpath, filename, 0, 9, 1, 1, 1, 1, 1, 0, 1)


# Documentation
script_sprite_create_help = _("""Build sprite files from the current image""")
script_sprite_create_description = _("""Build sprite files from the current image
The sprite file (.spritepos) can also be built (optional)
and a scale factor can also be set (optional)""")
script_sprite_create_label = _("""Build sprite files""")

def script_sprite_create(image, drawable, directory, adjustment, spritepos):
    pdb.gimp_progress_set_text("Create sprite files")
    pdb.gimp_progress_update(0.1)
    builder = SpriteBuilder(image=image, drawable=drawable, directory=directory,
adjustment=adjustment)
 
    if spritepos is True:
        pdb.gimp_progress_set_text("Build spritepos file (.spritepos)")
        pdb.gimp_progress_update(0.3)
        builder.createSpriteposContent()

        pdb.gimp_progress_set_text("Save spritepos file (.spritepos)")
        pdb.gimp_progress_update(0.5)
        builder.saveSpriteposFile()

    pdb.gimp_progress_set_text("Save sprite image (.png)")
    pdb.gimp_progress_update(0.7)
    builder.saveSpriteImage()
 
    pdb.gimp_progress_end()


# Register script_sprite_create script
register(
    proc_name=("bear_script_sprite_create"),
    blurb=(script_sprite_create_description),
    help=(script_sprite_create_help),
    author=("Sébastien Debrard"),
    copyright=("GPLv2"),
    date=("2011"),
    label=(script_sprite_create_label),
    imagetypes=("*"),
    params=[
        (PF_IMAGE, "image", "Input image", None),
        (PF_DRAWABLE, "drawable", "Input drawable", None),
        (PF_DIRNAME, "directory", _("Save directory"), os.getcwd()),
        (PF_ADJUSTMENT, "adjustment",  _("Scale ratio (percent)"), 50,
          (10, 1000, 10,
          10, 10, 10)),
        (PF_BOOL, "spritepos", _("Create .spritepos file"), True),
    ],
    results=[],
    function=(script_sprite_create),
    menu=("<Image>/Tools/Bear Engine/Sprite/"),
    domain=("gimp20-python", gimp.locale_directory)
)

if __name__ == "__main__":
    main()
