<?php
/**
 * Template Name: Order
 
 * @package WordPress
 * @subpackage Vanilla.PDF
 * @since Vanilla.PDF 1.0
 */
?>

<!DOCTYPE html>
<html lang="en">
  <head>
  <?php
		const PAGE_TITLE = 'Vanilla.PDF - Order';
		const PAGE_DESCRIPTION = "Choose your favorite product and fill the billing details details.";

		// Extra scripts
		add_action('wp_enqueue_scripts', function() {
			wp_enqueue_script('js-order', get_assets_folder() . '/js/order.js');
			wp_enqueue_script('js-mailer', get_assets_folder() . '/js/custom_mailer.js');
		});

		// Wordpress head
		wp_head();
	?>
  </head>

  <body>


    <!-- Navbar -->
    <nav class="navbar navbar-expand-lg navbar-dark" data-navbar="fixed">
      <div class="container">

        <?php get_template_part('inc/navbar-left'); ?>

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

			  <form id="order-form" action="<?php echo get_template_page(PAGE_CHECKOUT); ?>" method="POST" data-form="custom-mailer">
			  
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
				    <option value="personal-license" <?php if ($_POST['product'] == 'personal-license') { echo 'selected="selected"'; } ?><?php echo PERSONAL_LICENSE_DESCRIPTION ?></option>
				    <option value="commercial-license" <?php if ($_POST['product'] == 'commercial-license') { echo 'selected="selected"'; } ?> ><?php echo COMMERCIAL_LICENSE_DESCRIPTION ?></option>
					<option value="personal-support" <?php if ($_POST['product'] == 'personal-support') { echo 'selected="selected"'; } ?> ><?php echo PERSONAL_SUPPORT_DESCRIPTION ?></option>
					<option value="commercial-support" <?php if ($_POST['product'] == 'commercial-support') { echo 'selected="selected"'; } ?> ><?php echo COMMERCIAL_SUPPORT_DESCRIPTION ?></option>
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
                  <label class="custom-control-label">I agree to the <a class="ml-1" href="<?php echo get_template_page(PAGE_LICENSES); ?>">End-user license agreement</a></label>
                </div>

				<input type="hidden" name="subject" value="Order">
				
				<div class="row">
				  <div class="col-6">
				    <a class="btn btn-block btn-secondary" href="<?php echo get_home_url(); ?>#home"><i class="ti-angle-left fs-9"></i> Return to home</a>
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
		<?php get_template_part( 'inc/footer' ); ?>
	</footer><!-- /.footer -->


    <!-- Scripts -->
	<?php wp_footer(); ?>

  </body>
</html>
