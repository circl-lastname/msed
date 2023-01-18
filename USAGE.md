# Usage
Basic usage is present in MSED's Welcome and Help modes, this is a detailed list on how to use each mode.

## `F` - File selection
Use `TAB` to switch focus between the textbox, directory view, and buttons at the bottom.

The textbox allows standard text editing and `CTRL`+`V` to paste. It specifies either the directory to view or file to save/load.

The directory view shows the directory as specified by the textbox. **It will show `Not a directory` if a file is specified, this does not affect the buttons.**

The following keys can be used to browse the directory:

* Up and down arrows - Select previous or next entry.
* `PGUP` and `PGDN` - Scroll 10 entries at a time.
* `HOME` and `END` - Move to either the beggining or end of directory.
* `ENTER` - Enter selected file or directory. This will automatically update the textbox.

The buttons will either save or load the file specified by the textbox, they can be selected using the arrow keys. If an error occurs it will be displayed in the top right corner.

## `O` - Preview
A safe mode without selection highlighting and bound keys.

## `S` - Select
Allows selecting area which modes edit. Move using the arrow keys, pressing `ENTER` will switch between setting the start point and width/height of the selection.

## `B` - Background
Change background of selection using `-` and `=`.

## `N` - Foreground
Change foreground (text color) of selection using `-` and `=`.

## `M` - Character
Change the character of selection using `-` and `=`. This will replace any text in the selection.

## `C` - Copy
Press `ENTER` to copy the selection, move using the arrow keys.

## `P` - Paste
Press `ENTER` to paste the copied selection, move using the arrow keys.

## `T` - Text entry
Allows entering plain text, `BACKSPACE` and `DELETE` replace characters with a space, `ENTER` moves 1 line down and returns to the X position at which the mode was activated.
