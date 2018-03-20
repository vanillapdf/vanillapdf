'use strict';

$(function() {


  /*
  |--------------------------------------------------------------------------
  | Configure your website
  |--------------------------------------------------------------------------
  |
  | We provided several configuration variables for your ease of development.
  | Read their complete description and modify them based on your need.
  |
  */

  page.config({

    /*
    |--------------------------------------------------------------------------
    | Google API Key
    |--------------------------------------------------------------------------
    |
    | Here you may specify your Google API key if you need to use Google Maps
    | in your application
    |
    | https://developers.google.com/maps/documentation/javascript/get-api-key
    |
    */

    googleApiKey: '',

    /*
    |--------------------------------------------------------------------------
    | Google Analytics Tracking
    |--------------------------------------------------------------------------
    |
    | If you want to use Google Analytics, you can specify your Tracking ID in
    | this option. Your key would be a value like: UA-12345678-9
    |
    */

    googleAnalyticsId: 'UA-106797397-1',

    /*
    |--------------------------------------------------------------------------
    | Google reCAPTCHA
    |--------------------------------------------------------------------------
    |
    | reCAPTCHA protects you against spam and other types of automated abuse.
    | Please signup for an API key pair and insert your `Site key` value to the
    | following variable.
    |
    | http://www.google.com/recaptcha/admin
    |
    */

    reCaptchaSiteKey:  '6Lf55EsUAAAAAEnK6mTnEwXbG8NS8udciZJBpoOM',

    // See available languages: https://developers.google.com/recaptcha/docs/language
    reCaptchaLanguage: 'en',

    /*
    |--------------------------------------------------------------------------
    | Disable AOS on mobile
    |--------------------------------------------------------------------------
    |
    | If true, the Animate On Scroll animations don't run on mobile devices.
    |
    */

    disableAOSonMobile: true,

    /*
    |--------------------------------------------------------------------------
    | Smooth Scroll
    |--------------------------------------------------------------------------
    |
    | If true, the browser's scrollbar moves smoothly on scroll and gives your
    | visitor a better experience for scrolling.
    |
    */

    smoothScroll: true,

  });





  /*
  |--------------------------------------------------------------------------
  | Custom Javascript code
  |--------------------------------------------------------------------------
  |
  | Now that you configured your website, you can write additional Javascript
  | code below this comment. You might want to add more plugins and initialize
  | them in this file.
  |
  */
  
  $('#subscribe-form').submit(function(e) {
	
	$('#success-message').hide();
	$('#error-message').hide();
	
    $.ajax({
        type: 'POST',
        url: 'assets/php/subscribe.php',
        data: $('#subscribe-form').serialize(),
		dataType: 'json',
		error: function(err) {
            $('#error-message').html(err.responseText);
            $('#error-message').fadeIn();
		},
        success: function(json) {
            if(json.result == 'success') {
                $('#success-message').html(json.msg);
                $('#success-message').fadeIn();
            }
            else {
                $('#error-message').html(json.msg);
                $('#error-message').fadeIn();
            }
        }
    });

	return false;
  });
  
  $(document).ready(function() {
	  document.getElementById("personal-one").addEventListener("click", PersonalOneYearSupport);
	  document.getElementById("personal-two").addEventListener("click", PersonalTwoYearSupport);
	  document.getElementById("personal-three").addEventListener("click", PersonalThreeYearSupport);
	  
	  document.getElementById("commercial-one").addEventListener("click", CommercialOneYearSupport);
	  document.getElementById("commercial-two").addEventListener("click", CommercialTwoYearSupport);
	  document.getElementById("commercial-three").addEventListener("click", CommercialThreeYearSupport);
	  
	  document.getElementById("personal-one").click();
	  document.getElementById("commercial-one").click();
  }) 

function PersonalOneYearSupport() {
	PersonalYearsSupport(1, 200 + 300 - 1);
}

function PersonalTwoYearSupport() {
	PersonalYearsSupport(2, 200 + 2 * 250 - 1);
}

function PersonalThreeYearSupport() {
	PersonalYearsSupport(3, 200 + 3 * 200 - 1);
}

function PersonalYearsSupport(support, price) {
    document.getElementById("personal-price").innerHTML = "$" + price;
	document.getElementById("personal-support").innerHTML = support + " year support";
}

function CommercialOneYearSupport() {
	CommercialYearsSupport(1, 1000 + 1000 - 1);
}

function CommercialTwoYearSupport() {
	CommercialYearsSupport(2, 1000 + 2 * 900 - 1);
}

function CommercialThreeYearSupport() {
	CommercialYearsSupport(3, 1000 + 3 * 800 - 1);
}

function CommercialYearsSupport(support, price) {
    document.getElementById("commercial-price").innerHTML = "$" + price;
	document.getElementById("commercial-support").innerHTML = support + " year support";
}


});

