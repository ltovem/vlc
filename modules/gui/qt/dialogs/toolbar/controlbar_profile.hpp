/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef ControlbarProfile_HPP
#define ControlbarProfile_HPP

#include <QObject>
#include <QMap>
#include <QVector>
#include <array>

class PlayerControlbarModel;

class ControlbarProfile : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool dirty READ dirty RESET resetDirty NOTIFY dirtyChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(int profileId READ id WRITE setId NOTIFY idChanged FINAL)

    friend class ControlbarProfileModel;

public:
    explicit ControlbarProfile(QObject *parent = nullptr);

    PlayerControlbarModel* newModel(int identifier);
    Q_INVOKABLE PlayerControlbarModel* getModel(int identifier) const;

    void setModelData(int identifier, const std::array<QVector<int>, 3>& data);
    std::array<QVector<int>, 3> getModelData(int identifier) const;

    void deleteModel(int identifier);

    Q_INVOKABLE void injectDefaults(bool resetDirty = true);

    bool dirty() const;
    QString name() const;

    // The id is a unique number supplied to each profile
    // which differentiates it independently from its index
    // in the profiles array. Provides a more stable way to
    // identify a profile than using its name.
    int id() const;

public slots:
    void resetDirty();
    void setName(const QString& name);
    void setId( const int id );

private:
    // m_dirty indicates the count of PlayerControlbarModel
    // residing in m_models which has the dirty property
    // set true.
    int m_dirty = 0;

    int m_id = -1;
    QString m_name {"N/A"};
    bool m_pauseControlListGeneration = false;

    // According to benchmarks, QMap performs better than
    // QHash when item count is less than 32.
    // Assuming model (player) count to stay below that,
    // QMap is used here.
    QMap<int, PlayerControlbarModel *> m_models;

    struct Configuration {
        int identifier;
        std::array<QVector<int>, 3> data;
    };
    static const QVector<Configuration> m_defaults;

private:
    void injectModel(const QVector<Configuration>& modelData);

private slots:
    void generateLinearControlList();

signals:
    void dirtyChanged(bool dirty);
    void nameChanged(QString name);
    void idChanged(int id);

    void controlListChanged(const QVector<int>& linearControlList);
};

#endif // ControlbarProfile_HPP
