/* Data callbacks for contact form recaptcha */

function EnableContact() {
	document.getElementById("btn-contact").disabled = false;
}

function EnableDiscount() {
	document.getElementById("btn-discount").disabled = false;
}

function EnableReport() {
	document.getElementById("btn-report").disabled = false;
}

function EnableLicense() {
	document.getElementById("btn-license").disabled = false;
}

/* Button actions */

function GeneralQuestionClick() {
	$("#section-license").fadeOut();
	$("#section-report").fadeOut();
	$("#section-discount").fadeOut(function() {
		$("#section-contact").fadeIn();
	});
}

function TemporaryLicenseClick() {
	$("#section-contact").fadeOut();
	$("#section-report").fadeOut();
	$("#section-discount").fadeOut(function() {
		$("#section-license").fadeIn();
	});
}

function ReportBugClick() {
	$("#section-contact").fadeOut();
	$("#section-license").fadeOut();
	$("#section-discount").fadeOut(function() {
		$("#section-report").fadeIn();
	});
}

function DiscountClick() {
	$("#section-contact").fadeOut();
	$("#section-license").fadeOut();
	$("#section-report").fadeOut(function() {
		$("#section-discount").fadeIn();
	});
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
  		form.find('.alert-success').fadeIn(1000, function() {
			ScrollTo(form.find('.alert-success'));
		});
		
  		form.find(':input').val('');
  	  }
  	  else {
  		form.prepend('<div class="alert alert-danger">'+ response.message +'</div>');
		ScrollTo(form.prev());
  		console.log( response.reason );
  	  }
  	});
  
  	return false;
    });
  
    email.on('focus', function() {
      email_form_group.removeClass('has-danger');
    });
  });

});