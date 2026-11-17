/* Sea Defender on GFOS: the GLFW 2.7 calls Sea Defender makes
 * (amiga/include/GL/glfw.h), on GFOpenUp's SDL 2 and OpenGL. One window
 * (a screen when full screen), SDL's events turned into GLFW's key, char,
 * mouse button, position and wheel callbacks, GLFW's key codes from SDL's,
 * GL calls by name from a table made at build time (sd_gl_proc).
 * MIT, like Sea Defender. Copyright (c) 2026 AmiCradle Team. */
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <GL/glfw.h>

static SDL_Window *win;
static SDL_GLContext ctx;
static int fsaa, opened, active = 1, iconified;
static unsigned char keys[GLFW_KEY_LAST + 1], buttons[GLFW_MOUSE_BUTTON_LAST + 1];
static int mouse_x, mouse_y, wheel;
static int sticky_keys, sticky_buttons, key_repeat;
static GLFWkeyfun key_cb;
static GLFWcharfun char_cb;
static GLFWmousebuttonfun button_cb;
static GLFWmouseposfun pos_cb;
static GLFWmousewheelfun wheel_cb;
static Uint64 t0;
static double t_offset;

int glfwInit(void)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
		return GL_FALSE;
	t0 = SDL_GetPerformanceCounter();
	return GL_TRUE;
}

void glfwTerminate(void)
{
	glfwCloseWindow();
	SDL_Quit();
}

void glfwGetVersion(int *major, int *minor, int *rev)
{
	*major = 2; *minor = 7; *rev = 9;
}

void glfwOpenWindowHint(int target, int hint)
{
	if (target == GLFW_FSAA_SAMPLES)
		fsaa = hint;
}

int glfwOpenWindow(int w, int h, int r, int g, int b, int a, int depth, int stencil, int mode)
{
	Uint32 flags = SDL_WINDOW_OPENGL;

	if (win)
		return GL_TRUE;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, r ? r : 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, g ? g : 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, b ? b : 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, a);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, stencil);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, fsaa > 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, fsaa);
	if (mode == GLFW_FULLSCREEN)
		flags |= SDL_WINDOW_FULLSCREEN;
	win = SDL_CreateWindow("Sea Defender", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags);
	if (!win)
		return GL_FALSE;
	if (!(ctx = SDL_GL_CreateContext(win))) {
		SDL_DestroyWindow(win);
		win = NULL;
		return GL_FALSE;
	}
	opened = 1;
	return GL_TRUE;
}

void glfwCloseWindow(void)
{
	if (ctx)
		SDL_GL_DeleteContext(ctx);
	if (win)
		SDL_DestroyWindow(win);
	ctx = NULL;
	win = NULL;
	opened = 0;
}

void glfwSetWindowTitle(const char *title)
{
	if (win)
		SDL_SetWindowTitle(win, title);
}

void glfwGetWindowSize(int *w, int *h)
{
	*w = *h = 0;
	if (win)
		SDL_GetWindowSize(win, w, h);
}

void glfwSetWindowPos(int x, int y)
{
	if (win)
		SDL_SetWindowPosition(win, x, y);
}

void glfwSwapBuffers(void)
{
	if (win)
		SDL_GL_SwapWindow(win);
}

void glfwSwapInterval(int interval)
{
	SDL_GL_SetSwapInterval(interval);
}

int glfwGetWindowParam(int param)
{
	switch (param) {
	case GLFW_OPENED: return opened ? GL_TRUE : GL_FALSE;
	case GLFW_ACTIVE: return active ? GL_TRUE : GL_FALSE;
	case GLFW_ICONIFIED: return iconified ? GL_TRUE : GL_FALSE;
	case GLFW_ACCELERATED: return GL_TRUE;
	case GLFW_FSAA_SAMPLES: return fsaa;
	default: return 0;
	}
}

/* the modes worth offering on an Amiga screen, smallest first */
int glfwGetVideoModes(GLFWvidmode *list, int max)
{
	static const int sizes[][2] = { { 640, 400 }, { 640, 480 }, { 800, 500 }, { 800, 600 },
		{ 1024, 640 }, { 1024, 768 }, { 1280, 800 } };
	int i, n = 0;

	for (i = 0; i < (int)(sizeof sizes / sizeof sizes[0]) && n < max; i++, n++) {
		list[n].Width = sizes[i][0];
		list[n].Height = sizes[i][1];
		list[n].RedBits = list[n].GreenBits = list[n].BlueBits = 8;
	}
	return n;
}

