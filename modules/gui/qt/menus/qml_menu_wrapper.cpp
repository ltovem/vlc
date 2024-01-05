/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#include "qml_menu_wrapper.hpp"
#include "menus.hpp"
#include "medialibrary/medialib.hpp"
#include "medialibrary/mlplaylistlistmodel.hpp"
#include "medialibrary/mlplaylistmodel.hpp"
#include "medialibrary/mlbookmarkmodel.hpp"
#include "network/networkdevicemodel.hpp"
#include "network/networkmediamodel.hpp"
#include "playlist/playlist_controller.hpp"
#include "playlist/playlist_model.hpp"
#include "dialogs/dialogs_provider.hpp"

// Qt includes
#include <QPainter>
#include <QSignalMapper>
#include <QScreen>
#include <QActionGroup>

#include "widgets/native/platformagnosticmenu.hpp"
#include "widgets/native/platformagnosticaction.hpp"
#include "widgets/native/platformagnosticactiongroup.hpp"

static inline void popupMenu(PlatformAgnosticMenu* const menu, const QPoint& point, const bool above)
{
    assert(menu);
    if (above)
        menu->popup(QPoint(point.x(), point.y() - menu->sizeHint().height()));
    else
        menu->popup(point);
}

void StringListMenu::popup(const QPoint &point, const QVariantList &stringList)
{
    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));

    for (int i = 0; i != stringList.size(); ++i)
    {
        const auto str = stringList[i].toString();
        m_menu->addAction(str, this, [this, i, str]()
        {
            emit selected(i, str);
        });
    }

    m_menu->popup(point);
}

// SortMenu

void SortMenu::popup(const QPoint &point, const bool popupAbovePoint, const QVariantList &model)
{
    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));

    connect( m_menu.get(), &PlatformAgnosticMenu::aboutToShow, this, [this]() {
        m_shown = true;
        shownChanged();
    } );
    connect( m_menu.get(), &PlatformAgnosticMenu::aboutToHide, this, [this]() {
        m_shown = false;
        shownChanged();
    } );

    // model => [{text: "", checked: <bool>, order: <sort order> if checked else <invalid>}...]
    for (int i = 0; i != model.size(); ++i)
    {
        const auto obj = model[i].toMap();

        auto action = m_menu->addAction(obj.value("text").toString());
        action->setCheckable(true);

        const bool checked = obj.value("checked").toBool();
        action->setChecked(checked);

        if (checked)
        {
            const QLatin1String themeIconAscending{"view-sort-ascending"};
            const QLatin1String themeIconDescending{"view-sort-descending"};
            const QLatin1String *themeIcon;

            switch (static_cast<Qt::SortOrder>(obj.value("order").toInt())) {
            case Qt::AscendingOrder:
                themeIcon = &themeIconAscending;
                break;
            case Qt::DescendingOrder:
                themeIcon = &themeIconDescending;
                break;
            default:
                Q_UNREACHABLE();
            }

            action->setIcon(*themeIcon, false);
        }

        connect(action, &PlatformAgnosticAction::triggered, this, [this, i]()
        {
            emit selected(i);
        });
    }

    onPopup(m_menu.get());

    popupMenu(m_menu.get(), point, popupAbovePoint);
}

void SortMenu::close()
{
    if (m_menu)
        m_menu->close();
}

// Protected functions

/* virtual */ void SortMenu::onPopup(PlatformAgnosticMenu *) {}

// SortMenuVideo

// Protected SortMenu reimplementation

void SortMenuVideo::onPopup(PlatformAgnosticMenu * menu) /* override */
{
    if (!getctx())
        return;

    menu->addSeparator();

    struct
    {
        const char * title;

        MainCtx::Grouping grouping;
    }
    entries[] =
    {
        { N_("Do not group videos"), MainCtx::GROUPING_NONE },
        { N_("Group by name"), MainCtx::GROUPING_NAME },
        { N_("Group by folder"), MainCtx::GROUPING_FOLDER },
    };

    PlatformAgnosticActionGroup * group = PlatformAgnosticActionGroup::createActionGroup(menu);

    int index = getctx()->grouping();

    for (size_t i = 0; i < ARRAY_SIZE(entries); i++)
    {
        PlatformAgnosticAction * action = menu->addAction(qtr(entries[i].title));

        action->setCheckable(true);

        MainCtx::Grouping grouping = entries[i].grouping;

        connect(action, &PlatformAgnosticAction::triggered, this, [this, grouping]()
        {
            emit this->grouping(grouping);
        });

        group->addAction(action);

        if (index == grouping)
            action->setChecked(true);
    }
}

