/* Sea Defender on GFOS: GLU's polygon tessellator calls (amiga/include/
 * GL/glu.h) on libtess2. Contours are gathered as the game hands them in;
 * gluTessEndPolygon tessellates them with GLU's default odd winding rule
 * into triangles and gives them back through the game's callbacks: begin
 * (GL_TRIANGLES), a vertex call per corner (the corner's x, y, z as
 * doubles, kept until gluDeleteTess), end.
 * MIT, like Sea Defender. Copyright (c) 2026 AmiCradle Team. */
#include <stdlib.h>
#include <string.h>
#include <GL/glu.h>
#include "tesselator.h"

struct GLUtesselator {
	void (*begin)(GLenum);
	void (*vertex)(void *);
	void (*end)(void);
	void (*error)(GLenum);
	float *contour;               /* the contour being added, x y pairs */
	int ncontour, capcontour;
	double *out;                  /* the corners handed back */
	int nout;
	int contours;                 /* contours added to this polygon */
	TESStesselator *t;
};

GLUtesselator *gluNewTess(void)
{
	GLUtesselator *g = (GLUtesselator *)calloc(1, sizeof *g);

	if (g && !(g->t = tessNewTess(NULL))) {
		free(g);
		g = NULL;
	}
	return g;
}

void gluDeleteTess(GLUtesselator *g)
{
	if (!g)
		return;
	tessDeleteTess(g->t);
	free(g->contour);
	free(g->out);
	free(g);
}

void gluTessCallback(GLUtesselator *g, GLenum which, _GLUfuncptr fn)
{
	switch (which) {
	case GLU_TESS_BEGIN: g->begin = (void (*)(GLenum))fn; break;
	case GLU_TESS_VERTEX: g->vertex = (void (*)(void *))fn; break;
	case GLU_TESS_END: g->end = (void (*)(void))fn; break;
	case GLU_TESS_ERROR: g->error = (void (*)(GLenum))fn; break;
	default: break;               /* edge flags, combine: libtess2 does those */
	}
}

void gluTessBeginPolygon(GLUtesselator *g, GLvoid *data)
{
	(void)data;
	tessDeleteTess(g->t);
	g->t = tessNewTess(NULL);
	g->contours = 0;
}

void gluTessBeginContour(GLUtesselator *g)
{
	g->ncontour = 0;
}

void gluTessVertex(GLUtesselator *g, GLdouble *v, GLvoid *data)
{
	(void)data;
	if (g->ncontour == g->capcontour) {
		int cap = g->capcontour ? g->capcontour * 2 : 64;
		float *p = (float *)realloc(g->contour, cap * 2 * sizeof *p);

		if (!p)
			return;
		g->contour = p;
		g->capcontour = cap;
	}
	g->contour[g->ncontour * 2] = (float)v[0];
	g->contour[g->ncontour * 2 + 1] = (float)v[1];
	g->ncontour++;
}

void gluTessEndContour(GLUtesselator *g)
{
	if (g->t && g->ncontour >= 2) {
		tessAddContour(g->t, 2, g->contour, 2 * sizeof(float), g->ncontour);
		g->contours++;
	}
}

void gluTessEndPolygon(GLUtesselator *g)
{
	const TESSreal *verts;
	const TESSindex *elems;
	int nelems, i, k;

	if (!g->contours)
		return;               /* nothing to draw, as GLU does */
	if (!g->t || !tessTesselate(g->t, TESS_WINDING_ODD, TESS_POLYGONS, 3, 2, NULL)) {
		if (g->error)
			g->error(100151);      /* GLU_OUT_OF_MEMORY */
		return;
	}
	verts = tessGetVertices(g->t);
	elems = tessGetElements(g->t);
	nelems = tessGetElementCount(g->t);
	{
		double *p = (double *)realloc(g->out, (size_t)(g->nout + nelems * 3) * 3 * sizeof *p);

		if (!p)
			return;
		g->out = p;
	}
	if (g->begin && nelems)
		g->begin(GL_TRIANGLES);
	for (i = 0; i < nelems; i++) {
		for (k = 0; k < 3; k++) {
			TESSindex e = elems[i * 3 + k];
			double *c = g->out + (size_t)(g->nout++) * 3;

			c[0] = e == TESS_UNDEF ? 0 : verts[e * 2];
			c[1] = e == TESS_UNDEF ? 0 : verts[e * 2 + 1];
			c[2] = 0;
			if (g->vertex)
				g->vertex(c);
		}
	}
	if (g->end && nelems)
		g->end();
}
