# DoomBox

It's a Doom ~~source port~~ playing environment that requires the original DOS executables. If you're thinking that's a dumb idea, forget about it and check out:

* [GZDoom](https://zdoom.org/)
* [PrBoom+](http://prboom-plus.sourceforge.net/)
* [Chocolate Doom](https://www.chocolate-doom.org/)

## Compiling

This works on my machine (Windows 7 x64, Visual Studio 2015); send a pull-request if you figure out a better way to make this more useful for non-Windows users.

* DoomBox requires DOSBox (included in the repo).
	* DOSBox requires SDL 1.2 (SDL 2.0 is apparently not supported).
		* Download the source code for your platform from [the SDL website](https://www.libsdl.org/download-1.2.php).
		* Shove the contents of the `.zip`'s `include` directory into the repo's `SDL-1.2.15\include` directory.
		* SDL 1.2's development libraries don't work with VS2015, so you'll need to compile it yourself.
			* Change the active solution configuration to "Release".
			* Copy the following files to the repo's `visualc_net` directory:
				* `VisualC\SDLmain\Release\SDLmain.lib`
				* `VisualC\SDLmain\Release\SDLmain.pdb`
				* `VisualC\SDL\Release\SDL.dll`
				* `VisualC\SDL\Release\SDL.pdb`
				* `VisualC\Release\SDL.lib`
		* SDL 1.2 requires libpng:
			* Download the `.zip` from the "Source code" section of the [libpng website](http://www.libpng.org/pub/png/libpng.html).
			* Extract the `.zip` to a temporary directory, such that the temporary folder contains the `lpng1629` (or similar) folder.
			* Now download the `.zip` from the "Source code" section of the [zlib website](http://www.zlib.net/).
			* Extract the `.zip` to the same temporary directory, such that the temporary folder contains the `zlib-1.2.11` (or similar) folder.
				* Open `lpng1629\projects\vstudio\zlib.props` in a text editor and change the value of ZLibSrcDir to this new folder.
			* Copy the `.h` files from the `lpng1629` folder and the `zlib-1.2.11` folder to the repo's `include` directory.
			* Open `lpng1629\projects\vstudio\vstudio.sln` in Visual Studio.
				* Change the active solution configuration to "Release Library".
				* On the toolbar at the top of the screen, click `Build`, then `Configuration Manager...`.
				* Uncheck `pngstest` (**not `pngtest`**), `pngunknown`, and `pngvalid`.
				* Build the solution.
				* In the `lpng1629\projects\vstudio\Release Library\` folder, copy `zlib.lib` and `libpng16.lib` to the repo's `visualc_net` directory. Rename `libpng16.lib` to `libpng.lib`.
	* DOSBox requires SDL_Net.
		* Download the Binary `SDL_net-devel` `.zip` for your platform from [the SDL_Net webpage](https://www.libsdl.org/projects/SDL_net/release-1.2.html).
		* Shove the contents of the `.zip`'s `lib\x86` directory (assuming Windows x86) into the repo's `visualc_net` directory.
		* Shove the contents of the `.zip`'s `include` directory into the repo's `include` directory.
	* DOSBox requires Curses (even though it won't actually be used with the default settings).
		* [Download PDCurses here.](https://sourceforge.net/projects/pdcurses/files/pdcurses/3.4/pdc34dll.zip/download)
		* Copy the `.h` files from the `.zip` to the repo's `include` directory.
		* Copy the `.lib` and `.dll` files from the `.zip` to the repo's `visualc_net` directory, and rename them from `pdcurses.*` to `curses.*`.
	* Now DOSBox should compile and link correctly.
