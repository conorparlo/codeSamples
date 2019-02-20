# TrainingPeaks Code Test API

A simple plan search API mimicking the live TrainingPeaks Plan API behind the "Find a Training Plan" page.
There is no front end form - the API accepts queries via URI parameters.

##Parameters

```
@param {string} category - triathlon, running, cycling, other
@param {string} subCategory
@param {number} weeksFrom
@param {number} weeksTo
@param {number} hoursFrom
@param {number} hoursTo
@param {string} redactedResults - omit fields ['weeklyWorkoutAggregations', 'totalWorkoutAggregations', 'averageWorkoutAggregations', 'sampleWorkouts'] returned by setting to 'true'
```

##Example usage

```
api/plans/?category=cycling&weeksFrom=2&weeksTo=6&hoursFrom=4&hoursTo=8&redactedResults=true
```

##Notes
* The range values both have to be set for the range to be applied to the query
* Since all URI parameters are strings, redactedResults won't be applied unless the URI string is set to 'true'.  Setting it to aything else assumes 'false'.
* 'category' is the only required field
