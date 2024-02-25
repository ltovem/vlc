/*****************************************************************************
 * VLCLibraryDataTypes.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#import "VLCLibraryDataTypes.h"

#import "main/VLCMain.h"
#import "extensions/NSString+Helpers.h"
#import "library/VLCInputItem.h"

#import <vlc_media_library.h>
#import <vlc_url.h>

NSString * const VLCMediaLibraryMediaItemPasteboardType = @"VLCMediaLibraryMediaItemPasteboardType";
NSString * const VLCMediaLibraryMediaItemUTI = @"org.videolan.vlc.VLCMediaLibraryMediaItem";

const CGFloat VLCMediaLibrary4KWidth = 3840.;
const CGFloat VLCMediaLibrary4KHeight = 2160.;
const CGFloat VLCMediaLibrary720pWidth = 1280.;
const CGFloat VLCMediaLibrary720pHeight = 720.;
const long long int VLCMediaLibraryMediaItemDurationDenominator = 1000;

NSString *VLCMediaLibraryMediaItemLibraryID = @"VLCMediaLibraryMediaItemLibraryID";

typedef vlc_ml_media_list_t* (*library_mediaitem_list_fetch_f)(vlc_medialibrary_t*, const vlc_ml_query_params_t*, int64_t);
typedef vlc_ml_album_list_t* (*library_album_list_fetch_f)(vlc_medialibrary_t*, const vlc_ml_query_params_t*, int64_t);
typedef vlc_ml_artist_list_t* (*library_artist_list_fetch_f)(vlc_medialibrary_t*, const vlc_ml_query_params_t*, int64_t);

static vlc_medialibrary_t *getMediaLibrary(void)
{
    intf_thread_t *p_intf = getIntf();
    if (!p_intf) {
        return nil;
    }
    return vlc_ml_instance_get(p_intf);
}

static NSArray<VLCMediaLibraryMediaItem *> *fetchMediaItemsForLibraryItem(library_mediaitem_list_fetch_f fetchFunction, int64_t itemId)
{
    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_media_list_t *p_mediaList = fetchFunction(p_mediaLibrary, NULL, itemId);
    if(!p_mediaList) {
        return nil;
    }

    NSMutableArray *mutableArray = [[NSMutableArray alloc] initWithCapacity:p_mediaList->i_nb_items];
    for (size_t x = 0; x < p_mediaList->i_nb_items; x++) {
        VLCMediaLibraryMediaItem *mediaItem = [[VLCMediaLibraryMediaItem alloc] initWithMediaItem:&p_mediaList->p_items[x]];
        [mutableArray addObject:mediaItem];
    }
    vlc_ml_media_list_release(p_mediaList);
    return [mutableArray copy];
}

static NSArray<VLCMediaLibraryAlbum *> *fetchAlbumsForLibraryItem(library_album_list_fetch_f fetchFunction, int64_t itemId)
{
    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_album_list_t *p_albumList = fetchFunction(p_mediaLibrary, NULL, itemId);
    if(!p_albumList) {
        return nil;
    }

    NSMutableArray *mutableArray = [[NSMutableArray alloc] initWithCapacity:p_albumList->i_nb_items];
    for (size_t x = 0; x < p_albumList->i_nb_items; x++) {
        VLCMediaLibraryAlbum *album = [[VLCMediaLibraryAlbum alloc] initWithAlbum:&p_albumList->p_items[x]];
        [mutableArray addObject:album];
    }
    vlc_ml_album_list_release(p_albumList);
    return [mutableArray copy];
}

static NSArray<VLCMediaLibraryArtist *> *fetchArtistsForLibraryItem(library_artist_list_fetch_f fetchFunction, int64_t itemId)
{
    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_artist_list_t *p_artistList = fetchFunction(p_mediaLibrary, NULL, itemId);
    if(!p_artistList) {
        return nil;
    }

    NSMutableArray *mutableArray = [[NSMutableArray alloc] initWithCapacity:p_artistList->i_nb_items];
    for (size_t x = 0; x < p_artistList->i_nb_items; x++) {
        VLCMediaLibraryArtist *artist = [[VLCMediaLibraryArtist alloc] initWithArtist:&p_artistList->p_items[x]];
        [mutableArray addObject:artist];
    }
    vlc_ml_artist_list_release(p_artistList);
    return [mutableArray copy];
}

static NSString *genreArrayDisplayString(NSArray<VLCMediaLibraryGenre *> * const genres)
{
    const NSUInteger genreCount = genres.count;
    if (genreCount == 0) {
        return @"";
    }

    VLCMediaLibraryGenre * const firstGenre = genres.firstObject;
    if (genreCount == 1) {
        return firstGenre.name;
    }

    VLCMediaLibraryGenre * const secondGenre = [genres objectAtIndex:1];
    return [NSString stringWithFormat:_NS("%@, %@, and %lli other genres"),
                     firstGenre.name,
                     secondGenre.name,
                     genreCount - 2];
}

@implementation VLCMediaLibraryFile

- (instancetype)initWithFile:(struct vlc_ml_file_t *)p_file
{
    self = [super init];
    if (self && p_file != NULL) {
        _MRL = toNSStr(p_file->psz_mrl);
        _fileType = p_file->i_type;
        _external = p_file->b_external;
        _removable = p_file->b_removable;
        _present = p_file->b_present;
        _lastModificationDate = p_file->i_last_modification_date;
    }
    return self;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ — type: %i, MRL: %@", NSStringFromClass([self class]), _fileType, _MRL];
}

- (NSURL *)fileURL
{
    return [NSURL URLWithString:_MRL];
}

- (NSString *)readableFileType
{
    switch (_fileType) {
        case VLC_ML_FILE_TYPE_MAIN:
            return _NS("Main");
            break;

        case VLC_ML_FILE_TYPE_PART:
            return _NS("Part");
            break;

        case VLC_ML_FILE_TYPE_PLAYLIST:
            return _NS("Playlist");
            break;

        case VLC_ML_FILE_TYPE_SUBTITLE:
            return _NS("Subtitle");
            break;

        case VLC_ML_FILE_TYPE_SOUNDTRACK:
            return _NS("Soundtrack");

        default:
            return _NS("Unknown");
            break;
    }
}

@end

@implementation VLCMediaLibraryTrack

- (instancetype)initWithTrack:(struct vlc_ml_media_track_t *)p_track
{
    self = [super init];
    if (self && p_track != NULL) {
        _codec = toNSStr(p_track->psz_codec);
        _language = toNSStr(p_track->psz_language);
        _trackDescription = toNSStr(p_track->psz_description);
        _trackType = p_track->i_type;
        _bitrate = p_track->i_bitrate;

        _numberOfAudioChannels = p_track->a.i_nbChannels;
        _audioSampleRate = p_track->a.i_sampleRate;

        _videoHeight = p_track->v.i_height;
        _videoWidth = p_track->v.i_width;
        _sourceAspectRatio = p_track->v.i_sarNum;
        _sourceAspectRatioDenominator = p_track->v.i_sarDen;
        _frameRate = p_track->v.i_fpsNum;
        _frameRateDenominator = p_track->v.i_fpsDen;
    }
    return self;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ — type: %i, codec %@", NSStringFromClass([self class]), _trackType, _codec];
}

- (NSString *)readableCodecName
{
    vlc_fourcc_t fourcc = vlc_fourcc_GetCodecFromString(UNKNOWN_ES, _codec.UTF8String);
    return toNSStr(vlc_fourcc_GetDescription(UNKNOWN_ES, fourcc));
}

- (NSString *)readableTrackType
{
    switch (_trackType) {
        case VLC_ML_TRACK_TYPE_AUDIO:
            return _NS("Audio");
            break;

        case VLC_ML_TRACK_TYPE_VIDEO:
            return _NS("Video");

        default:
            return _NS("Unknown");
            break;
    }
}

@end

@implementation VLCMediaLibraryMovie

- (instancetype)initWithMovie:(struct vlc_ml_movie_t *)p_movie
{
    self = [super init];
    if (self && p_movie != NULL) {
        _summary = toNSStr(p_movie->psz_summary);
        _imdbID = toNSStr(p_movie->psz_imdb_id);
    }
    return self;
}

@end

@implementation VLCMediaLibraryShowEpisode

- (instancetype)initWithShowEpisode:(struct vlc_ml_show_episode_t *)p_showEpisode
{
    self = [super init];
    if (self && p_showEpisode != NULL) {
        _summary = toNSStr(p_showEpisode->psz_summary);
        _tvdbID = toNSStr(p_showEpisode->psz_tvdb_id);
        _episodeNumber = p_showEpisode->i_episode_nb;
        _seasonNumber = p_showEpisode->i_season_number;
    }
    return self;
}

@end

#pragma mark - Media library item classes

@interface VLCAbstractMediaLibraryItem ()

@property (readwrite, assign) int64_t libraryID;
@property (readwrite, assign) BOOL smallArtworkGenerated;
@property (readwrite, assign) BOOL primaryActionableDetail;
@property (readwrite, assign) BOOL secondaryActionableDetail;
@property (readwrite, atomic, strong) NSString *smallArtworkMRL;
@property (readwrite, atomic, strong) NSString *displayString;
@property (readwrite, atomic, strong) NSString *primaryDetailString;
@property (readwrite, atomic, strong) NSString *secondaryDetailString;
@property (readwrite, atomic, strong) NSString *durationString;

@end

@implementation VLCAbstractMediaLibraryItem

- (VLCMediaLibraryMediaItem *)firstMediaItem
{
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (NSArray<VLCMediaLibraryMediaItem *> *)mediaItems
{
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (id<VLCMediaLibraryItemProtocol>)primaryActionableDetailLibraryItem
{
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (id<VLCMediaLibraryItemProtocol>)secondaryActionableDetailLibraryItem
{
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (void)moveToTrash
{
    [self doesNotRecognizeSelector:_cmd];
}

- (void)revealInFinder
{
    [self doesNotRecognizeSelector:_cmd];
}

- (void)iterateMediaItemsWithBlock:(nonnull void (^)(VLCMediaLibraryMediaItem * _Nonnull))mediaItemBlock
{
    [self doesNotRecognizeSelector:_cmd];
}

@end

@interface VLCAbstractMediaLibraryAudioGroup ()
{
    NSArray<VLCMediaLibraryGenre *> *_genres;
}
@end

@implementation VLCAbstractMediaLibraryAudioGroup

- (NSArray<VLCMediaLibraryGenre *> *)genres
{
    if (_genres == nil) {
        NSMutableSet<NSNumber *> * const mutableGenreIDs = NSMutableSet.set;
        NSMutableArray<VLCMediaLibraryGenre *> * const mutableGenres = NSMutableArray.array;

        for (VLCMediaLibraryMediaItem * const mediaItem in self.mediaItems) {
            const int64_t genreID = mediaItem.genreID;
            NSNumber * const numberGenreID = @(mediaItem.genreID);
            if ([mutableGenreIDs containsObject:numberGenreID]) {
                continue;
            }

            VLCMediaLibraryGenre * const genre = [VLCMediaLibraryGenre genreWithID:genreID];
            if (genre == nil) {
                continue;
            }

            [mutableGenreIDs addObject:numberGenreID];
            [mutableGenres addObject:genre];
        }

        _genres = mutableGenres.copy;
    }

    return _genres;
}

- (NSArray<VLCMediaLibraryArtist *> *)artists
{
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (NSArray<VLCMediaLibraryAlbum *> *)albums
{
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (NSArray<VLCMediaLibraryMediaItem *> *)mediaItems
{
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (unsigned int)numberOfTracks
{
    [self doesNotRecognizeSelector:_cmd];
    return 0;
}

- (VLCMediaLibraryParentGroupType)matchingParentType
{
    [self doesNotRecognizeSelector:_cmd];
    return VLCMediaLibraryParentGroupTypeUnknown;
}

- (void)iterateMediaItemsWithBlock:(void (^)(VLCMediaLibraryMediaItem*))mediaItemBlock
{
    [self doesNotRecognizeSelector:_cmd];
}

- (VLCMediaLibraryMediaItem *)firstMediaItem
{
    return self.mediaItems.firstObject;
}

- (void)moveToTrash
{
    [self iterateMediaItemsWithBlock:^(VLCMediaLibraryMediaItem* childMediaItem) {
        [childMediaItem moveToTrash];
    }];
}

- (void)revealInFinder
{
    [self.firstMediaItem revealInFinder];
}

@end

@interface VLCMediaLibraryArtist ()
{
    NSString *_genreString;
}
@end

@implementation VLCMediaLibraryArtist

@synthesize numberOfTracks = _numberOfTracks;

+ (nullable instancetype)artistWithID:(int64_t)artistID
{
    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_artist_t *p_artist = vlc_ml_get_artist(p_mediaLibrary, artistID);
    VLCMediaLibraryArtist *artist = nil;
    if (p_artist) {
        artist = [[VLCMediaLibraryArtist alloc] initWithArtist:p_artist];
    }
    return artist;
}

- (instancetype)initWithArtist:(struct vlc_ml_artist_t *)p_artist;
{
    self = [super init];
    if (self && p_artist != NULL) {
        self.libraryID = p_artist->i_id;
        self.smallArtworkGenerated = p_artist->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl != NULL;
        self.smallArtworkMRL = self.smallArtworkGenerated ? toNSStr(p_artist->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl) : nil;
        self.primaryActionableDetail = NO;
        self.secondaryActionableDetail = YES;

        _name = toNSStr(p_artist->psz_name);
        if ([_name isEqualToString:@""]) {
            _name = _NS("Unknown Artist");
        }

        _shortBiography = toNSStr(p_artist->psz_shortbio);
        _musicBrainzID = toNSStr(p_artist->psz_mb_id);
        _numberOfAlbums = p_artist->i_nb_album;
        _numberOfTracks = p_artist->i_nb_tracks;
    }
    return self;
}

- (NSString *)displayString
{
    return _name;
}

- (NSString *)primaryDetailString
{
    return [self durationString];
}

- (NSString *)secondaryDetailString
{
    return self.genreString;
}

- (NSString *)durationString
{
    NSString *countMetadataString;
    if (_numberOfAlbums > 1) {
        countMetadataString = [NSString stringWithFormat:_NS("%u albums"), _numberOfAlbums];
    } else {
        countMetadataString = _NS("1 album");
    }
    if (_numberOfTracks > 1) {
        countMetadataString = [countMetadataString stringByAppendingFormat:@", %@", [NSString stringWithFormat:_NS("%u songs"), _numberOfTracks]];
    } else {
        countMetadataString = [countMetadataString stringByAppendingFormat:@", %@", _NS("1 song")];
    }

    return countMetadataString;
}

- (NSString *)genreString
{
    if (_genreString == nil || [_genreString isEqualToString:@""]) {
        _genreString = self.genres == nil ? @"" : genreArrayDisplayString(self.genres);
    }

    return _genreString;
}

- (NSArray<VLCMediaLibraryArtist *> *)artists
{
    return @[self];
}

- (NSArray<VLCMediaLibraryAlbum *> *)albums
{
    return fetchAlbumsForLibraryItem(vlc_ml_list_artist_albums, self.libraryID);
}

- (NSArray<VLCMediaLibraryMediaItem *> *)mediaItems
{
    return fetchMediaItemsForLibraryItem(vlc_ml_list_artist_tracks, self.libraryID);
}

- (VLCMediaLibraryParentGroupType)matchingParentType
{
    return VLCMediaLibraryParentGroupTypeArtist;
}

- (id<VLCMediaLibraryItemProtocol>)secondaryActionableDetailLibraryItem
{
    return self.genres.firstObject;
}

- (void)iterateMediaItemsWithBlock:(void (^)(VLCMediaLibraryMediaItem*))mediaItemBlock;
{
    for(VLCMediaLibraryAlbum* album in self.albums) {
        [album iterateMediaItemsWithBlock:mediaItemBlock];
    }
}

@end

@interface VLCMediaLibraryAlbum ()
{
    NSString *_genreString;
}
@end

@implementation VLCMediaLibraryAlbum

@synthesize numberOfTracks = _numberOfTracks;
@synthesize primaryActionableDetailLibraryItem = _primaryActionableDetailLibraryItem;

+ (nullable instancetype)albumWithID:(int64_t)albumID
{
    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_album_t *p_album = vlc_ml_get_album(p_mediaLibrary, albumID);
    VLCMediaLibraryAlbum *album = nil;
    if (p_album) {
        album = [[VLCMediaLibraryAlbum alloc] initWithAlbum:p_album];
    }
    return album;
}

- (instancetype)initWithAlbum:(struct vlc_ml_album_t *)p_album
{
    self = [super init];
    if (self && p_album != NULL) {
        self.libraryID = p_album->i_id;
        self.smallArtworkGenerated = p_album->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl != NULL;
        self.smallArtworkMRL = self.smallArtworkGenerated ? toNSStr(p_album->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl) : nil;
        self.primaryActionableDetail = YES;
        self.secondaryActionableDetail = YES;

        _title = toNSStr(p_album->psz_title);
        if ([_title isEqualToString:@""]) {
            _title = _NS("Unknown Album");
        }

        _summary = toNSStr(p_album->psz_summary);
        _artistName = toNSStr(p_album->psz_artist);
        _artistID = p_album->i_artist_id;
        _numberOfTracks = p_album->i_nb_tracks;
        _duration = p_album->i_duration;
        _year = p_album->i_year;
    }
    return self;
}

- (NSString *)displayString
{
    return _title;
}

- (NSString *)primaryDetailString
{
    return _artistName;
}

- (NSString *)secondaryDetailString
{
    return self.genreString;
}

- (NSString *)genreString
{
    if (_genreString == nil || [_genreString isEqualToString:@""]) {
        _genreString = genreArrayDisplayString(self.genres);
    }

    return _genreString;
}

- (NSString *)durationString
{
    return [NSString stringWithTime:_duration / VLCMediaLibraryMediaItemDurationDenominator];
}

- (NSArray<VLCMediaLibraryArtist *> *)artists
{
    return @[[VLCMediaLibraryArtist artistWithID:_artistID]];
}

- (NSArray<VLCMediaLibraryAlbum *> *)albums
{
    return @[self];
}

- (NSArray<VLCMediaLibraryMediaItem *> *)mediaItems
{
    return fetchMediaItemsForLibraryItem(vlc_ml_list_album_tracks, self.libraryID);
}

- (VLCMediaLibraryParentGroupType)matchingParentType
{
    return VLCMediaLibraryParentGroupTypeAlbum;
}

- (id<VLCMediaLibraryItemProtocol>)primaryActionableDetailLibraryItem
{
    if (_primaryActionableDetailLibraryItem == nil) {
        _primaryActionableDetailLibraryItem = [VLCMediaLibraryArtist artistWithID:self.artistID];
    }

    return _primaryActionableDetailLibraryItem;
}

- (id<VLCMediaLibraryItemProtocol>)secondaryActionableDetailLibraryItem
{
    return self.genres.firstObject;
}

- (void)iterateMediaItemsWithBlock:(void (^)(VLCMediaLibraryMediaItem*))mediaItemBlock
{
    for(VLCMediaLibraryMediaItem* mediaItem in self.mediaItems) {
        mediaItemBlock(mediaItem);
    }
}

@end

@implementation VLCMediaLibraryGenre

@synthesize numberOfTracks = _numberOfTracks;

- (instancetype)initWithGenre:(struct vlc_ml_genre_t *)p_genre
{
    self = [super init];
    if (self && p_genre != NULL) {
        self.libraryID = p_genre->i_id;
        self.smallArtworkGenerated = p_genre->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl != NULL;
        self.smallArtworkMRL = self.smallArtworkGenerated ? toNSStr(p_genre->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl) : nil;
        self.primaryActionableDetail = NO;
        self.secondaryActionableDetail = NO;

        _name = toNSStr(p_genre->psz_name);
        if ([_name isEqualToString:@""]) {
            _name = _NS("Unknown Genre");
        }

        _numberOfTracks = p_genre->i_nb_tracks;
    }
    return self;
}

+ (nullable instancetype)genreWithID:(int64_t)genreID
{
    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_genre_t *p_genre = vlc_ml_get_genre(p_mediaLibrary, genreID);
    VLCMediaLibraryGenre *genre = nil;
    if (p_genre) {
        genre = [[VLCMediaLibraryGenre alloc] initWithGenre:p_genre];
    }
    return genre;
}

- (NSString *)displayString
{
    return _name;
}

- (NSString *)primaryDetailString
{
    return [self durationString];
}

- (NSString *)durationString
{
    if (_numberOfTracks > 1) {
        return [NSString stringWithFormat:_NS("%u songs"), _numberOfTracks];
    } else {
        return _NS("1 song");
    }
}

- (NSArray<VLCMediaLibraryAlbum *> *)albums
{
    return fetchAlbumsForLibraryItem(vlc_ml_list_genre_albums, self.libraryID);
}

- (NSArray<VLCMediaLibraryArtist *> *)artists
{
    return fetchArtistsForLibraryItem(vlc_ml_list_genre_artists, self.libraryID);
}

- (NSArray<VLCMediaLibraryGenre *> *)genres
{
    // Overloading superclass behaviour
    return @[self];
}

- (NSArray<VLCMediaLibraryMediaItem *> *)mediaItems
{
    return fetchMediaItemsForLibraryItem(vlc_ml_list_genre_tracks, self.libraryID);
}

- (VLCMediaLibraryParentGroupType)matchingParentType
{
    return VLCMediaLibraryParentGroupTypeGenre;
}

- (void)iterateMediaItemsWithBlock:(void (^)(VLCMediaLibraryMediaItem*))mediaItemBlock
{
    // By default iterate album-by-album
    [self iterateMediaItemsWithBlock:mediaItemBlock orderedBy:VLC_ML_PARENT_ALBUM];
}

- (void)iterateMediaItemsWithBlock:(void (^)(VLCMediaLibraryMediaItem*))mediaItemBlock orderedBy:(int)mediaItemParentType
{
    NSArray<id<VLCMediaLibraryItemProtocol>> *childItems;
    switch(mediaItemParentType) {
        case VLC_ML_PARENT_ARTIST:
            childItems = self.artists;
            break;
        case VLC_ML_PARENT_ALBUM:
            childItems = self.albums;
            break;
        case VLC_ML_PARENT_UNKNOWN:
        default:
            childItems = self.mediaItems;
            break;
    }

    for(id<VLCMediaLibraryItemProtocol> childItem in childItems) {
        [childItem iterateMediaItemsWithBlock:mediaItemBlock];
    }
}

@end

@interface VLCMediaLibraryPlaylist ()
{
    NSArray<VLCMediaLibraryMediaItem *> *_mediaItems;
}
@end

@implementation VLCMediaLibraryPlaylist

+ (instancetype)playlistForLibraryID:(int64_t)libraryID
{
    vlc_medialibrary_t * const p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }

    vlc_ml_playlist_t * const p_playlist = vlc_ml_get_playlist(p_mediaLibrary, libraryID);
    if (p_playlist == NULL) {
        return nil;
    }

    return [[VLCMediaLibraryPlaylist alloc] initWithPlaylist:p_playlist];
}

- (instancetype)initWithPlaylist:(struct vlc_ml_playlist_t *)p_playlist
{
    self = [super init];
    if (self && p_playlist != NULL) {
        self.libraryID = p_playlist->i_id;
        self.smallArtworkMRL = toNSStr(p_playlist->psz_artwork_mrl);
        self.displayString = toNSStr(p_playlist->psz_name);
        self.detailString = [NSString stringWithFormat:@"%u items", p_playlist->i_nb_media];
        self.durationString = [NSString stringWithTime:p_playlist->i_duration / VLCMediaLibraryMediaItemDurationDenominator];

        _MRL = toNSStr(p_playlist->psz_mrl);

        _numberOfMedia = p_playlist->i_nb_media;
        _numberOfAudios = p_playlist->i_nb_audio;
        _numberOfVideos = p_playlist->i_nb_video;
        _numberOfUnknowns = p_playlist->i_nb_unknown;

        _numberOfPresentMedia = p_playlist->i_nb_present_media;
        _numberOfPresentAudios = p_playlist->i_nb_present_audio;
        _numberOfPresentVideos = p_playlist->i_nb_present_video;
        _numberOfPresentUnknowns = p_playlist->i_nb_present_unknown;

        _creationDate = [NSDate dateWithTimeIntervalSince1970:p_playlist->i_creation_date];

        _duration = p_playlist->i_duration;
        _numberDurationUnknown = p_playlist->i_nb_duration_unknown;

        _readOnly = p_playlist->b_is_read_only;
    }
    return self;
}

- (void)fetchMediaItems
{
    NSMutableArray<VLCMediaLibraryMediaItem *> * const fetchedMediaItems = NSMutableArray.array;
    vlc_ml_media_list_t * const p_media_list = vlc_ml_list_playlist_media(getMediaLibrary(), NULL, self.libraryID);

    for (NSUInteger i = 0; i < p_media_list->i_nb_items; ++i) {
        vlc_ml_media_t p_media_item = p_media_list->p_items[i];
        VLCMediaLibraryMediaItem * const item = [[VLCMediaLibraryMediaItem alloc] initWithMediaItem:&p_media_item];
        [fetchedMediaItems addObject:item];
    }

    vlc_ml_media_list_release(p_media_list);
    _mediaItems = fetchedMediaItems.copy;
}

- (NSArray<VLCMediaLibraryMediaItem *> *)mediaItems
{
    if (_mediaItems == nil) {
        [self fetchMediaItems];
    }
    return _mediaItems;
}
- (void)moveToTrash
{
    NSFileManager * const fileManager = NSFileManager.defaultManager;
    NSURL * const URL = [NSURL URLWithString:_MRL];
    [fileManager trashItemAtURL:URL
               resultingItemURL:nil
                          error:nil];
}

- (void)revealInFinder
{
    NSURL * const URL = [NSURL URLWithString:_MRL];
    if (URL) {
        [NSWorkspace.sharedWorkspace activateFileViewerSelectingURLs:@[URL]];
    }
}

@end

@interface VLCMediaLibraryMediaItem ()
{
    NSString *_primaryDetailString;
    NSString *_secondaryDetailString;
}

@property (readwrite, assign) vlc_medialibrary_t *p_mediaLibrary;

@end

@implementation VLCMediaLibraryMediaItem

@synthesize primaryActionableDetailLibraryItem = _primaryActionableDetailLibraryItem;
@synthesize secondaryActionableDetailLibraryItem = _secondaryActionableDetailLibraryItem;

#pragma mark - initialization

+ (nullable instancetype)mediaItemForLibraryID:(int64_t)libraryID
{
    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if(!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_media_t *p_mediaItem = vlc_ml_get_media(p_mediaLibrary, libraryID);
    VLCMediaLibraryMediaItem *returnValue = nil;
    if (p_mediaItem) {
        returnValue = [[VLCMediaLibraryMediaItem alloc] initWithMediaItem:p_mediaItem library:p_mediaLibrary];
    }
    return returnValue;
}

+ (nullable instancetype)mediaItemForURL:(NSURL *)url
{
    if (url == nil) {
        return nil;
    }

    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if (!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_media_t *p_mediaItem = vlc_ml_get_media_by_mrl(p_mediaLibrary,
                                                          [[url absoluteString] UTF8String]);
    VLCMediaLibraryMediaItem *returnValue = nil;
    if (p_mediaItem) {
        returnValue = [[VLCMediaLibraryMediaItem alloc] initWithMediaItem:p_mediaItem library:p_mediaLibrary];
    }
    return returnValue;
}

- (nullable instancetype)initWithMediaItem:(struct vlc_ml_media_t *)p_mediaItem
{
    if (p_mediaItem == NULL) {
        return nil;
    }

    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if (!p_mediaLibrary) {
        return nil;
    }
    if (p_mediaItem != NULL && p_mediaLibrary != NULL) {
        return [self initWithMediaItem:p_mediaItem library:p_mediaLibrary];
    }
    return nil;
}

- (instancetype)initWithMediaItem:(struct vlc_ml_media_t *)p_mediaItem library:(vlc_medialibrary_t *)p_mediaLibrary
{
    self = [super init];
    if (self && p_mediaItem != NULL && p_mediaLibrary != NULL) {
        self.libraryID = p_mediaItem->i_id;
        self.smallArtworkGenerated = p_mediaItem->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl != NULL;
        self.smallArtworkMRL = self.smallArtworkGenerated ? toNSStr(p_mediaItem->thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl) : nil;

        const BOOL isAlbumTrack = p_mediaItem->i_subtype == VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK;
        self.primaryActionableDetail = isAlbumTrack;
        self.secondaryActionableDetail = isAlbumTrack;

        _p_mediaLibrary = p_mediaLibrary;
        _mediaType = p_mediaItem->i_type;
        _mediaSubType = p_mediaItem->i_subtype;

        NSMutableArray *mutArray = [[NSMutableArray alloc] initWithCapacity:p_mediaItem->p_files->i_nb_items];
        for (size_t x = 0; x < p_mediaItem->p_files->i_nb_items; x++) {
            VLCMediaLibraryFile *file = [[VLCMediaLibraryFile alloc] initWithFile:&p_mediaItem->p_files->p_items[x]];
            if (file) {
                [mutArray addObject:file];
            }
        }
        _files = [mutArray copy];

        mutArray = [[NSMutableArray alloc] initWithCapacity:p_mediaItem->p_tracks->i_nb_items];
        for (size_t x = 0; x < p_mediaItem->p_tracks->i_nb_items; x++) {
            VLCMediaLibraryTrack *track = [[VLCMediaLibraryTrack alloc] initWithTrack:&p_mediaItem->p_tracks->p_items[x]];
            if (track) {
                [mutArray addObject:track];
                if (track.trackType == VLC_ML_TRACK_TYPE_VIDEO && _firstVideoTrack == nil) {
                    _firstVideoTrack = track;
                }
            }
        }
        _tracks = [mutArray copy];

        _year = p_mediaItem->i_year;
        _duration = p_mediaItem->i_duration;
        _playCount = p_mediaItem->i_playcount;
        _lastPlayedDate = p_mediaItem->i_last_played_date;
        _progress = p_mediaItem->f_progress;
        _favorited = p_mediaItem->b_is_favorite;

        _title = toNSStr(p_mediaItem->psz_title);
        if ([_title isEqualToString:@""]) {
            _title = _NS("Unknown Media Item");
        }

        switch (p_mediaItem->i_subtype) {
            case VLC_ML_MEDIA_SUBTYPE_MOVIE:
                _movie = [[VLCMediaLibraryMovie alloc] initWithMovie:&p_mediaItem->movie];
                break;

            case VLC_ML_MEDIA_SUBTYPE_SHOW_EPISODE:
                _showEpisode = [[VLCMediaLibraryShowEpisode alloc] initWithShowEpisode:&p_mediaItem->show_episode];
                break;

            case VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK:
                _artistID = p_mediaItem->album_track.i_artist_id;
                _albumID = p_mediaItem->album_track.i_album_id;
                _genreID = p_mediaItem->album_track.i_genre_id;
                _trackNumber = p_mediaItem->album_track.i_track_nb;
                _discNumber = p_mediaItem->album_track.i_disc_nb;
                break;

            default:
                break;
        }
    }
    return self;
}

- (nullable instancetype)initWithExternalURL:(NSURL *)url
{
    if (url == nil) {
        return nil;
    }

    NSString *urlString = url.absoluteString;
    if (!urlString) {
        return self;
    }

    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if (!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_media_t *p_media = vlc_ml_new_external_media(p_mediaLibrary, urlString.UTF8String);
    if (p_media) {
        self = [self initWithMediaItem:p_media library:p_mediaLibrary];
        vlc_ml_media_release(p_media);
    }
    return self;
}

- (nullable instancetype)initWithStreamURL:(NSURL *)url
{
    if (url == nil) {
        return nil;
    }

    NSString *urlString = url.absoluteString;
    if (!urlString) {
        return self;
    }

    vlc_medialibrary_t *p_mediaLibrary = getMediaLibrary();
    if (!p_mediaLibrary) {
        return nil;
    }
    vlc_ml_media_t *p_media = vlc_ml_new_stream(p_mediaLibrary, urlString.UTF8String);
    if (p_media) {
        self = [self initWithMediaItem:p_media library:p_mediaLibrary];
        vlc_ml_media_release(p_media);
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder
{
    if (aDecoder == nil) {
        return nil;
    }

    int64_t libraryID = [aDecoder decodeInt64ForKey:VLCMediaLibraryMediaItemLibraryID];
    self = [VLCMediaLibraryMediaItem mediaItemForLibraryID:libraryID];
    return self;
}

- (void)encodeWithCoder:(NSCoder *)aCoder
{
    if (aCoder == nil) {
        return;
    }

    [aCoder encodeInt64:self.libraryID forKey:VLCMediaLibraryMediaItemLibraryID];
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ — title: %@, ID: %lli, type: %i, artwork: %@",
            NSStringFromClass([self class]), _title, self.libraryID, _mediaType, self.smallArtworkMRL];
}

- (NSString *)readableMediaType
{
    switch (_mediaType) {
        case VLC_ML_MEDIA_TYPE_AUDIO:
            return _NS("Audio");
            break;

        case VLC_ML_MEDIA_TYPE_VIDEO:
            return _NS("Video");
            break;

        default:
            return _NS("Unknown");
            break;
    }
}

- (NSString *)readableMediaSubType
{
    switch (_mediaSubType) {
        case VLC_ML_MEDIA_SUBTYPE_MOVIE:
            return _NS("Movie");
            break;

        case VLC_ML_MEDIA_SUBTYPE_SHOW_EPISODE:
            return _NS("Show Episode");
            break;

        case VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK:
            return _NS("Album Track");
            break;

        default:
            return _NS("Unknown");
            break;
    }
}

- (NSString *)displayString
{
    return _title;
}

- (nullable NSString *)contextualPrimaryDetailString
{
    switch (self.mediaSubType) {
    case VLC_ML_MEDIA_SUBTYPE_SHOW_EPISODE:
        return self.inputItem.showName;
    case VLC_ML_MEDIA_SUBTYPE_MOVIE:
        return self.inputItem.director;
    case VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK:
    {
        VLCMediaLibraryArtist * const artist = [VLCMediaLibraryArtist artistWithID:_artistID];
        return artist.name;
    }
    default:
        return nil;
    }
}

- (NSString *)primaryDetailString
{
    if (_primaryDetailString == nil || [_primaryDetailString isEqualToString:@""]) {
        NSString * const contextualString = [self contextualPrimaryDetailString];
        const BOOL validContextualString = contextualString != nil && contextualString.length > 0;
        _primaryDetailString = validContextualString ? contextualString : self.durationString;
    }

    return _primaryDetailString;
}

- (nullable NSString *)contextualSecondaryDetailString
{
    switch (self.mediaSubType) {
    case VLC_ML_MEDIA_SUBTYPE_SHOW_EPISODE:
    {
        VLCInputItem * const inputItem = self.inputItem;
        return [NSString stringWithFormat:_NS("Season %u, Episode %u"),
                         inputItem.season,
                         inputItem.episode];
    }
    case VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK:
    {
        VLCMediaLibraryGenre * const genre = [VLCMediaLibraryGenre genreWithID:self.genreID];
        return genreArrayDisplayString(@[genre]);
    }
    default:
        return self.inputItem.date;
    }
}

- (NSString *)secondaryDetailString
{
    if (_secondaryDetailString == nil || [_secondaryDetailString isEqualToString:@""]) {
        _secondaryDetailString = [self contextualSecondaryDetailString];
    }

    return _secondaryDetailString;
}

- (NSString *)durationString
{
    return [NSString stringWithTime:_duration / VLCMediaLibraryMediaItemDurationDenominator];

}

- (VLCInputItem *)inputItem
{
    input_item_t *p_inputItem = vlc_ml_get_input_item(_p_mediaLibrary, self.libraryID);
    VLCInputItem *inputItem = nil;
    if (p_inputItem) {
        inputItem = [[VLCInputItem alloc] initWithInputItem:p_inputItem];
    }
    input_item_Release(p_inputItem);
    return inputItem;
}

- (VLCMediaLibraryMediaItem *)firstMediaItem
{
    return self;
}

- (NSArray<VLCMediaLibraryMediaItem *> *)mediaItems
{
    return @[self];
}

- (id<VLCMediaLibraryItemProtocol>)primaryActionableDetailLibraryItem
{
    if (_primaryActionableDetailLibraryItem == nil && self.mediaSubType == VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK) {
        _primaryActionableDetailLibraryItem = [VLCMediaLibraryAlbum albumWithID:self.albumID];
    }

    return _primaryActionableDetailLibraryItem;
}

- (id<VLCMediaLibraryItemProtocol>)secondaryActionableDetailLibraryItem
{
    if (_secondaryActionableDetailLibraryItem == nil && self.mediaSubType == VLC_ML_MEDIA_SUBTYPE_ALBUMTRACK) {
        _secondaryActionableDetailLibraryItem = [VLCMediaLibraryGenre genreWithID:self.genreID];
    }

    return _secondaryActionableDetailLibraryItem;
}

- (void)iterateMediaItemsWithBlock:(void (^)(VLCMediaLibraryMediaItem*))mediaItemBlock;
{
    mediaItemBlock(self);
}

#pragma mark - preference setters / getters

- (int)setIntegerPreference:(int)value forKey:(enum vlc_ml_playback_state)key
{
    return vlc_ml_media_set_playback_state(_p_mediaLibrary, self.libraryID, key, [[[NSNumber numberWithInt:value] stringValue] UTF8String]);
}

- (int)integerPreferenceForKey:(enum vlc_ml_playback_state)key
{
    int ret = 0;
    char *psz_value;

    if (vlc_ml_media_get_playback_state(_p_mediaLibrary, self.libraryID, key, &psz_value) == VLC_SUCCESS && psz_value != NULL) {
        ret = atoi(psz_value);
        free(psz_value);
    }

    return ret;
}

- (int)setFloatPreference:(float)value forKey:(enum vlc_ml_playback_state)key
{
    return vlc_ml_media_set_playback_state(_p_mediaLibrary, self.libraryID, key, [[[NSNumber numberWithFloat:value] stringValue] UTF8String]);
}

- (float)floatPreferenceForKey:(enum vlc_ml_playback_state)key
{
    float ret = .0;
    char *psz_value;

    if (vlc_ml_media_get_playback_state(_p_mediaLibrary, self.libraryID, key, &psz_value) == VLC_SUCCESS && psz_value != NULL) {
        ret = atof(psz_value);
        free(psz_value);
    }

    return ret;
}

- (int)setStringPreference:(NSString *)value forKey:(enum vlc_ml_playback_state)key
{
    return vlc_ml_media_set_playback_state(_p_mediaLibrary, self.libraryID, key, [value UTF8String]);
}

- (NSString *)stringPreferenceForKey:(enum vlc_ml_playback_state)key
{
    NSString *ret = @"";
    char *psz_value;

    if (vlc_ml_media_get_playback_state(_p_mediaLibrary, self.libraryID, key, &psz_value) == VLC_SUCCESS && psz_value != NULL) {
        ret = toNSStr(psz_value);
        free(psz_value);
    }

    return ret;
}

#pragma mark - preference properties

- (int)rating
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_RATING];
}

- (void)setRating:(int)rating
{
    [self setIntegerPreference:rating forKey:VLC_ML_PLAYBACK_STATE_RATING];
}

- (float)lastPlaybackRate
{
    return [self floatPreferenceForKey:VLC_ML_PLAYBACK_STATE_SPEED];
}

- (void)setLastPlaybackRate:(float)lastPlaybackRate
{
    [self setFloatPreference:lastPlaybackRate forKey:VLC_ML_PLAYBACK_STATE_SPEED];
}

- (int)lastTitle
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_TITLE];
}

- (void)setLastTitle:(int)lastTitle
{
    [self setIntegerPreference:lastTitle forKey:VLC_ML_PLAYBACK_STATE_TITLE];
}

- (int)lastChapter
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_CHAPTER];
}

- (void)setLastChapter:(int)lastChapter
{
    [self setIntegerPreference:lastChapter forKey:VLC_ML_PLAYBACK_STATE_CHAPTER];
}

- (int)lastProgram
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_PROGRAM];
}

- (void)setLastProgram:(int)lastProgram
{
    [self setIntegerPreference:lastProgram forKey:VLC_ML_PLAYBACK_STATE_PROGRAM];
}

- (int)lastVideoTrack
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_VIDEO_TRACK];
}

- (void)setLastVideoTrack:(int)lastVideoTrack
{
    [self setIntegerPreference:lastVideoTrack forKey:VLC_ML_PLAYBACK_STATE_VIDEO_TRACK];
}

- (NSString *)lastAspectRatio
{
    return [self stringPreferenceForKey:VLC_ML_PLAYBACK_STATE_ASPECT_RATIO];
}

- (void)setLastAspectRatio:(NSString *)lastAspectRatio
{
    [self setStringPreference:lastAspectRatio forKey:VLC_ML_PLAYBACK_STATE_ASPECT_RATIO];
}

- (NSString *)lastZoom
{
    return [self stringPreferenceForKey:VLC_ML_PLAYBACK_STATE_ZOOM];
}

- (void)setLastZoom:(NSString *)lastZoom
{
    [self setStringPreference:lastZoom forKey:VLC_ML_PLAYBACK_STATE_ZOOM];
}

- (NSString *)lastCrop
{
    return [self stringPreferenceForKey:VLC_ML_PLAYBACK_STATE_CROP];
}

- (void)setLastCrop:(NSString *)lastCrop
{
    [self setStringPreference:lastCrop forKey:VLC_ML_PLAYBACK_STATE_CROP];
}

- (NSString *)lastDeinterlaceFilter
{
    return [self stringPreferenceForKey:VLC_ML_PLAYBACK_STATE_DEINTERLACE];
}

- (void)setLastDeinterlaceFilter:(NSString *)lastDeinterlaceFilter
{
    [self setStringPreference:lastDeinterlaceFilter forKey:VLC_ML_PLAYBACK_STATE_DEINTERLACE];
}

- (NSString *)lastVideoFilters
{
    return [self stringPreferenceForKey:VLC_ML_PLAYBACK_STATE_VIDEO_FILTER];
}

- (void)setLastVideoFilters:(NSString *)lastVideoFilters
{
    [self setStringPreference:lastVideoFilters forKey:VLC_ML_PLAYBACK_STATE_VIDEO_FILTER];
}

- (int)lastAudioTrack
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_AUDIO_TRACK];
}

- (void)setLastAudioTrack:(int)lastAudioTrack
{
    [self setIntegerPreference:lastAudioTrack forKey:VLC_ML_PLAYBACK_STATE_AUDIO_TRACK];
}

- (float)lastGain
{
    return [self floatPreferenceForKey:VLC_ML_PLAYBACK_STATE_GAIN];
}

- (void)setLastGain:(float)lastGain
{
    [self setFloatPreference:lastGain forKey:VLC_ML_PLAYBACK_STATE_GAIN];
}

- (int)lastAudioDelay
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_AUDIO_DELAY];
}

- (void)setLastAudioDelay:(int)lastAudioDelay
{
    [self setIntegerPreference:lastAudioDelay forKey:VLC_ML_PLAYBACK_STATE_AUDIO_DELAY];
}

- (int)lastSubtitleTrack
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_SUBTITLE_TRACK];
}

- (void)setLastSubtitleTrack:(int)lastSubtitleTrack
{
    [self setIntegerPreference:lastSubtitleTrack forKey:VLC_ML_PLAYBACK_STATE_SUBTITLE_TRACK];
}

- (int)lastSubtitleDelay
{
    return [self integerPreferenceForKey:VLC_ML_PLAYBACK_STATE_SUBTITLE_DELAY];
}

- (void)setLastSubtitleDelay:(int)lastSubtitleDelay
{
    [self setIntegerPreference:lastSubtitleDelay forKey:VLC_ML_PLAYBACK_STATE_SUBTITLE_DELAY];
}

- (void)revealInFinder
{
    VLCMediaLibraryFile *firstFile = _files.firstObject;

    if (firstFile) {
        NSURL *URL = [NSURL URLWithString:firstFile.MRL];
        if (URL) {
            [NSWorkspace.sharedWorkspace activateFileViewerSelectingURLs:@[URL]];
        }
    }
}

- (void)moveToTrash
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    for (VLCMediaLibraryFile *fileToTrash in _files) {
        [fileManager trashItemAtURL:fileToTrash.fileURL
                   resultingItemURL:nil
                              error:nil];
    }
}

@end

@implementation VLCMediaLibraryEntryPoint

- (instancetype)initWithEntryPoint:(struct vlc_ml_folder_t *)p_entryPoint
{
    self = [super init];
    if (self && p_entryPoint != NULL) {

        _MRL = toNSStr(p_entryPoint->psz_mrl);
        _decodedMRL = toNSStr(vlc_uri_decode(p_entryPoint->psz_mrl));
        _isPresent = p_entryPoint->b_present;
        _isBanned = p_entryPoint->b_banned;
    }
    return self;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ — MRL: %@, present: %i, banned: %i",
            NSStringFromClass([self class]), _MRL, _isPresent, _isBanned];
}

@end

@implementation VLCMediaLibraryDummyItem

@synthesize primaryDetailString = _primaryDetailString;
@synthesize secondaryDetailString = _secondaryDetailString;
@synthesize displayString = _displayString;
@synthesize durationString = _durationString;
@synthesize firstMediaItem = _firstMediaItem;
@synthesize mediaItems = _mediaItems;
@synthesize libraryID = _libraryId;
@synthesize smallArtworkGenerated = _smallArtworkGenerated;
@synthesize smallArtworkMRL = _smallArtworkMRL;
@synthesize primaryActionableDetail = _primaryActionableDetail;
@synthesize primaryActionableDetailLibraryItem = _primaryActionableDetailLibraryItem;
@synthesize secondaryActionableDetail = _secondaryActionableDetail;
@synthesize secondaryActionableDetailLibraryItem = _secondaryActionableDetailLibraryItem;

- (instancetype)initWithDisplayString:(NSString *)displayString
              withPrimaryDetailString:(nullable NSString *)primaryDetailString
            withSecondaryDetailString:(nullable NSString *)secondaryDetailString
{
    self = [super init];
    if (self) {
        _displayString = displayString;
        _primaryDetailString = primaryDetailString;
        _secondaryDetailString = secondaryDetailString;
        _durationString = @"";
        _libraryId = -1;
        _smallArtworkGenerated = NO;
        _smallArtworkMRL = @"";
        _primaryActionableDetail = NO;
        _primaryActionableDetailLibraryItem = nil;
        _secondaryActionableDetail = NO;
        _secondaryActionableDetailLibraryItem = nil;
    }
    return self;
}

- (void)moveToTrash
{
    [self doesNotRecognizeSelector:_cmd];
    return;
}

- (void)revealInFinder
{
    [self doesNotRecognizeSelector:_cmd];
    return;
}

- (void)iterateMediaItemsWithBlock:(nonnull void (^)(VLCMediaLibraryMediaItem * _Nonnull))mediaItemBlock
{
    [self doesNotRecognizeSelector:_cmd];
    return;
}

@end
