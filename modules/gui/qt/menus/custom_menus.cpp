/*****************************************************************************
 * custom_menus.cpp : Qt custom menus classes
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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_renderer_discovery.h>

#include "custom_menus.hpp"
#include "util/renderer_manager.hpp"

// MediaLibrary includes
#include "medialibrary/mlbookmarkmodel.hpp"

// Dialogs includes
#include "dialogs/dialogs_provider.hpp"

// Menus includes
#include "menus/menus.hpp"

// Qt includes
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QWidgetAction>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaMethod>

#include "widgets/native/platformagnosticmenu.hpp"
#include "widgets/native/platformagnosticaction.hpp"
#include "widgets/native/platformagnosticactiongroup.hpp"

RendererAction::RendererAction( vlc_renderer_item_t *p_item_, QObject* parent )
    : QObject{parent}
{
    p_item = p_item_;
    vlc_renderer_item_hold( p_item );

    m_action = PlatformAgnosticAction::createAction( parent );
    assert(m_action);

    if( vlc_renderer_item_flags( p_item ) & VLC_RENDERER_CAN_VIDEO )
        m_action->setIcon( QStringLiteral( ":/menu/movie.svg" ) );
    else
        m_action->setIcon( QStringLiteral(":/menu/music.svg") );
    m_action->setText( vlc_renderer_item_name( p_item ) );
    m_action->setCheckable(true);
}

RendererAction::~RendererAction()
{
    vlc_renderer_item_release( p_item );
}

vlc_renderer_item_t * RendererAction::getItem()
{
    return p_item;
}

RendererMenu::RendererMenu( qt_intf_t *p_intf_, QObject* parent )
    : QObject{ parent }
    , p_intf( p_intf_ )
{
    m_menu = PlatformAgnosticMenu::createMenu(parent);
    assert(m_menu);

    connect(m_menu, &PlatformAgnosticMenu::aboutToHide, this, &RendererMenu::aboutToHide);
    connect(m_menu, &PlatformAgnosticMenu::aboutToShow, this, &RendererMenu::aboutToShow);

    m_menu->setTitle( qtr("&Renderer") );

    m_group = PlatformAgnosticActionGroup::createActionGroup( m_menu.data() );
    assert(m_group);

    const auto action = PlatformAgnosticAction::createAction( qtr("<Local>"), m_menu.data() );
    assert(action);

    action->setCheckable(true);
    m_menu->addAction( action );
    m_group->addAction( action );

    vlc_player_Lock( p_intf_->p_player );
    if ( vlc_player_GetRenderer( p_intf->p_player ) == nullptr )
        action->setChecked( true );
    vlc_player_Unlock( p_intf_->p_player );

    m_menu->addSeparator();

    if (qobject_cast<WidgetsMenu*>(m_menu))
    {
        QProgressBar *pb = new QProgressBar();
        pb->setObjectName( "statusprogressbar" );
        pb->setMaximumHeight( 10 );
        pb->setStyleSheet( QString("\
            QProgressBar:horizontal {\
                border: none;\
                background: transparent;\
                padding: 1px;\
            }\
            QProgressBar::chunk:horizontal {\
                background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, \
                            stop: 0 white, stop: 0.4 orange, stop: 0.6 orange, stop: 1 white);\
            }") );
        pb->setRange( 0, 0 );
        pb->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Maximum );
        QWidgetAction *qwa = new QWidgetAction( this );
        qwa->setDefaultWidget( pb );
        qwa->setDisabled( true );
        m_progressIndicator = qwa;
    }
    else if (qobject_cast<QuickControls2Menu*>(m_menu))
    {
        const auto menu = static_cast<QuickControls2Menu*>(m_menu.data());
        QQmlEngine* const engine = qmlEngine(menu->m_menu);
        assert(engine);
        QQmlComponent component(engine, nullptr);
        component.setData("import QtQuick.Controls 2.12; BusyIndicator { }", {});
        m_progressIndicator = component.create(qmlContext(menu->m_menu));
        assert(m_progressIndicator);
        QQmlEngine::setObjectOwnership(m_progressIndicator, QQmlEngine::CppOwnership);
        m_progressIndicator->setParent(menu->m_menu);
    }
    else
        Q_UNREACHABLE();

    m_statusAction = m_menu->addAction( qtr("Waiting...") );
    m_statusAction->setEnabled(false);
    m_menu->addItem(m_progressIndicator);

    RendererManager *manager = RendererManager::getInstance( p_intf );
    connect( m_menu, &PlatformAgnosticMenu::aboutToShow, manager, &RendererManager::StartScan );
    connect( m_group, &PlatformAgnosticActionGroup::triggered, this, &RendererMenu::RendererSelected );
    connect( manager, SIGNAL(rendererItemAdded( vlc_renderer_item_t * )),
             this, SLOT(addRendererItem( vlc_renderer_item_t * )), Qt::DirectConnection );
    connect( manager, SIGNAL(rendererItemRemoved( vlc_renderer_item_t * )),
             this, SLOT(removeRendererItem( vlc_renderer_item_t * )), Qt::DirectConnection );
    connect( manager, &RendererManager::statusUpdated, this, &RendererMenu::updateStatus );
}

RendererMenu::~RendererMenu()
{
    reset();
}

void RendererMenu::updateStatus( int val )
{
    if( val >= RendererManager::RendererStatus::RUNNING )
    {
        m_statusAction->setText( qtr("Scanning...").append( QString(" (%1s)").arg( val ) ) );
        m_progressIndicator->setProperty( "visible", true );
        m_statusAction->setVisible(true);
    }
    else if( val == RendererManager::RendererStatus::FAILED )
    {
        m_statusAction->setText( qtr("Failed (no discovery module available)") );
        m_progressIndicator->setProperty( "visible", false );
        m_statusAction->setVisible(true);
    }
    else
    {
        m_statusAction->setVisible(false);
    }
}

void RendererMenu::addRendererItem( vlc_renderer_item_t *p_item )
{
    const auto action = new RendererAction( p_item, m_menu );
    m_menu->insertAction( m_statusAction, action->m_action );
    m_group->addAction( action->m_action );
}

void RendererMenu::removeRendererItem( vlc_renderer_item_t *p_item )
{
    for (const auto action : m_menu->actions())
    {
        RendererAction *ra = qobject_cast<RendererAction *>( action );
        if( !ra || ra->getItem() != p_item )
            continue;
        removeRendererAction( ra );
        delete ra;
        break;
    }
}

void RendererMenu::addRendererAction(RendererAction *action)
{
    m_menu->insertAction( m_statusAction, action->m_action );
    m_group->addAction( action->m_action );
}

void RendererMenu::removeRendererAction(RendererAction *action)
{
    m_menu->removeAction( action->m_action );
    m_group->removeAction( action->m_action );
}

void RendererMenu::reset()
{
    /* reset the list of renderers */
    for (const auto action : m_menu->actions())
    {
        RendererAction *ra = qobject_cast<RendererAction *>( action );
        if( ra )
        {
            removeRendererAction( ra );
            ra->deleteLater();
        }
    }
}

