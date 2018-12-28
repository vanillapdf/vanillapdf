<?php

$hidden_field_id = 'b_50f1056f5b063f4f6212a7789_f584a2f38b';

function Subscribe($email, $hidden_field_value) {
	$url = 'https://vanillapdf.us16.list-manage.com/subscribe/post-json';
	$u = '50f1056f5b063f4f6212a7789';
	$id = 'f584a2f38b';
	$data = array(
		'EMAIL' => $email,
		'u' => $u,
		'id' => $id,
		'b_50f1056f5b063f4f6212a7789_f584a2f38b' => $hidden_field_value
	);
	
	$query = $url . '?' . http_build_query($data);
	$result = file_get_contents($query);
	
	if ($result === FALSE) {
		return FALSE;
	}

	return $result;
}

if(isset($_POST['EMAIL']) && isset($_POST[$hidden_field_id])) {
	$email = $_POST['EMAIL'];
	$hidden_field = $_POST[$hidden_field_id];
	
	if (empty($email)) {
		$response = array(
			'msg' => 'Please enter a valid email address',
			'result' => 'error'
		);
		
		echo json_encode($response);
		return http_response_code(200);
	}
	
	$result = Subscribe($email, $hidden_field);
	if ($result === FALSE) {
		$response = array(
			'msg' => 'Could not connect to the registration server. Please try again later.',
			'result' => 'error'
		);
		echo json_encode($response);
		return http_response_code(200);
	}
	
	echo $result;
	return http_response_code(200);
}

?>