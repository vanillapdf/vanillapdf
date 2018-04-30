<!DOCTYPE html>
<html lang="en">
  <head>
    <!-- Meta tags -->
    <meta charset="utf-8">
<meta name="author" content="Vanilla.PDF Labs">
<meta name="robots" content="index,follow">
<meta name="description" content="Choose your favorite product and fill the billing details details.">
<meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

<title>Vanilla.PDF - Order</title>

<meta property="og:title" content="Vanilla.PDF - Order">
<meta property="og:description" content="Choose your favorite product and fill the billing details details.">
<meta property="og:type" content="website">
<meta property="og:site_name" content="Vanilla.PDF - Order">

<meta name="apple-mobile-web-app-title" content="Vanilla.PDF - Order">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="mobile-web-app-capable" content="yes">

    <!-- Styles -->
    <link href="../assets/css/page.min.css" rel="stylesheet">
<link href="../assets/css/style.css" rel="stylesheet">
<link href="../assets/css/custom.css" rel="stylesheet">

    <!-- Favicons -->
    <link rel="apple-touch-icon" sizes="180x180" href="../assets/img/apple-touch-icon.png">
<link rel="icon" type="image/png" sizes="16x16" href="../assets/img/favicon-16x16.png">
<link rel="icon" type="image/png" sizes="32x32" href="../assets/img/favicon-32x32.png">
<link rel="shortcut icon" href="../assets/img/favicon.ico">

    <!-- Google Analytics -->
    <!-- Global site tag (gtag.js) - Google Analytics -->
<script async src="https://www.googletagmanager.com/gtag/js?id=UA-106797397-1"></script>
<script>
  window.dataLayer = window.dataLayer || [];
  function gtag(){dataLayer.push(arguments);}
  gtag('js', new Date());

  gtag('config', 'UA-106797397-1');
</script>

  </head>

  <body>


    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark" data-navbar="fixed">
      <div class="container">

        <div class="navbar-left">
  <button class="navbar-toggler" type="button">&#9776;</button>
  <a class="navbar-brand" href="../index.html#home">
	<img class="logo-dark" src="../assets/img/logo/vanilla-black.png" alt="Vanilla.PDF logo">
	<img class="logo-light" src="../assets/img/logo/vanilla-white.png" alt="Vanilla.PDF logo">
  </a>