void glfwGetDesktopMode(GLFWvidmode *mode)
{
	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) == 0) {
		mode->Width = dm.w;
		mode->Height = dm.h;
	} else {
		mode->Width = 640;
		mode->Height = 480;
	}
	mode->RedBits = mode->GreenBits = mode->BlueBits = 8;
}

/* SDL's key to GLFW 2's: printable keys as their upper-case character */
int sd_glfw_key(SDL_Keycode k)
{
	if (k >= SDLK_a && k <= SDLK_z)
		return 'A' + (k - SDLK_a);
	if (k >= 32 && k < 127)
		return (int)k;
	if (k >= SDLK_F1 && k <= SDLK_F12)
		return GLFW_KEY_F1 + (k - SDLK_F1);
	if (k >= SDLK_KP_1 && k <= SDLK_KP_9)
		return GLFW_KEY_KP_0 + 1 + (k - SDLK_KP_1);
	switch (k) {
	case SDLK_ESCAPE: return GLFW_KEY_ESC;
	case SDLK_UP: return GLFW_KEY_UP;
	case SDLK_DOWN: return GLFW_KEY_DOWN;
	case SDLK_LEFT: return GLFW_KEY_LEFT;
	case SDLK_RIGHT: return GLFW_KEY_RIGHT;
	case SDLK_LSHIFT: return GLFW_KEY_LSHIFT;
	case SDLK_RSHIFT: return GLFW_KEY_RSHIFT;
	case SDLK_LCTRL: return GLFW_KEY_LCTRL;
	case SDLK_RCTRL: return GLFW_KEY_RCTRL;
	case SDLK_LALT: return GLFW_KEY_LALT;
	case SDLK_RALT: return GLFW_KEY_RALT;
	case SDLK_TAB: return GLFW_KEY_TAB;
	case SDLK_RETURN: return GLFW_KEY_ENTER;
	case SDLK_BACKSPACE: return GLFW_KEY_BACKSPACE;
	case SDLK_INSERT: return GLFW_KEY_INSERT;
	case SDLK_DELETE: return GLFW_KEY_DEL;
	case SDLK_PAGEUP: return GLFW_KEY_PAGEUP;
	case SDLK_PAGEDOWN: return GLFW_KEY_PAGEDOWN;
	case SDLK_HOME: return GLFW_KEY_HOME;
	case SDLK_END: return GLFW_KEY_END;
	case SDLK_KP_0: return GLFW_KEY_KP_0;
	case SDLK_KP_ENTER: return GLFW_KEY_KP_ENTER;
	default: return GLFW_KEY_UNKNOWN;
	}
}

static void key_event(int key, int action, int repeat)
{
	if (key < 0 || key > GLFW_KEY_LAST)
		return;
	if (repeat && !key_repeat)
		return;
	if (action == GLFW_PRESS)
		keys[key] = 1;
	else
		keys[key] = 0;
	if (key_cb)
		key_cb(key, action);
}

void glfwPollEvents(void)
{
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			opened = 0;
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			key_event(sd_glfw_key(e.key.keysym.sym), e.type == SDL_KEYDOWN ? GLFW_PRESS : GLFW_RELEASE,
				e.key.repeat);
			break;
		case SDL_TEXTINPUT:
			if (char_cb) {
				const unsigned char *s = (const unsigned char *)e.text.text;

				/* Latin-1 from UTF-8; the game's font is ASCII */
				while (*s) {
					int c = *s++;

					if (c >= 0xc0 && (*s & 0xc0) == 0x80)
						c = ((c & 0x1f) << 6) | (*s++ & 0x3f);
					else if (c >= 0x80)
						continue;
					char_cb(c, GLFW_PRESS);
					char_cb(c, GLFW_RELEASE);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP: {
			int b = e.button.button == SDL_BUTTON_LEFT ? GLFW_MOUSE_BUTTON_LEFT :
				e.button.button == SDL_BUTTON_RIGHT ? GLFW_MOUSE_BUTTON_RIGHT :
				e.button.button == SDL_BUTTON_MIDDLE ? GLFW_MOUSE_BUTTON_MIDDLE : -1;
			int action = e.type == SDL_MOUSEBUTTONDOWN ? GLFW_PRESS : GLFW_RELEASE;

			if (b < 0)
				break;
			if (action == GLFW_PRESS || !sticky_buttons)
				buttons[b] = action == GLFW_PRESS;
			if (button_cb)
				button_cb(b, action);
			break;
		}
		case SDL_MOUSEMOTION:
			mouse_x = e.motion.x;
			mouse_y = e.motion.y;
			if (pos_cb)
				pos_cb(mouse_x, mouse_y);
			break;
		case SDL_MOUSEWHEEL:
			wheel += e.wheel.y;
			if (wheel_cb)
				wheel_cb(wheel);
			break;
		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
				active = 1;
			else if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
				active = 0;
			else if (e.window.event == SDL_WINDOWEVENT_MINIMIZED)
				iconified = 1;
			else if (e.window.event == SDL_WINDOWEVENT_RESTORED)
				iconified = 0;
			else if (e.window.event == SDL_WINDOWEVENT_CLOSE)
				opened = 0;
			break;
		default:
			break;
		}
	}
}

