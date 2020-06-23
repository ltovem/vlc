export default {
    fetchVideos(params = {}) {
        return $.ajax({
            url: 'medialib/video',
            data: params
        })
        .then((data) => {
            let jsonData = JSON.parse(data);
            const origin = location.origin;
            jsonData = jsonData.map((d) => {
                d.src = d.hasThumbnail ? origin + '/medialib/thumbnail?mediaId=' + d.id : '';
                return d;
            });
            return jsonData;
        });
    }
};
