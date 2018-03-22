/* Main page pricing drop-downs */

$(document).ready(function() {
  document.getElementById("personal-one").addEventListener("click", PersonalOneYearSupport);
  document.getElementById("personal-two").addEventListener("click", PersonalTwoYearSupport);
  document.getElementById("personal-three").addEventListener("click", PersonalThreeYearSupport);
  
  document.getElementById("commercial-one").addEventListener("click", CommercialOneYearSupport);
  document.getElementById("commercial-two").addEventListener("click", CommercialTwoYearSupport);
  document.getElementById("commercial-three").addEventListener("click", CommercialThreeYearSupport);
  
  document.getElementById("personal-one").click();
  document.getElementById("commercial-one").click();
});

/* Pricing */
var PERSONAL_LICENSE_PRICE				= 300 - 1;
var PERSONAL_ONE_YEAR_SUPPORT_PRICE		= 200 - 1;
var PERSONAL_TWO_YEAR_SUPPORT_PRICE		= 2 * (175 - 1);
var PERSONAL_THREE_YEAR_SUPPORT_PRICE	= 3 * (150 - 1);

var COMMERCIAL_LICENSE_PRICE			= 1500 - 1;
var COMMERCIAL_ONE_YEAR_SUPPORT_PRICE	= 500 - 1;
var COMMERCIAL_TWO_YEAR_SUPPORT_PRICE	= 2 * (450 - 1);
var COMMERCIAL_THREE_YEAR_SUPPORT_PRICE	= 3 * (400 - 1);

/* Orders */
var BASE_URL = 'https://order.shareit.com/cart/add';
var VANILLA_URL = 'https://vanillapdf.com/';
var VENDOR_ID = 200276374;

var PERSONAL_LICENSE_ID					= 300846419;
var PERSONAL_ONE_YEAR_SUPPORT_ID		= 300846425;
var PERSONAL_TWO_YEAR_SUPPORT_ID		= 0;
var PERSONAL_THREE_YEAR_SUPPORT_ID		= 0;

var COMMERCIAL_LICENSE_ID				= 300846421;
var COMMERCIAL_ONE_YEAR_SUPPORT_ID		= 300846426;
var COMMERCIAL_TWO_YEAR_SUPPORT_ID		= 0;
var COMMERCIAL_THREE_YEAR_SUPPORT_ID	= 0;

function GetOrderLink(products) {
	var result = '';
	
	result += BASE_URL;
	result += '?';
	result += 'vendorid=' + VENDOR_ID;
	
	products.forEach(function(element) {
		result += '&PRODUCT[' + element + ']=1';
	});
	
	result += '&backlink=' + encodeURIComponent(VANILLA_URL);
	return result;
}

function PersonalOneYearSupport() {
	var purchase_link = GetOrderLink([PERSONAL_LICENSE_ID]);
	var support_link = GetOrderLink([PERSONAL_ONE_YEAR_SUPPORT_ID]);
	PersonalYearsSupport(1, PERSONAL_LICENSE_PRICE + PERSONAL_ONE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function PersonalTwoYearSupport() {
	var purchase_link = GetOrderLink([PERSONAL_LICENSE_ID]);
	var support_link = GetOrderLink([PERSONAL_TWO_YEAR_SUPPORT_ID]);
	PersonalYearsSupport(2, PERSONAL_LICENSE_PRICE + PERSONAL_TWO_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function PersonalThreeYearSupport() {
	var purchase_link = GetOrderLink([PERSONAL_LICENSE_ID]);
	var support_link = GetOrderLink([PERSONAL_THREE_YEAR_SUPPORT_ID]);
	PersonalYearsSupport(3, PERSONAL_LICENSE_PRICE + PERSONAL_THREE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function PersonalYearsSupport(support, price, purchase_link, support_link) {
    document.getElementById("personal-price").innerHTML = "$" + price;
	document.getElementById("personal-support").innerHTML = support + " year support";
	
	document.getElementById("personal-purchase-link").href = purchase_link;
	document.getElementById("personal-support-link").href = support_link;
}

function CommercialOneYearSupport() {
	var purchase_link = GetOrderLink([COMMERCIAL_LICENSE_ID]);
	var support_link = GetOrderLink([COMMERCIAL_ONE_YEAR_SUPPORT_ID]);
	CommercialYearsSupport(1, COMMERCIAL_LICENSE_PRICE + COMMERCIAL_ONE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function CommercialTwoYearSupport() {
	var purchase_link = GetOrderLink([COMMERCIAL_LICENSE_ID]);
	var support_link = GetOrderLink([COMMERCIAL_TWO_YEAR_SUPPORT_ID]);
	CommercialYearsSupport(2, COMMERCIAL_LICENSE_PRICE + COMMERCIAL_TWO_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function CommercialThreeYearSupport() {
	var purchase_link = GetOrderLink([COMMERCIAL_LICENSE_ID]);
	var support_link = GetOrderLink([COMMERCIAL_THREE_YEAR_SUPPORT_ID]);
	CommercialYearsSupport(3, COMMERCIAL_LICENSE_PRICE + COMMERCIAL_THREE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function CommercialYearsSupport(support, price, purchase_link, support_link) {
    document.getElementById("commercial-price").innerHTML = "$" + price;
	document.getElementById("commercial-support").innerHTML = support + " year support";
	
	document.getElementById("commercial-purchase-link").href = purchase_link;
	document.getElementById("commercial-support-link").href = support_link;
}