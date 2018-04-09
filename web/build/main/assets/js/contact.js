/* Data callbacks for contact form recaptcha */

function EnableContact() {
	document.getElementById("btn-contact").disabled = false;
}

function EnableDiscount() {
	document.getElementById("btn-discount").disabled = false;
}

function EnableReport() {
	document.getElementById("btn-report").disabled = false;
}

/* Button actions */

function GeneralQuestionClick() {
	$("#section-report").fadeOut(function() {
		$("#section-discount").fadeOut(function() {
			$("#section-contact").fadeIn();
		});
	});
}

function ReportBugClick() {
	$("#section-contact").fadeOut(function() {
		$("#section-discount").fadeOut(function() {
			$("#section-report").fadeIn();
		});
	});
}

function DiscountClick() {
	$("#section-report").fadeOut(function() {
		$("#section-contact").fadeOut(function() {
			$("#section-discount").fadeIn();
		});
	});
}