QmlGlobalMenu::QmlGlobalMenu(QObject *parent)
    : VLCMenuBar(parent)
{
}

void QmlGlobalMenu::popup(QPoint pos)
{
    if (!m_ctx)
        return;

    qt_intf_t* p_intf = m_ctx->getIntf();
    if (!p_intf)
        return;

    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));
    PlatformAgnosticMenu* submenu;

    connect( m_menu.get(), &PlatformAgnosticMenu::aboutToShow, this, [this]() {
        m_shown = true;
        shownChanged();
        aboutToShow();
    });
    connect( m_menu.get(), &PlatformAgnosticMenu::aboutToHide, this, [this]() {
        m_shown = false;
        shownChanged();
        aboutToHide();
    });

    submenu = m_menu->addMenu(qtr( "&Media" ));
    FileMenu( p_intf, submenu );

    /* Dynamic menus, rebuilt before being showed */
    submenu = m_menu->addMenu(qtr( "P&layback" ));
    NavigMenu( p_intf, submenu );

    submenu = m_menu->addMenu(qtr( "&Audio" ));
    AudioMenu( p_intf, submenu );

    submenu = m_menu->addMenu(qtr( "&Video" ));
    VideoMenu( p_intf, submenu );

    submenu = m_menu->addMenu(qtr( "Subti&tle" ));
    SubtitleMenu( p_intf, submenu );

    submenu = m_menu->addMenu(qtr( "Tool&s" ));
    ToolsMenu( p_intf, submenu );

    /* View menu, a bit different */
    submenu = m_menu->addMenu(qtr( "V&iew" ));
    ViewMenu( p_intf, submenu );

    submenu = m_menu->addMenu(qtr( "&Help" ));
    HelpMenu(submenu);

    m_menu->popup(pos);
}

QmlMenuBar::QmlMenuBar(QObject *parent)
    : VLCMenuBar(parent)
{
}

bool QmlMenuBar::eventFilter(QObject *watched, QEvent *event)
{
    if (!(qobject_cast<QMenu*>(watched) || qobject_cast<QQuickItem*>(watched)))
        return false;

    switch (event->type())
    {
    case QEvent::MouseMove: // X11
    {
        const auto globalPos = static_cast<QMouseEvent*>(event)->globalPos();

        if (getmenubar()->contains(getmenubar()->mapFromGlobal(globalPos))
            && !m_button->contains(m_button->mapFromGlobal(globalPos)))
        {
            setopenMenuOnHover(true);
            m_menu->close();

            event->accept();
            return true;
        }

        return false;
    }
    case QEvent::KeyPress:
    {
        const auto keyEvent = static_cast<QKeyEvent*>(event);

        if ((keyEvent->key() == Qt::Key_Left  || keyEvent->key() == Qt::Key_Right))
        {
            event->accept();
            emit navigateMenu(keyEvent->key() == Qt::Key_Left ? -1 : 1);
            return true;
        }

        return false;
    }

    default:
        return false;
    }
}

void QmlMenuBar::popupMenuCommon( QQuickItem* button, std::function<void(PlatformAgnosticMenu*)> createMenuFunc)
{
    if (!m_ctx || !m_menubar || !button)
        return;

    qt_intf_t* p_intf = m_ctx->getIntf();
    if (!p_intf)
        return;

    if (m_menu) // This is necessary on Wayland that previous menu must be closed before.
        m_menu->close();

    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));
    m_menu->installEventFilter(this);
    createMenuFunc(m_menu.get());
    m_button = button;
    m_openMenuOnHover = false;
    connect(m_menu.get(), &PlatformAgnosticMenu::aboutToHide, this, &QmlMenuBar::onMenuClosed);
    QPointF position = button->mapToGlobal(QPoint(0, button->height()));
    m_menu->popup(position.toPoint());
}

