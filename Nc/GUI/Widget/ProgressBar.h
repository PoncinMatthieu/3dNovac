
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        28/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_PROGRESS_BAR_H_
#define NC_GUI_WIDGET_PROGRESS_BAR_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Defines a progressBar Widget.
        /**
            \todo Recode the progress bar to use the Widget Looks.
        */
        class LIB_NC_GUI  ProgressBar : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::ProgressBar);

            public:
                ProgressBar(const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0), const Utils::FileName &file = "Nc:GUI:ProgressBar.png");
                ProgressBar(const AlignmentMask &alignment, const Vector2i &size, const Graphic::GL::Texture &texture);
                virtual ~ProgressBar();

                virtual ISceneNode  *Clone() const                              {return new ProgressBar(*this);}

                /** Set the box of the progress bar. */
                void    SetProgressBox(const Box2f &b)                          {if (_progressBox != b) {_progressBox = b; _size = b.Size(); _stateChanged = true;}}
                /** Set the color left and right of the progress bar. */
                void    SetColor(const Color &c)                                {if (_colorLeft != c || _colorRight != c) {_colorLeft = _colorRight = c; _stateChanged = true;}}
                /** Set the color left and right of the progress bar. */
                void    SetColor(const Color &c1, const Color &c2)              {if (_colorLeft != c1 || _colorRight != c2) {_colorLeft = c1; _colorRight = c2; _stateChanged = true;}}

                /** Set the percent of the progress bar. */
                void    Percent(float p);
                /** Set the number of evolution of the progress bar. */
                void    NbEvolution(unsigned int nb);
                /** Add an evolution to the progress bar. */
                void    Evolution();

            protected:
                /** update the geometry of the progress bar. */
                virtual void UpdateState();
                /** Render the progress bar. */
                virtual void Draw(Graphic::SceneGraph *scene);

            private:
                void    Init(const Graphic::GL::Texture &texture);

            protected:
                unsigned int            _indexDrawable;

                Box2f                   _progressBox;           ///< The progress box to delimit the progress bar.
                unsigned int            _nbEvolution;           ///< The number of evolution, used to compute the percent of the progress bar.
                float                   _percent;               ///< The progression in percent.
                Color                   _colorLeft;             ///< The color left of the progress bar.
                Color                   _colorRight;            ///< The color right of the progress bar.
        };
    }
}

#endif
