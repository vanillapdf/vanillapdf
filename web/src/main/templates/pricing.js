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

/* Orders */
var ORDER_URL = 'page/{{ order_page }}';

var PERSONAL_LICENSE_ID					= 'personal-license';
var PERSONAL_SUPPORT_ID					= 'personal-support';

var COMMERCIAL_LICENSE_ID				= 'commercial-license';
var COMMERCIAL_SUPPORT_ID				= 'commercial-support';

function GetOrderLink(product, support) {
	var result = '';
	
	result += ORDER_URL;
	result += '?';
	result += 'product=' + encodeURIComponent(product);
	result += '&';
	result += 'support=' + encodeURIComponent(support);
	return result;
}

function PersonalOneYearSupport() {
	var purchase_link = GetOrderLink(PERSONAL_LICENSE_ID, 1);
	var support_link = GetOrderLink(PERSONAL_SUPPORT_ID, 1);
	PersonalYearsSupport(1, PERSONAL_LICENSE_PRICE + PERSONAL_ONE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function PersonalTwoYearSupport() {
	var purchase_link = GetOrderLink(PERSONAL_LICENSE_ID, 2);
	var support_link = GetOrderLink(PERSONAL_SUPPORT_ID, 2);
	PersonalYearsSupport(2, PERSONAL_LICENSE_PRICE + PERSONAL_TWO_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function PersonalThreeYearSupport() {
	var purchase_link = GetOrderLink(PERSONAL_LICENSE_ID, 3);
	var support_link = GetOrderLink(PERSONAL_SUPPORT_ID, 3);
	PersonalYearsSupport(3, PERSONAL_LICENSE_PRICE + PERSONAL_THREE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function PersonalYearsSupport(support, price, purchase_link, support_link) {
    document.getElementById("personal-price").innerHTML = price + '{{ CURRENCY }}';
	document.getElementById("personal-support").innerHTML = support + " year support";
	
	document.getElementById("personal-purchase-link").href = purchase_link;
	document.getElementById("personal-support-link").href = support_link;
}

function CommercialOneYearSupport() {
	var purchase_link = GetOrderLink(COMMERCIAL_LICENSE_ID, 1);
	var support_link = GetOrderLink(COMMERCIAL_SUPPORT_ID, 1);
	CommercialYearsSupport(1, COMMERCIAL_LICENSE_PRICE + COMMERCIAL_ONE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function CommercialTwoYearSupport() {
	var purchase_link = GetOrderLink(COMMERCIAL_LICENSE_ID, 2);
	var support_link = GetOrderLink(COMMERCIAL_SUPPORT_ID, 2);
	CommercialYearsSupport(2, COMMERCIAL_LICENSE_PRICE + COMMERCIAL_TWO_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function CommercialThreeYearSupport() {
	var purchase_link = GetOrderLink(COMMERCIAL_LICENSE_ID, 3);
	var support_link = GetOrderLink(COMMERCIAL_SUPPORT_ID, 3);
	CommercialYearsSupport(3, COMMERCIAL_LICENSE_PRICE + COMMERCIAL_THREE_YEAR_SUPPORT_PRICE, purchase_link, support_link);
}

function CommercialYearsSupport(support, price, purchase_link, support_link) {
    document.getElementById("commercial-price").innerHTML = price + '{{ CURRENCY }}';
	document.getElementById("commercial-support").innerHTML = support + " year support";
	
	document.getElementById("commercial-purchase-link").href = purchase_link;
	document.getElementById("commercial-support-link").href = support_link;
}