void QmlMenuBar::popupMediaMenu( QQuickItem* button )
{
    popupMenuCommon(button, [this](PlatformAgnosticMenu* menu) {
        qt_intf_t* p_intf = m_ctx->getIntf();
        FileMenu( p_intf, menu );
    });
}

void QmlMenuBar::popupPlaybackMenu( QQuickItem* button )
{
    popupMenuCommon(button, [this](PlatformAgnosticMenu* menu) {
        NavigMenu( m_ctx->getIntf(), menu );
    });
}

void QmlMenuBar::popupAudioMenu(QQuickItem* button )
{
    popupMenuCommon(button, [this](PlatformAgnosticMenu* menu) {
        AudioMenu( m_ctx->getIntf(), menu );
    });
}

void QmlMenuBar::popupVideoMenu( QQuickItem* button )
{
    popupMenuCommon(button, [this](PlatformAgnosticMenu* menu) {
        VideoMenu( m_ctx->getIntf(), menu );
    });
}

void QmlMenuBar::popupSubtitleMenu( QQuickItem* button )
{
    popupMenuCommon(button, [this](PlatformAgnosticMenu* menu) {
        SubtitleMenu( m_ctx->getIntf(), menu );
    });
}


void QmlMenuBar::popupToolsMenu( QQuickItem* button )
{
    popupMenuCommon(button, [this](PlatformAgnosticMenu* menu) {
        ToolsMenu( m_ctx->getIntf(), menu );
    });
}

void QmlMenuBar::popupViewMenu( QQuickItem* button )
{
    popupMenuCommon(button, [this](PlatformAgnosticMenu* menu) {
        qt_intf_t* p_intf = m_ctx->getIntf();
        ViewMenu( p_intf, menu );
    });
}

void QmlMenuBar::popupHelpMenu( QQuickItem* button )
{
    popupMenuCommon(button, [](PlatformAgnosticMenu* menu) {
        HelpMenu(menu);
    });
}

void QmlMenuBar::onMenuClosed()
{
    if (!m_openMenuOnHover)
        emit menuClosed();
}

// QmlBookmarkMenu

/* explicit */ QmlBookmarkMenu::QmlBookmarkMenu(QObject * parent)
    : QObject(parent)
{}

// Interface

/* Q_INVOKABLE */ void QmlBookmarkMenu::popup(const QPoint & position, const bool above)
{
    if (m_ctx == nullptr || m_player == nullptr)
        return;

    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));

    connect(m_menu.get(), &PlatformAgnosticMenu::aboutToHide, this, &QmlBookmarkMenu::aboutToHide);
    connect(m_menu.get(), &PlatformAgnosticMenu::aboutToShow, this, &QmlBookmarkMenu::aboutToShow);

    m_menu->addSeparator();
    PlatformAgnosticAction * sectionTitles    = m_menu->addAction(qtr("Titles"));
    m_menu->addSeparator();
    PlatformAgnosticAction * sectionChapters  = m_menu->addAction(qtr("Chapters"));
    m_menu->addSeparator();
    PlatformAgnosticAction * sectionBookmarks = nullptr;

    if (m_ctx->hasMediaLibrary())
    {
        m_menu->addSeparator();
        sectionBookmarks = m_menu->addAction(qtr("Bookmarks"));
        m_menu->addSeparator();
    }

    // Titles

    TitleListModel * titles = m_player->getTitles();

    sectionTitles->setEnabled(titles->rowCount() != 0);

    ListMenuHelper * helper = new ListMenuHelper(m_menu.get(), titles, sectionChapters, m_menu.get());

    connect(helper, &ListMenuHelper::select, [titles](int index)
    {
        titles->setData(titles->index(index), true, Qt::CheckStateRole);
    });

    connect(helper, &ListMenuHelper::countChanged, [sectionTitles](int count)
    {
        // NOTE: The section should only be visible when the model has content.
        sectionTitles->setEnabled(count != 0);
    });

    // Chapters

    ChapterListModel * chapters = m_player->getChapters();

    sectionChapters->setEnabled(chapters->rowCount() != 0);

    helper = new ListMenuHelper(m_menu.get(), chapters, sectionBookmarks, m_menu.get());

    connect(helper, &ListMenuHelper::select, [chapters](int index)
    {
        chapters->setData(chapters->index(index), true, Qt::CheckStateRole);
    });

    connect(helper, &ListMenuHelper::countChanged, [sectionChapters](int count)
    {
        // NOTE: The section should only be visible when the model has content.
        sectionChapters->setEnabled(count != 0);
    });

    // Bookmarks
    if (m_ctx->hasMediaLibrary())
    {
        // FIXME: Do we really need a translation call for the string shortcut ?
        m_menu->addAction(qtr("&Manage"), THEDP, &DialogsProvider::bookmarksDialog, qtr("Ctrl+B"));

        m_menu->addSeparator();

        MLBookmarkModel * bookmarks = new MLBookmarkModel(m_menu.get());
        bookmarks->setPlayer(m_player->getPlayer());
        bookmarks->setMl(m_ctx->getMediaLibrary());

        helper = new ListMenuHelper(m_menu.get(), bookmarks, nullptr, m_menu.get());

        connect(helper, &ListMenuHelper::select, [bookmarks](int index)
        {
            bookmarks->select(bookmarks->index(index, 0));
        });
    }

    popupMenu(m_menu.get(), position, above);
}

