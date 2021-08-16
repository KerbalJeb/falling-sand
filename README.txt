README

Build/Installation notes:
    - If CMake fails to find GLM, it will download the header files from GitHub
        This may make the generation stage take slightly longer than usual
    - The software will run significantly faster when build as a release
        and may lag with a large number of particles in debug
    - After installation, there should be a bin, resources and example_saves folder in the installation director
        The bin folder with contain the executable and bash script
        The resources folder contains images and shader code files
        The example_saves folder contains some example save files that demonstrate many of the simulation rules

    - Running the demo bash script should result in a GUI window titled 'Falling Sand' appearing.

User controls
    Left Mouse Button        : Place currently selected element on screen
    Right Mouse Button       : Delete elements at the cursors current position
    Delete                   : Clear all elements from the screen (reset the simulation)
    Left square bracket ([)  : Decrease brush size
    Right square bracket (]) : Increase brush size
    Space Bar                : Toggle pausing/playing the simulation
    Ctrl-S                   : Saves the current simulation to a file in the cwd,
                                with a name of: falling-sand-n.sav where n where n is incremented for each save
                                to prevent overwriting existing files.
                                "Saving..." will be printed to the console
    Dropping file on window  : Save file will be loaded
                                "Loaded Save" or "Invalid Save File" will be printed to the
                                console to indicate if the file was successfully loaded