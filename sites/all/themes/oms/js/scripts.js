(function($) {
    var STNScript = {
        // Equal height function
        // Replace "obj" param with your selector
        equalHeight: function (obj) {
            var currentTallest = 0,
                currentRowStart = 0,
                rowDivs = [],
                $el,
                topPosition = 0;
            $(obj).each(function () {

                $el = $(this);
                $el.height('auto');
                topPostion = $el.offset().top;

                if (currentRowStart != topPostion) {
                    for (currentDiv = 0; currentDiv < rowDivs.length; currentDiv++) {
                        rowDivs[currentDiv].height(currentTallest);
                    }
                    rowDivs.length = 0;
                    currentRowStart = topPostion;
                    currentTallest = $el.height();
                    rowDivs.push($el);
                } else {
                    rowDivs.push($el);
                    currentTallest = (currentTallest < $el.height()) ? ($el.height()) : (currentTallest);
                }
                for (currentDiv = 0; currentDiv < rowDivs.length; currentDiv++) {
                    rowDivs[currentDiv].height(currentTallest);
                }
            });
        },
        //
        initSlick:function(){

            $('.banner-home .view-banner .view-content').slick({
                autoplay: true,
                autoplaySpeed: 2000,
                arrows: false,
            })
            $('.related-products .view-content').slick({
                infinite: true,
                slidesToShow: 4,
                slidesToScroll: 1,
                autoplay: true,
                autoplaySpeed: 2000,
            })
        },

        autoHeight:function(){
            STNScript.equalHeight('.view-id-product_list .views-row');
            STNScript.equalHeight('.view-technical-list .views-row');
        },

        createMenuMobile:function(){
            $( "#navigation" ).clone().appendTo( ".mobile-menu-container" );
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

        searchMobile:function () {
            $('.search-icon').click(function(){
                $('#block-search-form').addClass('open-popup');
            })
        }
    }


    $(document).ready(function(){
        STNScript.initSlick();
        STNScript.detectStar();
        STNScript.createMenuMobile();
        STNScript.searchMobile();


    })
    $(window).on('load',function(){
        STNScript.autoHeight();
        STNScript.searchMobile();
        $(window).resize(function () {
            STNScript.autoHeight();
        })
    })
})(jQuery)