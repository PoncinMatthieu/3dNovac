
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "XWindowInput.h"
#include "../Window/XWindow.h"

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::Graphic;

XWindowInput::XWindowInput(Window *win) : WindowInput(win)
{
    _inputContext = NULL;
    _inputMethod = NULL;
    if (_win == NULL)
        throw Utils::Exception("XWindowInput", "Can't create input, window is null");
}

XWindowInput::~XWindowInput()
{
}

void XWindowInput::Destroy()
{
    if (_inputContext)
        XDestroyIC(_inputContext);
	if (_inputMethod)
		XCloseIM(_inputMethod);
	_inputContext = NULL;
}

void XWindowInput::Create()
{
    _lastKeyReleaseEvent.type = -1;

    // Get the atom defining the close event
    _atomClose = XInternAtom(static_cast<Window*>(_win)->_display, "WM_DELETE_WINDOW", false);
    XSetWMProtocols(static_cast<Window*>(_win)->_display, static_cast<Window*>(_win)->_xwin, &_atomClose, 1);
    if (!_atomClose)
        throw Utils::Exception("XWindowInput", "Failed to fetch the atom defining the close event");

    // Create the input context
    _inputMethod = XOpenIM(static_cast<Window*>(_win)->_display, NULL, NULL, NULL);
    if (!_inputMethod)
        throw Utils::Exception("XWindowInput", "Failed to create input method");
    _inputContext = XCreateIC( _inputMethod,
                                XNClientWindow,  static_cast<Window*>(_win)->_xwin,
                                XNFocusWindow,   static_cast<Window*>(_win)->_xwin,
                                XNInputStyle,    XIMPreeditNothing | XIMStatusNothing,
                                NULL);
    if (!_inputContext)
        throw Utils::Exception("XWindowInput", "Failed to create input context");

    // select the inputs that the window should notify
    unsigned long  eventMask =  FocusChangeMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask |
                                PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask |
                                EnterWindowMask | LeaveWindowMask;
    XSelectInput(static_cast<Window*>(_win)->_display, static_cast<Window*>(_win)->_xwin, eventMask);
}

Bool CheckEvent(Display*, XEvent* event, XPointer userData)
{
    // Just check if the event matches the window
    return event->xany.window == reinterpret_cast< ::Window >(userData);
}

void XWindowInput::CheckEvents()
{
    if (_inputContext)
    {
        //XLockDisplay(static_cast<Window*>(_win)->_display);
        XEvent Event;
        while (XCheckIfEvent(static_cast<Window*>(_win)->_display, &Event, &CheckEvent,
                             reinterpret_cast<XPointer>(static_cast<Window*>(_win)->_xwin)))
        {
            if (!ListenerListEmpty())
            {
                // Detect repeated key events
                if ((Event.type == KeyPress) || (Event.type == KeyRelease))
                {
                    if (Event.xkey.keycode < 256)
                    {
                        // To detect if it is a repeated key event, we check the current state of the key.
                        // - If the state is "down", KeyReleased events must obviously be discarded.
                        // - KeyPress events are a little bit harder to handle: they depend on the EnableKeyRepeat state,
                        //   and we need to properly forward the first one.
                        char Keys[32];
                        XQueryKeymap(static_cast<Window*>(_win)->_display, Keys);
                        if (Keys[Event.xkey.keycode >> 3] & (1 << (Event.xkey.keycode % 8)))
                        {
                            // KeyRelease event + key down = repeated event --> discard
                            if (Event.type == KeyRelease)
                            {
                                _lastKeyReleaseEvent = Event;
                                continue;
                            }

                            // KeyPress event + key repeat disabled + matching KeyRelease event = repeated event --> discard
                            if ((Event.type == KeyPress) && !_keyRepeat &&
                                (_lastKeyReleaseEvent.xkey.keycode == Event.xkey.keycode) &&
                                (_lastKeyReleaseEvent.xkey.time == Event.xkey.time))
                                continue;
                        }
                    }
                }
                ProcessEvent(Event); // Process the event
            }
        }
        //XUnlockDisplay(static_cast<Window*>(_win)->_display);
    }
}

