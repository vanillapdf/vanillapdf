/* Data callbacks for contact form recaptcha */

function EnableOrder() {
	document.getElementById("btn-order").disabled = false;
}

/* Order form checkout */
var PRODUCT_ID = 'order-product';
var LICENSE_GROUP_ID = 'license-group';
var LICENSE_KEY_ID = 'license-key';

/* Hide unnecessary fields when submitting an order */

function ProductChanged() {
  var product = document.getElementById(PRODUCT_ID);
  var licenseGroup = document.getElementById(LICENSE_GROUP_ID);
  var licenseKey = document.getElementById(LICENSE_KEY_ID);
  
  var selected = product.options[product.selectedIndex];
  var isSupport = ((selected.value == "personal-support") || (selected.value == "commercial-support"));
  
  if (isSupport) {
	  $(licenseGroup).show();
	  licenseKey.disabled = false;
  } else {
	  $(licenseGroup).hide();
	  licenseKey.disabled = true;
  }
};

$(function() {
	ProductChanged();
});
