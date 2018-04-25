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

function EnableLicense() {
	document.getElementById("btn-license").disabled = false;
}

/* Button actions */

function GeneralQuestionClick() {
	$("#section-license").fadeOut();
	$("#section-report").fadeOut();
	$("#section-discount").fadeOut(function() {
		$("#section-contact").fadeIn();
	});
}

function TemporaryLicenseClick() {
	$("#section-contact").fadeOut();
	$("#section-report").fadeOut();
	$("#section-discount").fadeOut(function() {
		$("#section-license").fadeIn();
	});
}

function ReportBugClick() {
	$("#section-contact").fadeOut();
	$("#section-license").fadeOut();
	$("#section-discount").fadeOut(function() {
		$("#section-report").fadeIn();
	});
}

function DiscountClick() {
	$("#section-contact").fadeOut();
	$("#section-license").fadeOut();
	$("#section-report").fadeOut(function() {
		$("#section-discount").fadeIn();
	});
}
