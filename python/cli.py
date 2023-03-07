"""
Andy Mina

The Seam Carving Shell cli.
"""
from PIL import Image
from clint.textui import colored, puts, indent
import cmd, sys, os
import seam_carving as sc

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
            path {`str`} -- the path to the directory containing sample images.

        Returns:
            `list[str]` -- a list containing the names of each image in sample images.
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

    def do_clear(self, args):
        """
        Clears the screen.
        """
        if os.name == "nt":
            os.system("cls")
        elif os.name == "posix":
            os.system("clear")

    def do_list_enums(self, args):
        """
        List all of the enums available.
        """
        puts(f"\nUsed with {colored.cyan('carver')} to display the corresponding image\n")
        descs = [
            ": the original image supplied to the carver",
            ": the result image after performing seam operations (highlight, remove, insert, carve)",
            ": the transposed result image; stored for internal computations",
            ": the energy of the result image calculated using the 3x3 Sobel edge detector",
            ": the vertical energy map of the result image",
            ": the horizontal energymap of the result image"
        ]
        enums = [colored.magenta(e) for e in list(self.enums.keys())[:-2]]

        for idx in range(len(enums)):
            puts(enums[idx] + descs[idx])

        print("\n============================")

        puts(f"\nUsed with seam operations ({colored.cyan('highlight_seams')}, {colored.cyan('remove_seams')}, {colored.cyan('insert_seams')}) to specify the direction of the target seam\n")
        puts(f"{colored.magenta('vert')}: seam that runs vertically across the image; corresponds to a col")
        puts(f"{colored.magenta('horz')}: seam that runs horizontally across the image; corresponds to a row\n")
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
        puts(f"Set {colored.green(path)} to be carved")

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
        path = f"samples/{sample_name}{self.samples[sample_name]}"
        self.carver = sc.SeamCarver(path)
        puts(f"Set {colored.green(sample_name)} to be carved")

    def do_which(self, arg):
        """
        Returns the image that is currently set to be carved.
        """
        if self.current_img is None:
            puts("No image set to be carved")
        else:
            puts(f"{colored.green(self.current_img)} set to be carved")

    # endregion

    # region - seam carving

    def do_size(self, args):
        """
        Prints the size of the currently selected image
        """
        puts(f"{colored.green(self.current_img)}: {self.carver.rows()} x {self.carver.cols()}")

    def do_reset(self, args):
        """
        Resets the seam carver back to its original set image.
        """
        if self.carver is None:
            puts(colored.red("Error: no image set prior"))
        self.carver.reset()

    def do_carve(self, args):
        """
        Carves the set image to the specified size. Can be used to remove or insert seams.

        Arguments:
            target_rows {int} -- the new number of rows in the image; can't be 0
            target_cols {int} -- the new number of cols in the image; can't be 0
        """
        # make sure we've set an image
        if self.carver is None:
            puts(colored.red(f"Error: image has not been selected with {colored.cyan('set')} {colored.red('or')} {colored.cyan('load')}"))
            return

        # args checking
        args = args.split()
        if len(args) != 2:
            puts(colored.red("Error: bad syntax. Expected two arguments: {width} {height}"))
            return

        target_rows, target_cols = None, None
        try:
            target_rows, target_cols = int(args[0]), int(args[1])
        except ValueError:
            puts(colored.red("Error: can't convert target_rows and target_cols values to ints"))
            return

        if target_rows == 0 or target_cols == 0:
            puts(colored.red("Error: can't have size 0 in any axis"))
            return

        # logic
        puts(f"Carving {colored.green(self.current_img)}...")
        self.carver.carve(target_rows, target_cols)
        puts(f"Carved!")

    def do_highlight_seams(self, args):
        """
        Highlight the optimal seam in the selected image

        Arguments:
            dir {str} -- the direction to find the optimal seam in. Must be one of ["vert", "horz"]
            k {int} -- optional; the number of seams to highlight, defaults to one
            rgb {int} -- optional; the rgb to highlight in space separated. defaults to (176, 38, 255)
        """
        # make sure we've set an image
        if self.carver is None:
            puts(colored.red(f"Error: image has not been selected with {colored.cyan('set')} {colored.red('or')} {colored.cyan('load')}"))
            return

        # args checking
        args = args.split()
        if len(args) < 1:
            puts(colored.red("Error: bad syntax. Expected at least one arguments: {dir} {k} {r} {g} {b}"))
            return

        dir_ = self.enums.get(args[0])
        if dir_ is None:
            puts(colored.red("Error: dir must be 'vert' or 'horz'"))
            return

        k = 1 if len(args) < 2 else args[1]
        r, g, b = [None] * 3 if len(args) < 5 else args[2:]
        try:
            k = int(k)
            if r is not None:
                r, g, b = int(r), int(g), int(b)
        except:
            puts(colored.red("Error: failed to convert {k} to int"))
            return

        seams = self.carver.find_k_seams(k, dir_)
        if r is not None:
            self.carver.highlight_k_seams(seams, r, g, b)
        else:
            self.carver.highlight_k_seams(seams)

    def do_remove_seams(self, args):
        """
        Removes one seam in the specified direction

        Arguments:
            dir {str} -- the direction to remove one seam in
            k {int} -- optional; the number of seams to remove. defaults to 1
        """
        # make sure we've set an image
        if self.carver is None:
            puts(colored.red(f"Error: image has not been selected with {colored.cyan('set')} {colored.red('or')} {colored.cyan('load')}"))
            return

        # args checking
        args = args.split()
        if len(args) == 0:
            puts(colored.red("Error: bad syntax. Expected two arguments: {dir} {k}"))
            return

        dir_ = self.enums.get(args[0])
        if dir_ is None:
            puts(colored.red("Error: dir must be 'vert' or 'horz'"))
            return

        k = 1 if len(args) != 1 else args[1]
        try:
            k = int(k)
        except:
            puts(colored.red("Error: failed to convert {k} to int"))
            return

        # logic
        for i in range(k):
            self.carver.remove_seam(dir_)

    def do_insert_seams(self, args):
        """
        Inserts one seam in the specified direction

        Arguments:
            dir {str} -- the direction to insert one seam in
            k {int} -- optional; the number of seams to insert. defaults to 1
        """
        # make sure we've set an image
        if self.carver is None:
            puts(colored.red(f"Error: image has not been selected with {colored.cyan('set')} {colored.red('or')} {colored.cyan('load')}"))
            return

        # args checking
        args = args.split()
        if len(args) == 0:
            puts(colored.red("Error: bad syntax. Expected two arguments: {dir} {k}"))
            return

        dir_ = self.enums.get(args[0])
        if dir_ is None:
            puts(colored.red("Error: dir must be 'vert' or 'horz'"))
            return

        k = 1 if len(args) != 1 else args[1]
        try:
            k = int(k)
        except:
            puts(colored.red("Error: failed to convert {k} to int"))
            return

        if k == 1:
            self.carver.insert_seam(dir_)
        else:
            self.carver.insert_k_seams(dir_, k)

    # endregion

    # region - display

    def do_show(self, arg):
        """
        Opens a window to display the selected image. Downloads a temporary image in out/ to show.

        Arguments:
            arg {str} -- if not set, displays the result of the current image set for carving.
            otherwise displays the image specified by path or by name.
        """
        if len(arg) == 0:
            self.do_carver("result")
            return

        path = None
        if arg in self.samples:
            path = f"samples/{arg}{self.samples[arg]}"
        elif os.path.isfile(arg):
            path = arg
        else:
            puts(colored.red("Error: argument passed is not sample name or path to image"))
            return

        img = Image.open(path)
        img.show()

    def do_carver(self, args):
        """
        Displays the specified image of the SeamCarver. img at out.jpg will be overwritten.

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
            puts(colored.red("Error: bad syntax. Expected one arguments: {type}"))
            return

        sc_enum = self.enums.get(args[0].lower())
        if sc_enum is None:
            puts(colored.red("Error: type must be one of sc.ImageType"))

        path = "out.jpg"
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
            puts(colored.red("Error: image has not been selected with ") + colored.cyan("set"))
            return

        # arg checking
        args = args.split()
        if len(args) != 2:
            puts(colored.red("Error: bad syntax. Expected two arguments: {type} {path}"))
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