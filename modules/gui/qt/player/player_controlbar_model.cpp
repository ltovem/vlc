/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "player_controlbar_model.hpp"

#include <QMetaEnum>
#include <QJSEngine>

#include "qt.hpp"
#include "control_list_model.hpp"

decltype (PlayerControlbarModel::playerIdentifierDictionary)
    PlayerControlbarModel::playerIdentifierDictionary {
        {Videoplayer, N_("Video player")},
        {Audioplayer, N_("Audio player")},
        {Miniplayer,  N_("Mini player")}
    };

QJSValue PlayerControlbarModel::getPlaylistIdentifierListModel(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)

    static const QMetaEnum metaEnum = QMetaEnum::fromType<PlayerIdentifier>();

    QJSValue array = scriptEngine->newArray();

    for (int i = 0; i < metaEnum.keyCount(); ++i)
    {
       QJSValue obj = scriptEngine->newObject();

       const int val = metaEnum.value(i);
       obj.setProperty("identifier", val);

       QString key;
       if ( playerIdentifierDictionary.contains(static_cast<PlayerControlbarModel::PlayerIdentifier>(i)) )
       {
           key = qfut( playerIdentifierDictionary[static_cast<PlayerControlbarModel::PlayerIdentifier>(i)] );
       }
       else
       {
           key = metaEnum.key(i);
       }

       obj.setProperty("name", key);

       array.setProperty(i, obj);
    }

    QJSValue value = scriptEngine->newObject();
    value.setProperty("model", array);

    return value;
}

PlayerControlbarModel::PlayerControlbarModel(QObject *parent) : QObject(parent)
{
    m_left = new ControlListModel(this);
    m_center = new ControlListModel(this);
    m_right = new ControlListModel(this);

    connect(m_left, &ControlListModel::countChanged, this, &PlayerControlbarModel::contentChanged);
    connect(m_center, &ControlListModel::countChanged, this, &PlayerControlbarModel::contentChanged);
    connect(m_right, &ControlListModel::countChanged, this, &PlayerControlbarModel::contentChanged);

    connect(m_left, &QAbstractListModel::dataChanged, this, &PlayerControlbarModel::contentChanged);
    connect(m_center, &QAbstractListModel::dataChanged, this, &PlayerControlbarModel::contentChanged);
    connect(m_right, &QAbstractListModel::dataChanged, this, &PlayerControlbarModel::contentChanged);
}

PlayerControlbarModel::~PlayerControlbarModel()
{
    setDirty(false);
}

bool PlayerControlbarModel::dirty() const
{
    return m_dirty;
}

std::array<QVector<int>, 3> PlayerControlbarModel::serializeModels() const
{
    return { left()->getControls(),
            center()->getControls(),
            right()->getControls() };
}

void PlayerControlbarModel::loadModels(const std::array<QVector<int>, 3> &array)
{
    left()->setControls(array.at(0));
    center()->setControls(array.at(1));
    right()->setControls(array.at(2));
}

ControlListModel *PlayerControlbarModel::left() const
{
    return m_left;
}

ControlListModel *PlayerControlbarModel::center() const
{
    return m_center;
}

ControlListModel *PlayerControlbarModel::right() const
{
    return m_right;
}

void PlayerControlbarModel::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(m_dirty);
}

void PlayerControlbarModel::contentChanged()
{
    setDirty(true);

    emit controlListChanged();
}