void RendererMenu::popup(const QPoint &pos)
{
    assert(m_menu);
    m_menu->popup(pos);
}

void RendererMenu::RendererSelected(QObject *_action)
{
    const auto action = _action->property("platformAgnosticAction").value<PlatformAgnosticAction *>();
    assert(action);
    const auto ra = qobject_cast<RendererAction *>(action->parent());
    if( ra )
        RendererManager::getInstance( p_intf )->SelectRenderer( ra->getItem() );
    else
        RendererManager::getInstance( p_intf )->SelectRenderer( NULL );
}

/*   CheckableListMenu   */

CheckableListMenu::CheckableListMenu(QString title, QAbstractListModel* model , GroupingMode grouping, QObject* parent)
    : QObject{parent}
    , m_model(model)
    , m_grouping(grouping)
{
    m_menu = PlatformAgnosticMenu::createMenu(parent);
    assert(m_menu);
    m_menu->setTitle(title);
    if (m_grouping != UNGROUPED)
    {
        m_actionGroup = PlatformAgnosticActionGroup::createActionGroup(m_menu);
        if (m_grouping == GROUPED_OPTIONAL)
        {
            m_actionGroup->setExclusive(false);
        }
    }

    connect(m_model, &QAbstractListModel::rowsAboutToBeRemoved, this, &CheckableListMenu::onRowsAboutToBeRemoved);
    connect(m_model, &QAbstractListModel::rowsInserted, this, &CheckableListMenu::onRowInserted);
    connect(m_model, &QAbstractListModel::dataChanged, this, &CheckableListMenu::onDataChanged);
    connect(m_model, &QAbstractListModel::modelAboutToBeReset, this, &CheckableListMenu::onModelAboutToBeReset);
    connect(m_model, &QAbstractListModel::modelReset, this, &CheckableListMenu::onModelReset);
    onModelReset();
}