int glfwGetKey(int key)
{
	int r;

	if (key < 0 || key > GLFW_KEY_LAST)
		return GLFW_RELEASE;
	r = keys[key] ? GLFW_PRESS : GLFW_RELEASE;
	return r;
}

int glfwGetMouseButton(int b)
{
	return b >= 0 && b <= GLFW_MOUSE_BUTTON_LAST && buttons[b] ? GLFW_PRESS : GLFW_RELEASE;
}

void glfwGetMousePos(int *x, int *y)
{
	*x = mouse_x;
	*y = mouse_y;
}

void glfwSetMousePos(int x, int y)
{
	mouse_x = x;
	mouse_y = y;
	if (win)
		SDL_WarpMouseInWindow(win, x, y);
}

int glfwGetMouseWheel(void) { return wheel; }

void glfwSetKeyCallback(GLFWkeyfun f) { key_cb = f; }
void glfwSetCharCallback(GLFWcharfun f) { char_cb = f; if (f) SDL_StartTextInput(); }
void glfwSetMouseButtonCallback(GLFWmousebuttonfun f) { button_cb = f; }
void glfwSetMousePosCallback(GLFWmouseposfun f) { pos_cb = f; }
void glfwSetMouseWheelCallback(GLFWmousewheelfun f) { wheel_cb = f; }

double glfwGetTime(void)
{
	return t_offset + (double)(SDL_GetPerformanceCounter() - t0) / (double)SDL_GetPerformanceFrequency();
}

void glfwSetTime(double t)
{
	t0 = SDL_GetPerformanceCounter();
	t_offset = t;
}

int glfwExtensionSupported(const char *name)
{
	const char *ext = (const char *)glGetString(GL_EXTENSIONS);
	size_t n = strlen(name);

	while (ext && (ext = strstr(ext, name)) != NULL) {
		if ((ext[n] == ' ' || ext[n] == 0))
			return GL_TRUE;
		ext += n;
	}
	return GL_FALSE;
}

void *glfwGetProcAddress(const char *name)
{
	return sd_gl_proc(name);
}

void glfwGetGLVersion(int *major, int *minor, int *rev)
{
	const char *v = (const char *)glGetString(GL_VERSION);

	*major = *minor = *rev = 0;
	if (v) {
		*major = atoi(v);
		while (*v && *v != '.') v++;
		if (*v) *minor = atoi(++v);
	}
}

void glfwEnable(int token)
{
	switch (token) {
	case GLFW_MOUSE_CURSOR: SDL_ShowCursor(SDL_ENABLE); break;
	case GLFW_STICKY_KEYS: sticky_keys = 1; break;
	case GLFW_STICKY_MOUSE_BUTTONS: sticky_buttons = 1; break;
	case GLFW_KEY_REPEAT: key_repeat = 1; break;
	default: break;
	}
}

void glfwDisable(int token)
{
	switch (token) {
	case GLFW_MOUSE_CURSOR: SDL_ShowCursor(SDL_DISABLE); break;
	case GLFW_STICKY_KEYS: sticky_keys = 0; break;
	case GLFW_STICKY_MOUSE_BUTTONS: sticky_buttons = 0; break;
	case GLFW_KEY_REPEAT: key_repeat = 0; break;
	default: break;
	}
}
