#!/usr/bin/env python3
"""Unit tests of amiga/gen_gl_procs.py. MIT, Copyright (c) 2026 AmiCradle Team."""
import os
import sys
sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'amiga'))
import gen_gl_procs as g  # noqa: E402

NM = """
00000010 T _glBindBuffer
00000020 T _glGenFramebuffersEXT
00000030 W _glBindFramebuffer
00000040 U _glMissing
00000050 T _SDL_Init
"""
SRC = '''
a = (X)(glfwGetProcAddress("glBindBufferARB"));
b = (X)(glfwGetProcAddress( "glGenFramebuffersEXT" ));
c = (X)(glfwGetProcAddress("glBindFramebufferEXT"));
d = (X)(glfwGetProcAddress("glMissing"));
e = (X)(glfwGetProcAddress("glBindBuffer"));
'''
ok = 0
names = g.wanted_names([SRC])
assert names == ['glBindBuffer', 'glBindBufferARB', 'glBindFramebufferEXT', 'glGenFramebuffersEXT', 'glMissing'], names
ok += 1
lib = g.lib_names(NM)
assert lib == {'glBindBuffer', 'glGenFramebuffersEXT', 'glBindFramebuffer'}, lib
ok += 1
pairs = dict(g.resolve(names, lib))
assert pairs == {'glBindBuffer': 'glBindBuffer', 'glBindBufferARB': 'glBindBuffer',
                 'glBindFramebufferEXT': 'glBindFramebuffer',
                 'glGenFramebuffersEXT': 'glGenFramebuffersEXT'}, pairs
ok += 1
text, _ = g.generate(names, NM)
assert text.count('extern char glBindBuffer[];') == 1 and '"glMissing"' not in text
# bsearch needs the table sorted by name
rows = [l.split('"')[1] for l in text.splitlines() if l.startswith('\t{ "')]
assert rows == sorted(rows), rows
ok += 1
print('gen_gl_procs: %d checks pass' % ok)
