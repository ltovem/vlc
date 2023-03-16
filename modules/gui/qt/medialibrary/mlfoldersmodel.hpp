// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef ML_FOLDERS_MODEL_HPP
#define ML_FOLDERS_MODEL_HPP

#ifdef HAVE_CONFIG_H

# include "config.h"

#endif

#include "qt.hpp"
#include <QAbstractListModel>
#include <QUrl>
#include "mlhelper.hpp"

#include <maininterface/mainctx.hpp>
#include <vlc_media_library.h>

class MLFoldersBaseModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(MainCtx* ctx READ getCtx WRITE setCtx NOTIFY ctxChanged FINAL)

public:
    enum Roles
    {
        Banned = Qt::UserRole + 1,
        DisplayUrl,
        MRL
    };

    enum Operation
    {
        Add,
        Remove,
        Ban,
        Unban
    };

    MLFoldersBaseModel( QObject *parent = nullptr );
    virtual ~MLFoldersBaseModel() = default;

    void setCtx(MainCtx* ctx);
    inline MainCtx* getCtx() { return m_ctx; }

    int rowCount( QModelIndex const &parent = {} ) const  override;
    QVariant data( QModelIndex const &index , const int role = Qt::DisplayRole ) const  override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    virtual void remove( const QUrl &mrl ) = 0;
    virtual void add( const QUrl &mrl ) = 0;
    void removeAt( int index );

signals:
    void ctxChanged();
    void operationFailed( int op, QUrl url ) const;
    void onMLEntryPointModified(QPrivateSignal);

protected:
    struct EntryPoint
    {
        EntryPoint(const vlc_ml_folder_t &entryPoint );
        QString mrl;
        bool banned;
    };

    virtual bool failed( const vlc_ml_event_t* event ) const = 0; // will be called outside the main thread

protected:
    static void onMlEvent( void* data , const vlc_ml_event_t* event );
    virtual void update() = 0;
    void updateImpl(vlc_ml_folder_list_t* (*folderListFunc)( vlc_medialibrary_t* p_ml, const vlc_ml_query_params_t* params));

    using EventCallbackPtr = std::unique_ptr<vlc_ml_event_callback_t, std::function<void( vlc_ml_event_callback_t* )>>;

    std::vector<EntryPoint> m_mrls;
    MediaLib *m_mediaLib = nullptr;
    MainCtx* m_ctx = nullptr;
    EventCallbackPtr m_ml_event_handle;
    bool m_updatePending = false;
};

class MLFoldersModel : public MLFoldersBaseModel
{
public:
    using MLFoldersBaseModel::MLFoldersBaseModel;

    void remove( const QUrl &mrl ) override;
    void add( const QUrl &mrl ) override;

private:
    void update() override final;
    bool failed( const vlc_ml_event_t* event ) const override;
};

class MLBannedFoldersModel : public MLFoldersBaseModel
{
public:
    using MLFoldersBaseModel::MLFoldersBaseModel;

    void remove( const QUrl &mrl ) override;
    void add( const QUrl &mrl ) override;

private:
    void update() override final;
    bool failed( const vlc_ml_event_t* event ) const override;
};

#endif // ML_FOLDERS_MODEL_HPP
