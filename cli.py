from clint.textui import colored, puts, indent
import cmd, sys, os

class SeamCarvingShell(cmd.Cmd):
  intro = colored.red("Seam Carving Shell (SCS)\nType 'help' or '?' to list commands.\n")
  noop = colored.yellow("This command hasn't been implemented yet.")

  def __init__(self):
    cmd.Cmd.__init__(self)
    self.prompt = colored.blue(">>> ")
    self.sample_imgs = self.load_samples("samples/")

  # region - helpers

  def load_samples(self, path: str) -> list[str]:
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

  def do_set(self, img):
    """
    Sets the specified image to be prepared for seam carving.

    Arguments:
        img_name {str} -- the name and extension of the image to be set for seam carving.
    """
    puts(self.noop)

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
    Opens a window to display the selected image.
    Close the image with 'esc'. Save the image with 's'.
    """
    puts(self.noop)

  def do_showres(self, arg):
    """
    Displays the result of the seam carving operation.
    """
    puts(self.noop)

  # endregion

if __name__ == "__main__":
  SeamCarvingShell().cmdloop()

# colors available
# red, green, blue, yellow, black, magenta, cyan, white