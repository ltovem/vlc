#! /usr/bin/python3
#
# Copyright (C) 2020 Rémi Denis-Courmont
#
# SPDX-License-Identifier: LGPL-2.1-or-later

import sys
import json
import urllib.parse
import yt_dlp

class logger(object):
    def debug(self, msg):
        pass

    def warning(self, msg):
        pass

    def error(self, msg):
        sys.stderr.write(msg + '\n')

def url_extract(url):
    opts = {
        'extract_flat': 'in_playlist',
        'logger': logger(),
        'youtube_include_dash_manifest': False,
    }

    dl = yt_dlp.YoutubeDL(opts)

    # Process a given URL
    infos = dl.extract_info(url, download=False)

    if 'entries' in infos:
        for entry in infos['entries']:
             if 'ie_key' in entry and entry['ie_key']:
                 # Flat-extracted playlist entry
                 url = 'ytdl:///?' + urllib.parse.urlencode(entry)
                 entry['url'] = url;

    print(json.dumps(infos))

def url_process(ie_url):
    opts = {
        'logger': logger(),
        'youtube_include_dash_manifest': False,
    }

    dl = yt_dlp.YoutubeDL(opts)

    # Rebuild the original IE entry
    entry = { }

    for p in urllib.parse.parse_qsl(url[9:]):
        entry[p[0]] = p[1]

    infos = dl.process_ie_result(entry, download=False)
    print(json.dumps(infos))

url = sys.argv[1]

if url.startswith('ytdl:///?'):
    url_process(url)
else:
    url_extract(url)
