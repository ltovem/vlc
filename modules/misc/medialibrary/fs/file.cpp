/*****************************************************************************
 * file.cpp: Media library network file
 *****************************************************************************
 * Copyright (C) 2018 VLC authors, VideoLAN and VideoLabs
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "file.h"
#include "util.h"

namespace vlc {
  namespace medialibrary {

SDFile::SDFile( const std::string mrl, const int64_t size, const time_t lastModificationDate )
    : m_mrl( std::move( mrl ) )
    , m_name( utils::fileName( m_mrl ) )
    , m_extension( utils::extension( m_mrl ) )
    , m_isNetwork( m_mrl.find( "file://" ) != 0 )
    , m_size( size )
    , m_lastModificationTime( lastModificationDate )
{
}
SDFile::SDFile( const std::string mrl,
                const LinkedFileType fType,
                const std::string linkedFile,
                const int64_t size,
                const time_t lastModificationDate )
    : m_mrl( std::move( mrl ) )
    , m_name( utils::fileName( m_mrl ) )
    , m_extension( utils::extension( m_mrl ) )
    , m_linkedFile( std::move( linkedFile ) )
    , m_linkedType( fType )
    , m_isNetwork( m_mrl.find( "file://" ) != 0 )
    , m_size( size )
    , m_lastModificationTime( lastModificationDate )
{
}

const std::string &
SDFile::mrl() const
{
    return m_mrl;
}

const std::string &
SDFile::name() const
{
    return m_name;
}

const std::string &
SDFile::extension() const
{
    return m_extension;
}

time_t
SDFile::lastModificationDate() const
{
    return m_lastModificationTime;
}

bool
SDFile::isNetwork() const
{
  return m_isNetwork;
}

int64_t
SDFile::size() const
{
    return m_size;
}

IFile::LinkedFileType SDFile::linkedType() const
{
    return m_linkedType;
}

const std::string &SDFile::linkedWith() const
{
    return m_linkedFile;
}

  } /* namespace medialibrary */
} /* namespace vlc */
