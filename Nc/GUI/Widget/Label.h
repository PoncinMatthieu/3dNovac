
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

    File Created At:        03/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_LABEL_H_
#define NC_GUI_WIDGET_LABEL_H_

#include <string.h>

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Defines a label Widget
        /**
            Allow to display text and also to display a description text next to another widget by using the method 'AddBuddy'.
            The method will add the widget as a child and configure it so it can be displayed next to the label depending on it's alignment.
            The label can also display an icon next to the text using the method Icon().
        */
        class LIB_NC_GUI  Label : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::Label);

            public:
                Label(  const Utils::Unicode::UTF32 &label, float fontSize, const Color &fontColor, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0,0),
                        const std::string &ttf = "arial", Graphic::PlainTextFormater::Style s = Graphic::PlainTextFormater::Regular);
                Label(const Label &w);
                Label &operator = (const Label &w);
                virtual ~Label();

                virtual ISceneNode  *Clone() const                                              {return new Label(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Add the given widget as a child to the label and setup the widget so it will be positioned next to the label. */
                void                            AddBuddy(Widget *w);

                /** \return the label size. */
                inline const Vector2f           &TextSize()                                     {return _label->Size();}
                /** Set the label text. */
                inline void                     PlainText(const Utils::Unicode::UTF32 &text)    {_label->PlainText(text); UpdateLabel(); _stateChanged = true;}
                /** \return the label. */
                inline const Graphic::Text      *Text() const                                   {return _label;}
                /** \return the label. */
                inline Graphic::Text            *Text()                                         {return _label;}
                /** Initialize the text with a string, a color, a font and a style. */
                void                            Text(const Utils::Unicode::UTF32 &l, float fontSize = 15, const Color &fontColor = Color(0,0,0), const std::string &ttf = "arial", Graphic::PlainTextFormater::Style fontStyle = Graphic::PlainTextFormater::Regular);

                /** Set the icon of the label. */
                void                            Icon(const Graphic::GL::Texture &texture, const Box2i &textureBox);
                /** Set the icon of the label. */
                void                            Icon(const Graphic::GL::Texture &texture);

                /** \return the alignment mask of the label. */
                inline const AlignmentMask      &Alignment() const                              {return _labelAlignment;}
                /** \return the alignment mask of the label. */
                inline AlignmentMask            &Alignment()                                    {return _labelAlignment;}
                /** Set the alignment of the label. */
                inline void                     Alignment(const AlignmentMask &mask)            {_labelAlignment = mask;}

                /** Highlight the text. */
                inline void                     Highlight(bool state)                           {_highlight = state; _stateChanged = true;}
                /** \return whether the text is highlighted or not. */
                inline bool                     Highlighted() const                             {return _highlight;}
                /** Set the highlight color. */
                inline void                     HighlightColor(const Color &c)                  {_highlightColor = c; _stateChanged = true;}

            protected:
                /** Update the geometry of the label. */
                virtual void        UpdateState();
                /** Render the widget labeled. */
                virtual void        Draw(Graphic::SceneGraph *scene);

                /** Set the size of the widget. */
                void                RecomputeWidgetSize();
                /** Update the label matrix. */
                virtual void        UpdateLabel();
                /** Update the icon matrix. */
                virtual void        UpdateIcon();

                /** \return a vector to translate the childs when we call the RelativePos method. */
                virtual void        PosChild(const Widget *, Vector2i &) const;
                /** \return the size that the child should use to set up it's percent size when we call the Resized method. */
                virtual void        SizeChild(const Widget *, Vector2i &size) const;

            private:
                /** Copy the widget properties. */
                void                Copy(const Label &w);

            protected:
                Graphic::Text       *_label;            ///< the label if not null.
                Graphic::Sprite     *_icon;             ///< the icon if not null.
                AlignmentMask       _labelAlignment;    ///< describe the alignment of the label.

                bool                _highlight;         ///< if true, the label will be highlighted.
                Color               _highlightColor;    ///< color used to render the highlight.
                Graphic::Sprite     *_highlightSprite;  ///< sprite used to render the highlight text.
        };
    }
}

#endif
