$(document).ready(function() {
	
	$('.async_form').each(function(){
		
		$(this).submit(function(e) {
			e.preventDefault();
			responseTarget = $(this).data("response-target");
			$.ajax({
				type: 'GET',
				url: 'processors/form_process.php',
				data: $(this).serialize(),
				dataType: 'json',
				success: function (json) {
					console.log('Success');
					parseJson(json, responseTarget);
				},
				error: function (e) {
		            console.log('Fail');
					$('#'+responseTarget+'').html('There was an error with the query');
		        }
			});
		});
		
	});
	
});

function parseJson(json, responseTarget){
	var data = JSON.stringify(json);
	console.log(data);
	$('#'+responseTarget+'').html(data);
}