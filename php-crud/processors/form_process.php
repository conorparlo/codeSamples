<?php

////////////////////////////////////////////////////////////
// MAKE SURE TO RESTRICT DIRECT SCRIPT ACCESS FOR PRODUCTION
////////////////////////////////////////////////////////////

error_reporting(E_ALL);
ini_set("display_errors", "On");

header('Content-type: application/json');
include "../classes/database.class.php";
include "../classes/account.class.php";

$data = new db();
$account = new account();

//Assign all form data to variables and add to data array

$input = $_GET;

$db_data = array();
foreach ($input as $key => $value) {
	$db_data[$key] = $value;
}

$formtype = $db_data['formtype'];

if($formtype == 'read'){
	
	echo $data->read($db_data);
	
} else if($formtype == 'insert'){
	
	echo $data->insert($db_data);
	
} else if($formtype == 'update'){
	
	$datatable = $db_data['datatable'];
	$id = $db_data['id'];
	
	unset($db_data);
	
	$db_data = array();
	foreach ($input as $key => $value) {
		$db_data[$key] = "'$value'";
	}
	
	unset($db_data['formtype']);
	unset($db_data['datatable']);
	unset($db_data['id']);
	
	echo $data->update($datatable, $id, $db_data);
	
} else if($formtype == 'delete'){

	echo $data->delete($db_data);
	
}else if($formtype == 'create_account'){

	echo $account->create_account($db_data);

} else {
	return;
}

?>

