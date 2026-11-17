Sea Defender for GFOS
=====================

Sea Defender, Jan Dvorak's 2010 missile-defence game in 96 KB: two
warships, missiles raining from the sky, and your own missiles to burst
them before they hit. Everything is drawn from vector curves with GLSL
shaders. For AmigaOS 3.2 with GFOpenUp, on a 68040 or 68060 with an FPU.

Running it
----------

- Double-click SeaDefender. It opens a 640 x 400 screen through
  GFOpenUp's SDL 2 and OpenGL (LIBS:GFOpenGPU/SDL2.module, GL.module),
  which is where the drawing goes: to the host's GPU on AmiCradle. It
  needs GLSL shaders, which GFOpenGPU has.
- The resolution, full screen or window, and the mouse speed are in the
  game's Options menu, kept in sea.cfg.
- Controls: see Controls.

What the port changes
---------------------

- GLFW: Sea Defender is written on GLFW 2.7. amiga/sd_glfw.c is our own
  version of the GLFW calls it makes (window and GL context, keys, mouse,
  timer, video modes) on SDL 2.
- GLU: the game tessellates its curves with GLU's tessellator.
  amiga/sd_glutess.c is our own GLU tessellator interface (gluNewTess,
  gluTessCallback, gluTessBeginPolygon and the rest the game calls) on
  libtess2, the same SGI tessellator GLU's is, fetched at build time.
- GL extensions: the game looks up its framebuffer, shader and buffer
  calls by name. amiga/gen_gl_procs.py writes sd_gl_procs.c from the names
  the game asks for and the symbols GFOpenGPU's libGL has, mapping an EXT
  or ARB name to the core call where libGL only has that.
- amiga/patches/0001-amigaos.patch:
  - snoutlib/staticmesh.cpp, snoutlib/mfont.cpp: the built-in models and
    font store 16-bit little-endian numbers; they are read byte by byte,
    so the shapes come out right on the 68k too;
  - snoutlib/settings.cpp: 640 x 400 full screen by default (upstream
    1024 x 768 in a window).
- The drawer is the current directory (amiga/amiga_start.c), so sea.hsc
  and sea.cfg are kept beside the game, and the stack is 512 KB.

Building
--------

package/build-amiga.sh on a Cradle stove (os32-gcc16-open6) fetches
upstream and libtess2 at the commits package/seadefender.lock names,
builds the game, and writes the LhA archive and the GFOpenUp part. The
host tests are in tests/host.

Designed, tested and delivered by The AmiCradle Team.