void CheckableListMenu::onRowsAboutToBeRemoved(const QModelIndex &, int first, int last)
{
    assert(m_menu);
    for (int i = last; i >= first; i--)
    {
        PlatformAgnosticAction* action = m_menu->actions()[i];
        if (m_actionGroup)
            m_actionGroup->removeAction(action);
        delete action;
    }
    if (m_menu->actions().count() == 0)
        m_menu->setEnabled(false);
}

void CheckableListMenu::onRowInserted(const QModelIndex &, int first, int last)
{
    assert(m_menu);
    for (int i = first; i <= last; i++)
    {
        QModelIndex index = m_model->index(i);
        QString title = m_model->data(index, Qt::DisplayRole).toString();
        bool checked = m_model->data(index, Qt::CheckStateRole).toBool();

        PlatformAgnosticAction *choiceAction = PlatformAgnosticAction::createAction(title, (*this)());
        m_menu->addAction(choiceAction);
        if (m_actionGroup)
            m_actionGroup->addAction(choiceAction);
        connect(choiceAction, &PlatformAgnosticAction::triggered, this, [this, i](bool checked){
            QModelIndex dataIndex = m_model->index(i);
            m_model->setData(dataIndex, QVariant::fromValue<bool>(checked), Qt::CheckStateRole);
        });
        choiceAction->setCheckable(true);
        choiceAction->setChecked(checked);
        m_menu->setEnabled(true);
    }
}

void CheckableListMenu::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> & )
{
    assert(m_menu);
    for (int i = topLeft.row(); i <= bottomRight.row(); i++)
    {
        const auto actions = m_menu->actions();
        if (i >= actions.size())
            break;
        PlatformAgnosticAction *choiceAction = actions[i];

        QModelIndex index = m_model->index(i);
        QString title = m_model->data(index, Qt::DisplayRole).toString();
        bool checked = m_model->data(index, Qt::CheckStateRole).toBool();

        choiceAction->setText(title);
        choiceAction->setChecked(checked);
    }
}

void CheckableListMenu::onModelAboutToBeReset()
{
    assert(m_menu);
    for (PlatformAgnosticAction* action : m_menu->actions())
    {
        if (m_actionGroup)
            m_actionGroup->removeAction(action);
        delete action;
    }
    m_menu->setEnabled(false);
}

void CheckableListMenu::onModelReset()
{
    assert(m_menu);
    int nb_rows = m_model->rowCount();
    if (nb_rows == 0)
        m_menu->setEnabled(false);
    else
        onRowInserted({}, 0, nb_rows - 1);
}

// ListMenuHelper

ListMenuHelper::ListMenuHelper(PlatformAgnosticMenu * menu, QAbstractListModel * model, PlatformAgnosticAction * before,
                               QObject * parent)
    : QObject(parent), m_menu(menu), m_model(model), m_before(before)
{
    m_group = PlatformAgnosticActionGroup::createActionGroup(m_menu);

    onModelReset();

    connect(m_model, &QAbstractListModel::rowsInserted, this, &ListMenuHelper::onRowsInserted);
    connect(m_model, &QAbstractListModel::rowsRemoved,  this, &ListMenuHelper::onRowsRemoved);

    connect(m_model, &QAbstractListModel::dataChanged, this, &ListMenuHelper::onDataChanged);

    connect(m_model, &QAbstractListModel::modelReset, this, &ListMenuHelper::onModelReset);
}

// Interface

int ListMenuHelper::count() const
{
    return m_actions.count();
}

