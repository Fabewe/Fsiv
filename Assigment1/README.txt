This program is made to higlight part of an original image given by the user, in the shape of a rectangle.

The program executable is in the /build directory.
Cmakelist and Makefiles are given to make the user able to change them.


The program is meant to be executed with only one argument, the path to the image.

    ./ForegroundHiglight <path to the image>

It is given a /resources diretory which contains diferents images to try the program, nevertheless the program can be executed with any image passed through argument by the user.

Once it is executed, a windows with the image will be opened, the user will have to click one point on the window which will be one of the corner, then the opposite corner must be clicked.Te area that the rectangle is selecting will be displayed over the image.When the second corner is clicked a window will be opened with final result.

If the user wants to repeat their selection,they must press the 'ESCAPE' key and will be able to make again their selection(the "Result" window can be closed whenever the user wants, a new one will be opened when another selection is made).
