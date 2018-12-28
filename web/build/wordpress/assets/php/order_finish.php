<?php

require 'sendmail_inc.php';
$email_response = SendMail();

// Failed to send an email
if ($email_response.status != 'success') {
	echo $email_response;
	exit;
}

echo $email_response;

// Order e-mail was already sent, try to create and invoice

// define("ORGANIZATION_ID", 664665182);

// $create_contact_params = array(
	// 'contact_name' => $_POST['name'],
// );

// echo json_encode($arr);

// $postvars = array(
    // 'JSONString'  => json_encode($create_contact_params)
// );

// $headers = [
    // 'Authorization: Zoho-authtoken ba4604e8e433g9c892e360d53463oec5',
    // 'Content-Type: application/json;charset=UTF-8'
// ];

// $ch = curl_init();
// curl_setopt($ch, CURLOPT_URL,"https://invoice.zoho.com/api/v3/contacts?organization_id=" . $constant('ORGANIZATION_ID'));
// curl_setopt($ch, CURLOPT_POST, 1);
// curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
// curl_setopt($ch, CURLOPT_POSTFIELDS, $postvars);  //Post Fields
// curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);

// $server_output = curl_exec ($ch);

// curl_close ($ch);

// print  $server_output ;

?>
