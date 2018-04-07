<!DOCTYPE html>
<html lang="en">
  <head>
      <!-- Meta tags -->
    {% include 'meta.html' %}

    <!-- Styles -->
    {% include 'styles.html' %}

    <!-- Favicons -->
    {% include 'favicons.html' %}
  </head>

  <body>


    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark" data-navbar="fixed">
      <div class="container">

        {% include 'navbar-left.html' %}

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
        <p class="lead-2 mt-6">Review your billing address before submitting the request</p>
      </div>
    </header><!-- /.header -->


    <!-- Main Content -->
    <main class="main-content">
	  
	  <!--
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  | Checkout form
	  |‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒‒
	  !-->
	  <section id="section-checkout" class="section">
		<div class="container">

		  <div class="row">
			<div class="col-11 col-lg-6 mx-auto">
			
				<?php
					$tax_percent = {{ TAX_PERCENTAGE }};
					
					if (!isset($tax_percent)) {
						trigger_error("Tax is not set", E_USER_ERROR);
					}
					
					$product_name;
					$product_price;
					$product_personal;
					
					$support_name;
					$support_price;
					
					if ($_POST['product'] == 'personal-license') {
						$product_name = '{{ PERSONAL_LICENSE_DESCRIPTION }}';
						$product_price = {{ PERSONAL_LICENSE_PRICE }};
						$product_personal = true;
					}
					
					if ($_POST['product'] == 'commercial-license') {
						$product_name = '{{ COMMERCIAL_LICENSE_DESCRIPTION }}';
						$product_price = {{ COMMERCIAL_LICENSE_PRICE }};
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
							$support_price = {{ PERSONAL_ONE_YEAR_SUPPORT_PRICE }};
						} else {
							$support_price = {{ COMMERCIAL_ONE_YEAR_SUPPORT_PRICE }};
						}
					}
					
					if ($_POST['support'] == '2') {
						$support_name = '2 Year';
						
						if ($product_personal === true) {
							$support_price = {{ PERSONAL_TWO_YEAR_SUPPORT_PRICE }};
						} else {
							$support_price = {{ COMMERCIAL_TWO_YEAR_SUPPORT_PRICE }};
						}
					}
					
					if ($_POST['support'] == '3') {
						$support_name = '3 Year';
						
						if ($product_personal === true) {
							$support_price = {{ PERSONAL_THREE_YEAR_SUPPORT_PRICE }};
						} else {
							$support_price = {{ COMMERCIAL_THREE_YEAR_SUPPORT_PRICE }};
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
					$total = $subtotal /*+ $tax_value*/;
				?>
				
				<h5 class="mb-6">Product</h5>
			      <div class="cart-price">
                    <div class="flexbox">
                      <div>
					    <p><strong>Product:</strong> <?= $product_name ?></p>
					    <p><strong>Support:</strong> <?= $support_name ?></p>
                      </div>
                  
                      <div class="text-right">
					    <p><?= $product_price ?>{{ CURRENCY }}</p>
					    <p><?= $support_price ?>{{ CURRENCY }}</p>
                      </div>
                    </div>
                  
                    <hr>
                  
                    <div class="flexbox">
                      <div>
                        <p><strong>Total:</strong></p>
                      </div>
                  
                      <div class="text-right">
                        <p id="total-price" class="fw-600"><?= $total ?> {{ CURRENCY }}</p>
                      </div>
                    </div>
                  </div>
				  
				</div>
		      </div>
			   
			  <div class="row">
			    <div class="col-11 col-lg-6 mx-auto">
				  
				  <h5 class="mb-6">Billing address</h5>
			      <table class="table table-bordered table-hover">
                    <thead class="d-none">
                      <tr>
                        <th>&nbsp;</th>
                        <th>&nbsp;</th>
                      </tr>
                    </thead>
                    <tbody>
                      <tr>
                        <td><strong>First name</strong></td>
						<td><?= $_POST['firstname'] ?></td>
                      </tr>
					  <tr>
                        <td><strong>Last name</strong></td>
						<td><?= $_POST['lastname'] ?></td>
                      </tr>
					  <?php 
					    if (!empty($_POST['company'])) {
							echo
							'<tr>
							  <td><strong>Company name</strong></td>
							  <td>' . $_POST['company'] . '</td>
							</tr>';
						}
					  ?>
					  <tr>
                        <td><strong>Email address</strong></td>
						<td><?= $_POST['email'] ?></td>
                      </tr>
					  <tr>
                        <td><strong>Phone number</strong></td>
						<td><?= $_POST['phone'] ?></td>
                      </tr>
					  <tr>
                        <td><strong>Country</strong></td>
						<td><?= $_POST['country'] ?></td>
                      </tr>
					  <?php 
					    if (!empty($_POST['state'])) {
							echo
							'<tr>
							  <td><strong>State</strong></td>
							  <td>' . $_POST['state'] . '</td>
							</tr>';
						}
					  ?>
					  <tr>
                        <td><strong>City</strong></td>
						<td><?= $_POST['city'] ?></td>
                      </tr>
					  <tr>
                        <td><strong>Zip code</strong></td>
						<td><?= $_POST['zipcode'] ?></td>
                      </tr>
					  <tr>
                        <td><strong>Address line 1</strong></td>
						<td><?= $_POST['address'] ?></td>
                      </tr>
					  <?php 
					    if (!empty($_POST['address2'])) {
							echo
							'<tr>
							  <td><strong>Address line 2</strong></td>
							  <td>' . $_POST['address2'] . '</td>
							</tr>';
						}
					  ?>
                    </tbody>
                  </table>
				  
				  <div id="success-message" class="alert alert-success d-on-success" style="display: none">We received your order and will contact you back soon.</div>
				  
				  <form id="form-back" action="{{ pages_folder }}{{ order_page }}" method="POST">
					<?php foreach( $_POST as $key => $val ): ?>
						<input type="hidden" name="<?= htmlspecialchars($key, ENT_COMPAT, 'UTF-8') ?>" value="<?= htmlspecialchars($val, ENT_COMPAT, 'UTF-8') ?>">
					<?php endforeach; ?>
				  </form>
				  
				  <form id="form-checkout" action="{{ assets_folder }}php/sendmail.php" method="POST" data-form="custom-mailer">
					<div class="alert alert-success d-on-success">We received your message and will contact you back soon.</div>
					
					<?php foreach( $_POST as $key => $val ): ?>
						<input type="hidden" name="<?= htmlspecialchars($key, ENT_COMPAT, 'UTF-8') ?>" value="<?= htmlspecialchars($val, ENT_COMPAT, 'UTF-8') ?>">
					<?php endforeach; ?>
					
					<input type="hidden" name="product-price" value="<?= $product_price ?>{{ CURRENCY }}">
					<input type="hidden" name="support-price" value="<?= $support_price ?>{{ CURRENCY }}">
					<input type="hidden" name="total-price" value="<?= $total ?>{{ CURRENCY }}">
					
					<div id="recaptcha" class="text-center w-75 d-block mx-auto p-5" data-provide="recaptcha" data-callback="EnableOrder">
					</div>
				  </form>
                  
                  <div class="row">
                    <div class="col-6">
						<button id="btn-back" class="btn btn-block btn-secondary" type="submit" form="form-back"><i class="ti-angle-left fs-9"></i> Back</button>
                    </div>
                  
                    <div class="col-6">
						<button id="btn-checkout" class="btn btn-block btn-primary" type="submit" form="form-checkout" disabled>Checkout <i class="ti-angle-right fs-9"></i></button>
                    </div>
                  </div>
				  
				  <div id="return-home" class="row" style="display: none;">
				    <a class="btn btn-block btn-primary" href="{{ index_file }}#home">Return to home page <i class="ti-angle-right fs-9"></i></a>
				  </div>

			</div>
		  </div>


		</div>
	  </section>
	</main><!-- /.main-content -->


	<!-- Footer -->
	<footer id="footer" class="footer py-7">
		{% include 'footer.html' %}
	</footer><!-- /.footer -->

    <!-- Scripts -->
    {% include 'scripts.html' %}
	<script src="{{ assets_folder }}js/checkout.js"></script>

  </body>
</html>
