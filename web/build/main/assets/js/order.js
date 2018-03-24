/* Data callbacks for contact form recaptcha */

function EnableContact() {
	document.getElementById("btn-contact").disabled = false;
}

function EnableOrder() {
	document.getElementById("btn-order").disabled = false;
}

function EnableDiscount() {
	document.getElementById("btn-discount").disabled = false;
}

/* Order form checkout */
var PRODUCT_ID = 'order-product';
var LICENSE_GROUP_ID = 'license-group';
var LICENSE_KEY_ID = 'license-key';

/* Hide unnecessary fields when submitting an order */

function ProductChanged() {
  var product = document.getElementById(PRODUCT_ID);
  var licenseGroup = document.getElementById(LICENSE_GROUP_ID);
  var licenseKey = document.getElementById(LICENSE_KEY_ID);
  
  var selected = product.options[product.selectedIndex];
  var isSupport = ((selected.value == "personal-support") || (selected.value == "commercial-support"));
  
  if (isSupport) {
	  $(licenseGroup).show();
	  licenseKey.disabled = false;
  } else {
	  $(licenseGroup).hide();
	  licenseKey.disabled = true;
  }
};

/* Email validation and dynamic submit response */

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
  
    email.on('focus', function() {
      email_form_group.removeClass('has-danger');
    });
  });

});