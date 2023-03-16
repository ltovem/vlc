// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * HTTPConnectionManager.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef HTTPCONNECTIONMANAGER_H_
#define HTTPCONNECTIONMANAGER_H_

#include "../logic/IDownloadRateObserver.h"
#include "BytesRange.hpp"

#include <vlc_common.h>

#include <vector>
#include <list>
#include <string>

namespace adaptive
{
    namespace http
    {
        class ConnectionParams;
        class AbstractConnectionFactory;
        class AbstractConnection;
        class Downloader;
        class AbstractChunkSource;
        class HTTPChunkBufferedSource;
        enum class ChunkType;

        class AbstractConnectionManager : public IDownloadRateObserver
        {
            public:
                AbstractConnectionManager(vlc_object_t *);
                ~AbstractConnectionManager();
                virtual void    closeAllConnections () = 0;
                virtual AbstractConnection * getConnection(ConnectionParams &) = 0;
                virtual AbstractChunkSource *makeSource(const std::string &,
                                                        const ID &, ChunkType,
                                                        const BytesRange &) = 0;
                virtual void recycleSource(AbstractChunkSource *) = 0;

                virtual void start(AbstractChunkSource *) = 0;
                virtual void cancel(AbstractChunkSource *) = 0;

                virtual void updateDownloadRate(const ID &, size_t,
                                                vlc_tick_t, vlc_tick_t) override;
                void setDownloadRateObserver(IDownloadRateObserver *);

            protected:
                void deleteSource(AbstractChunkSource *);
                vlc_object_t                                       *p_object;

            private:
                IDownloadRateObserver                              *rateObserver;
        };

        class HTTPConnectionManager : public AbstractConnectionManager
        {
            public:
                HTTPConnectionManager           (vlc_object_t *p_object);
                virtual ~HTTPConnectionManager  ();

                virtual void    closeAllConnections ()  override;
                virtual AbstractConnection * getConnection(ConnectionParams &)  override;
                virtual AbstractChunkSource *makeSource(const std::string &,
                                                        const ID &, ChunkType,
                                                        const BytesRange &) override;
                virtual void recycleSource(AbstractChunkSource *) override;

                virtual void start(AbstractChunkSource *)  override;
                virtual void cancel(AbstractChunkSource *)  override;
                void         setLocalConnectionsAllowed();
                void         addFactory(AbstractConnectionFactory *);

            private:
                void    releaseAllConnections ();
                Downloader                                         *downloader;
                Downloader                                         *downloaderhp;
                vlc_mutex_t                                         lock;
                std::vector<AbstractConnection *>                   connectionPool;
                std::list<AbstractConnectionFactory *>              factories;
                bool                                                localAllowed;
                AbstractConnection * reuseConnection(ConnectionParams &);
                Downloader * getDownloadQueue(const AbstractChunkSource *) const;
                std::list<HTTPChunkBufferedSource *> cache;
                size_t cache_total;
                size_t cache_max;
        };
    }
}

#endif /* HTTPCONNECTIONMANAGER_H_ */
