# node-elasticsearch-api

A simple search API to query an Elasticsearch instance.  Accepts GET requests only; POST, PUT, PATCH & DELETE return a HTTP 405 "Method Not Allowed."
There is no front end form - the API accepts queries via URI parameters.

### Node Packages

* BodyBuilder: Used to build a single Elasticsearch query string from conditionals dependent on the URI request.
* Express: Used to define and route API requests.
* Elasticsearch: Offical Elasticsearch client for Node.

### Parameters

```
@param {string} category - triathlon, running, cycling, other
@param {string} subCategory
@param {number} weeksFrom
@param {number} weeksTo
@param {number} hoursFrom
@param {number} hoursTo
@param {string} redactedResults - omit fields ['weeklyWorkoutAggregations', 'totalWorkoutAggregations', 'averageWorkoutAggregations', 'sampleWorkouts'] returned by setting to 'true'
```

### Example usage

```
api/plans/?category=cycling&weeksFrom=2&weeksTo=6&hoursFrom=4&hoursTo=8&redactedResults=true
```

### Notes
* The range values both have to be set for the range to be applied to the query
* Since all URI parameters are strings, redactedResults won't be applied unless the URI string is set to 'true'.  Setting it to anything else assumes 'false'.
* 'category' is the only required field
