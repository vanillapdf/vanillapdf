
jQuery(document).ready(function() {
	
    /*
        Fullscreen background
    */
    $.backstretch("assets/img/backgrounds/1.jpg");
    
    /*
        Wow
    */
    new WOW().init();
    
    /*
	    Countdown initializer
	*/
	var now = new Date();
	var countTo = new Date(2018, 5, 1);
	$('.timer').countdown(countTo, function(event) {
		$(this).find('.days').text(event.offset.totalDays);
		$(this).find('.hours').text(event.offset.hours);
		$(this).find('.minutes').text(event.offset.minutes);
		$(this).find('.seconds').text(event.offset.seconds);
	});
	
	/*
	    Subscription form
	*/
	$('.success-message').hide();
	$('.error-message').hide();
	
	$('.subscribe form').submit(function(e) {
		
		$('.success-message').hide();
		$('.error-message').hide();
		
	    $.ajax({
	        type: 'POST',
	        url: 'assets/php/subscribe.php',
	        data: $('.subscribe form').serialize(),
			dataType: 'json',
			error: function(err) {
	            $('.error-message').html('Could not connect to the registration server. Please try again later.');
	            $('.error-message').fadeIn();
			},
	        success: function(json) {
	            if(json.result == 'success') {
	                $('.success-message').html(json.msg);
	                $('.success-message').fadeIn();
	            }
	            else {
	                $('.error-message').html(json.msg);
	                $('.error-message').fadeIn();
	            }
	        }
	    });

		return false;
	});
    
});

