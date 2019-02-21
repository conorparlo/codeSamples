# php-crud

A PHP OOP example of an asynchronous CRUD model.  The database class is ambiguous enough to handle any CRUD operation on any table - the table and fields being defined by the HTML form itself.  The input is an array of form data including the operation type, which is then processed and sent to the model.

### MVC Workflow
* The controller and model are setup to accept transaction requests of any type and any schema as defined in the html form.
* The html form class makes the form asynchronous and a data attribute defines the div id for the response to be inserted into. ```<form class="async_form" data-response-target="response-div">```
* The form requires a hidden field defining the request type. ```<input type="hidden" name="formtype" value="read">```
* The additional fields can be anything matching (BUT MUST MATCH) the targeted tables' schema.  The controller and model will parse the array of key-value pairs and generate the SQL statement.
* The form submission is sent through an ajax call to the controller, which routes it to the correct function in the model.
* The model returns a json-encoded response to the view of success, error, or data.

### Additional Considerations
* This example does not sanitize the data at all. This would have to be included if it was ever to be used outside of a testing environment.
* There is no authentication at all.  Auth, CSRF, XSS and SQL injection safeguards would need to be added.
