#include <stdio.h>
#include <stdlib.h>

#include "scbloader.h"

int main(int argc, char **argv) {
    printf("scb2obj v1.0 aka 'eat my dust' - by nddrylliog, thanks to DragonGeo2\n");

    if(argc < 2) {
        printf("\nUsage: scb2obj FILE.scb\n");
        exit(1);
    }

    SCB model = scb_load(argv[1]);

    printf("magic = %s, version = %u.%u, object name = %s\n", model->magic, model->minorVer, model->majorVer, model->object_name);
    printf("%u vertices, %u faces, %u.. sections?\n", model->nVerts, model->nFaces, model->unk);
    printf("center: (%.2f, %.2f, %.2f), extents: (%.2f, %.2f, %.2f)\n", model->center[0], model->center[0], model->center[0], model->extents[0], model->extents[0], model->extents[0]);

    int i;

    float vertFactor = 1.0f / (float) model->nVerts;
    float3 realCenter = { 0.0f, 0.0f, 0.0f };

    for(i = 0; i < model->nVerts; i++)
    {
        realCenter[0] += model->verts[i][0] * vertFactor;
        realCenter[1] += model->verts[i][1] * vertFactor;
        realCenter[2] += model->verts[i][2] * vertFactor;
    }
    printf("real center: (%.2f, %.2f, %.2f)\n", realCenter[0], realCenter[1], realCenter[1]);
    for(i = 0; i < model->nVerts; i++)
    {
        model->verts[i][0] -= realCenter[0];
        model->verts[i][1] -= realCenter[1];
        model->verts[i][2] -= realCenter[2];
    }

    for(i = 0; i < /* model->nVerts */ 10; i++)
    {
        printf("vertex #%d (%.2f, %.2f, %.2f)\n",
               i,
               model->verts[i][0],
               model->verts[i][1],
               model->verts[i][2]);
    }

    for(i = 0; i < /* model->nFaces */ 10; i++)
    {
        face f = model->faceslist[i];
        printf("face #%d (%u, %u, %u), material %s\n",
               i,
               f.indices[0],
               f.indices[1],
               f.indices[2],
               f.materialname);
    }

    obj_save_from_scb(model);

    free(model);

    return 0;
}
