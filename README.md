Libunr Project - Editor

libunr-editor is an open-source GUI editor for both the libunr project and Unreal Engine 1.x series of games.
The editor is primarily used for developing environments and working with asset formats for the engine.

Dependencies:

libunr-editor requires libunr (included with openue solution), and wxWidgets (not included).
To ready wxWidgets for use with libunr-editor, download the source archive from https://www.wxwidgets.org/downloads/
Extract and rename so that the top-level directory is named "wxWidgets" and is located in libunr-editor/deps/.
Build wxWidgets using the matching target (Release/Debug) in lib mode and matching architecture (x86 or x64). 
The Project should find the files from there.

Goals

Primarily to have an open-source editor of capable of utilizing new libunr features, as well as supporting the old 
engines, with a less buggy UI and better bsp building.

Licensing

GNU General Public License v3