void XWindowInput::ProcessEvent(XEvent &event)
{
    switch (event.type)
    {
        case DestroyNotify :
        {
            _win->Close();
            break;
        }

        // Gain focus event
        case FocusIn :
        {
            // Update the input context
            if (_inputContext)
                XSetICFocus(_inputContext);
            GenereEvent(Event(this, Event::GainedFocus));
            break;
        }

        // Lost focus event
        case FocusOut :
        {
            // Update the input context
            if (_inputContext)
                XUnsetICFocus(_inputContext);
            GenereEvent(Event(this, Event::LostFocus));
            break;
        }

        // Resize event
        case ConfigureNotify :
        {
            if ((event.xconfigure.width != static_cast<int>(_win->Width())) || (event.xconfigure.height != static_cast<int>(_win->Height())))
            {
                Event e(this, Event::Resized);
                e.size.width = event.xconfigure.width;
                e.size.height = event.xconfigure.height;
                GenereEvent(e);
            }
            break;
        }

        // Close event
        case ClientMessage :
        {
            if ((event.xclient.format == 32) && (event.xclient.data.l[0]) == static_cast<long>(_atomClose))
                GenereEvent(Event(this, Event::Closed));
            break;
        }

        // Key down event
        case KeyPress :
        {
            // Get the keysym of the key that has been pressed
            static XComposeStatus KeyboardStatus;
            char Buffer[32];
            KeySym Sym;
            XLookupString(&event.xkey, Buffer, sizeof(Buffer), &Sym, &KeyboardStatus);

            // Fill the event parameters
            Event e(this, Event::KeyPressed);
            e.key.code    = ConvertKeySym(Sym);
            e.key.alt     = event.xkey.state & Mod1Mask;
            e.key.control = event.xkey.state & ControlMask;
            e.key.shift   = event.xkey.state & ShiftMask;
            GenereEvent(e);
            break;
        }
            // Generate a TextEntered event
/*
            if (!XFilterEvent(&event, None))
            {
                #ifdef X_HAVE_UTF8_STRING
                if (myInputContext)
                {
                    Status ReturnedStatus;
                    Uint8  KeyBuffer[16];
                    int Length = Xutf8LookupString(myInputContext, &event.xkey, reinterpret_cast<char*>(KeyBuffer), sizeof(KeyBuffer), NULL, &ReturnedStatus);
                    if (Length > 0)
                    {
                        Uint32 Unicode[2]; // just in case, but 1 character should be enough
                        const Uint32* End = Unicode::UTF8ToUTF32(KeyBuffer, KeyBuffer + Length, Unicode);

                        if (End > Unicode)
                        {
                            Event TextEvt;
                            TextEvt.Type         = Event::TextEntered;
                            TextEvt.Text.Unicode = Unicode[0];
                            SendEvent(TextEvt);
                        }
                    }
                }
                else
                #endif
                {
                    static XComposeStatus ComposeStatus;
                    char KeyBuffer[16];
                    if (XLookupString(&event.xkey, KeyBuffer, sizeof(KeyBuffer), NULL, &ComposeStatus))
                    {
                        Event TextEvt;
                        TextEvt.Type         = Event::TextEntered;
                        TextEvt.Text.Unicode = static_cast<Uint32>(KeyBuffer[0]);
                        SendEvent(TextEvt);
                    }
                }
            }
*/
            //break;

        // Key up event
        case KeyRelease :
        {
            // Get the keysym of the key that has been pressed
            char Buffer[32];
            KeySym Sym;
            XLookupString(&event.xkey, Buffer, 32, &Sym, NULL);

            // Fill the event parameters
            Event e(this, Event::KeyReleased);
            e.key.code    = ConvertKeySym(Sym);
            e.key.alt     = event.xkey.state & Mod1Mask;
            e.key.control = event.xkey.state & ControlMask;
            e.key.shift   = event.xkey.state & ShiftMask;
            GenereEvent(e);
            break;
        }

        // Mouse button pressed
        case ButtonPress :
        {
            unsigned int Button = event.xbutton.button;
            if ((Button == Button1) || (Button == Button2) || (Button == Button3) || (Button == 8) || (Button == 9))
            {
                Event e(this, Event::MouseButtonPressed);
                e.mouseButton.x = event.xbutton.x;
                e.mouseButton.y = event.xbutton.y;
                switch (Button)
                {
                    case Button1 : e.mouseButton.button = Mouse::Left;     break;
                    case Button2 : e.mouseButton.button = Mouse::Middle;   break;
                    case Button3 : e.mouseButton.button = Mouse::Right;    break;
                    case 8 :       e.mouseButton.button = Mouse::XButton1; break;
                    case 9 :       e.mouseButton.button = Mouse::XButton2; break;
                }
                GenereEvent(e);
            }
            break;
        }

        // Mouse button released
        case ButtonRelease :
        {
            unsigned int Button = event.xbutton.button;
            if ((Button == Button1) || (Button == Button2) || (Button == Button3) || (Button == 8) || (Button == 9))
            {
                Event e(this, Event::MouseButtonReleased);
                e.mouseButton.x = event.xbutton.x;
                e.mouseButton.y = event.xbutton.y;
                switch (Button)
                {
                    case Button1 : e.mouseButton.button = Mouse::Left;     break;
                    case Button2 : e.mouseButton.button = Mouse::Middle;   break;
                    case Button3 : e.mouseButton.button = Mouse::Right;    break;
                    case 8 :       e.mouseButton.button = Mouse::XButton1; break;
                    case 9 :       e.mouseButton.button = Mouse::XButton2; break;
                }
                GenereEvent(e);
            }
            else if ((Button == Button4) || (Button == Button5))
            {
                Event e(this, Event::MouseWheelMoved);
                e.mouseWheel.delta = event.xbutton.button == Button4 ? 1 : -1;
                GenereEvent(e);
            }
            break;
        }

        // Mouse moved
        case MotionNotify :
        {
            Event e(this, Event::MouseMoved);
            e.mouseMove.x = event.xmotion.x;
            e.mouseMove.y = event.xmotion.y;
            GenereEvent(e);
            break;
        }

        // Mouse entered
        case EnterNotify :
        {
            GenereEvent(Event(this, Event::MouseEntered));
            break;
        }

        // Mouse left
        case LeaveNotify :
        {
            GenereEvent(Event(this, Event::MouseLeft));
            break;
        }
    }
}

