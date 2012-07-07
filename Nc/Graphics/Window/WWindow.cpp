
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
#include "../../Core/Image.h"

using namespace Nc;
using namespace Nc::System;
using namespace Nc::Graphic;

const char *WWindow::_classNameA = "Nc_Window";

WWindow::WWindow() : Window()
{
    _handle = NULL;

    // Choose the window style according to the Style parameter
    _win32Style = WS_VISIBLE;
}

WWindow::~WWindow()
{
    if (_isCreate)
        Close();
}

void WWindow::Create(const std::string &title, const Vector2ui &size, unsigned long pattern, const Utils::FileName &icon, unsigned int antialiasingLevel)
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
    int left   = (GetDeviceCaps(screenDC, HORZRES) - size.Data[0])  / 2;
    int top    = (GetDeviceCaps(screenDC, VERTRES) - size.Data[1]) / 2;
    ReleaseDC(NULL, screenDC);

    // Choose the window style according to the Style parameter
    if (pattern == 0)
    {
        _win32Style |= WS_POPUP;
    }
    else
    {
        if (pattern & Titlebar)     _win32Style |= WS_CAPTION | WS_MINIMIZEBOX;
        if (pattern & Resizeable)   _win32Style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        if (pattern & Closeable)    _win32Style |= WS_SYSMENU;
    }

    // In windowed mode, adjust width and height so that window will have the requested client area
    Vector2i winsize(size);
    if ((pattern & Fullscreen) != 0)
    {
        RECT Rect = {0, 0, winsize.Data[0], winsize.Data[1]};
        AdjustWindowRect(&Rect, _win32Style, false);
        winsize.Data[0]  = Rect.right - Rect.left;
        winsize.Data[1] = Rect.bottom - Rect.top;
    }

	_antialiasingLevel = antialiasingLevel;

    // Create the window
    _input = new WWindowInput(this);
    _handle = CreateWindowA(_classNameA, title.c_str(), _win32Style, left, top, winsize.Data[0], winsize.Data[1], NULL, NULL, GetModuleHandle(NULL), _input);
    if (_handle == NULL)
        throw Utils::Exception("WWindow", "Can't create the window");

    // Switch to fullscreen if requested
    if ((pattern & Fullscreen) != 0)
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

void WWindow::UseExistingWindow(void *disp, int winId, const Vector2ui &size, unsigned int antialiasingLevel)
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

GLContext *WWindow::CreateGLContext()
{
    _context = new WGLContext(this);
    _context->Create();
    return _context;
}

void WWindow::Close()
{
    // Destroy the renderer attach to the window
    if (_context)
        delete _context;

    // Destroy the input context
    if (_input)
        delete _input;

    // Destroy the custom icon, if any
  if (_icon)
      DestroyIcon(_icon);

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

void WWindow::Resize(unsigned int width, unsigned int height)
{
    RECT Rect = {0, 0, width, height};
    AdjustWindowRect(&Rect, _win32Style, false);
    _width  = Rect.right - Rect.left;
    _height = Rect.bottom - Rect.top;
    Resized();
}

void	WWindow::SwitchToFullscreen(const Vector2ui &size)
{
    DEVMODE DevMode;
    DevMode.dmSize       = sizeof(DEVMODE);
    DevMode.dmPelsWidth  = size.Data[0];
    DevMode.dmPelsHeight = size.Data[1];
    DevMode.dmBitsPerPel = 32;
    DevMode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

    // Apply fullscreen mode
    if (ChangeDisplaySettings(&DevMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		throw Utils::Exception("WWindow", "Failed to change display mode for fullscreen");

	// Make the window flags compatible with fullscreen mode
    SetWindowLong(_handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    SetWindowLong(_handle, GWL_EXSTYLE, WS_EX_APPWINDOW);

    // Resize the window so that it fits the entire screen
    SetWindowPos(_handle, HWND_TOP, 0, 0, size.Data[0], size.Data[1], SWP_FRAMECHANGED);
    ShowWindow(_handle, SW_SHOW);
}

bool	WWindow::SetIcon(const Utils::FileName &f)
{
    Utils::FileName filename = CONFIG->Block("RessourcesPath")->Line("Image")->Param("path") + f;
    if (f.empty() || !filename.IsReadable()) // si l'icone n'existe pas, on retourne false sans afficher de message d'erreur
        return false;

	Core::Image image;
    image.LoadFromFile(filename);

    // First destroy the previous one
    if (_icon)
        DestroyIcon(_icon);

    // Windows wants BGRA pixels : swap red and blue channels
	const unsigned char *pix = image.GetPixels();
    unsigned char *iconPixels = new unsigned char[image.Size().Data[0] * image.Size().Data[1] * 4];
    for (std::size_t i = 0; i < image.Size().Data[1] * image.Size().Data[1]; ++i)
    {
        iconPixels[i * 4 + 0] = pix[i * 4 + 2];
        iconPixels[i * 4 + 1] = pix[i * 4 + 1];
        iconPixels[i * 4 + 2] = pix[i * 4 + 0];
        iconPixels[i * 4 + 3] = pix[i * 4 + 3];
    }

    // Create the icon from the pixels array
    _icon = CreateIcon(GetModuleHandle(NULL), image.Size().Data[0], image.Size().Data[1], 1, 32, NULL, iconPixels);
	if (!_icon)
	{
		delete[] iconPixels;
		throw Utils::Exception("WWindow", "Failed to create the icone");
	}

    // Set it as both big and small icon of the window
	SendMessage(_handle, WM_SETICON, ICON_BIG,   (LPARAM)_icon);
	SendMessage(_handle, WM_SETICON, ICON_SMALL, (LPARAM)_icon);
	delete[] iconPixels;
	return true;
}
