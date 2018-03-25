<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="Choose your favorite product and fill the billing details details.">

    <title>Vanilla.PDF - Order</title>

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
			<a class="nav-link" href="#section-order">Order</a>
            <a class="nav-link" href="#footer">References</a>
          </nav>
        </section>

      </div>
    </nav><!-- /.navbar -->


    <!-- Header -->
    <header id="home" class="header pt-10 pb-0">
      <div class="container text-center">
        <h1 class="display-4">Submit an order</h1>
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
	  <section id="section-order" class="section bg-gray">
		<div class="container">

		  <div class="row gap-y">
			<div class="col-11 col-lg-6 mx-auto">

			  <form id="order-form" action="{{ pages_folder }}{{ checkout_page }}" method="POST" data-form="custom-mailer">
				<div class="form-group">
				  <label for="order-name">Your name:</label>
				  <input class="form-control form-control-lg" id="order-name" type="text" name="name" placeholder="Your name (required)" required
				  value="<?php if (isset($_POST['name'])) { echo $_POST['name']; } ?>">
				</div>
				
				<div class="form-group">
				  <label for="order-company">Your company name:</label>
				  <input class="form-control form-control-lg" id="order-company" type="text" name="company" placeholder="Your company name"
				  value="<?php if (isset($_POST['company'])) { echo $_POST['company']; } ?>">
				</div>

				<div class="form-group">
				  <label for="order-email">Your email address:</label>
				  <input class="form-control form-control-lg" id="order-email" type="email" name="email" placeholder="Your email address (required)" required
				  value="<?php if (isset($_POST['email'])) { echo $_POST['email']; } ?>">
				</div>
				
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
				  <label for="order-product">Product:</label>
				  <select id="order-product" name="product" class="form-control form-control-lg" onchange="ProductChanged();">
				    <option value="personal-license" <?php if ($_POST['product'] == 'personal-license') { echo 'selected="selected"'; } ?> >{{ PERSONAL_LICENSE_DESCRIPTION }}</option>
				    <option value="commercial-license" <?php if ($_POST['product'] == 'commercial-license') { echo 'selected="selected"'; } ?> >{{ COMMECRIAL_LICENSE_DESCRIPTION }}</option>
					<option value="personal-support" <?php if ($_POST['product'] == 'personal-support') { echo 'selected="selected"'; } ?> >{{ PERSONAL_SUPPORT_DESCRIPTION }}</option>
					<option value="commercial-support" <?php if ($_POST['product'] == 'commercial-support') { echo 'selected="selected"'; } ?> >{{ COMMECRIAL_SUPPORT_DESCRIPTION }}</option>
				  </select>
				</div>
				
				<div id="license-group" class="form-group">
				  <label for="license-key">Your license key:</label>
				  <input class="form-control form-control-lg" id="license-key" type="text" name="license" placeholder="Your license key (required)" required>
				</div>
				
				<div class="form-group">
				  <label for="order-support">Support:</label>
				  <select id="order-support" name="support" class="form-control form-control-lg">
				    <option value="1" <?php if ($_POST['support'] == '1') { echo 'selected="selected"'; } ?> >1 Year</option>
					<option value="2" <?php if ($_POST['support'] == '2') { echo 'selected="selected"'; } ?> >2 Year</option>
					<option value="3" <?php if ($_POST['support'] == '3') { echo 'selected="selected"'; } ?> >3 Year</option>
				  </select>
				</div>

				<div class="form-group">
				  <label for="order-message">Special request:</label>
				  <textarea class="form-control form-control-lg" id="order-message" name="message" rows="4" placeholder="Special request">
				    <?php if (isset($_POST['message'])) { echo $_POST['message']; } ?>
				  </textarea>
				</div>
				
				<div class="text-center w-75 d-block mx-auto p-5" data-provide="recaptcha" data-callback="EnableOrder">
				</div>

				<input type="hidden" name="subject" value="Order">
				<button id="btn-order" class="btn btn-lg btn-primary btn-block" type="submit">Proceed <i class="ti-angle-right fs-9"></i></button>
			  </form>

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
	<script src="{{ assets_folder }}js/custom_mailer.js"></script>
	<script src="{{ assets_folder }}js/order.js"></script>

  </body>
</html>
