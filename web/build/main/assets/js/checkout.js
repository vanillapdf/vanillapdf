/* Dynamic submit response */

$(function() {

  $('[data-form="custom-mailer"]').each(function(){
	  
    var form = $(this);
    form.on('submit', function(){
  
  	$.ajax({
  	  type: "POST",
  	  url: form.attr('action'),
  	  data: form.serializeArray(),
  	})
  	.done( function( data ) {
  	  var response = $.parseJSON( data );
  	  if ( 'success' == response.status ) {
  		form.find('.alert-success').fadeIn(1000);
  		form.find(':input').val('');
  	  }
  	  else {
  		form.prepend('<div class="alert alert-danger">'+ response.message +'</div>');
  		console.log( response.reason )
  	  }
  	});
  
  	return false;
    });
  });

});