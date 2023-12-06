/*****************************************************************************
 * custom_menus.hpp : Qt custom menus classes
 *****************************************************************************
 * Copyright © 2006-2018 VideoLAN authors
 *                  2018 VideoLabs
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#ifndef CUSTOM_MENUS_HPP
#define CUSTOM_MENUS_HPP

#include "qt.hpp"


#include <QMenu>
#include <QPointer>
#include "medialibrary/mlrecentsmodel.hpp"

class QAbstractListModel;
class PlatformAgnosticMenu;
class PlatformAgnosticAction;
class PlatformAgnosticActionGroup;

class RendererAction : public QObject
{
    Q_OBJECT

    friend class RendererMenu;

    public:
        RendererAction( vlc_renderer_item_t *, QObject* parent = nullptr );
        ~RendererAction();
        vlc_renderer_item_t *getItem();

    private:
        QPointer<PlatformAgnosticAction> m_action;
        vlc_renderer_item_t *p_item;
};

class RendererMenu : public QObject
{
    Q_OBJECT

    friend class VLCMenuBar;
    friend class QmlRendererMenu;

public:
    RendererMenu( qt_intf_t *, QObject * parent = nullptr );
    virtual ~RendererMenu();
    void reset();

    void popup(const QPoint& pos);

signals:
    void aboutToHide();
    void aboutToShow();

private slots:
    void addRendererItem( vlc_renderer_item_t * );
    void removeRendererItem( vlc_renderer_item_t * );
    void updateStatus( int );
    void RendererSelected( QObject* );

private:
    void addRendererAction( RendererAction * );
    void removeRendererAction( RendererAction * );
    static vlc_renderer_item_t* getMatchingRenderer( const QVariant &,
                                                     vlc_renderer_item_t* );
    QPointer<PlatformAgnosticMenu> m_menu;
    QPointer<QObject> m_progressIndicator;
    QPointer<PlatformAgnosticAction> m_statusAction;
    QPointer<PlatformAgnosticActionGroup> m_group;
    qt_intf_t *p_intf;
};


/*
 * Construct a menu from a QAbstractListModel with Qt::DisplayRole and Qt::CheckStateRole
 */
class CheckableListMenu : public QObject
{
    Q_OBJECT
public:
    enum GroupingMode {
        GROUPED_EXLUSIVE,
        GROUPED_OPTIONAL,
        UNGROUPED
    };

    /**
     * @brief CheckableListMenu
     * @param title the title of the menu
     * @param model the model to observe, the model should provide at least Qt::DisplayRole and Qt::CheckStateRole
     * @param grouping whether the menu should use an ActionGroup or not
     * @param parent QObject parent
     */
    CheckableListMenu(QString title, QAbstractListModel* model,  GroupingMode grouping = UNGROUPED, QObject* parent = nullptr);

    PlatformAgnosticMenu *operator()() const;

private slots:
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onRowInserted(const QModelIndex &parent, int first, int last);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    void onModelAboutToBeReset();
    void onModelReset();

private:
    QPointer<PlatformAgnosticMenu> m_menu;
    QAbstractListModel* m_model;
    GroupingMode m_grouping;
    PlatformAgnosticActionGroup* m_actionGroup = nullptr;
};

// NOTE: This class is a helper to populate and maintain a QMenu from an QAbstractListModel.
class ListMenuHelper : public QObject
{
    Q_OBJECT

public:
    // NOTE: The model actions will be inserted before 'before' or at the end if it's NULL.
    ListMenuHelper(PlatformAgnosticMenu * menu, QAbstractListModel * model, PlatformAgnosticAction * before = nullptr,
                   QObject * parent = nullptr);

public: // Interface
    int count() const;

private slots:
    void onRowsInserted(const QModelIndex & parent, int first, int last);
    void onRowsRemoved (const QModelIndex & parent, int first, int last);

    void onDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight,
                       const QVector<int> & roles = QVector<int>());

    void onModelReset();

    void onTriggered(bool checked);

signals:
    void select(int index);

    void countChanged(int count);

private:
    PlatformAgnosticMenu * m_menu = nullptr;

    PlatformAgnosticActionGroup * m_group = nullptr;

    QAbstractListModel * m_model = nullptr;

    QList<PlatformAgnosticAction *> m_actions;

    PlatformAgnosticAction * m_before = nullptr;
};

/**
 * @brief The BooleanPropertyAction class allows to bind a boolean Q_PROPERTY to a QAction
 */
class BooleanPropertyAction: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief BooleanPropertyAction
     * @param title the title of the menu
     * @param model the object to observe
     * @param propertyName the name of the property on the @a model
     * @param parent QObject parent
     */
    BooleanPropertyAction(QString title, QObject* model , QString propertyName, QObject* parent = nullptr);

    PlatformAgnosticAction *operator()() const;

private slots:
    void setModelChecked(bool checked);
private:
    QPointer<PlatformAgnosticAction> m_action;

    QObject* m_model;
    QString m_propertyName;
};

class RecentMenu : public QObject
{
    Q_OBJECT
public:
    RecentMenu(MLRecentsModel* model, MediaLib* ml, QObject* parent = nullptr);

    PlatformAgnosticMenu *operator()() const;

private slots:
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onRowInserted(const QModelIndex &parent, int first, int last);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
    void onModelReset();

private:
    QPointer<PlatformAgnosticMenu> m_menu;

    MLRecentsModel* m_model = nullptr;
    PlatformAgnosticAction* m_separator = nullptr;
    MediaLib* m_ml = nullptr;

    QList<PlatformAgnosticAction *> m_actions;
};

class BookmarkMenu : public QObject
{
    Q_OBJECT

public:
    BookmarkMenu(MediaLib * mediaLib, vlc_player_t * player, QObject* parent = nullptr);

    PlatformAgnosticMenu *operator()() const;

private:
    QPointer<PlatformAgnosticMenu> m_menu;
};

#endif // CUSTOM_MENUS_HPP