Key::Code XWindowInput::ConvertKeySym(KeySym sym)
{
    // First convert to uppercase (to avoid dealing with two different keysyms for the same key)
    KeySym lower, key;
    XConvertCase(sym, &lower, &key);
    switch (key)
    {
        case XK_KP_Add :       return Key::Add;
        case 0x2b :            return Key::Add;
        case XK_KP_Subtract :  return Key::Subtract;
        case XK_minus :        return Key::Subtract;
        case 0x5f :            return Key::Underscore;
        case XK_KP_Multiply :  return Key::Multiply;
        case XK_KP_Divide :    return Key::Divide;
        case XK_semicolon :    return Key::SemiColon;
        case 0x3a :            return Key::SemiColon;
        case 0x3e :            return Key::Sup;
        case 0x3c :            return Key::Inf;
        case XK_Shift_L :      return Key::LShift;
        case XK_Shift_R :      return Key::RShift;
        case XK_Control_L :    return Key::LControl;
        case XK_Control_R :    return Key::RControl;
        case XK_Alt_L :        return Key::LAlt;
        case XK_Alt_R :        return Key::RAlt;
        case XK_Super_L :      return Key::LSystem;
        case XK_Super_R :      return Key::RSystem;
        case XK_Menu :         return Key::Menu;
        case XK_Escape :       return Key::Escape;
        case XK_slash :        return Key::Slash;
        case XK_backslash :    return Key::BackSlash;
        case 0x7c:             return Key::Pipe;
        case XK_equal :        return Key::Equal;
        case XK_bracketleft :  return Key::LBracket;
        case XK_bracketright : return Key::RBracket;
        case XK_comma :        return Key::Comma;
        case XK_period :       return Key::Period;
        case XK_dead_acute :   return Key::Quote;
        case 0x60/*XK_dead_grave*/ :    return Key::Tilde;
        case XK_space :        return Key::Space;
        case XK_Return :       return Key::Return;
        case XK_KP_Enter :     return Key::Return;
        case XK_BackSpace :    return Key::Back;
        case XK_Tab :          return Key::Tab;
        case XK_Prior :        return Key::PageUp;
        case XK_Next :         return Key::PageDown;
        case XK_End :          return Key::End;
        case XK_Home :         return Key::Home;
        case XK_Insert :       return Key::Insert;
        case XK_Delete :       return Key::Delete;
        case XK_Pause :        return Key::Pause;
        case XK_F1 :           return Key::F1;
        case XK_F2 :           return Key::F2;
        case XK_F3 :           return Key::F3;
        case XK_F4 :           return Key::F4;
        case XK_F5 :           return Key::F5;
        case XK_F6 :           return Key::F6;
        case XK_F7 :           return Key::F7;
        case XK_F8 :           return Key::F8;
        case XK_F9 :           return Key::F9;
        case XK_F10 :          return Key::F10;
        case XK_F11 :          return Key::F11;
        case XK_F12 :          return Key::F12;
        case XK_F13 :          return Key::F13;
        case XK_F14 :          return Key::F14;
        case XK_F15 :          return Key::F15;
        case XK_Left :         return Key::Left;
        case XK_Right :        return Key::Right;
        case XK_Up :           return Key::Up;
        case XK_Down :         return Key::Down;
        case XK_KP_0 :         return Key::Numpad0;
        case XK_KP_1 :         return Key::Numpad1;
        case XK_KP_2 :         return Key::Numpad2;
        case XK_KP_3 :         return Key::Numpad3;
        case XK_KP_4 :         return Key::Numpad4;
        case XK_KP_5 :         return Key::Numpad5;
        case XK_KP_6 :         return Key::Numpad6;
        case XK_KP_7 :         return Key::Numpad7;
        case XK_KP_8 :         return Key::Numpad8;
        case XK_KP_9 :         return Key::Numpad9;
        case XK_A :            return Key::A;
        case XK_Z :            return Key::Z;
        case XK_E :            return Key::E;
        case XK_R :            return Key::R;
        case XK_T :            return Key::T;
        case XK_Y :            return Key::Y;
        case XK_U :            return Key::U;
        case XK_I :            return Key::I;
        case XK_O :            return Key::O;
        case XK_P :            return Key::P;
        case XK_Q :            return Key::Q;
        case XK_S :            return Key::S;
        case XK_D :            return Key::D;
        case XK_F :            return Key::F;
        case XK_G :            return Key::G;
        case XK_H :            return Key::H;
        case XK_J :            return Key::J;
        case XK_K :            return Key::K;
        case XK_L :            return Key::L;
        case XK_M :            return Key::M;
        case XK_W :            return Key::W;
        case XK_X :            return Key::X;
        case XK_C :            return Key::C;
        case XK_V :            return Key::V;
        case XK_B :            return Key::B;
        case XK_N :            return Key::N;
        case XK_0 :            return Key::Num0;
        case XK_1 :            return Key::Num1;
        case XK_2 :            return Key::Num2;
        case XK_3 :            return Key::Num3;
        case XK_4 :            return Key::Num4;
        case XK_5 :            return Key::Num5;
        case XK_6 :            return Key::Num6;
        case XK_7 :            return Key::Num7;
        case XK_8 :            return Key::Num8;
        case XK_9 :            return Key::Num9;
    }
    return Key::Code(0);
}
