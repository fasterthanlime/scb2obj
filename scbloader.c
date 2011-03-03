
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scbloader.h"

SCB scb_load(char *path)
{

    SCB scb = calloc(1, sizeof(struct SCB));

    scb->path = path;
    FILE *file = fopen(path, "rb");

    fread(scb->magic, 1, 8, file);
    fread(&(scb->majorVer), 1, sizeof(unsigned short), file);
    fread(&(scb->minorVer), 1, sizeof(unsigned short), file);
    fread(scb->object_name, 1, 128, file);

    fread(&(scb->nVerts), 1, sizeof(unsigned), file);
    fread(&(scb->nFaces), 1, sizeof(unsigned), file);
    fread(&(scb->unk),    1, sizeof(unsigned), file);

    fread(&(scb->center),  1, sizeof(float3), file);
    fread(&(scb->extents), 1, sizeof(float3), file);

    printf("Reading %d vertices (of size %d)\n", scb->nVerts, sizeof(float3));
    scb->verts =   calloc(scb->nVerts, sizeof(float3));
    fread(scb->verts,     scb->nVerts, sizeof(float3), file);
    fseek(file, sizeof(float3), SEEK_CUR); // apparently we need to skip one in order to get meaningful data...

    long int pos = ftell(file);
    printf("At offset %X\n", pos);

    printf("Reading %d faces (of size %d)\n", scb->nFaces, sizeof(face));
    scb->faceslist = calloc(scb->nFaces, sizeof(face));
    fread(scb->faceslist,   scb->nFaces, sizeof(face), file);

    fclose(file);
    return scb;

}

void obj_save_from_scb(SCB model)
{

    char* path = calloc(1, strlen(model->path) + 4);
    sprintf(path, "%s.obj", model->path);

    printf("Saving to %s\n", path);
    FILE *file = fopen(path, "wb");

    fprintf(file, "# Converted from .scb model '%s'\n", model->object_name);

    fprintf(file, "\n# Vertices\n", model->object_name);

    int i;
    for(i = 0; i < model->nVerts; i++)
    {
        fprintf(file, "v %.2f %.2f %.2f\n", model->verts[i][0], model->verts[i][1], model->verts[i][2]);
    }

    fprintf(file, "\n# Faces\n", model->object_name);

    for(i = 0; i < model->nFaces; i++)
    {
        face f = model->faceslist[i];
        fprintf(file, "f %d %d %d\n", f.indices[0] + 1, f.indices[1] + 1, f.indices[2] + 1);
    }

    fclose(file);

}

