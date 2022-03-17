export default {
    getServiceDiscovery(params) {
        return $.ajax({
            url: 'requests/sd.json',
            data: params
        })
        .then((data) => {
            const jsonData = JSON.parse(data);

            return jsonData;
        });
    }
};