// QmlProgramMenu

/* explicit */ QmlProgramMenu::QmlProgramMenu(QObject * parent)
    : QObject(parent)
{}

// Interface

/* Q_INVOKABLE */ void QmlProgramMenu::popup(const QPoint & position, const bool above)
{
    if (m_player == nullptr)
        return;

    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));

    connect(m_menu.get(), &PlatformAgnosticMenu::aboutToHide, this, &QmlProgramMenu::aboutToHide);
    connect(m_menu.get(), &PlatformAgnosticMenu::aboutToShow, this, &QmlProgramMenu::aboutToShow);

    m_menu->addSeparator();
    m_menu->addAction(qtr("Programs"));
    m_menu->addSeparator();

    ProgramListModel * programs = m_player->getPrograms();

    ListMenuHelper * helper = new ListMenuHelper(m_menu.get(), programs, nullptr, m_menu.get());

    connect(helper, &ListMenuHelper::select, [programs](int index)
    {
        programs->setData(programs->index(index), true, Qt::CheckStateRole);
    });

    popupMenu(m_menu.get(), position, above);
}

// QmlRendererMenu

/* explicit */ QmlRendererMenu::QmlRendererMenu(QObject * parent)
    : QObject(parent)
{}

// Interface

/* Q_INVOKABLE */ void QmlRendererMenu::popup(const QPoint & position, const bool above)
{
    if (m_ctx == nullptr)
        return;

    m_menu = std::make_unique<RendererMenu>(m_ctx->getIntf(), m_ctx->quickWindow());

    connect(m_menu.get(), &RendererMenu::aboutToHide, this, &QmlRendererMenu::aboutToHide);
    connect(m_menu.get(), &RendererMenu::aboutToShow, this, &QmlRendererMenu::aboutToShow);

    popupMenu(m_menu->m_menu, position, above);
}

// Tracks

// QmlTrackMenu

/* explicit */ QmlTrackMenu::QmlTrackMenu(QObject * parent) : QObject(parent) {}

// Interface

/* Q_INVOKABLE */ void QmlTrackMenu::popup(const QPoint & position)
{
    m_menu = std::make_unique<QMenu>();

    beforePopup(m_menu.get());

    m_menu->popup(position);
}

// QmlSubtitleMenu

/* explicit */ QmlSubtitleMenu::QmlSubtitleMenu(QObject * parent) : QmlTrackMenu(parent) {}

// Protected QmlTrackMenu implementation

void QmlSubtitleMenu::beforePopup(QMenu * menu) /* override */
{
    menu->addAction(qtr("Open file"), this, [this]()
    {
        emit triggered(Open);
    });

    menu->addAction(QIcon(":/menu/sync.svg"), qtr("Synchronize"), this, [this]()
    {
        emit triggered(Synchronize);
    });

    menu->addAction(QIcon(":/menu/download.svg"), qtr("Search online"), this, [this]()
    {
        emit triggered(Download);
    });

    menu->addSeparator();

    QAction * action = menu->addAction(qtr("Select multiple"), this, [this]()
    {
        TrackListModel * tracks = this->m_player->getSubtitleTracks();

        tracks->setMultiSelect(!(tracks->getMultiSelect()));
    });

    action->setCheckable(true);

    action->setChecked(m_player->getSubtitleTracks()->getMultiSelect());
}

