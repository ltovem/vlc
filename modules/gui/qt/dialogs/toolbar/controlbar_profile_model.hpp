// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef CONTROLBARPROFILEMODEL_H
#define CONTROLBARPROFILEMODEL_H

#include <QAbstractListModel>
#include <array>

#include "controlbar_profile.hpp"
#include "qt.hpp"

class ControlbarProfileModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int selectedProfile READ selectedProfile WRITE setSelectedProfile NOTIFY selectedProfileChanged FINAL)
    Q_PROPERTY(ControlbarProfile* currentModel READ currentModel NOTIFY selectedProfileChanged FINAL)

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged FINAL)

public:
    explicit ControlbarProfileModel(qt_intf_t *p_intf, QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;
    enum {DEFAULT_STYLE, MINIMALIST_STYLE, ONE_LINER_STYLE, SIMPLEST_STYLE, CLASSIC_STYLE};

    // Editable:
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::DisplayRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    Q_INVOKABLE bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

public:
    enum Roles {
        MODEL_ROLE = Qt::UserRole,
    };

    int selectedProfile() const;
    ControlbarProfile* currentModel() const;
    bool setSelectedProfileFromId(int id);

    ControlbarProfile* cloneProfile(const ControlbarProfile* profile);
    Q_INVOKABLE void cloneSelectedProfile(const QString& newProfileName);

    Q_INVOKABLE ControlbarProfile* getProfile(int index) const;

    Q_INVOKABLE ControlbarProfile* newProfile(const QString& name, const int id);
    ControlbarProfile* newProfile();

    Q_INVOKABLE void deleteSelectedProfile();

public slots:
    void save(bool clearDirty = true) const;
    bool reload();

    bool setSelectedProfile(int selectedProfile);

signals:
    void countChanged();
    void selectedProfileChanged();

    void selectedProfileControlListChanged(const QVector<int>& linearControlList);

private:
    QVector<ControlbarProfile *> m_profiles;

    int m_selectedProfile = -1;
    int m_maxId = 0;

    struct Profile {
        const int id;
        const char* name;
        QVector<ControlbarProfile::Configuration> modelData;
    };

    static const QVector<Profile> m_defaults;

private:
    void insertDefaults();

    QString generateUniqueName(const QString& name);

protected:
    qt_intf_t *m_intf = nullptr;
};

#endif // CONTROLBARPROFILEMODEL_H
