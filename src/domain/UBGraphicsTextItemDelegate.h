/*
 * Copyright (C) 2010-2013 Groupement d'Intérêt Public pour l'Education Numérique en Afrique (GIP ENA)
 *
 * This file is part of Open-Sankoré.
 *
 * Open-Sankoré is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License,
 * with a specific linking exception for the OpenSSL project's
 * "OpenSSL" library (or with modified versions of it that use the
 * same license as the "OpenSSL" library).
 *
 * Open-Sankoré is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Open-Sankoré.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef UBGRAPHICSTEXTITEMDELEGATE_H_
#define UBGRAPHICSTEXTITEMDELEGATE_H_

#include <QtGui>

#include <QtSvg>

#include "core/UB.h"
#include "UBGraphicsItemDelegate.h"
#include "gui/UBMainWindow.h"
#include "gui/UBCreateTablePalette.h"
#include "gui/UBCreateHyperLinkPalette.h"

class UBGraphicsTextItem;

class UBGraphicsTextItemDelegate : public UBGraphicsItemDelegate
{
    Q_OBJECT

    enum textChangeMode
    {
        changeSize = 0,
        scaleSize
    };

    public:
        UBGraphicsTextItemDelegate(UBGraphicsTextItem* pDelegated, QObject * parent = 0);
        virtual ~UBGraphicsTextItemDelegate();
        bool isEditable();
        void scaleTextSize(qreal multiplyer);
        virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value);

        UBCreateTablePalette* tablePalette();
        UBCreateHyperLinkPalette* linkPalette();

        void changeDelegateButtonsMode(bool htmlMode);

    public slots:
        void contentsChanged();
        virtual void setEditable(bool);
        virtual void remove(bool canUndo);

    protected:
        virtual void buildButtons();
        virtual void decorateMenu(QMenu *menu);
        virtual void updateMenuActionState();

        virtual void positionHandles();

    private:

        UBGraphicsTextItem* delegated();

        DelegateButton* mFontButton;
        DelegateButton* mFontBoldButton;        
        DelegateButton* mFontItalicButton;
        DelegateButton* mFontUnderlineButton;
        DelegateButton* mColorButton;
        DelegateButton* mDecreaseSizeButton;
        DelegateButton* mIncreaseSizeButton;
        DelegateButton* mBackgroundColorButton;
        DelegateButton* mTableButton;
        DelegateButton* mLeftAlignmentButton;
        DelegateButton* mCenterAlignmentButton;
        DelegateButton* mRightAlignmentButton;
        DelegateButton* mCodeButton;
        DelegateButton* mListButton;
        DelegateButton* mAddIndentButton;
        DelegateButton* mRemoveIndentButton;
        DelegateButton* mHyperLinkButton;

        UBCreateTablePalette* mTablePalette;
        UBCreateHyperLinkPalette* mLinkPalette;

        int mLastFontPixelSize;

        static const int sMinPixelSize;
        static const int sMinPointSize;

    private:
        void customize(QFontDialog &fontDialog);
        void ChangeTextSize(qreal factor, textChangeMode changeMode);

        QFont createDefaultFont();
        QAction *mEditableAction;

    private slots:

        void pickFont();
        void setFontBold();
        void setFontItalic();
        void setFontUnderline();
        void pickColor();
        void alternHtmlMode();

        void decreaseSize();
        void increaseSize();

        void pickBackgroundColor();
        void setTableSize();
        void insertTable();
        void addIndent();
        void removeIndent();
        void insertList();
        void setAlignmentToLeft();
        void setAlignmentToCenter();
        void setAlignmentToRight();
        void addLink();
        void insertLink();

private:
      const int delta;

};

#endif /* UBGRAPHICSTEXTITEMDELEGATE_H_ */
