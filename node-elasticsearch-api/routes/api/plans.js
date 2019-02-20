const express = require('express');
const router = express.Router();
const elasticsearch = require('elasticsearch');
const bodybuilder = require('bodybuilder');
const esConf = require('../../config/es').esURI;

const es = new elasticsearch.Client({
    host: esConf,
    log: 'info'
});

/**
    *   @route   GET api/plans
    *   @desc    Get all plans in json format via HTTP URI params
    *   @access  Public
    *   @param {string} category - triathlon, running, cycling, other
    *   @param {string} subCategory
    *   @param {number} weeksFrom
    *   @param {number} weeksTo
    *   @param {number} hoursFrom
    *   @param {number} hoursTo
    *   @param {string} redactedResults - omit fields ['weeklyWorkoutAggregations', 'totalWorkoutAggregations', 'averageWorkoutAggregations', 'sampleWorkouts'] returned by setting to 'true'
    *   @example
    *   api/plans/?category=cycling&weeksFrom=2&weeksTo=6&hoursFrom=4&hoursTo=8&redactedResults=true
    *   @exports router
*/
router.get('/', (req, res) => {

    const queryParams = {
        category,
        subCategory,
        weeksFrom,
        weeksTo,
        hoursFrom,
        hoursTo,
        redactedResults
    } = req.query;

    const checkParams = (param) => {
        return (typeof param === "undefined" || param === '') ? true : false;
    }

    const buildQuery = () => {
        let body = bodybuilder();

        checkParams(queryParams.redactedResults) ? body
            : (queryParams.redactedResults.toUpperCase() == 'TRUE') ? body = body.rawOption('_source', {"excludes": ['weeklyWorkoutAggregations', 'totalWorkoutAggregations', 'averageWorkoutAggregations', 'sampleWorkouts']})
            : body
        checkParams(queryParams.category) ? body : body = body.query('match', 'category', queryParams.category);
        checkParams(queryParams.subCategory) ? body : body = body.query('match', 'subcategory', queryParams.subCategory);
        (checkParams(queryParams.weeksFrom) || checkParams(queryParams.weeksTo)) ? body : body = body.query('range', 'weeksDuration', {gte: queryParams.weeksFrom, lte: queryParams.weeksTo});
        (checkParams(queryParams.hoursFrom) || checkParams(queryParams.hoursTo)) ? body : body = body.query('range', 'averageHoursPerWeek', {gte: queryParams.hoursFrom, lte: queryParams.hoursTo});
        body = body.build();

        return body;
    }

    const body = buildQuery();

    const searchParams = {
        index: 'training-plans',
        type: 'training-plan',
        body //built by buildQuery
    }

    checkParams(queryParams.category) ? res.status(400).json({message: "A category is required"})
        : es.search(searchParams).then(plans => {
            const planCount = plans.hits.total;
            const response =
                (planCount == 0 && typeof queryParams.category !== "undefined") ? {status: '400', responseBody: {message: "No plans found. Please check the request syntax."}}
                : {status: '200', responseBody: plans};

            res.status(response.status).json(response.responseBody);
        })
        .catch(err => console.trace(err.message));
});

router.post('/', (req, res) => {
    res.status(405).json({message: "Method Not Allowed"});
});

router.put('/', (req, res) => {
    res.status(405).json({message: "Method Not Allowed"});
});

router.patch('/', (req, res) => {
    res.status(405).json({message: "Method Not Allowed"});
});

router.delete('/', (req, res) => {
    res.status(405).json({message: "Method Not Allowed"});
});

module.exports = router;