// QmlAudioMenu

/* explicit */ QmlAudioMenu::QmlAudioMenu(QObject * parent) : QmlTrackMenu(parent) {}

// Protected QmlTrackMenu implementation

void QmlAudioMenu::beforePopup(QMenu * menu) /* override */
{
    menu->addAction(qtr("Open file"), this, [this]()
    {
        emit triggered(Open);
    });

    menu->addAction(QIcon(":/menu/sync.svg"), qtr("Synchronize"), this, [this]()
    {
        emit triggered(Synchronize);
    });
}

//=================================================================================================
// PlaylistListContextMenu
//=================================================================================================

PlaylistListContextMenu::PlaylistListContextMenu(QObject * parent)
    : QObject(parent)
{}


void PlaylistListContextMenu::popup(const QModelIndexList & selected, QPoint pos, QVariantMap)
{
    if (!m_model)
        return;

    QVariantList ids;

    for (const QModelIndex & modelIndex : selected)
        ids.push_back(m_model->data(modelIndex, MLPlaylistListModel::PLAYLIST_ID));

    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));

    MediaLib * ml = m_model->ml();

    PlatformAgnosticAction * action = m_menu->addAction(qtr("Add and play"));

    connect(action, &PlatformAgnosticAction::triggered, ml, [ml, ids]() {
        ml->addAndPlay(ids);
    });

    action = m_menu->addAction(qtr("Enqueue"));

    connect(action, &PlatformAgnosticAction::triggered, ml, [ml, ids]() {
        ml->addToPlaylist(ids);
    });

    if (ids.count() == 1)
    {
        action = m_menu->addAction(qtr("Rename"));

        QModelIndex index = selected.first();

        connect(action, &PlatformAgnosticAction::triggered, ml, [this, index]() {
            m_model->showDialogRename(index);
        });
    }

    action = m_menu->addAction(qtr("Delete"));

    connect(action, &PlatformAgnosticAction::triggered, ml, [this, ids]() {
        m_model->deletePlaylists(ids);
    });

    m_menu->popup(pos);
}

//=================================================================================================
// PlaylistMediaContextMenu
//=================================================================================================

PlaylistMediaContextMenu::PlaylistMediaContextMenu(QObject * parent) : QObject(parent) {}

void PlaylistMediaContextMenu::popup(const QModelIndexList & selected, QPoint pos,
                                     QVariantMap options)
{
    if (!m_model)
        return;

    QVariantList ids;

    for (const QModelIndex& modelIndex : selected)
        ids.push_back(m_model->data(modelIndex, MLPlaylistModel::MEDIA_ID));

    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));

    MediaLib * ml = m_model->ml();

    PlatformAgnosticAction * action = m_menu->addAction(qtr("Add and play"));

    connect(action, &PlatformAgnosticAction::triggered, ml, [ml, ids]() {
        ml->addAndPlay(ids);
    });

    action = m_menu->addAction(qtr("Enqueue"));

    connect(action, &PlatformAgnosticAction::triggered, ml, [ml, ids]() {
        ml->addToPlaylist(ids);
    });

    action = m_menu->addAction(qtr("Add to playlist"));

    connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), [ids]() {
        DialogsProvider::getInstance()->playlistsDialog(ids);
    });

    action = m_menu->addAction(qtr("Play as audio"));

    connect(action, &PlatformAgnosticAction::triggered, ml, [ml, ids]() {
        ml->addAndPlay(ids, {":no-video"});
    });

    if (options.contains("information") && options["information"].type() == QVariant::Int) {
        action = m_menu->addAction(qtr("Information"));

        QSignalMapper * mapper = new QSignalMapper(m_menu.get());

        connect(action, &PlatformAgnosticAction::triggered, mapper, QOverload<>::of(&QSignalMapper::map));

        mapper->setMapping(action, options["information"].toInt());
        connect(mapper, QSIGNALMAPPER_MAPPEDINT_SIGNAL,
                this, &PlaylistMediaContextMenu::showMediaInformation);
    }

    m_menu->addSeparator();

    action = m_menu->addAction(qtr("Remove Selected"));

    action->setIcon(":/menu/remove.svg");

    connect(action, &PlatformAgnosticAction::triggered, this, [this, selected]() {
        m_model->remove(selected);
    });

    m_menu->popup(pos);
}

