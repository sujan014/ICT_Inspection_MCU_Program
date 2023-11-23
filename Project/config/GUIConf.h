#ifndef GUICONF_H
#define GUICONF_H

#include "main.h"

#define GUI_NUM_LAYERS            2    // Maximum number of available layers

#ifdef OS_SUPPORT
 #define GUI_OS                    (1)  // Compile with multitasking support
#else
 #define GUI_OS                    (0)
#endif

#ifndef   GUI_SUPPORT_TOUCH
  #define GUI_SUPPORT_TOUCH       (1)  // Support touchscreen
#endif

#define GUI_DEFAULT_FONT          &GUI_Font6x8

#define GUI_SUPPORT_MOUSE             (1)    /* Support a mouse */
#define GUI_WINSUPPORT                (1)    /* Use window manager */
#define GUI_SUPPORT_MEMDEV            (1)    /* Memory device package available */
#define GUI_SUPPORT_DEVICES           (1)    /* Enable use of device pointers */

#endif  /* Avoid multiple inclusion */
