<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="Choose your favorite product and fill the billing details details.">

    <title>Vanilla.PDF - Checkout</title>

    <!-- Styles -->
    <link href="../assets/css/page.min.css" rel="stylesheet">
<link href="../assets/css/style.css" rel="stylesheet">
<link href="../assets/css/custom.css" rel="stylesheet">

    <!-- Favicons -->
    <link rel="apple-touch-icon" sizes="180x180" href="../assets/img/apple-touch-icon.png">
<link rel="icon" type="image/png" sizes="16x16" href="../assets/img/favicon-16x16.png">
<link rel="icon" type="image/png" sizes="32x32" href="../assets/img/favicon-32x32.png">
<link rel="shortcut icon" href="../assets/img/favicon.ico">
  </head>

  <body>


    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark" data-navbar="fixed">
      <div class="container">

        <div class="navbar-left">
  <button class="navbar-toggler" type="button">&#9776;</button>
  <a class="navbar-brand" href="../index.html#home">
	<img class="logo-dark" src="../assets/img/logo/vanilla.png" alt="logo">
	<img class="logo-light" src="../assets/img/logo/vanilla.png" alt="logo">
  </a>
</div>

        <section class="navbar-mobile">
          <nav class="nav nav-navbar ml-auto">
            <a class="nav-link" href="#home">Home</a>
			<a class="nav-link" href="#section-checkout">Checkout</a>
            <a class="nav-link" href="#footer">References</a>
          </nav>
        </section>

      </div>
    </nav><!-- /.navbar -->


    <!-- Header -->
    <header id="home" class="header pt-10 pb-0">
      <div class="container text-center">
        <h1 class="display-4">Checkout your order</h1>
        <p class="lead-2 mt-6">Review the details before submitting the request</p>
      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">
	  
	  <!--
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  | Checkout form
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  !-->
	  <section id="section-checkout" class="section bg-gray">
		<div class="container">

		  <div class="row gap-y">
			<div class="col-11 col-lg-6 mx-auto">
			
				<?php
					$tax_percent = 19;
					
					if (!isset($tax_percent)) {
						trigger_error("Tax is not set", E_USER_ERROR);
					}
					
					$product_name;
					$product_price;
					$product_personal;
					
					$support_name;
					$support_price;
					
					if ($_POST['product'] == 'personal-license') {
						$product_name = 'Vanilla.PDF Personal license';
						$product_price = 299;
						$product_personal = true;
					}
					
					if ($_POST['product'] == 'commercial-license') {
						$product_name = 'Vanilla.PDF Commercial license';
						$product_price = 1499;
					}
					
					if (!isset($product_name)) {
						trigger_error("Invalid product name", E_USER_ERROR);
					}
					
					if (!isset($product_price)) {
						trigger_error("Invalid product price", E_USER_ERROR);
					}
					
					if ($_POST['support'] == '1') {
						$support_name = '1 Year';
						
						if ($product_personal === true) {
							$support_price = 199;
						} else {
							$support_price = 499;
						}
					}
					
					if ($_POST['support'] == '2') {
						$support_name = '2 Year';
						
						if ($product_personal === true) {
							$support_price = 348;
						} else {
							$support_price = 898;
						}
					}
					
					if ($_POST['support'] == '3') {
						$support_name = '3 Year';
						
						if ($product_personal === true) {
							$support_price = 447;
						} else {
							$support_price = 1197;
						}
					}
					
					if (!isset($product_name)) {
						trigger_error("Invalid support name", E_USER_ERROR);
					}
					
					if (!isset($support_price)) {
						trigger_error("Invalid support price", E_USER_ERROR);
					}
					
					$subtotal = $product_price + $support_price;
					$tax_value = $subtotal * ($tax_percent / 100);
					$total = $subtotal + $tax_value;
				?>
				
				<div id="success-message" class="alert alert-success d-on-success" style="display: none">We received your order and will contact you back soon.</div>
			      <div id="checkout-cart" class="cart-price">
                    <div class="flexbox">
                      <div>
					    <p><strong>Product:</strong> <?= $product_name ?></p>
					    <p><strong>Support:</strong> <?= $support_name ?></p>
                        <p><strong>Subtotal:</strong></p>
                        <p><strong>Tax (%<?= $tax_percent ?>):</strong></p>
                      </div>
                  
                      <div>
					    <p><?= $product_price ?> &euro;</p>
					    <p><?= $support_price ?> &euro;</p>
                        <p><?= $subtotal ?> &euro;</p>
                        <p><?= $tax_value ?> &euro;</p>
                      </div>
                    </div>
                  
                    <hr>
                  
                    <div class="flexbox">
                      <div>
                        <p><strong>Total:</strong></p>
                      </div>
                  
                      <div>
                        <p id="total-price" class="fw-600"><?= $total ?> &euro;</p>
                      </div>
                    </div>
                  </div>
                  
                  <div class="row">
                    <div class="col-6">
					  <form action="order.php" method="POST">
						<?php foreach( $_POST as $key => $val ): ?>
							<input type="hidden" name="<?= htmlspecialchars($key, ENT_COMPAT, 'UTF-8') ?>" value="<?= htmlspecialchars($val, ENT_COMPAT, 'UTF-8') ?>">
						<?php endforeach; ?>
						
						<button class="btn btn-block btn-secondary" type="submit"><i class="ti-angle-left fs-9"></i> Back</a>
					  </form>
                    </div>
                  
                    <div class="col-6">
					  <form action="../assets/php/sendmail.php" method="POST" data-form="custom-mailer">
						<div class="alert alert-success d-on-success">We received your message and will contact you back soon.</div>
						
						<?php foreach( $_POST as $key => $val ): ?>
							<input type="hidden" name="<?= htmlspecialchars($key, ENT_COMPAT, 'UTF-8') ?>" value="<?= htmlspecialchars($val, ENT_COMPAT, 'UTF-8') ?>">
						<?php endforeach; ?>
						
						<button class="btn btn-block btn-primary" type="submit">Checkout <i class="ti-angle-right fs-9"></i></button>
					  </form>
                    </div>
                  </div>

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
		<a class="nav-link" href="about.html">About</a>
		<a class="nav-link" href="contact.html">Contact</a>
		<a class="nav-link" href="download.html">Download</a>
	  </div>
	</div>

	<div class="col-4 col-xl-2">
	  <div class="nav flex-column">
		<a class="nav-link" href="faq.html">FAQ</a>
		<a class="nav-link" href="../doc/page_licensing.html">Licensing</a>
		<a class="nav-link" href="#">EULA</a>
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
	<script src="../assets/js/checkout.js"></script>

  </body>
</html>