//=================================================================================================

NetworkMediaContextMenu::NetworkMediaContextMenu(QObject* parent)
    : QObject(parent)
{}

void NetworkMediaContextMenu::popup(const QModelIndexList& selected, QPoint pos)
{
    if (!m_model)
        return;

    m_menu = std::make_unique<QMenu>();
    QAction* action;

    action = m_menu->addAction( qtr("Add and play") );
    connect(action, &QAction::triggered, [this, selected]( ) {
        m_model->addAndPlay(selected);
    });

    action = m_menu->addAction( qtr("Enqueue") );
    connect(action, &QAction::triggered, [this, selected]( ) {
        m_model->addToPlaylist(selected);
    });

    bool canBeIndexed = false;
    unsigned countIndexed = 0;
    for (const QModelIndex& idx : selected)
    {
        QVariant canIndex = m_model->data(m_model->index(idx.row()), NetworkMediaModel::NETWORK_CANINDEX );
        if (canIndex.isValid() && canIndex.toBool())
            canBeIndexed = true;
        else
            continue;
        QVariant isIndexed = m_model->data(m_model->index(idx.row()), NetworkMediaModel::NETWORK_INDEXED );
        if (!isIndexed.isValid())
            continue;
        if (isIndexed.toBool())
            ++countIndexed;
    }

    if (canBeIndexed)
    {
        bool removeFromML = countIndexed > 0;
        action = m_menu->addAction(removeFromML
            ? qtr("Remove from Media Library")
            : qtr("Add to Media Library"));

        connect(action, &QAction::triggered, [this, selected, removeFromML]( ) {
            for (const QModelIndex& idx : selected) {
                m_model->setData(m_model->index(idx.row()), !removeFromML, NetworkMediaModel::NETWORK_INDEXED);
            }
        });
    }

    m_menu->popup(pos);
}

NetworkDeviceContextMenu::NetworkDeviceContextMenu(QObject* parent)
    : QObject(parent)
{}

void NetworkDeviceContextMenu::popup(const QModelIndexList& selected, QPoint pos)
{
    if (!m_model)
        return;

    m_menu = std::make_unique<QMenu>();
    QAction* action;

    action = m_menu->addAction( qtr("Add and play") );
    connect(action, &QAction::triggered, [this, selected]( ) {
        m_model->addAndPlay(selected);
    });

    action = m_menu->addAction( qtr("Enqueue") );
    connect(action, &QAction::triggered, [this, selected]( ) {
        m_model->addToPlaylist(selected);
    });

    m_menu->popup(pos);
}

PlaylistContextMenu::PlaylistContextMenu(QObject* parent)
    : QObject(parent)
{}

