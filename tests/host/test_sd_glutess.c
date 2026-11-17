/* Host test of amiga/sd_glutess.c (GLU's tessellator calls on libtess2):
 * shapes handed in the way Sea Defender's snoutlib/tesselate.cpp does,
 * checked for triangles only, the area they cover (odd winding, so a hole
 * is cut out), and corners that stay inside the shape's bounds.
 * MIT, Copyright (c) 2026 AmiCradle Team. */
#include <math.h>
#include <stdio.h>
#include <GL/glu.h>

static int fails, begins, ends, errors;
static GLenum last_type;
static double tri[3][2], area, minx, miny, maxx, maxy;
static int ncorner, outside;

static void cb_begin(GLenum type) { begins++; last_type = type; }
static void cb_end(void) { ends++; }
static void cb_error(GLenum e) { (void)e; errors++; }
static void cb_vertex(double *v)
{
	if (v[0] < minx - 1e-4 || v[0] > maxx + 1e-4 || v[1] < miny - 1e-4 || v[1] > maxy + 1e-4)
		outside++;
	tri[ncorner % 3][0] = v[0];
	tri[ncorner % 3][1] = v[1];
	if (++ncorner % 3 == 0)
		area += fabs((tri[1][0] - tri[0][0]) * (tri[2][1] - tri[0][1]) -
			(tri[2][0] - tri[0][0]) * (tri[1][1] - tri[0][1])) / 2;
}

static void check(int ok, const char *what)
{
	printf("%s: %s\n", ok ? "PASS" : "FAIL", what);
	if (!ok)
		fails++;
}

/* contours: n of them, each counts[i] x y pairs */
static void run(int n, const int *counts, double *pts, double bx0, double by0, double bx1, double by1)
{
	GLUtesselator *t = gluNewTess();
	int c, i, p = 0;

	begins = ends = errors = ncorner = outside = 0;
	area = 0;
	minx = bx0; miny = by0; maxx = bx1; maxy = by1;
	gluTessCallback(t, GLU_TESS_BEGIN, (_GLUfuncptr)cb_begin);
	gluTessCallback(t, GLU_TESS_END, (_GLUfuncptr)cb_end);
	gluTessCallback(t, GLU_TESS_ERROR, (_GLUfuncptr)cb_error);
	gluTessCallback(t, GLU_TESS_VERTEX, (_GLUfuncptr)cb_vertex);
	gluTessBeginPolygon(t, NULL);
	for (c = 0; c < n; c++) {
		gluTessBeginContour(t);
		for (i = 0; i < counts[c]; i++, p++) {
			pts[p * 3 + 2] = 0;
			gluTessVertex(t, &pts[p * 3], &pts[p * 3]);
		}
		gluTessEndContour(t);
	}
	gluTessEndPolygon(t);
	gluDeleteTess(t);
}

int main(void)
{
	/* a unit square: two triangles, area 1 */
	double sq[] = { 0,0,0, 1,0,0, 1,1,0, 0,1,0 };
	int sqn[] = { 4 };
	/* a 4 x 4 square with a 2 x 2 hole (both wound the same way): area 12 */
	double hole[] = { 0,0,0, 4,0,0, 4,4,0, 0,4,0,  1,1,0, 3,1,0, 3,3,0, 1,3,0 };
	int holen[] = { 4, 4 };
	/* a concave L: area 3 */
	double ell[] = { 0,0,0, 2,0,0, 2,1,0, 1,1,0, 1,2,0, 0,2,0 };
	int elln[] = { 6 };
	/* a circle of 64 points, radius 1: area close to pi */
	double circ[64 * 3];
	int circn[] = { 64 }, i;
	/* a bow tie (self-crossing): odd winding gives both lobes, area 2 x 0.25 */
	double bow[] = { 0,0,0, 1,1,0, 1,0,0, 0,1,0 };
	int bown[] = { 4 };

	run(1, sqn, sq, 0, 0, 1, 1);
	check(begins == 1 && ends == 1 && last_type == GL_TRIANGLES, "square: one GL_TRIANGLES begin and end");
	check(ncorner == 6 && fabs(area - 1) < 1e-4 && !outside && !errors, "square: 2 triangles, area 1");

	run(2, holen, hole, 0, 0, 4, 4);
	check(ncorner % 3 == 0 && fabs(area - 12) < 1e-3 && !outside, "square with a hole: area 12 (odd winding)");

	run(1, elln, ell, 0, 0, 2, 2);
	check(ncorner == 12 && fabs(area - 3) < 1e-4 && !outside, "concave L: 4 triangles, area 3");

	for (i = 0; i < 64; i++) {
		circ[i * 3] = cos(i * 2 * M_PI / 64);
		circ[i * 3 + 1] = sin(i * 2 * M_PI / 64);
	}
	run(1, circn, circ, -1, -1, 1, 1);
	check(ncorner == 62 * 3 && fabs(area - 32 * sin(2 * M_PI / 64)) < 1e-3, "64-point circle: 62 triangles, its area");

	run(1, bown, bow, 0, 0, 1, 1);
	check(fabs(area - 0.5) < 1e-4 && !outside, "bow tie: both lobes, area 0.5 (the crossing point added)");

	run(0, NULL, NULL, 0, 0, 0, 0);
	check(ncorner == 0 && begins == 0 && !errors, "no contours: no triangles, no error");

	if (fails)
		printf("FAILED %d\n", fails);
	else
		printf("tessellator: all checks pass\n");
	return fails != 0;
}
