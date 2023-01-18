#include "text_helper.h"

void help_draw() {
  TEXT_HELPER_INIT();
  
  TEXT_HELPER_DRAW("MSED is organized into modes, switch between them using ", 255, 255, 255);
  TEXT_HELPER_DRAW("ESC", 255, 255, 0);
  TEXT_HELPER_DRAW(" followed by one of", 255, 255, 255);
  TEXT_HELPER_RETURN();
  TEXT_HELPER_DRAW("the keys:", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_SET_BASE(2);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("H", 255, 255, 0);
  TEXT_HELPER_DRAW(" for help", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("F", 255, 255, 0);
  TEXT_HELPER_DRAW(" for file selection", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("O", 255, 255, 0);
  TEXT_HELPER_DRAW(" for preview", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("S", 255, 255, 0);
  TEXT_HELPER_DRAW(" for selection", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("B", 255, 255, 0);
  TEXT_HELPER_DRAW(" for background setting", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("N", 255, 255, 0);
  TEXT_HELPER_DRAW(" for foreground setting", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("M", 255, 255, 0);
  TEXT_HELPER_DRAW(" for character setting", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("C", 255, 255, 0);
  TEXT_HELPER_DRAW(" for copy", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("P", 255, 255, 0);
  TEXT_HELPER_DRAW(" for paste", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("T", 255, 255, 0);
  TEXT_HELPER_DRAW(" for text entry", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_SET_BASE(1);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("and additionally, ", 255, 255, 255);
  TEXT_HELPER_DRAW("Q", 255, 255, 0);
  TEXT_HELPER_DRAW(" to quit.", 255, 255, 255);
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_RETURN();
  
  TEXT_HELPER_DRAW("For more information regarding each mode, view the ", 255, 255, 255);
  TEXT_HELPER_DRAW("USAGE.md", 255, 255, 0);
  TEXT_HELPER_DRAW(" file in the ", 255, 255, 255);
  TEXT_HELPER_DRAW("GitHub", 255, 255, 0);
  TEXT_HELPER_RETURN();
  TEXT_HELPER_DRAW("repository.", 255, 255, 255);
}