void PlaylistContextMenu::popup(int currentIndex, QPoint pos )
{
    if (!m_controler || !m_model || !m_selectionModel)
        return;

    m_menu = std::unique_ptr<PlatformAgnosticMenu>(PlatformAgnosticMenu::createMenu(m_ctx->quickWindow()));
    PlatformAgnosticAction* action;

    QList<QUrl> selectedUrlList;
    for (const int modelIndex : m_selectionModel->selectedIndexesFlat())
        selectedUrlList.push_back(m_model->itemAt(modelIndex).getUrl());

    PlaylistItem currentItem;
    if (currentIndex >= 0)
        currentItem = m_model->itemAt(currentIndex);

    if (currentItem)
    {
        action = m_menu->addAction( qtr("Play") );
        connect(action, &PlatformAgnosticAction::triggered, this, [this, currentIndex]( ) {
            m_controler->goTo(currentIndex, true);
        });

        m_menu->addSeparator();
    }

    if (m_selectionModel->hasSelection()) {
        action = m_menu->addAction( qtr("Stream") );
        connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), [selectedUrlList]( ) {
            DialogsProvider::getInstance()->streamingDialog(selectedUrlList, false);
        });

        action = m_menu->addAction( qtr("Save") );
        connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), [selectedUrlList]( ) {
            DialogsProvider::getInstance()->streamingDialog(selectedUrlList, true);
        });

        m_menu->addSeparator();
    }

    if (currentItem) {
        action = m_menu->addAction( qtr("Information") );
        action->setIcon(":/menu/info.svg");
        connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), [currentItem]( ) {
            DialogsProvider::getInstance()->mediaInfoDialog(currentItem);
        });

        m_menu->addSeparator();

        action = m_menu->addAction( qtr("Show Containing Directory...") );
        action->setIcon(":/menu/folder.svg");
        connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), [this, currentItem]( ) {
            m_controler->explore(currentItem);
        });

        m_menu->addSeparator();
    }

    action = m_menu->addAction( qtr("Add File...") );
    action->setIcon(":/menu/add.svg");
    connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), []( ) {
        DialogsProvider::getInstance()->simpleOpenDialog(false);
    });

    action = m_menu->addAction( qtr("Add Directory...") );
    action->setIcon(":/menu/add.svg");
    connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), []( ) {
        DialogsProvider::getInstance()->PLAppendDir();
    });

    action = m_menu->addAction( qtr("Advanced Open...") );
    action->setIcon(":/menu/add.svg");
    connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), []( ) {
        DialogsProvider::getInstance()->PLAppendDialog();
    });

    m_menu->addSeparator();

    if (m_selectionModel->hasSelection())
    {
        action = m_menu->addAction( qtr("Save Playlist to File...") );
        connect(action, &PlatformAgnosticAction::triggered, DialogsProvider::getInstance(), []( ) {
            DialogsProvider::getInstance()->savePlayingToPlaylist();
        });

        m_menu->addSeparator();

        action = m_menu->addAction( qtr("Remove Selected") );
        action->setIcon(":/menu/remove.svg");
        connect(action, &PlatformAgnosticAction::triggered, this, [this]( ) {
            m_model->removeItems(m_selectionModel->selectedIndexesFlat());
        });
    }


    if (m_model->rowCount() > 0)
    {
        action = m_menu->addAction( qtr("Clear the playlist") );
        action->setIcon(":/menu/clear.svg");
        connect(action, &PlatformAgnosticAction::triggered, this, [this]( ) {
            m_controler->clear();
        });

        m_menu->addSeparator();

        using namespace vlc::playlist;
        PlaylistController::SortKey currentKey = m_controler->getSortKey();
        PlaylistController::SortOrder currentOrder = m_controler->getSortOrder();

        PlatformAgnosticMenu* sortMenu = m_menu->addMenu(qtr("Sort by"));
        PlatformAgnosticActionGroup * group = PlatformAgnosticActionGroup::createActionGroup(sortMenu);

        auto addSortAction = [&](const QString& label, PlaylistController::SortKey key, PlaylistController::SortOrder order) {
            PlatformAgnosticAction* action = sortMenu->addAction(label);
            connect(action, &PlatformAgnosticAction::triggered, this, [this, key, order]( ) {
                m_controler->sort(key, order);
            });
            action->setCheckable(true);
            action->setActionGroup(group);
            if (key == currentKey && currentOrder == order)
                action->setChecked(true);
        };

        for (const QVariant& it: m_controler->getSortKeyTitleList())
        {
            const QVariantMap varmap = it.toMap();

            auto key = static_cast<PlaylistController::SortKey>(varmap.value("key").toInt());
            QString label = varmap.value("text").toString();

            addSortAction(qtr("%1 Ascending").arg(label), key, PlaylistController::SORT_ORDER_ASC);
            addSortAction(qtr("%1 Descending").arg(label), key, PlaylistController::SORT_ORDER_DESC);
        }

        action = m_menu->addAction( qtr("Shuffle the playlist") );
        action->setIcon(":/menu/ic_fluent_arrow_shuffle_on.svg");
        connect(action, &PlatformAgnosticAction::triggered, this, [this]( ) {
            m_controler->shuffle();
        });

    }

    m_menu->popup(pos);
}

