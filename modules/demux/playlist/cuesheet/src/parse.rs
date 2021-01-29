use crate::cuesheet::*;

pub fn parse_line(line: &str) -> Vec<String> {
    let characters = line.trim_start_matches('\u{feff}').trim().chars();
    let mut parts: Vec<String> = Vec::new();
    let mut inside_quotes: bool = false;
    let mut temp_str: String = String::new();
    for char in characters {
        if char == '"' {
            inside_quotes = !inside_quotes;
        } else if inside_quotes {
            temp_str.push(char);
        } else if char.is_whitespace() {
            parts.push(temp_str.clone());
            temp_str.clear();
        } else {
            temp_str.push(char);
        }
    }
    parts.push(temp_str.clone());
    parts
}

impl Cuesheet {
    pub fn process_line(&mut self, line: &str) -> Option<()> {
        let parts = parse_line(line);
        let keyword = parts[0].as_str();
        if !self.is_processing_tracks {
            match keyword {
                "FILE" => {
                    self.file_name = parts[1].to_owned();
                    self.file_type = parts[2].to_owned();
                }
                "TITLE" => self.title = parts[1].to_owned(),
                "PERFORMER" => self.performer = parts[1].to_owned(),
                "REM" => {
                    self.comments
                        .insert(parts[1].to_owned().to_lowercase(), parts[2].to_owned());
                }
                "TRACK" => self.is_processing_tracks = true,
                _ => {}
            };
        }
        if self.is_processing_tracks {
            if keyword == "TRACK" {
                let mut track = CuesheetTrack::default();
                track.item_position = parts[1].to_owned().parse::<u8>().ok()?;
                track.item_type = parts[2].to_owned();
                self.tracks.push(track);
            } else {
                self.tracks.last_mut()?.process_track(line);
            }
        }
        Some(())
    }
}

impl CuesheetTrack {
    fn process_track(&mut self, line: &str) -> Option<()> {
        let parts = parse_line(line);
        let keyword = parts[0].as_str();
        match keyword {
            "TITLE" => self.item_title = parts[1].to_owned(),
            "PERFORMER" => self.item_performer = parts[1].to_owned(),
            "INDEX" => {
                let index_position = parts[1].to_owned().parse::<u8>().ok()?;
                if index_position == 1 {
                    self.item_begin_duration = parts[2].to_owned();
                }
            }
            _ => {}
        };
        Some(())
    }
}
