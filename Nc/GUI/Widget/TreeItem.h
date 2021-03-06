
#ifndef NC_GUI_WIDGET_TREEITEM_H_
#define NC_GUI_WIDGET_TREEITEM_H_

#include "Item.h"

namespace Nc
{
    namespace GUI
    {
        /// Define an Item meant to be used with a TreeWidget.
        class LIB_NC_GUI TreeItem : public Item
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Item, Nc::System::Object, Nc::GUI::TreeItem);

            public:
                TreeItem(const Utils::Unicode::UTF32 &label = "", float fontSize = 15, const Color &fontColor = Color(0,0,0), const std::string &fontName = "arial", Nc::Graphic::PlainTextFormater::Style s = Nc::Graphic::PlainTextFormater::Regular);
                ~TreeItem();

                /** \return a clone of the item. */
                virtual Graphic::ISceneNode         *Clone() const                          {return new TreeItem(*this);}

                /** Select or unselect the item. */
                bool            Selected() const        {return _selected;}
                /** Select or unselect the item. */
                virtual void    Selected(bool st);

                /** \return true if the item is folded. */
                bool            Folded() const          {return _folded;}
                /** Fold or unfold the item. */
                virtual void    Folded(bool st);
                /** Fold the item. */
                void            Fold()                  {Folded(true);}
                /** Unfold the item. */
                void            Unfold()                {Folded(false);}

                /** Revursively compute the full size of the item tree and set the position of every item inside the tree. */
                Vector2i    ComputeSizePosRecursif(unsigned int depth);

                /** Called when the item is double clicked. */
                virtual void    DoubleClicked();

            protected:
                virtual bool    RenderChildsBegin(Graphic::SceneGraph *scene);

                virtual void    PosChild(const Widget *w, Vector2i &pos) const;

                TreeWidget      *GetParentTreeWidget();

            protected:
                bool        _folded;
        };
    }
}

#endif