// Private slots

void ListMenuHelper::onRowsInserted(const QModelIndex &, int first, int last)
{
    PlatformAgnosticAction * before;

    if (first < m_actions.count())
        before = m_actions.at(first);
    else
        before = m_before;

    for (int i = first; i <= last; i++)
    {
        QModelIndex index = m_model->index(i, 0);

        QString name = m_model->data(index, Qt::DisplayRole).toString();

        PlatformAgnosticAction * action = PlatformAgnosticAction::createAction(name, m_menu);

        action->setCheckable(true);

        bool checked = m_model->data(index, Qt::CheckStateRole).toBool();

        action->setChecked(checked);

        // NOTE: We are adding sequentially *before* the next action in the list.
        m_menu->insertAction(before, action);

        m_group->addAction(action);

        m_actions.insert(i, action);

        connect(action, &PlatformAgnosticAction::triggered, this, &ListMenuHelper::onTriggered);
    }

    emit countChanged(m_actions.count());
}

void ListMenuHelper::onRowsRemoved(const QModelIndex &, int first, int last)
{
    for (int i = first; i <= last; i++)
    {
        PlatformAgnosticAction * action = m_actions.at(i);

        m_group->removeAction(action);

        delete action;
    }

    QList<PlatformAgnosticAction *>::iterator begin = m_actions.begin();

    m_actions.erase(begin + first, begin + last + 1);

    emit countChanged(m_actions.count());
}

void ListMenuHelper::onDataChanged(const QModelIndex & topLeft,
                                   const QModelIndex & bottomRight, const QVector<int> &)
{
    for (int i = topLeft.row(); i <= bottomRight.row(); i++)
    {
        PlatformAgnosticAction * action = m_actions.at(i);

        QModelIndex index = m_model->index(i, 0);

        QString name = m_model->data(index, Qt::DisplayRole).toString();

        action->setText(name);

        bool checked = m_model->data(index, Qt::CheckStateRole).toBool();

        action->setChecked(checked);
    }
}

void ListMenuHelper::onModelReset()
{
    for (PlatformAgnosticAction * action : m_actions)
    {
        m_group->removeAction(action);

        delete action;
    }

    m_actions.clear();

    int count = m_model->rowCount();

    if (count)
        onRowsInserted(QModelIndex(), 0, count - 1);
}

void ListMenuHelper::onTriggered(bool)
{
    PlatformAgnosticAction * action = static_cast<PlatformAgnosticAction *> (sender());

    emit select(m_actions.indexOf(action));
}

/*     BooleanPropertyAction    */

