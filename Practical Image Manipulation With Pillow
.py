# -------------------------------------------------
# -- Practical => Image Manipulation With Pillow --
# -------------------------------------------------

from PIL import Image

# Open The Image
myImage = Image.open("/home/muayad/Pictures/Screenshots/Screenshot From 2026-07-06 15-30-55.png")

# Show The Image
myImage.show()

# My Cropped Image
myBox = (300, 300, 800, 800)
myNewImage = myImage.crop(myBox)

# Show The New Image
myNewImage.show()

# My Converted Mode Image
# myConverted = myImage.convert("L")
# myConverted.show()