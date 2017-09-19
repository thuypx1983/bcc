(function($) {
    var STNScript = {

        //
        initSlick:function(){

            $('.banner-home .view-banner .view-content').slick({
                autoplay: true,
                autoplaySpeed: 2000,
            })
            $('.related-products .view-content').slick({
                infinite: true,
                slidesToShow: 4,
                slidesToScroll: 1,
                autoplay: true,
                autoplaySpeed: 2000,
            })
        },

        /*
         * detect star for display
         */
        detectStar:function() {
            $('.views-field-field-rate').each(function () {
                var rate = parseInt($(this).find('.field-content').text());
                var stars_html = "";
                if (!isNaN(rate) || rate > 1) {
                    for (var i = 1; i <= 5; i++) {
                        if (i <= rate) {
                            stars_html += '<span class="star star-active"></span>';
                        } else {
                            stars_html += '<span class="star"></span>';
                        }
                    }
                    $(this).html(stars_html);
                }

            })
        },
    }


    $(document).ready(function(){
        STNScript.initSlick();
        STNScript.detectStar();
    })
})(jQuery)