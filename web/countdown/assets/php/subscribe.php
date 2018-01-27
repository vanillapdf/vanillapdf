<?php

$hidden_field = 'b_50f1056f5b063f4f6212a7789_f584a2f38b';

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

if(isset($_POST['EMAIL']) && isset($_POST[$hidden_field])) {
	$result = Subscribe($_POST['EMAIL'], $_POST[$hidden_field]);
	if ($result === FALSE) {
		return http_response_code(500);
	}
	
	echo $result;
}

?>