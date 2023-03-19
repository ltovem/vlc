// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * bookmarks.hpp : bookmarks
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Antoine Lejeune <phytos@via.ecp.fr>
 *****************************************************************************/


#ifndef QVLC_BOOKMARKS_H_
#define QVLC_BOOKMARKS_H_ 1

#include "widgets/native/qvlcframe.hpp"
#include "util/singleton.hpp"

class QPushButton;
class MLBookmarkModel;
class QTreeView;

class BookmarksDialog : public QVLCFrame, public Singleton<BookmarksDialog>
{
    Q_OBJECT
public:
    void toggleVisible();
private:
    BookmarksDialog( qt_intf_t * );
    virtual ~BookmarksDialog();

    QTreeView *bookmarksList;
    QPushButton *clearButton;
    QPushButton *delButton;
    QPushButton *addButton;
    MLBookmarkModel* m_model;

private slots:
    void add();
    void del();
    void clear();
    void extract();
    void activateItem( const QModelIndex& index );
    void updateButtons();

    friend class    Singleton<BookmarksDialog>;
};

#endif

