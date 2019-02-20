# codeSamples

A collection of code samples authored by Conor Parlo Simon <conorparlo@gmail.com>, distributed under the [Apache Version 2.0 License](https://www.apache.org/licenses/LICENSE-2.0).  This code is for review only.

### [node-elasticsearch-api](node-elasticsearch-api)

A simple search API to query an Elasticsearch instance. Accepts GET requests only; POST, PUT, PATCH & DELETE return a HTTP 405 "Method Not Allowed." There is no front end form - the API accepts queries via URI parameters.

### [php-crud](php-crud)

A PHP OOP example of an asynchronous CRUD model.  The database class is ambiguous enough to handle any CRUD operation on any table - the table and fields being defined by the HTML form itself.  The input is an array of form data including the operation type, which is then processed and sent to the model.
