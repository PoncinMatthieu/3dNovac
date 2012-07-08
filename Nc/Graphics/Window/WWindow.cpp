
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, The 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        02/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "WWindow.h"
#include "../Input/WWindowInput.h"
#include "../Context/WGLContext.h"
#include "../Data/Image.h"

using namespace Nc;
using namespace Nc::System;
using namespace Nc::Graphic;

const char *Graphic::Window::_classNameA = "Nc_Window";

Graphic::Window::Window(SceneGraphManager *sceneGraphManager) 
	: IWindow(sceneGraphManager), _handle(NULL)
{
    // Choose the window style according to the Style parameter
    _win32Style = WS_VISIBLE;

	// create the window input instance
    _input = new WWindowInput(this);
}

Graphic::Window::Window(const std::string &title, const Math::Vector2ui &size, const WindowStyle &style, const Utils::FileName &icon, unsigned int antialiasingLevel, SceneGraphManager *sceneGraphManager)
	: IWindow(title, size, style, icon, antialiasingLevel, sceneGraphManager), _handle(NULL)
{
    // Choose the window style according to the Style parameter
    _win32Style = WS_VISIBLE;

	// create the window input instance
    _input = new WWindowInput(this);
}

Graphic::Window::~Window()
{
    if (_isCreate)
        Close();
	delete _input;
}

void Graphic::Window::Create(const std::string &title, const Vector2ui &size, const WindowStyle &style, const Utils::FileName &icon, unsigned int antialiasingLevel)
{
    // register the window class
    WNDCLASSA windowClass;
    windowClass.style         = 0;
    windowClass.lpfnWndProc   = &WWindowInput::GlobalOnEvent;
    windowClass.cbClsExtra    = 0;
    windowClass.cbWndExtra    = 0;
    windowClass.hInstance     = GetModuleHandle(NULL);
    windowClass.hIcon         = NULL;
    windowClass.hCursor       = 0;
    windowClass.hbrBackground = 0;
    windowClass.lpszMenuName  = NULL;
    windowClass.lpszClassName = _classNameA;
    RegisterClassA(&windowClass);

// Compute position and size
    HDC screenDC = GetDC(NULL);
    int left   = (GetDeviceCaps(screenDC, HORZRES) - size.data[0])  / 2;
    int top    = (GetDeviceCaps(screenDC, VERTRES) - size.data[1]) / 2;
    ReleaseDC(NULL, screenDC);

    // Choose the window style according to the Style parameter
	if (style.GetMask() == 0)
    {
        _win32Style |= WS_POPUP;
    }
    else
    {
        if (style.Enabled(Titlebar))
			_win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        if (style.Enabled(Resizeable))
			_win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        if (style.Enabled(Closeable))
			_win32Style |= WS_SYSMENU;
    }

    // In windowed mode, adjust width and height so that window will have the requested client area
    Vector2i winsize(size);
    if (style.Disabled(Fullscreen))
    {
        RECT Rect = {0, 0, winsize.data[0], winsize.data[1]};
        AdjustWindowRect(&Rect, _win32Style, false);
        winsize.data[0]  = Rect.right - Rect.left;
        winsize.data[1] = Rect.bottom - Rect.top;
    }

	_antialiasingLevel = antialiasingLevel;

    // Create the window
    _handle = CreateWindowA(_classNameA, title.c_str(), _win32Style, left, top, winsize.data[0], winsize.data[1], NULL, NULL, GetModuleHandle(NULL), _input);
    if (_handle == NULL)
        throw Utils::Exception("WWindow", "Can't create the window");

    // Switch to fullscreen if requested
    if (style.Enabled(Fullscreen))
        SwitchToFullscreen(winsize);

    // Create the inputs of the window
    _own = true;
    _input->Create();

    // recompute the window size, to be sure that the window as the good size
    RECT ActualRect;
    GetClientRect(_handle, &ActualRect);
    _width  = ActualRect.right - ActualRect.left;
    _height = ActualRect.bottom - ActualRect.top;

	SetIcon(icon);

    // creation du curseur par defaut
    _defaultCursor = new Cursor(this);
    _defaultCursor->Enable();

    _isCreate = true;
}

