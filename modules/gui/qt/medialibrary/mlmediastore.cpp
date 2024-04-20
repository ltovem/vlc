/*****************************************************************************
 * Copyright (C) 2024 VLC authors and VideoLAN
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
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


#include "mlmediastore.hpp"

#include "mlhelper.hpp"
#include "medialib.hpp"
#include "mlmedia.hpp"

MLMediaStore::~MLMediaStore() = default;

MLMediaStore::MLMediaStore(MediaLib *ml, QObject *parent)
    : QObject(parent)
    , m_ml {ml}
    , m_ml_event_handle( nullptr, [this](vlc_ml_event_callback_t* cb )
    {
        assert( m_ml != nullptr );
        m_ml->unregisterEventListener( cb );
    })
{
    m_ml_event_handle.reset
    (
        m_ml->registerEventListener(MLMediaStore::onVlcMlEvent, this)
    );
}

void MLMediaStore::insert(const QString &mrl)
{
    struct Ctx
    {
        MLMedia media;
    };

    m_ml->runOnMLThread<Ctx>(this,
    //ML thread
    [mrl](vlc_medialibrary_t *ml, Ctx &ctx)
    {
        ml_unique_ptr<vlc_ml_media_t> media {vlc_ml_get_media_by_mrl(ml, qtu(mrl))};
        if (!media)
            media.reset(vlc_ml_new_external_media( ml, qtu(mrl)));

        ctx.media = media ? MLMedia(media.get()) : MLMedia {};
    },
    //UI thread
    [this, mrl](quint64, Ctx &ctx)
    {
        if (!ctx.media.valid())
            return; // failed to get media, TODO: notify??

        // update states
        m_mrls[ctx.media.getId()] = mrl;
        emit updated(mrl, ctx.media);
    });
}

void MLMediaStore::remove(const MLItemId &id)
{
    m_mrls.remove(id);
}

void MLMediaStore::clear()
{
    m_mrls.clear();
}

void MLMediaStore::onVlcMlEvent(void *data, const vlc_ml_event_t *event)
{
    if (event->i_type != VLC_ML_EVENT_MEDIA_UPDATED)
        return;

    auto self = static_cast<MLMediaStore*>(data);

    const MLItemId id(event->modification.i_entity_id, VLC_ML_PARENT_UNKNOWN);
    QMetaObject::invokeMethod(self, [self, id] () mutable
    {
        self->update(id);
    });
}

void MLMediaStore::update(const MLItemId &id)
{
    if (!m_mrls.contains(id))
        return;

    struct Ctx
    {
        MLMedia media;
    };

    m_ml->runOnMLThread<Ctx>(this,
    //ML thread
    [id](vlc_medialibrary_t *ml, Ctx &ctx)
    {
        ml_unique_ptr<vlc_ml_media_t> media {vlc_ml_get_media(ml, id.id)};
        ctx.media = media ? MLMedia(media.get()) : MLMedia {};
    },
    //UI thread
    [this, id](quint64, Ctx &ctx)
    {
        if (!m_mrls.contains(id))
            return; // item was removed?

        const QString mrl = m_mrls[id];
        emit updated(mrl, ctx.media);
    });
}
