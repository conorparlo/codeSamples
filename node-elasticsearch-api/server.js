const express = require('express');

const plans = require('./routes/api/plans');

const app = express();

app.use('/api/plans', plans);

app.get('/', function (req, res) {
  res.redirect('/api/plans');
});

const port = process.env.PORT || 3001;

app.listen(port, () => console.log(`Server started on port ${port}`));