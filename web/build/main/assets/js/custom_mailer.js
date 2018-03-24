/* Email validation */

$(function() {

  var validEmail = /^(([^<>()\[\]\\.,;:\s@"]+(\.[^<>()\[\]\\.,;:\s@"]+)*)|(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/;

  $('[data-form="custom-mailer"]').each(function(){
	  
    var form = $(this);
    var email = form.find('[name="email"]');
    var email_form_group = email.closest('.form-group');
  
    form.on('submit', function(){
		
  	form.children('.alert-danger').remove();
  
  	if ( email.length ) {
  	  if ( email.val().length < 1 || ! validEmail.test( email.val() ) ) {
  		email_form_group.addClass('has-danger');
  		return false;
  	  }
  	}
	
	return true;
    });
  
    email.on('focus', function() {
      email_form_group.removeClass('has-danger');
    });
  });

});