/* Utilities */

function ScrollTo(element) {
	var elOffset = element.offset().top;
	var elHeight = element.height();
	var windowHeight = $(window).height();
	var offset;
	
	if (elHeight < windowHeight) {
	  offset = elOffset - ((windowHeight / 2) - (elHeight / 2));
	}
	else {
	  offset = elOffset;
	}
	
	$('html, body').animate({scrollTop: offset}, 700);
}
