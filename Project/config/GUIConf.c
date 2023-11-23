#include "main.h"

#define GUI_NUMBYTES2  (1024) *  1024    // x KByt
U32 extMem[GUI_NUMBYTES2/4] __attribute__((at(0xD0100000)));
                                              
void GUI_X_Config(void)
{
    GUI_ALLOC_AssignMemory(extMem, GUI_NUMBYTES2);
}
/*************************** End of file ****************************/
