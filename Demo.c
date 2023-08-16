// cl Demo.c opengl32.lib user32.lib gdi32.lib dwmapi.lib shell32.lib
#include <windows.h>
#include <gl/gl.h>
#include <stdio.h>
#include <dwmapi.h>
#include <shellapi.h>

#define SCREENWIDTH 128
#define SCREENHEIGHT 128

typedef unsigned int (WINAPI *PFNGLCREATEPROGRAMPROC) ();
typedef unsigned int (WINAPI *PFNGLCREATESHADERPROC) (unsigned int type);
typedef void (WINAPI *PFNGLSHADERSOURCEPROC) (unsigned int shader, int count, const char*const*string, const int *length);
typedef void (WINAPI *PFNGLCOMPILESHADERPROC) (unsigned int shader);
typedef void (WINAPI *PFNGLATTACHSHADERPROC) (unsigned int program, unsigned int shader);
typedef void (WINAPI *PFNGLLINKPROGRAMPROC) (unsigned int program);
typedef void (WINAPI *PFNGLUSEPROGRAMPROC) (unsigned int program);
typedef void (WINAPI *PFNGLGETSHADERIVPROC) (unsigned int shader, unsigned int pname, int *params);
typedef void (WINAPI *PFNGLGETSHADERINFOLOGPROC) (unsigned int shader, int bufSize, int *length, char *infoLog);
typedef void (WINAPI *PFNGLGENVERTEXARRAYSPROC) (int n, unsigned int *arrays);
typedef void (WINAPI *PFNGLBINDVERTEXARRAYPROC) (unsigned int array);
typedef int (WINAPI *PFNGLGETUNIFORMLOCATIONPROC) (unsigned int program, const char *name);
typedef void (WINAPI *PFNGLUNIFORM1FPROC) (int location, float v0);
typedef int (WINAPI *PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef HGLRC (WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef int (WINAPI *PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define WGL_DRAW_TO_WINDOW_ARB            0x2001
#define WGL_ACCELERATION_ARB              0x2003
#define WGL_SUPPORT_OPENGL_ARB            0x2010
#define WGL_DOUBLE_BUFFER_ARB             0x2011
#define WGL_PIXEL_TYPE_ARB                0x2013
#define WGL_COLOR_BITS_ARB                0x2014
#define WGL_DEPTH_BITS_ARB                0x2022
#define WGL_STENCIL_BITS_ARB              0x2023
#define WGL_FULL_ACCELERATION_ARB         0x2027
#define WGL_TYPE_RGBA_ARB                 0x202B
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_TRANSPARENT_ARB               0x200A
#define WGL_ALPHA_BITS_ARB                0x201B

PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM1FPROC glUniform1f;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;

void wglInit()
{
	WNDCLASSA wndClassA = {CS_HREDRAW|CS_VREDRAW|CS_OWNDC, DefWindowProcA, 0, 0, GetModuleHandle(0), 0, 0, 0, 0, "OpenGL"};
	RegisterClassA(&wndClassA);
	HWND hwnd = CreateWindowExA(0, wndClassA.lpszClassName, "OpenGL", 0, 0, 0, 0, 0, 0, 0, wndClassA.hInstance, 0);
	HDC hdc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd), 1, PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, 0, 0, 0, 0, 0};
	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);
	HGLRC hglrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, hglrc);
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	wglMakeCurrent(hdc, 0);
	wglDeleteContext(hglrc);
	ReleaseDC(hwnd, hdc);
	DestroyWindow(hwnd);
}

void glInit()
{
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");	
}

static const char* VertexShader = \
	"#version 440 \n"
	"void main()"
	"{"
		"vec2 vertices[3] = vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1, 3));"
		"gl_Position = vec4(vertices[gl_VertexID], 0, 1);"
	"}";

static const char* FragmentShader = \
	"#version 440 \n"
	"layout (location = 0) out vec4 fragColor;"
	"uniform float iTime;"
	"uniform float isVisible;"
	"float ProgressBar(vec2 uv, float size, float thickness)"
	"{"
		"float radius = length(uv);"
		"float theta = -atan(uv.y, uv.x);"
		"return fract(0.5 * (1.0 + theta / 3.14) -iTime) * smoothstep(2., 0., abs(radius - size) - thickness);"
	"}"
	"void main()"
	"{"
		"vec2 resolution = vec2(128, 128);"
		"float bar = ProgressBar(gl_FragCoord.xy - resolution / 2, 25.0, 10.0);"
		"fragColor = (bar > 0.0) && (isVisible > 0.5) ? vec4(bar) : vec4(0.0);"
	"}";

void DebugShader(int shader)
{
	int isCompiled = 0;
	glGetShaderiv(shader, GL_LINK_STATUS, &isCompiled);
	if(!isCompiled)
	{
		int maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
		int length = 0;
		char* infoLog = (char*)malloc(sizeof(char) * maxLength);
		glGetShaderInfoLog(shader, maxLength, &length, infoLog);
		if (maxLength > 1)
		{
			FILE *file = fopen ("debug.log","a");
			fprintf(file, "%s\n%s\n", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION), infoLog);
			fclose(file);
			free(infoLog);
			ExitProcess(0);
		}
	}
}

