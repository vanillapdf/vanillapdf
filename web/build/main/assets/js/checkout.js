/* Dynamic submit response */

$(function() {
  $('#btn-return').hide();

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
          $('#success-message').fadeIn(1000);
		  
		  $('#btn-back').fadeOut(1000);
		  $('#btn-checkout').fadeOut(1000), function() {
			$('#btn-return').fadeIn(1000);
		  }
        }
        else {
          $('#checkout-cart').before('<div class="alert alert-danger">'+ response.message +'</div>');
          console.log( response.reason )
        }
      });
  
  	return false;
    });
  });

});