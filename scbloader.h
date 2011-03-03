#ifndef SCBLOADER_H_INCLUDED
#define SCBLOADER_H_INCLUDED

#include "scbformat.h"

SCB scb_load(char *path);
void obj_save_from_scb(SCB model);

#endif // SCBLOADER_H_INCLUDED
