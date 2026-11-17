/* Sea Defender on GFOS: the GLFW 2.7 calls Sea Defender makes, on SDL 2
 * (amiga/sd_glfw.c). Names and values as GLFW 2.7's glfw.h documents them;
 * the code is ours. MIT, like Sea Defender.
 * Copyright (c) 2026 AmiCradle Team. */
#ifndef SD_AMIGA_GLFW_H
#define SD_AMIGA_GLFW_H

/* GL/gl.h only, as GLFW 2.7's glfw.h has it: Sea Defender's gldefs.h
 * declares its own extension types and fetches the calls by name, which
 * glext.h's newer declarations would clash with. */
#ifndef GL_GLEXT_LEGACY
#define GL_GLEXT_LEGACY 1
#endif
#include <GL/gl.h>
#include <GL/glu.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef APIENTRY
#define APIENTRY
#endif
#define GLFWCALL

#define GLFW_RELEASE 0
#define GLFW_PRESS 1

#define GLFW_KEY_UNKNOWN -1
#define GLFW_KEY_SPACE 32
#define GLFW_KEY_SPECIAL 256
#define GLFW_KEY_ESC (GLFW_KEY_SPECIAL + 1)
#define GLFW_KEY_F1 (GLFW_KEY_SPECIAL + 2)
#define GLFW_KEY_F12 (GLFW_KEY_SPECIAL + 13)
#define GLFW_KEY_F2 (GLFW_KEY_SPECIAL + 3)
#define GLFW_KEY_F3 (GLFW_KEY_SPECIAL + 4)
#define GLFW_KEY_F4 (GLFW_KEY_SPECIAL + 5)
#define GLFW_KEY_F5 (GLFW_KEY_SPECIAL + 6)
#define GLFW_KEY_F6 (GLFW_KEY_SPECIAL + 7)
#define GLFW_KEY_F7 (GLFW_KEY_SPECIAL + 8)
#define GLFW_KEY_F8 (GLFW_KEY_SPECIAL + 9)
#define GLFW_KEY_F9 (GLFW_KEY_SPECIAL + 10)
#define GLFW_KEY_F10 (GLFW_KEY_SPECIAL + 11)
#define GLFW_KEY_F11 (GLFW_KEY_SPECIAL + 12)
#define GLFW_KEY_F13 (GLFW_KEY_SPECIAL + 14)
#define GLFW_KEY_F14 (GLFW_KEY_SPECIAL + 15)
#define GLFW_KEY_F15 (GLFW_KEY_SPECIAL + 16)
#define GLFW_KEY_F16 (GLFW_KEY_SPECIAL + 17)
#define GLFW_KEY_F17 (GLFW_KEY_SPECIAL + 18)
#define GLFW_KEY_F18 (GLFW_KEY_SPECIAL + 19)
#define GLFW_KEY_F19 (GLFW_KEY_SPECIAL + 20)
#define GLFW_KEY_F20 (GLFW_KEY_SPECIAL + 21)
#define GLFW_KEY_F21 (GLFW_KEY_SPECIAL + 22)
#define GLFW_KEY_F22 (GLFW_KEY_SPECIAL + 23)
#define GLFW_KEY_F23 (GLFW_KEY_SPECIAL + 24)
#define GLFW_KEY_F24 (GLFW_KEY_SPECIAL + 25)
#define GLFW_KEY_F25 (GLFW_KEY_SPECIAL + 26)
#define GLFW_KEY_UP (GLFW_KEY_SPECIAL + 27)
#define GLFW_KEY_DOWN (GLFW_KEY_SPECIAL + 28)
#define GLFW_KEY_LEFT (GLFW_KEY_SPECIAL + 29)
#define GLFW_KEY_RIGHT (GLFW_KEY_SPECIAL + 30)
#define GLFW_KEY_LSHIFT (GLFW_KEY_SPECIAL + 31)
#define GLFW_KEY_RSHIFT (GLFW_KEY_SPECIAL + 32)
#define GLFW_KEY_LCTRL (GLFW_KEY_SPECIAL + 33)
#define GLFW_KEY_RCTRL (GLFW_KEY_SPECIAL + 34)
#define GLFW_KEY_LALT (GLFW_KEY_SPECIAL + 35)
#define GLFW_KEY_RALT (GLFW_KEY_SPECIAL + 36)
#define GLFW_KEY_TAB (GLFW_KEY_SPECIAL + 37)
#define GLFW_KEY_ENTER (GLFW_KEY_SPECIAL + 38)
#define GLFW_KEY_BACKSPACE (GLFW_KEY_SPECIAL + 39)
#define GLFW_KEY_INSERT (GLFW_KEY_SPECIAL + 40)
#define GLFW_KEY_DEL (GLFW_KEY_SPECIAL + 41)
#define GLFW_KEY_PAGEUP (GLFW_KEY_SPECIAL + 42)
#define GLFW_KEY_PAGEDOWN (GLFW_KEY_SPECIAL + 43)
#define GLFW_KEY_HOME (GLFW_KEY_SPECIAL + 44)
#define GLFW_KEY_END (GLFW_KEY_SPECIAL + 45)
#define GLFW_KEY_KP_0 (GLFW_KEY_SPECIAL + 46)
#define GLFW_KEY_KP_ENTER (GLFW_KEY_SPECIAL + 62)
#define GLFW_KEY_LAST (GLFW_KEY_SPECIAL + 69)

