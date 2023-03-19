// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef PLAYERCONTROLBARMODEL_HPP
#define PLAYERCONTROLBARMODEL_HPP

#include <QObject>
#include <QJSValue>
#include <QMap>

#include <array>

class ControlListModel;

class PlayerControlbarModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged FINAL)

    Q_PROPERTY(ControlListModel* left READ left CONSTANT FINAL)
    Q_PROPERTY(ControlListModel* center READ center CONSTANT FINAL)
    Q_PROPERTY(ControlListModel* right READ right CONSTANT FINAL)

public:
    // When there is a need to add a new Player, just
    // add its identifier in this enum and set QML buttons layout
    // identifier to it. Such as `property int identifier =
    // PlayerControlbarModel.Videoplayer`.
    // To make it translatable, add a corresponding entry to
    // the static member playerIdentifierDictionary which is
    // initialized in the source file.
    enum PlayerIdentifier {
        Videoplayer = 0,
        Audioplayer,
        Miniplayer
    };
    Q_ENUM(PlayerIdentifier)
    // This enum is iterated through QMetaEnum, and
    // a model out of this enum is generated
    // and used in the configuration editor.
    // Thanks to MOC, adding an entry to this enum
    // is enough for the editor to consider the
    // added entry without any other modification.
    // (except for the translation)

    static const QMap<PlayerIdentifier, const char*> playerIdentifierDictionary;

    static QJSValue getPlaylistIdentifierListModel(class QQmlEngine *engine,
                                                   class QJSEngine *scriptEngine);

    explicit PlayerControlbarModel(QObject *parent = nullptr);
    ~PlayerControlbarModel();

    bool dirty() const;

    std::array<QVector<int>, 3> serializeModels() const;
    void loadModels(const std::array<QVector<int>, 3>& array);

    ControlListModel* left() const;
    ControlListModel* center() const;
    ControlListModel* right() const;

public slots:
    void setDirty(bool dirty);

signals:
    void dirtyChanged(bool dirty);
    void controlListChanged();

private:
    bool m_dirty = false;

    ControlListModel* m_left = nullptr;
    ControlListModel* m_center = nullptr;
    ControlListModel* m_right = nullptr;

private slots:
    void contentChanged();
};

#endif
