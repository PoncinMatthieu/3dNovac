
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

#ifndef NC_GUI_LABEL_H_
#define NC_GUI_LABEL_H_

#include <string.h>

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a widget with a label.
        class LGUI  Label : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::Label);

            public:
                Label(  const Utils::Unicode::UTF32 &label, float charSize, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0,0),
                        const std::string &ttf = "arial", Graphic::String::Style s = Graphic::String::Regular);
                Label(const Label &w);
                Label &operator = (const Label &w);
                virtual ~Label();

                virtual ISceneNode  *Clone() const                                              {return new Label(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Add the given widget as a child to the label and setup the widget so it will be positioned next to the label. */
                void                            AddBuddy(Widget *w);

                /** Create the label with a text, a font and a style. */
                virtual void                    CreateLabel(const Utils::Unicode::UTF32 &l, float charSize, const std::string &ttf, Graphic::String::Style s);
                /** Set the char size of the label. */
                inline void                     CharSize(int charSize)                          {_label->CharSize(charSize);}
                /** \return the label size. */
                inline const Vector2f           &LabelSize()                                    {return _label->Size();}
                /** Set the label text. */
                inline void                     Text(const Utils::Unicode::UTF32 &text)         {_label->Text(text); UpdateLabel(); _stateChanged = true;}
                /** Set the label color. */
                inline void                     TextColor(const Color &color)                   {_label->SetColor(color); _stateChanged = true;}
                /** \return the label. */
                inline const Graphic::String    *GetLabel() const                               {return _label;}
                /** \return the label. */
                inline Graphic::String          *GetLabel()                                     {return _label;}

                /** \return the alignment mask of the label. */
                inline const AlignmentMask      &LabelAlignment() const                         {return _labelAlignment;}
                /** \return the alignment mask of the label. */
                inline AlignmentMask            &LabelAlignment()                               {return _labelAlignment;}
                /** Set the alignment of the label. */
                inline void                     LabelAlignment(const AlignmentMask &mask)       {_labelAlignment = mask;}

            protected:
                /** Update the geometry of the label. */
                virtual void        Update();
                /** Render the widget labeled. */
                virtual void        Draw(Graphic::SceneGraph *scene);

                /** Update the label matrix. */
                void                UpdateLabel();

                /** \return a vector to translate the childs when we call the GetReelPos method. */
                virtual void        PosChild(const Widget *, Vector2i &) const;
                /** \return the size that the child should use to set up it's percent size when we call the Resized method. */
                virtual void        SizeChild(const Widget *, Vector2i &size) const;

            private:
                /** Copy the widget properties. */
                void                Copy(const Label &w);

            protected:
                Graphic::String     *_label;            ///< the label (string).
                AlignmentMask       _labelAlignment;    ///< describe the alignement of the label.
        };
    }
}

#endif
