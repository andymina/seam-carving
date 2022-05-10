from PIL import Image
from clint.textui import colored, puts, indent
import cmd, sys, os
import build.seam_carving as sc
class SeamCarvingShell(cmd.Cmd):
  intro = colored.magenta("Seam Carving Shell (SCS)\nType 'help' or '?' to list commands.\n")
  noop = colored.yellow("This command hasn't been implemented yet.")

  def __init__(self):
    cmd.Cmd.__init__(self)
    self.prompt = colored.blue(">>> ")
    self.sample_imgs = self.load_samples("samples/")
    self.carver = None

  # region - helpers

  def load_samples(self, path: str):
    """Loads the images in samples/ by name to be used.

    Arguments:
        path {str} -- the path to the directory containing sample images.

    Returns:
        list[str] -- a list containing the names of each image in sample images.
    """
    imgs = []
    files = os.listdir("samples/")
    for file in files:
      if file != "decision_tree.JPG" and file != ".DS_Store":
        imgs.append(file.split(".")[0])

    return imgs

  def str_to_enum(self, s: str):
    """Maps strings to enums in the sc module.

    Arguments:
        s {str} -- the string to be matched

    Returns:
        enum -- the matching enum from the sc module
    """
    key = {
      "ORIGINAL": sc.ORIGINAL,
      "RESULT": sc.RESULT,
      "TRANSPOSE": sc.TRANSPOSE,
      "ENERGY": sc.ENERGY,
      "VERT_MAP": sc.VERT_MAP,
      "HORZ_MAP": sc.HORZ_MAP,
      "VERT": sc.VERT,
      "HORZ": sc.HORZ
    }

    if key.get(s) is None:
      puts(colored.red("Error: enum not found."))
      return None
    else:
      return key.get(s)


  # endregion

  # region - utils

  def do_exit(self, arg):
    """
    Closes the shell.
    """
    sys.exit(1)

  def do_bye(self, arg):
    """
    Closes the shell.
    """
    sys.exit(1)
  
  def do_quit(self, arg):
    """
    Closes the shell.
    """
    sys.exit(1)

  # endregion

  # region - img handling

  def do_list(self, arg):
    """
    Lists the sample images available.
    """    
    with indent(4):
      for idx in range(0, len(self.sample_imgs), 5):
        sample_str = " ".join(self.sample_imgs[idx:idx+5])
        puts(colored.green(sample_str))

  def do_loadimg(self, path):
    """
    Loads the image at the path into the seam carving shell and prepares it to be carved.

    Arguments:
        path {str} -- path to the user img
    """
    puts(self.noop)

  def do_set(self, img_name):
    """
    Sets the specified image to be prepared for seam carving.

    Arguments:
        img_name {str} -- the name and extension of the image to be set for seam carving.
    """
    self.carver = sc.SeamCarver("samples/dali.jpeg")

  def do_which(self, arg):
    """
    Returns the image that is currently set to be carved.
    """
    puts(self.noop)

  # endregion

  # region - seam carving

  def do_carve(self, args):
    """
    Carves the set image to the specified size. Can be used to remove or insert seams.

    Arguments:
        args {str} -- a string in the format '{width} {height}' where width and height are the
        new dimensions of the image and path represents the path to save the image to.
    """
    puts(self.noop)

  def do_highlight_seam(self, dir):
    """
    Highlight the optimal seam in the selected image

    Arguments:
        dir {str} -- the direction to find the optimal seam in. Must be one of ["vert", "horz"].
    """
    puts(self.noop)

  def do_remove_seam(self, dir):
    """
    Removes one seam in the specified direction

    Arguments:
        dir {str} -- the direction to remove one seam in
    """
    puts(self.noop)

  # endregion

  # region - display

  def do_show(self, path):
    """
    Opens a window to display the selected image. Downloads a temporary image in out/ to show.
    """
    puts(self.noop)

  def do_show_carver(self, args):
    """
    Displays the specified image of the SeamCarver. img at out/out.jpg will be overwritten.

    Arguments:
        type {str} -- must be one of sc.ImageType
    """
    # make sure we've set an image
    if self.carver is None:
      puts(colored.red("Error: image has not been selected with 'set'."))
      return

    # arg checking
    args = args.split()
    if len(args) != 1:
      puts(colored.red("Error: bad syntax. Expected one arguments: type."))
      return

    sc_enum = self.str_to_enum(args[0])
    if sc_enum is None:
      puts(colored.red("Error: type must be one of sc.ImageType"))

    path = "out/out.jpg"
    self.carver.export(sc_enum, path)
    img = Image.open(path)
    img.show()

  def do_export(self, args):
    """
    Exports the specified img type of the carving. Can only output jpg :(

    Arguments:
        type {str} -- must be one of sc.ImageType
        path {str} -- the path to output the image to. extension must be .jpg
    """
    # make sure we've set an image
    if self.carver is None:
      puts(colored.red("Error: image has not been selected with 'set'."))
      return
    
    # arg checking
    args = args.split()
    if len(args) != 2:
      puts(colored.red("Error: bad syntax. Expected two arguments: type path."))
      return

    sc_enum = self.str_to_enum(args[0])
    if sc_enum is None:
      puts(colored.red("Error: type must be one of sc.ImageType"))
    
    # op
    self.carver.export(sc_enum, args[1])

  # endregion

if __name__ == "__main__":
  SeamCarvingShell().cmdloop()

# colors available
# red, green, blue, yellow, black, magenta, cyan, white