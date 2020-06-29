# MPEG-2 TS Video Decoder
**Created by Ailin Saggau-Lyons and Bhavesh Singhal under the supervision of Robert Xiao for CPSC 448B.**

This is a simple MPEG-2 TS video decoder that works with a single TS stream video file. The decoder allows the user to
set a destination folder where they can store all the decoded frames of a desired input file.

To compile, run the cmake program with the CMakeLists.txt in the project-code directory.

### Bugs/Problems

- Test suite is not perfect and does not cover all the essential elements of the project
- One frame from the end of the video file is missing for some test files
- Threading is implemented crudly; should be changed to a worker based model
- Creating 2 separate CMake profiles - Debug and Release