void Graphic::Window::UseExistingWindow(void *disp, int winId, const Vector2ui &size, unsigned int antialiasingLevel)
{
    _handle = reinterpret_cast<HWND>(disp);
    if (_handle == NULL)
        throw Utils::Exception("WWindow", "The parameter disp is not an handle HWND valid.");

    // Get window client size
    RECT Rect;
    GetClientRect(_handle, &Rect);
    _width  = Rect.right - Rect.left;
    _height = Rect.bottom - Rect.top;
	_antialiasingLevel = antialiasingLevel;

    _own = false;
    // initialize the inputs of the window
    _input = new WWindowInput(this);
    _isCreate = true;
}

GLContext *Graphic::Window::CreateGLContext()
{
    _context = new WGLContext(this);
    _context->Create();
    return _context;
}

void Graphic::Window::Close()
{
    // Destroy the renderer attach to the window
    if (_context)
        delete _context;
	_context = NULL;

    // Destroy the input context
	_input->Destroy();

    // Destroy the custom icon, if any
  if (_hicon)
      DestroyIcon(_hicon);
  _hicon = NULL;

    // Destroy the window
    if (_own)
    {
        // Destroy the window
        if (_handle)
            DestroyWindow(_handle);
        UnregisterClassA(_classNameA, GetModuleHandle(NULL));
    }
    _isCreate = false;
}

void Graphic::Window::Resize(unsigned int width, unsigned int height)
{
    RECT Rect = {0, 0, width, height};
    AdjustWindowRect(&Rect, _win32Style, false);
    _width  = Rect.right - Rect.left;
    _height = Rect.bottom - Rect.top;
    Resized();
}

void	Graphic::Window::SwitchToFullscreen(const Vector2ui &size)
{
    DEVMODE DevMode;
    DevMode.dmSize       = sizeof(DEVMODE);
    DevMode.dmPelsWidth  = size.data[0];
    DevMode.dmPelsHeight = size.data[1];
    DevMode.dmBitsPerPel = 32;
    DevMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    // Apply fullscreen mode
    if (ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		throw Utils::Exception("WWindow", "Failed to change display mode for fullscreen");

	// Make the window flags compatible with fullscreen mode
    SetWindowLong(_handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    SetWindowLong(_handle, GWL_EXSTYLE, WS_EX_APPWINDOW);

    // Resize the window so that it fits the entire screen
    SetWindowPos(_handle, HWND_TOP, 0, 0, size.data[0], size.data[1], SWP_FRAMECHANGED);
    ShowWindow(_handle, SW_SHOW);
}

bool	Graphic::Window::SetIcon(const Utils::FileName &f)
{
    Utils::FileName filename = CONFIG->Block("RessourcesPath")->Line("Image")->Param("path") + f;
    if (f.empty() || !filename.IsReadable()) // si l'icone n'existe pas, on retourne false sans afficher de message d'erreur
        return false;

	Image image;
    image.LoadFromFile(filename);

    // First destroy the previous one
    if (_hicon)
        DestroyIcon(_hicon);

    // Windows wants BGRA pixels : swap red and blue channels
	const unsigned char *pix = image.GetPixels();
    unsigned char *iconPixels = new unsigned char[image.Size().data[0] * image.Size().data[1] * 4];
    for (std::size_t i = 0; i < image.Size().data[1] * image.Size().data[1]; ++i)
    {
        iconPixels[i * 4 + 0] = pix[i * 4 + 2];
        iconPixels[i * 4 + 1] = pix[i * 4 + 1];
        iconPixels[i * 4 + 2] = pix[i * 4 + 0];
        iconPixels[i * 4 + 3] = pix[i * 4 + 3];
    }

    // Create the icon from the pixels array
    _hicon = CreateIcon(GetModuleHandle(NULL), image.Size().data[0], image.Size().data[1], 1, 32, NULL, iconPixels);
	if (!_hicon)
	{
		delete[] iconPixels;
		throw Utils::Exception("WWindow", "Failed to create the icone");
	}

    // Set it as both big and small icon of the window
	SendMessage(_handle, WM_SETICON, ICON_BIG,   (LPARAM)_hicon);
	SendMessage(_handle, WM_SETICON, ICON_SMALL, (LPARAM)_hicon);
	delete[] iconPixels;
	return true;
}
