
/*-----------------------------------------------------------------------------

	3dNovac Core
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
/*-----------------------------------------------------------------------------

    Provide definitions for different event :
        System
            namespace Key     enum Code
            namespace Mouse   enum Button
            class Event
            {
                enum EventType
                union
                {
                    struct KeyEvent
                    struct MouseMoveEvent
                    struct MouseButtonEvent
                    struct MouseWheelEvent
                    struct SizeEvent
                }
            }

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_EVENT_H_
#define NC_CORE_SYSTEM_EVENT_H_

#include <queue>
#include "../../Define.h"

namespace Nc
{
    namespace System
    {
        namespace Key
        {
            enum Code
            {
                A = 'a',
                B = 'b',
                C = 'c',
                D = 'd',
                E = 'e',
                F = 'f',
                G = 'g',
                H = 'h',
                I = 'i',
                J = 'j',
                K = 'k',
                L = 'l',
                M = 'm',
                N = 'n',
                O = 'o',
                P = 'p',
                Q = 'q',
                R = 'r',
                S = 's',
                T = 't',
                U = 'u',
                V = 'v',
                W = 'w',
                X = 'x',
                Y = 'y',
                Z = 'z',
                Num0 = '0',
                Num1 = '1',
                Num2 = '2',
                Num3 = '3',
                Num4 = '4',
                Num5 = '5',
                Num6 = '6',
                Num7 = '7',
                Num8 = '8',
                Num9 = '9',
                Escape = 256,
                LControl,
                LShift,
                LAlt,
                LSystem,      // OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
                RControl,
                RShift,
                RAlt,
                RSystem,      // OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
                Menu,
                LBracket,     // [
                RBracket,     // ]
                Inf,          // <
                Sup,          // >
                SemiColon,    // ;
                Comma,        // ,
                Period,       // .
                Quote,        // '
                Slash,        // /
                BackSlash,
                Pipe,         // |
                Underscore,   // _
                Tilde,        // ~
                Equal,        // =
                Space,
                Return,
                Back,
                Tab,
                PageUp,
                PageDown,
                End,
                Home,
                Insert,
                Delete,
                Add,          // +
                Subtract,     // -
                Multiply,     // *
                Divide,       // /
                Left,         // Left arrow
                Right,        // Right arrow
                Up,           // Up arrow
                Down,         // Down arrow
                Numpad0,
                Numpad1,
                Numpad2,
                Numpad3,
                Numpad4,
                Numpad5,
                Numpad6,
                Numpad7,
                Numpad8,
                Numpad9,
                F1,
                F2,
                F3,
                F4,
                F5,
                F6,
                F7,
                F8,
                F9,
                F10,
                F11,
                F12,
                F13,
                F14,
                F15,
                Pause,

                Count // total number of keyboard keys
            };
        }

        namespace Mouse
        {
            enum Button
            {
                Left,
                Right,
                Middle,
                XButton1,
                XButton2,

                Count // total number of mouse buttons
            };
        }

        struct LCORE Event
        {
            struct KeyEvent
            {
                Key::Code Code;
                bool      Alt;
                bool      Control;
                bool      Shift;
            };

            struct MouseMoveEvent
            {
                int X;
                int Y;
            };

            struct MouseButtonEvent
            {
                Mouse::Button Button;
                int           X;
                int           Y;
            };

            struct MouseWheelEvent
            {
                int Delta;
            };

            struct SizeEvent
            {
                unsigned int Width;
                unsigned int Height;
            };

            enum EventType
            {
                Closed,
                Resized,
                LostFocus,
                GainedFocus,
                TextEntered, // TODO
                KeyPressed,
                KeyReleased,
                MouseWheelMoved,
                MouseButtonPressed,
                MouseButtonReleased,
                MouseMoved,
                MouseEntered,
                MouseLeft,

                Count // total number of event types
            };

            EventType Type; // Type of the event

            union
            {
                KeyEvent            Key;
                MouseMoveEvent      MouseMove;
                MouseButtonEvent    MouseButton;
                MouseWheelEvent     MouseWheel;
                SizeEvent           Size;
                void                *Data;
            };

            Event() {}
            Event(EventType type) {Type = type;}

            bool operator == (const Event &e);
        };
    }
}

#endif