BooleanPropertyAction::BooleanPropertyAction(QString title, QObject *model, QString propertyName, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_propertyName(propertyName)
{
    m_action = PlatformAgnosticAction::createAction(parent);
    assert(m_action);
    m_action->setText(title);
    assert(model);
    const QMetaObject* meta = model->metaObject();
    int propertyId = meta->indexOfProperty(qtu(propertyName));
    assert(propertyId != -1);
    QMetaProperty property = meta->property(propertyId);
    assert(property.type() ==  QVariant::Bool);
    const QMetaObject* metaObject = m_action->metaObject();

    assert(property.hasNotifySignal());
    QMetaMethod checkedSlot = metaObject->method(metaObject->indexOfSlot( "setChecked(bool)" ));
    connect( model, property.notifySignal(), m_action, checkedSlot );
    connect( m_action, &PlatformAgnosticAction::triggered, this, &BooleanPropertyAction::setModelChecked );

    m_action->setCheckable(true);
    m_action->setChecked(property.read(model).toBool());
}

void BooleanPropertyAction::setModelChecked(bool checked)
{
    m_model->setProperty(qtu(m_propertyName), QVariant::fromValue<bool>(checked) );
}


RecentMenu::RecentMenu(MLRecentsModel* model, MediaLib* ml, QObject* parent)
    : QObject(parent)
    , m_model(model)
    , m_ml(ml)
{
    m_menu = PlatformAgnosticMenu::createMenu(parent);
    assert(m_menu);

    connect(m_model, &MLRecentsModel::rowsRemoved, this, &RecentMenu::onRowsRemoved);
    connect(m_model, &MLRecentsModel::rowsInserted, this, &RecentMenu::onRowInserted);
    connect(m_model, &MLRecentsModel::dataChanged, this, &RecentMenu::onDataChanged);
    connect(m_model, &MLRecentsModel::modelReset, this, &RecentMenu::onModelReset);
    m_menu->addSeparator();
    m_menu->addAction( qtr("&Clear"), this, [this]() { m_model->clearHistory(); } );
    onModelReset();
}

void RecentMenu::onRowsRemoved(const QModelIndex&, int first, int last)
{
    for (int i = first; i <= last; i++)
    {
        delete m_actions.at(i);
    }

    QList<PlatformAgnosticAction *>::iterator begin = m_actions.begin();

    m_actions.erase(begin + first, begin + last + 1);

    if (m_actions.isEmpty())
        m_menu->setEnabled(false);
}

void RecentMenu::onRowInserted(const QModelIndex&, int first, int last)
{
    PlatformAgnosticAction * before;

    if (first < m_actions.count())
        before = m_actions.at(first);
    else
        // NOTE: In that case we insert *before* the 'Clear' separator.
        before = m_separator;

    for (int i = first; i <= last; i++)
    {
        QModelIndex index = m_model->index(i);
        QString url = m_model->data(index, MLRecentsModel::RECENT_MEDIA_URL).toString();

        PlatformAgnosticAction *choiceAction = PlatformAgnosticAction::createAction(url, m_menu);
        assert(choiceAction);

        // NOTE: We are adding sequentially *before* the next action in the list.
        m_menu->insertAction(before, choiceAction);

        m_actions.insert(i, choiceAction);

        connect(choiceAction, &PlatformAgnosticAction::triggered, this, [this, choiceAction](){
            QModelIndex index = m_model->index(m_actions.indexOf(choiceAction));

            MLItemId id = m_model->data(index, MLRecentsModel::RECENT_MEDIA_ID).value<MLItemId>();
            m_ml->addAndPlay(id);
        });
        m_menu->setEnabled(true);
    }
}

void RecentMenu::onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QVector<int>& )
{
    for (int i = topLeft.row(); i <= bottomRight.row(); i++)
    {
        QModelIndex index = m_model->index(i);
        QString title = m_model->data(index, MLRecentsModel::RECENT_MEDIA_URL).toString();

        m_actions.at(i)->setText(title);
    }
}

void RecentMenu::onModelReset()
{
    qDeleteAll(m_actions);
    m_actions.clear();

    int nb_rows = m_model->rowCount();
    if (nb_rows == 0 || nb_rows == -1)
        m_menu->setEnabled(false);
    else
        onRowInserted({}, 0, nb_rows - 1);
}

// BookmarkMenu

BookmarkMenu::BookmarkMenu(MediaLib * mediaLib, vlc_player_t * player, QObject* parent)
    : QObject{parent}
{
    m_menu = PlatformAgnosticMenu::createMenu(parent);

    // FIXME: Do we really need a translation call for the string shortcut ?
    m_menu->addAction(qtr("&Manage"), THEDP, []() {
            DialogsProvider::getInstance()->bookmarksDialog();
        }, QKeySequence("Ctrl+B"));

    m_menu->addSeparator();

    MLBookmarkModel * model = new MLBookmarkModel(this);
    model->setPlayer(player);
    model->setMl(mediaLib);

    ListMenuHelper * helper = new ListMenuHelper(m_menu.data(), model, nullptr, this);

    connect(helper, &ListMenuHelper::select, [model](int index)
    {
        model->select(model->index(index, 0));
    });

    if (qobject_cast<WidgetsMenu*>(m_menu.data()))
        m_menu->setTearOffEnabled(true);
}
PlatformAgnosticMenu *CheckableListMenu::operator()() const {
    return m_menu.data();
};
PlatformAgnosticAction *BooleanPropertyAction::operator()() const {
    return m_action.data();
};
PlatformAgnosticMenu *RecentMenu::operator()() const { return m_menu.data(); };
PlatformAgnosticMenu *BookmarkMenu::operator()() const {
    return m_menu.data();
};
