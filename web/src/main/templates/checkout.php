<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="Choose your favorite product and fill the billing details details.">

    <title>Vanilla.PDF - Checkout</title>

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
			
				<div id="order-checkout">
			      <div class="cart-price">
                    <div class="flexbox">
                      <div>
					    <p><strong>Product:</strong></p>
					    <p><strong>Support:</strong></p>
                        <p><strong>Subtotal:</strong></p>
                        <p><strong>Tax (%19):</strong></p>
                      </div>
                  
                      <div>
					    <p id="product-name"></p>
					    <p id="support-name"></p>
                        <p id="subtotal-price"></p>
                        <p id="tax-price"></p>
                      </div>
                    </div>
                  
                    <hr>
                  
                    <div class="flexbox">
                      <div>
                        <p><strong>Total:</strong></p>
                      </div>
                  
                      <div>
                        <p id="total-price" class="fw-600"></p>
                      </div>
                    </div>
                  </div>
                  
                  <div class="row">
                    <div class="col-6">
					  <form action="{{ pages_folder }}{{ order_page }}" method="POST">
						<?php foreach( $_POST as $key => $val ): ?>
							<input type="hidden" name="<?= htmlspecialchars($key, ENT_COMPAT, 'UTF-8') ?>" value="<?= htmlspecialchars($val, ENT_COMPAT, 'UTF-8') ?>">
						<?php endforeach; ?>
						
						<button class="btn btn-block btn-secondary" type="submit"><i class="ti-angle-left fs-9"></i> Back</a>
					  </form>
                    </div>
                  
                    <div class="col-6">
					  <form action="{{ assets_folder }}php/sendmail.php" method="POST" data-form="custom-mailer">
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


		</div>
	  </section>
	</main><!-- /.main-content -->


	<!-- Footer -->
	<footer id="footer" class="footer py-7">
		{% include 'footer.html' %}
	</footer><!-- /.footer -->

    <!-- Scripts -->
    {% include 'scripts.html' %}
	<script src="{{ assets_folder }}js/price_constants.js"></script>
	<script src="{{ assets_folder }}js/checkout.js"></script>

  </body>
</html>
