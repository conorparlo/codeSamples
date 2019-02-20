# php-crud

A PHP OOP example of an asynchronous CRUD model.  The database class is ambiguous enough to handle any CRUD operation on any table - the table and fields being defined by the HTML form itself.  The input is an array of form data including the operation type, which is then processed and sent to the model.

### Creating an ajax form
	1. Ajax'ify any form by giving it the class 'ajax_form'
	2. Define the 'data-response-target' as the id of the element to insert the results into
	3. Define the data table
