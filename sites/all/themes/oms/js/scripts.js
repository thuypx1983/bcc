(function($) {
    var STNScript = {

        //
        initSlick:function(){
            $('.banner-home .view-banner .view-content').slick({
                autoplay: true,
                autoplaySpeed: 2000,
            })
        },
    }


    $(document).ready(function(){
        STNScript.initSlick();
    })
})(jQuery)