#define GLFW_MOUSE_BUTTON_1 0
#define GLFW_MOUSE_BUTTON_2 1
#define GLFW_MOUSE_BUTTON_3 2
#define GLFW_MOUSE_BUTTON_LAST 7
#define GLFW_MOUSE_BUTTON_LEFT GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE GLFW_MOUSE_BUTTON_3

#define GLFW_WINDOW 0x00010001
#define GLFW_FULLSCREEN 0x00010002

#define GLFW_OPENED 0x00020001
#define GLFW_ACTIVE 0x00020002
#define GLFW_ICONIFIED 0x00020003
#define GLFW_ACCELERATED 0x00020004
#define GLFW_REFRESH_RATE 0x0002000B
#define GLFW_WINDOW_NO_RESIZE 0x00020012
#define GLFW_FSAA_SAMPLES 0x00020013

#define GLFW_MOUSE_CURSOR 0x00030001
#define GLFW_STICKY_KEYS 0x00030002
#define GLFW_STICKY_MOUSE_BUTTONS 0x00030003
#define GLFW_SYSTEM_KEYS 0x00030004
#define GLFW_KEY_REPEAT 0x00030005
#define GLFW_AUTO_POLL_EVENTS 0x00030006

typedef struct {
	int Width, Height;
	int RedBits, BlueBits, GreenBits;
} GLFWvidmode;

typedef void (GLFWCALL *GLFWkeyfun)(int key, int action);
typedef void (GLFWCALL *GLFWcharfun)(int character, int action);
typedef void (GLFWCALL *GLFWmousebuttonfun)(int button, int action);
typedef void (GLFWCALL *GLFWmouseposfun)(int x, int y);
typedef void (GLFWCALL *GLFWmousewheelfun)(int pos);

int glfwInit(void);
void glfwTerminate(void);
void glfwGetVersion(int *major, int *minor, int *rev);
int glfwOpenWindow(int width, int height, int redbits, int greenbits, int bluebits,
		int alphabits, int depthbits, int stencilbits, int mode);
void glfwOpenWindowHint(int target, int hint);
void glfwCloseWindow(void);
void glfwSetWindowTitle(const char *title);
void glfwGetWindowSize(int *width, int *height);
void glfwSetWindowPos(int x, int y);
void glfwSwapBuffers(void);
void glfwSwapInterval(int interval);
int glfwGetWindowParam(int param);
int glfwGetVideoModes(GLFWvidmode *list, int maxcount);
void glfwGetDesktopMode(GLFWvidmode *mode);
void glfwPollEvents(void);
int glfwGetKey(int key);
int glfwGetMouseButton(int button);
void glfwGetMousePos(int *xpos, int *ypos);
void glfwSetMousePos(int xpos, int ypos);
int glfwGetMouseWheel(void);
void glfwSetKeyCallback(GLFWkeyfun cbfun);
void glfwSetCharCallback(GLFWcharfun cbfun);
void glfwSetMouseButtonCallback(GLFWmousebuttonfun cbfun);
void glfwSetMousePosCallback(GLFWmouseposfun cbfun);
void glfwSetMouseWheelCallback(GLFWmousewheelfun cbfun);
double glfwGetTime(void);
void glfwSetTime(double time);
int glfwExtensionSupported(const char *extension);
void *glfwGetProcAddress(const char *procname);
void glfwGetGLVersion(int *major, int *minor, int *rev);
void glfwEnable(int token);
void glfwDisable(int token);

/* the GL calls by name: a table made at build time (amiga/gen_gl_procs.py) */
void *sd_gl_proc(const char *name);

#ifdef __cplusplus
}
#endif
#endif
