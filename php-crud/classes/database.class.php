<?php

// error_reporting(E_ALL);
// ini_set("display_errors", "On");

/**
 * Database class
 *
 * Connects to the database and performs CRUD operations
 *
 * @author Conor Parlo Simon <conorparlo@gmail.com>
 * @copyright 2015 Factor 2, LLC
 * @license
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

class db
{
	protected $mysqli;
	
	public function __construct()
	{		
		define('DOC_ROOT_PATH', $_SERVER['DOCUMENT_ROOT'].'/codebase/');
		require DOC_ROOT_PATH . "config/database.config.php";

		$this->mysqli = new mysqli($db_hostname, $db_username, $db_password, $db_database);
		if ($this->mysqli->connect_errno) {
		    echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
		}
	}
	
	public function __destruct()
	{
		$this->mysqli->close();
	}

	/**
	* Insert a record
	*
	* @param 	array 		$db_data 	An array containing the insert data. HTML form parameters define the array.
	* @return 	json					Returns a json encoded status.
	*/
	
	public function insert($db_data)
	{
		$datatable = $db_data['datatable'];

		unset($db_data['formtype']);
		unset($db_data['datatable']);
		
		$data_keys = array_keys($db_data);
		$data_values = array_values($db_data);
		
		$keys = implode(", ", $data_keys);
		$vals = "'".implode("', '", $data_values)."'";
		
		$sql = "INSERT INTO ".$datatable." (".$keys.") VALUES (".$vals.")";
		if($result = $this->mysqli->query($sql)){
			return json_encode(['status' => 'success']);
		} else {
			die('Error : ('. $this->mysqli->errno .') '. $this->mysqli->error);
		}		
	}
	
	/**
	* Read from the database
	*
	* @param 	mixed		$columns 	The column or columns to select. Can be a string or array. 
	* @param 	string 		$table 		The table to select from.
	* @param 	string 		$param 		Optional. The parameter for a WHERE statement.
	* @param 	string 		$val 		Optional. The value for a WHERE statement.
	* @return 	json					Returns a json encoded array of results.
	*/
	
	public function read($db_data)
	{
		foreach ($db_data as $key => $value) {
			${"$key"} = $value;
		}
		
		if ( (array) $columns !== $columns ) { 
		    $c = $columns;
		} else { 
		    $c = implode(", ", $columns);
		}
		
		$sql = "SELECT ".$c." FROM ".$datatable;
		if(!empty($param) && !empty($val)){
			$sql .= " WHERE ".$param." = '".$val."'";
		}
		$result = $this->mysqli->query($sql);
		
		if ($result->num_rows > 0) {
		    while($r = $result->fetch_assoc()) {
		        $rows[] = $r;
		    }
			return json_encode($rows);
		} else {
			return json_encode($result);
		}
		$result->close(); 
	}

	/**
	* Update a record
	*
	* @param 	string 		$datatable 	The table of the record to update.
	* @param 	int			$id		 	Required. The id of the record to update.
	* @param 	string 		$db_data 	Required. The data being updated.
	* @return 	json					Returns a json encoded status.
	*/
	
	public function update($datatable, $id, $db_data)
	{
		$update_data = urldecode(http_build_query($db_data,'',', '));
		
		$sql = "UPDATE ".$datatable." SET ".$update_data." WHERE id=".$id;
		if($result = $this->mysqli->query($sql)){
			return json_encode(['status' => 'success']);
		} else {
			die('Error : ('. $this->mysqli->errno .') '. $this->mysqli->error);
		}
	}

	/**
	* Delete a record
	*
	* @param 	array 		$db_data 	An array containing the table and id of the record to delete.
	* @return 	json					Returns a json encoded status.
	*/
	
	public function delete($db_data)
	{
		$datatable = $db_data['datatable'];

		unset($db_data['formtype']);
		unset($db_data['datatable']);
		
		foreach ($db_data as $key => $value) {
			${"$key"} = $value;
		}
		
		$sql = "DELETE FROM ".$datatable." WHERE id =".$id;
		if($result = $this->mysqli->query($sql)){
			return json_encode(['status' => 'success']);
		} else {
			die('Error : ('. $this->mysqli->errno .') '. $this->mysqli->error);
		}
	}
}

?>