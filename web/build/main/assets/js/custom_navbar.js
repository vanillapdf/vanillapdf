// Custom navigation bar, that is light on the cover page and dark on the others

$(window).on('scroll', function() {
  var window_top = $(window).scrollTop();
  var navbar_height = page.navbar.innerHeight();

  if (window_top > navbar_height) {
    page.navbar.addClass('navbar-dark');
    page.navbar.removeClass('navbar-light');
  }
  else {
    page.navbar.addClass('navbar-light');
    page.navbar.removeClass('navbar-dark');
  }
});