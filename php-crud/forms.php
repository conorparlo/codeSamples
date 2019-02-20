
<?php

error_reporting(E_ALL);
ini_set("display_errors", "On");

include "classes/database.class.php";
include "shared/header.php";

$data = new db();
?>

<h4>Read data</h4>
<form class="async_form" data-response-target="response">
	<input type="hidden" name="formtype" value="read">

	data table: <input name="datatable" type="text">
	columns: <input name="columns" type="text">
	param: <input name="param" type="text">
	val: <input name="val" type="text">
	<input id="submit" type="submit" value="Submit">
</form>

<div id="response" style="background:#ddd; color:#333; padding:20px; margin:20px 0;"></div>

<h4>Insert data</h4>
<form class="async_form" data-response-target="second">
	<input type="hidden" name="formtype" value="insert">

	data table: <input name="datatable" type="text">
	name: <input name="name" type="text">
	email: <input name="email" type="text">
	phone: <input name="phone" type="text">
	<input id="submit" type="submit" value="Submit">
</form>

<div id="second" style="background:#ddd; color:#333; padding:20px; margin:20px 0;"></div>

<h4>Delete data</h4>
<form class="async_form" data-response-target="third">
	<input type="hidden" name="formtype" value="delete">

	data table: <input name="datatable" type="text">
	id: <input name="id" type="text">
	<input id="submit" type="submit" value="Submit">
</form>

<div id="third" style="background:#ddd; color:#333; padding:20px; margin:20px 0;"></div>

<h4>Update data</h4>
<form class="async_form" data-response-target="fourth">
	<input type="hidden" name="formtype" value="update">

	data table: <input name="datatable" type="text">
	id: <input name="id" type="text">
	name: <input name="name" type="text">
	email: <input name="email" type="text">
	phone: <input name="phone" type="text">
	<input id="submit" type="submit" value="Submit">
</form>

<div id="fourth" style="background:#ddd; color:#333; padding:20px; margin:20px 0;"></div>

<?php include "shared/footer.php"; ?>
