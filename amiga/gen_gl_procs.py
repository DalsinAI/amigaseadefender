#!/usr/bin/env python3
"""Writes sd_gl_procs.c: sd_gl_proc(name), the address of each GL call Sea
Defender asks for by name (glfwGetProcAddress("glX") in its sources) that
GFOpenGPU's libGL has, from that libGL's symbol list (m68k-amigaos-nm). An
EXT or ARB name libGL lacks takes the core call of the same name without
the suffix, when libGL has that (GL 3.0 made the framebuffer and buffer
extensions core with the same entry points). Calls libGL lacks at all give
NULL, as a driver's GetProcAddress does.

    gen_gl_procs.py NM_OUTPUT OUT_C SOURCE...

MIT, like Sea Defender. Copyright (c) 2026 AmiCradle Team."""
import re
import sys

SUFFIXES = ('EXT', 'ARB')


def wanted_names(texts):
    names = set()
    for t in texts:
        names.update(re.findall(r'glfwGetProcAddress\(\s*"(gl[A-Za-z0-9_]+)"\s*\)', t))
    return sorted(names)


def lib_names(nm_text):
    """Defined text symbols in nm output, with the a.out underscore taken off."""
    out = set()
    for line in nm_text.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in ('T', 'W'):
            name = parts[2]
            if name.startswith('_gl'):
                name = name[1:]
            if name.startswith('gl'):
                out.add(name)
    return out


def resolve(names, lib):
    """[(asked name, libGL symbol)] for every name libGL can answer."""
    pairs = []
    for n in names:
        if n in lib:
            pairs.append((n, n))
            continue
        for suf in SUFFIXES:
            if n.endswith(suf) and n[:-len(suf)] in lib:
                pairs.append((n, n[:-len(suf)]))
                break
    return pairs


def generate(names, nm_text):
    pairs = resolve(names, lib_names(nm_text))
    lines = ['/* Written by amiga/gen_gl_procs.py: do not edit. */',
             '#include <string.h>', '#include <stdlib.h>', '']
    for sym in sorted(set(s for _, s in pairs)):
        lines.append('extern char %s[];' % sym)
    lines += ['', 'struct sd_gl_entry { const char *name; void *fn; };', '',
              'static const struct sd_gl_entry sd_gl_procs[] = {']
    for n, sym in pairs:
        lines.append('\t{ "%s", (void *)%s },' % (n, sym))
    lines += ['};', '',
              'static int sd_gl_cmp(const void *k, const void *e)', '{',
              '\treturn strcmp((const char *)k, ((const struct sd_gl_entry *)e)->name);', '}', '',
              'void *sd_gl_proc(const char *name)', '{',
              '\tconst struct sd_gl_entry *e = bsearch(name, sd_gl_procs,',
              '\t\tsizeof(sd_gl_procs) / sizeof(sd_gl_procs[0]), sizeof(sd_gl_procs[0]), sd_gl_cmp);',
              '\treturn e ? e->fn : NULL;', '}', '',
              'int sd_gl_proc_count = %d;' % len(pairs), '']
    return '\n'.join(lines), pairs


def main():
    nm, out = sys.argv[1:3]
    texts = [open(p, encoding='latin-1').read() for p in sys.argv[3:]]
    names = wanted_names(texts)
    text, pairs = generate(names, open(nm).read())
    if not pairs:
        sys.exit('gen_gl_procs: libGL has none of the GL calls the game asks for')
    open(out, 'w').write(text)
    missing = sorted(set(names) - set(n for n, _ in pairs))
    print('%s: %d of %d GL calls%s' % (out, len(pairs), len(names),
          (' (not in libGL: %s)' % ' '.join(missing)) if missing else ''))


if __name__ == '__main__':
    main()
