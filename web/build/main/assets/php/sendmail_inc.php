<?php

// Load PHP Mailer
//
require 'PHPMailer/PHPMailerAutoload.php';

function SendMail() {

  $mail = new PHPMailer;
  
  /*
  |--------------------------------------------------------------------------
  | Configure your contact form
  |--------------------------------------------------------------------------
  |
  | Set value of '$reciever' to email address that want to receive inquiries.
  | Also, '$default_subject' is the subject that you'll see in your inbox.
  |
  | It's better to set `$sender_email` and `$sender_name` values, so there's
  | more chance to receive the email at gmail, yahoo, hotmail, etc.
  |
  */
  $reciever        = "info@vanillapdf.com";
  $default_subject = "Email from vanillapdf.com";
  
  $sender_email    = "noreply@vanillapdf.com";
  $sender_name     = "vanillapdf.com";
  $error_message   = "An error occured. Please try again later.";
  
  /*
  |--------------------------------------------------------------------------
  | Verify captcha
  |--------------------------------------------------------------------------
  */
  
  $secret_key		= '6Lf55EsUAAAAAGM6e9hntK-ZuNaDvToBDkbcqJBi';
  
  $post_data = http_build_query(
      array(
          'secret' => $secret_key,
          'response' => $_POST['g-recaptcha-response'],
          'remoteip' => $_SERVER['REMOTE_ADDR']
      )
  );
  
  $opts = array('http' =>
      array(
          'method'  => 'POST',
          'header'  => 'Content-type: application/x-www-form-urlencoded',
          'content' => $post_data
      )
  );
  
  
  $context  = stream_context_create($opts);
  $response = file_get_contents('https://www.google.com/recaptcha/api/siteverify', false, $context);
  $result = json_decode($response);
  if (!$result->success) {
  	return json_encode( array(
        'status'  => 'error',
        'message' => $error_message,
        'reason'  => $result->error,
      ));
  }
  
  /*
  |--------------------------------------------------------------------------
  | Configure PHP Mailer
  |--------------------------------------------------------------------------
  |
  | By default, we're using the default configuration. If you need to change
  | default settings or use a custion SMTP server, do it here.
  |
  | More info: https://github.com/PHPMailer/PHPMailer
  |
  */
  $mail->isHTML(true);
  
  
  /*
  |--------------------------------------------------------------------------
  | Sending email
  |--------------------------------------------------------------------------
  |
  | This part of code is responsible to send the email. So you don't need to
  | change anything here.
  |
  */
  
  $email = $_POST['email'];
  if ( ! empty( $email ) && filter_var( $email, FILTER_VALIDATE_EMAIL ) )
  {
  
    // detect & prevent header injections
    //
    $test = "/(content-type|bcc:|cc:|to:)/i";
    foreach ( $_POST as $key => $val ) {
      if ( preg_match( $test, $val ) ) {
         return json_encode( array(
          'status'  => 'error',
          'message' => 'Header injection prevented',
          'reason'  => $val,
        ));
      }
    }
  
  
    // Sender name
    //
    $name = '';
    if ( isset( $_POST['name'] ) ) {
      $name = $_POST['name'];
    }
  
    if ( isset( $_POST['firstname'] ) && isset( $_POST['lastname'] ) ) {
      $name = $_POST['firstname'] .' '. $_POST['lastname'];
    }
  
  
    // Email subject
    //
    $subject = '';
    if ( isset( $_POST['subject'] ) ) {
      $subject = 'Vanilla.PDF: ' . $_POST['subject'];
    }
  
    if ($subject == "") {
      $subject = $default_subject;
    }
	
	// Handle file upload
    //
    if (isset($_FILES['input-file']) && $_FILES['input-file']['error'] == UPLOAD_ERR_OK) {
        $mail->AddAttachment($_FILES['input-file']['tmp_name'], $_FILES['input-file']['name']);
    }
  
    // Message content
    //
    $message = '';
    $message_backup = $_POST['message'];
  
    // These values have been already stored for processing and shall be excluded from the loop
    unset( $_POST['subject'], $_POST['message'], $_POST['input-file'], $_POST['g-recaptcha-response'] );
	
	// Attach other input values to the end of message
    foreach ($_POST as $key => $value) {
      $key = str_replace( array('-', '_'), ' ', $key);
      $message .= '<p><b>'. ucfirst($key) .'</b>: '. nl2br( $value ) .'<p>';
    }
    
    if (isset($message_backup)) {
      $message .= '<p><b>'. ucfirst('Message:') .'</b><br>'. nl2br($message_backup) .'<p>';
    }
  
  
    // Prepare PHP Mailer
    //
    $mail->setFrom($sender_email, $sender_name);
    $mail->addAddress($reciever);
    $mail->addReplyTo($email, $name);
  
    $mail->Subject = $subject;
    $mail->Body    = $message;
    $mail->AltBody = strip_tags($message);
  
    if( ! $mail->send() ) {
      return json_encode( array(
        'status'  => 'error',
        'message' => $error_message,
        'reason'  => $mail->ErrorInfo,
      ));
    } else {
      return json_encode( array( 'status' => 'success' ) );
    }
  
  }
}


?>