</div>

        <section class="navbar-mobile">
          <nav class="nav nav-navbar ml-auto">
            <a class="nav-link" href="#home">Home</a>
			<a class="nav-link" href="#section-order">Order</a>
            <a class="nav-link" href="#footer">References</a>
          </nav>
        </section>

      </div>
    </nav><!-- /.navbar -->


    <!-- Header -->
    <header id="home" class="header pt-10 pb-0">
      <div class="container text-center">
        <h1 class="display-4">Create an order</h1>
        <p class="lead-2 mt-6">Choose your favorite product and fill the billing details details</p>
      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">
	  
	  <!--
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  | Order form
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  !-->
	  <section id="section-order" class="section">
		<div class="container">

		  <div class="row gap-y">
			<div class="col-11 col-lg-6 mx-auto">

			  <form id="order-form" action="checkout.php" method="POST" data-form="custom-mailer">
			  
			    <h5 class="mb-6">Product details</h5>

				<?php
					// Index page uses GET
					if (isset($_GET['product'])) {
						$_POST['product'] = $_GET['product'];
					}
					
					if (isset($_GET['support'])) {
						$_POST['support'] = $_GET['support'];
					}
				?>
				
				<div class="form-group">
				  <select id="order-product" name="product" class="form-control" onchange="ProductChanged();">
				    <option value="personal-license" <?php if ($_POST['product'] == 'personal-license') { echo 'selected="selected"'; } ?> >Vanilla.PDF Personal license</option>
				    <option value="commercial-license" <?php if ($_POST['product'] == 'commercial-license') { echo 'selected="selected"'; } ?> >Vanilla.PDF Commercial OEM license</option>
					<option value="personal-support" <?php if ($_POST['product'] == 'personal-support') { echo 'selected="selected"'; } ?> >Vanilla.PDF Extended personal support</option>
					<option value="commercial-support" <?php if ($_POST['product'] == 'commercial-support') { echo 'selected="selected"'; } ?> >Vanilla.PDF Extended commercial support</option>
				  </select>
				</div>
				
				<div id="license-group" class="form-group">
				  <input class="form-control form-control-lg" id="license-key" type="text" name="license" placeholder="Your license key (required)" required>
				</div>
				
				<div class="form-group">
				  <select id="order-support" name="support" class="form-control">
				    <option value="1" <?php if ($_POST['support'] == '1') { echo 'selected="selected"'; } ?> >1 Year</option>
					<option value="2" <?php if ($_POST['support'] == '2') { echo 'selected="selected"'; } ?> >2 Year</option>
					<option value="3" <?php if ($_POST['support'] == '3') { echo 'selected="selected"'; } ?> >3 Year</option>
				  </select>
				</div>

				<div class="form-group">
				  <textarea class="form-control" id="order-message" name="message" rows="4" placeholder="Special request"><?php if (isset($_POST['message'])) { echo $_POST['message']; } ?></textarea>
				</div>
				
				<hr>
				
			    <h5 class="mb-6">Billing address</h5>
                
                <div class="form-row">
                  <div class="col-md-6 form-group">
                    <input name="firstname" class="form-control" type="text" title="First name" placeholder="First name" value="<?php if (isset($_POST['firstname'])) { echo $_POST['firstname']; } ?>" required>
                  </div>
                
                  <div class="col-md-6 form-group">
                    <input name="lastname" class="form-control" type="text" title="Last name" placeholder="Last name" value="<?php if (isset($_POST['lastname'])) { echo $_POST['lastname']; } ?>" required>
                  </div>
				  
				  <div class="col-md-12 form-group">
				    <input name="company" class="form-control" type="text" title="Company name" placeholder="Company name (optional)" value="<?php if (isset($_POST['company'])) { echo $_POST['company']; } ?>">
				  </div>
                
                  <div class="col-md-6 form-group">
                    <input name="email" class="form-control" type="email" title="Email address" placeholder="Email address" value="<?php if (isset($_POST['email'])) { echo $_POST['email']; } ?>" required>
                  </div>
                
                  <div class="col-md-6 form-group">
                    <input name="phone" class="form-control" type="tel" title="Phone number" placeholder="Phone number" value="<?php if (isset($_POST['phone'])) { echo $_POST['phone']; } ?>" required>
                  </div>
                
                  <div class="col-md-6 form-group">
                    <input name="country" class="form-control" type="text" title="Country" placeholder="Country" value="<?php if (isset($_POST['country'])) { echo $_POST['country']; } ?>" required>
                  </div>
                
                  <div class="col-md-6 form-group">
                    <input name="state" class="form-control" type="text" title="State" placeholder="State (optional)" value="<?php if (isset($_POST['state'])) { echo $_POST['state']; } ?>">
                  </div>
                
                  <div class="col-md-6 form-group">
                    <input name="city" class="form-control" type="text" title="City" placeholder="City" value="<?php if (isset($_POST['city'])) { echo $_POST['city']; } ?>" required>
                  </div>
                
                  <div class="col-md-6 form-group">
                    <input name="zipcode" class="form-control" type="text" title="Zip code" placeholder="Zip code" value="<?php if (isset($_POST['zipcode'])) { echo $_POST['zipcode']; } ?>" required>
                  </div>
                
                  <div class="col-12 form-group">
                    <input name="address" class="form-control" type="text" title="Address line 1" placeholder="Address line 1" value="<?php if (isset($_POST['address'])) { echo $_POST['address']; } ?>" required>
                  </div>
                
                  <div class="col-12 form-group">
                    <input name="address2" class="form-control" type="text" title="Address line 2 (optional)" placeholder="Address line 2" value="<?php if (isset($_POST['address2'])) { echo $_POST['address2']; } ?>">
                  </div>
                </div>
				
				<hr>
				
				<h5 class="mb-6">Special</h5>
				
				<div class="form-group custom-control custom-checkbox">
                  <input id="terms-conditions" name="terms-conditions" type="checkbox" class="custom-control-input" <?php if (isset($_POST['terms-conditions'])) { echo 'checked="checked"'; } ?> onchange="EnableProceed()" required>
                  <label class="custom-control-label">I agree to the <a class="ml-1" href="#">End-user license agreement</a></label>
                </div>

				<input type="hidden" name="subject" value="Order">
				
				<div class="row">
				  <div class="col-6">
				    <a class="btn btn-block btn-secondary" href="../index.html#home"><i class="ti-angle-left fs-9"></i> Return to home</a>
				  </div>
                  <div class="col-6">
				    <button id="btn-proceed" class="btn btn-primary btn-block" type="submit" disabled>Proceed <i class="ti-angle-right fs-9"></i></button>
				  </div>
				</div>
			  </form>

			</div>
		  </div>


		</div>
	  </section>
	</main><!-- /.main-content -->


	<!-- Footer -->
	<footer id="footer" class="footer py-7">
		<div class="container">
  <div class="row">

	<div class="col-12">
	  <p><a href="../index.html#home"><img src="../assets/img/logo/vanilla-black.png" alt="logo"></a></p>
	</div>

	<div class="col-xl-5">
	  <p>Vanilla.PDF can help you create more awesome products and services. In case of any questions, we would love to hear your feedback!</p>
	</div>

	<div class="col-4 col-xl-2 offset-xl-1">
	  <div class="nav flex-column">
		<a class="nav-link" href="about.php">About</a>
		<a class="nav-link" href="contact.php">Contact</a>
		<a class="nav-link" href="download.html">Download</a>
	  </div>
	</div>

	<div class="col-4 col-xl-2">
	  <div class="nav flex-column">
		<a class="nav-link" href="faq.html">FAQ</a>
		<a class="nav-link" href="licenses.html">EULA</a>
		<a class="nav-link" href="../doc/page_licensing.html">Licensing</a>
	  </div>
	</div>

	<div class="col-4 col-xl-2">
	  <div class="nav flex-column">
		<a class="nav-link" href="../doc/index.html">Tutorial</a>
		<a class="nav-link" href="../doc/hierarchy.html">API reference</a>
		<a class="nav-link" href="../doc/examples.html">Examples</a>
	  </div>
	</div>

  </div>
</div>
	</footer><!-- /.footer -->

    <!-- Scripts -->
    <script src="../assets/js/page.min.js"></script>
<script src="../assets/js/script.js"></script>
	<script src="../assets/js/custom_mailer.js"></script>
	<script src="../assets/js/order.js"></script>

  </body>
</html>
