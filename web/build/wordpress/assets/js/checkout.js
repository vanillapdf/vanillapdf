/* Data callbacks for contact form recaptcha */

function EnableOrder() {
	document.getElementById("btn-checkout").disabled = false;
}

/* Utilities */

function ScrollTo(element) {
	var elOffset = element.offset().top;
	var elHeight = element.height();
	var windowHeight = $(window).height();
	var offset;
	
	if (elHeight < windowHeight) {
	  offset = elOffset - ((windowHeight / 2) - (elHeight / 2));
	}
	else {
	  offset = elOffset;
	}
	
	$('html, body').animate({scrollTop: offset}, 700);
}


/* Dynamic submit response */

$(function() {
  $('[data-form="custom-mailer"]').each(function(){
	  
    var form = $(this);
    form.on('submit', function(){

      $('#section-checkout').children('.alert-danger').remove();
  
      $.ajax({
        type: "POST",
        url: form.attr('action'),
        data: form.serializeArray(),
      })
      .done( function( data ) {
        var response = $.parseJSON( data );
        if ('success' == response.status) {
		  $('#btn-back').fadeOut(1000);
		  $('#recaptcha').fadeOut(1000);
		  $('#btn-checkout').fadeOut(1000, function() {
			$('#success-message').fadeIn(1000);
			$('#return-home').fadeIn(1000);
			
			ScrollTo($('#return-home'));
		  });
        }
        else {
          $('#success-message').before('<div class="alert alert-danger">'+ response.message +'</div>');
		  
		  ScrollTo($('#success-message').prev());
		  
          console.log( response.reason );
        }
      });
  
  	return false;
    });
  });

});