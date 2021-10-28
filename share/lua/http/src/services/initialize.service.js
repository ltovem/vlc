import store from '../store/index.js';
import router from './router.service.js';

function vueInit() {
    return new Vue({
        router,
        el: '#app',
        data: {
            playlistItems: []
        },
        store,
        mounted() { }
    });
}

$(() => {
    vueInit();
    let didScroll;
    let lastScrollTop = 0;
    const delta = 5;
    const navbarHeight = $('.fixed-top.secondary-navbar').outerHeight();

    $(window).scroll(() => {
        didScroll = true;
    });

    setInterval(() => {
        if (didScroll) {
            hasScrolled();
            didScroll = false;
        }
    }, 250);

    function hasScrolled() {
        const sValue = $(this).scrollTop();
        const secondaryNavbar = $('.fixed-top.secondary-navbar');
        if (Math.abs(lastScrollTop - sValue) <= delta) {
            return;
        }

        if (sValue > lastScrollTop && sValue > navbarHeight) {
            secondaryNavbar.removeClass('nav-down').addClass('nav-up');
        } else {
            if (sValue + $(window).height() < $(document).height()) {
                secondaryNavbar.removeClass('nav-up').addClass('nav-down');
            }
        }
        lastScrollTop = sValue;
    }
});
