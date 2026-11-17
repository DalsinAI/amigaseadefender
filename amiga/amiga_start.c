/* The Amiga port's start: the stack, the version string, and the drawer
 * as the current directory (the hiscores and settings are kept beside the
 * game), set before main().
 * MIT, like Sea Defender. Copyright (c) 2026 AmiCradle Team. */
#include <unistd.h>

/* A Shell gives 4 KB of stack; the SVG tessellator recurses. libnix's
 * startup sets this much up. */
unsigned long __stack = 512 * 1024;
static const char version[] __attribute__((used)) =
    "$VER: SeaDefender 1.0 (17.11.2026)";

static void __attribute__((constructor)) amiga_drawer_dir(void)
{
    chdir("PROGDIR:");
}
