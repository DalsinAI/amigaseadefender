# Sea Defender for GFOS

Sea Defender, Jan Dvorak's 2010 vector missile-defence game, for
AmigaOS 3.2 with GFOpenUp (68040/68060 with an FPU), on GFOpenUp's SDL 2
and OpenGL. See [AMIGA_PORT.md](AMIGA_PORT.md) for what the port changes,
and [amiga/Licences.txt](amiga/Licences.txt) for the licences.

Upstream Sea Defender and libtess2 are not kept here: `package/build-amiga.sh`
fetches them at the commits in `package/seadefender.lock`.

- `amiga/`: the GLFW and GLU interfaces on SDL 2 and libtess2, the GL
  call table generator, the patch, the icons, Controls and the Installer spec.
- `package/`: the build and package scripts (Cradle stove_build, target `package`).
- `tests/host/`: host tests (`sh tests/host/test_all.sh`).
- `tools/`: the LhA packer.

MIT, Copyright (c) 2026 AmiCradle Team.
Designed, tested and delivered by The AmiCradle Team.