int BuildShaders(const char* VS, const char* FS)
{
	int program = glCreateProgram();
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &VS, 0);
	glShaderSource(fragmentShader, 1, &FS, 0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	DebugShader(vertexShader);
	DebugShader(fragmentShader);
	return program;
}

int IsTextEqual(char* chars, TCHAR* tchars)
{
	int a = strlen(chars);
	int b = strlen(tchars);
	int count = a < b ? a : b;
	int equal = count > 0 ? 1 : 0;
	for (int i = 0; i < count; i++)
	{
		if (chars[i] != tchars[i])
		{
			equal = 0;
			break;
		}
	}
	return equal;
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg==WM_CLOSE || uMsg==WM_DESTROY)
	{
		PostQuitMessage(0); return 0;
	}
	else
	{
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	char** argv;
	LPWSTR* lpArgv = CommandLineToArgvW( GetCommandLineW(), &argc );
	argv = (char**)malloc( argc*sizeof(char*) );
	int size = 0;
	for(int i = 0; i < argc; ++i)
	{
		size = wcslen(lpArgv[i]) + 1;
		argv[i] = (char*)malloc( size );
		wcstombs( argv[i], lpArgv[i], size );
	}
	if (argc < 4) // Not enough arguments. Exit...
	{
		ExitProcess(0);
	}
	int exit = 0;
	MSG msg;
	WNDCLASSA wndClassA = {CS_HREDRAW|CS_VREDRAW|CS_OWNDC, WindowProc, 0, 0, hInstance, 0, LoadCursor(0, IDC_ARROW), 0, 0, "Demo"};
	RegisterClassA(&wndClassA);
	HWND hwnd = CreateWindowExA(0, wndClassA.lpszClassName, "Demo", WS_VISIBLE|WS_POPUP, 0, 0, SCREENWIDTH, SCREENHEIGHT, 0, 0, hInstance, 0);
	HDC hdc = GetDC(hwnd);
	DWM_BLURBEHIND bb = {DWM_BB_ENABLE | DWM_BB_BLURREGION, 1, CreateRectRgn(0, 0, -1, -1), 1};
	DwmEnableBlurBehindWindow(hwnd, &bb);
	SetWindowLongPtr(hwnd, GWL_EXSTYLE, GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);
	wglInit();
	int piAttribIList[] = {WGL_DRAW_TO_WINDOW_ARB, 1, WGL_SUPPORT_OPENGL_ARB, 1, WGL_DOUBLE_BUFFER_ARB, 1, WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, WGL_TRANSPARENT_ARB, 1, WGL_COLOR_BITS_ARB, 32, WGL_ALPHA_BITS_ARB, 8, WGL_DEPTH_BITS_ARB, 24, WGL_STENCIL_BITS_ARB, 8, 0 };
	int piFormats;
	UINT nNumFormats;
	wglChoosePixelFormatARB(hdc, piAttribIList, 0, 1, &piFormats, &nNumFormats);
	PIXELFORMATDESCRIPTOR pfd;
	DescribePixelFormat(hdc, piFormats, sizeof(pfd), &pfd);
	SetPixelFormat(hdc, piFormats, &pfd);
	int attribList[] = {WGL_CONTEXT_MAJOR_VERSION_ARB, 4, WGL_CONTEXT_MINOR_VERSION_ARB, 4, WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0};
	HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribList);
	wglMakeCurrent(hdc, hglrc);
	glInit();
	wglSwapIntervalEXT(0);
	unsigned int vertexArrayObject;
	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	int program = BuildShaders(VertexShader, FragmentShader);
	glUseProgram(program);
	int iTime = glGetUniformLocation(program, "iTime");
	int isVisible = glGetUniformLocation(program, "isVisible");
	DWORD startTime = GetTickCount();
	TCHAR title[256];
	SetForegroundWindow(FindWindowA(NULL, argv[3]));
	while (!exit)
	{
		while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if( msg.message == WM_QUIT ) exit = 1;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		HWND mainApp = GetForegroundWindow();
		GetWindowText(mainApp, title, 256);
		RECT rect;
		GetWindowRect(mainApp, &rect);
		SetWindowPos(hwnd, HWND_TOPMOST, atoi(argv[1]) + rect.left, atoi(argv[2]) + rect.top, 0, 0, SWP_NOSIZE);
		glUniform1f(isVisible, (float) IsTextEqual(argv[3], title));
		glUniform1f(iTime, (GetTickCount() - startTime) * 0.001f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		wglSwapLayerBuffers(hdc, WGL_SWAP_MAIN_PLANE);
	}
	for (int i = 0; i < argc; i++) {free(argv[i]);}
	free(argv);
	return 0;
}