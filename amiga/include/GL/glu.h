/* Sea Defender on GFOS: GFOpenGPU has no GLU. Sea Defender uses GLU's
 * polygon tessellator (for its font and its ship outlines), declared here
 * and given in amiga/sd_glutess.c on libtess2 (the SGI tessellator GLU's
 * own is, as a library). MIT, like Sea Defender.
 * Copyright (c) 2026 AmiCradle Team. */
#ifndef SD_AMIGA_GLU_H
#define SD_AMIGA_GLU_H
#include <GL/gl.h>
#ifdef __cplusplus
extern "C" {
#endif
#ifndef APIENTRY
#define APIENTRY
#endif

typedef struct GLUtesselator GLUtesselator;
typedef void (APIENTRY *_GLUfuncptr)(void);

#define GLU_TESS_BEGIN 100100
#define GLU_TESS_VERTEX 100101
#define GLU_TESS_END 100102
#define GLU_TESS_ERROR 100103
#define GLU_TESS_EDGE_FLAG 100104
#define GLU_TESS_COMBINE 100105

GLUtesselator *gluNewTess(void);
void gluDeleteTess(GLUtesselator *tess);
void gluTessCallback(GLUtesselator *tess, GLenum which, _GLUfuncptr fn);
void gluTessBeginPolygon(GLUtesselator *tess, GLvoid *data);
void gluTessBeginContour(GLUtesselator *tess);
void gluTessVertex(GLUtesselator *tess, GLdouble *location, GLvoid *data);
void gluTessEndContour(GLUtesselator *tess);
void gluTessEndPolygon(GLUtesselator *tess);

#ifdef __cplusplus
}
#endif
#endif
