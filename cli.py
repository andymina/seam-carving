from random import sample
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

    self.samples = self.__load_samples("samples/")
    self.enums = self.__map_enums()

    self.current_img = None
    self.carver = None

  # region - helpers

  def __load_samples(self, path: str):
    """Loads the images in samples/ by name to be used.

    Arguments:
        path {str} -- the path to the directory containing sample images.

    Returns:
        list[str] -- a list containing the names of each image in sample images.
    """
    files = os.listdir("samples/")
    exclude_files = ["decision_tree.JPG", ".DS_Store"]
    samples = dict(os.path.splitext(file) for file in files if file not in exclude_files)
    return samples

  def __map_enums(self):
    """Maps strings to enums in the sc module.

    Arguments:
        s {str} -- the string to be matched

    Returns:
        enum -- the matching enum from the sc module
    """
    return {
      "original": sc.ImageType.ORIGINAL,
      "result": sc.ImageType.RESULT,
      "transpose": sc.ImageType.TRANSPOSE,
      "energy": sc.ImageType.ENERGY,
      "vert_map": sc.ImageType.VERT_MAP,
      "horz_map": sc.ImageType.HORZ_MAP,
      "vert": sc.Dir.VERT,
      "horz": sc.Dir.HORZ
    }

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

  def do_list_samples(self, arg):
    """
    Lists the sample images available.
    """
    names = list(self.samples.keys())
    for idx in range(0, len(names), 10):
      sample_str = " ".join(names[idx:idx+10])
      puts(colored.green(sample_str))

  def do_loadimg(self, path):
    """
    Loads the image at the path into the seam carving shell and prepares it to be carved.

    Arguments:
        path {str} -- path to the user img
    """
    # args checking
    if len(path) == 0:
      puts(colored.red("Error: path not provided"))
      return

    if not os.path.isfile(path):
      puts(colored.red("Error: image not found at path."))
      return

    self.current_img = path
    self.carver = sc.SeamCarver(path)
    puts(colored.white(f"Set {colored.green(path)} to be carved"))

  def do_set(self, sample_name):
    """
    Sets the specified image to be prepared for seam carving.

    Arguments:
        sample_name {str} -- the name of the image as seen in list_samples to be set for seam carving.
    """
    # arg checking
    if len(sample_name) == 0:
      puts(colored.red("Error: name not provided"))
      return
    if sample_name not in self.samples:
      puts(colored.red("Error: sample by that name not found"))
      return

    self.current_img = sample_name
    self.carver = sc.SeamCarver(f"samples/{sample_name}{self.samples[sample_name]}")
    puts(colored.white(f"Set {colored.green(sample_name)} to be carved"))

  def do_which(self, arg):
    """
    Returns the image that is currently set to be carved.
    """
    if self.current_img is None:
      puts(colored.white("No image set to be carved"))
    else:
      puts(colored.green(self.current_img) + colored.white(" set to be carved"))

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

  def do_show(self, arg):
    """
    Opens a window to display the selected image. Downloads a temporary image in out/ to show.

    Arguments:
        arg {str} -- if not set, displays the current set image.
        otherwise displays the image specified by path or by name.
    """
    if len(arg) == 0:
      self.do_show_carver("original")
      return
    
    path = None
    if arg in self.samples:
      path = f"samples/{arg}{self.samples[arg]}"
    elif os.path.isfile(arg):
      path = arg
    else:
      puts(colored.red("Error: arugment passed is not sample name or path to image"))
      return
    
    img = Image.open(path)
    img.show()

  def do_show_carver(self, args):
    """
    Displays the specified image of the SeamCarver. img at out/out.jpg will be overwritten.

    Arguments:
        type {str} -- must be one of sc.ImageType
    """
    # make sure we've set an image
    if self.carver is None:
      puts(colored.red(f"Error: image has not been selected with {colored.cyan('set')} {colored.red('or')} {colored.cyan('load')}"))
      return

    # arg checking
    args = args.split()
    if len(args) != 1:
      puts(colored.red("Error: bad syntax. Expected one arguments: type"))
      return

    sc_enum = self.enums.get(args[0].lower())
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
      puts(colored.red("Error: image has not been selected with 'set'"))
      return
    
    # arg checking
    args = args.split()
    if len(args) != 2:
      puts(colored.red("Error: bad syntax. Expected two arguments: type path"))
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