/* Dynamic submit response */

$(function() {

  $('[data-form="custom-mailer"]').each(function(){
	  
    var form = $(this);
	var success = $('#success-message');
	var cart = $('#checkout-cart');
    form.on('submit', function(){
  
  	$.ajax({
  	  type: "POST",
  	  url: form.attr('action'),
  	  data: form.serializeArray(),
  	})
  	.done( function( data ) {
  	  var response = $.parseJSON( data );
  	  if ( 'success' == response.status ) {
  		success.fadeIn(1000);
  	  }
  	  else {
  		cart.prepend('<div class="alert alert-danger">'+ response.message +'</div>');
  		console.log( response.reason )
  	  }
  	});
  
  	return false;
    });
  });

});