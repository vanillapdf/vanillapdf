/* Main form subscribe event */

  $('#subscribe-form').submit(function(e) {
	
	$('#success-message').hide();
	$('#error-message').hide();
	
    $.ajax({
        type: 'POST',
        url: '<?php echo get_assets_folder(); ?>/php/subscribe.php',
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