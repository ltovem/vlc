use std::borrow::Borrow;
use std::collections::HashMap;
use vlccore_sys::input_item::*;
use vlccore_sys::url::*;
use vlccore_sys::vlc_core as ffi;
use vlccore_sys::vlc_core::*;

#[derive(Clone, Debug)]
pub struct CuesheetTrack {
    pub item_type: String,
    pub item_position: u8,
    pub item_performer: String,
    pub item_title: String,
    pub item_begin_duration: String,
}

impl Default for CuesheetTrack {
    fn default() -> Self {
        CuesheetTrack {
            item_type: "".to_owned(),
            item_position: 0,
            item_performer: "".to_string(),
            item_title: "".to_string(),
            item_begin_duration: "".to_string(),
        }
    }
}

fn add_to_start_time(
    start_time: i32,
    time_to_add: Option<&str>,
    conversion_factor: i32,
) -> Option<i32> {
    let val = time_to_add?.parse::<u32>().ok()?;
    Some(start_time + val as i32 * conversion_factor)
}

impl CuesheetTrack {
    pub fn begin_time_in_seconds(&self) -> i32 {
        let start: &str = &self.item_begin_duration;
        let mut duration_parts: Vec<&str> = start.split(":").collect();

        let frame_rate = 75;
        let convert_min_to_s = 60;

        duration_parts
            .pop()
            .and_then(|part| part.parse::<u32>().ok())
            .map(|part| part / frame_rate)
            .and_then(|start| add_to_start_time(start as i32, duration_parts.pop(), 1))
            .and_then(|start| {
                add_to_start_time(start as i32, duration_parts.pop(), convert_min_to_s)
            })
            .unwrap_or(-1)
    }

    pub fn get_property(&self, key: &str) -> Option<String> {
        match key.to_ascii_lowercase().as_str() {
            "type" => Some(self.item_type.to_string()),
            "begin_duration" => Some(self.item_begin_duration.to_string()),
            "performer" => Some(self.item_performer.to_string()),
            "title" => Some(self.item_title.to_string()),
            "position" => Some(self.item_position.to_string()),
            _ => None,
        }
    }
}

#[derive(Clone, Debug)]
pub struct Cuesheet {
    pub file_name: String,
    pub file_type: String,
    pub tracks: Vec<CuesheetTrack>,
    pub performer: String,
    pub title: String,
    pub comments: HashMap<String, String>,
    pub is_processing_tracks: bool,
}

impl Default for Cuesheet {
    fn default() -> Self {
        Cuesheet {
            file_name: "".to_string(),
            file_type: "".to_string(),
            tracks: Vec::new(),
            performer: "".to_string(),
            title: "".to_string(),
            comments: HashMap::new(),
            is_processing_tracks: false,
        }
    }
}

impl Cuesheet {
    pub fn get_track_at_index(&self, index: usize) -> Option<&CuesheetTrack> {
        if index >= self.tracks.len() {
            return None;
        }
        Some(self.tracks[index].borrow())
    }

    pub fn get_property(&self, key: &str) -> Option<&str> {
        match key.to_ascii_lowercase().as_str() {
            "file_name" => Some(self.file_name.as_str()),
            "file_type" => Some(self.file_type.as_str()),
            "performer" => Some(self.performer.as_str()),
            "title" => Some(self.title.as_str()),
            _ => None,
        }
    }

    pub fn track_property(&self, index: usize, line: &str) -> Option<String> {
        self.get_track_at_index(index)
            .zip(Some(line))
            .and_then(|(track, property)| Some(track.get_property(property)))
            .unwrap_or(None)
    }

    pub fn tracks_number(&self) -> usize {
        self.tracks.len()
    }

    pub fn comment_value(&self, line: &str) -> Option<&str> {
        self.comments
            .get(&line.to_lowercase())
            .map(|comment| comment.as_str())
            .or_else(|| None)
    }

    pub fn get_track_start(&self, index: usize) -> i32 {
        self.get_track_at_index(index)
            .map(|track| track.begin_time_in_seconds())
            .unwrap_or(-1)
    }

    #[allow(unused_must_use)]
    pub fn read_dir(&self, subitems: &mut CInputItemNode, url: &str) -> i32 {
        let i_ret = VLC_EGENERIC;
        let tracks = self.tracks_number();
        let album = self.get_property("TITLE");
        let album_artist = self.get_property("PERFORMER");
        let description = self.comment_value("COMMENT");
        let genre = self.comment_value("GENRE");
        let date = self.comment_value("DATE");
        let audio_file = self.get_property("FILE_NAME");

        if audio_file.is_none() {
            return i_ret;
        }
        let audio_file_uri = CUrl::encode(audio_file.unwrap());
        let url_resolve = CUrl::resolve(url, &audio_file_uri);

        for idx in 0..tracks {
            let val = self.track_property(idx as usize, "TITLE");
            let index = self.track_property(idx as usize, "POSITION");

            if val.is_none() {
                return i_ret;
            }

            let mut item = CInputItem::new(
                &url_resolve,
                &val.unwrap(),
                0 as i64, //TODO when bindgen support INT64_C(0) replace this hardcoded value
                ffi::input_item_type_e::ITEM_TYPE_FILE,
                ffi::input_item_net_type::ITEM_NET_UNKNOWN,
            );

            if album.is_some() {
                item.set_meta(ffi::vlc_meta_type_t::vlc_meta_Title, album.unwrap());
            }
            if album_artist.is_some() {
                item.set_meta(
                    ffi::vlc_meta_type_t::vlc_meta_AlbumArtist,
                    album_artist.unwrap(),
                );
            }
            if description.is_some() {
                item.set_meta(
                    ffi::vlc_meta_type_t::vlc_meta_Description,
                    description.unwrap(),
                );
            }
            if date.is_some() {
                item.set_meta(ffi::vlc_meta_type_t::vlc_meta_Date, date.unwrap());
            }
            if genre.is_some() {
                item.set_meta(ffi::vlc_meta_type_t::vlc_meta_Genre, genre.unwrap());
            }
            if index.is_some() {
                item.set_meta(ffi::vlc_meta_type_t::vlc_meta_TrackNumber, &index.unwrap());
            }

            let val = self.track_property(idx as usize, "PERFORMER");
            if val.is_some() {
                item.set_meta(ffi::vlc_meta_type_t::vlc_meta_Artist, &val.unwrap());
            }

            let i_time = self.get_track_start(idx);
            let mut options = Vec::new();
            options.push(format!(":start-time={}", i_time));

            if idx < tracks - 1 {
                let i_time = self.get_track_start(idx + 1);
                options.push(format!(":stop-time={}", i_time));
            }

            item.add_options(
                options,
                ffi::input_item_option_e::VLC_INPUT_OPTION_TRUSTED as u32,
            );
            subitems.append_item(item);
        }
        VLC_SUCCESS as i32
    }
}
