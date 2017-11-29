<?php

// contents
//  contents
//  custom_form
//  custom_form_sooner_id
//  custom_submit_form_sooner_id
// form builder for settings page


function illiad_sso_custom_form_settings(){

  $form = array (
    'illiad_sso_sooner_id_profilefield_name' => array (
      '#type' => 'textfield',
      '#title' => 'Sooner ID profile field name',
      '#description' => 'the name of the profile field that represents the Sooner ID',
      '#default_value' => 'field_sooner_id',
    ),
    'illiad_sso_illiad_sso_access_permission' => array (
      '#type' => 'textfield',
      '#title' => 'ILLiad access permission name',
      '#description' => 'the user_access() parameter to check before redirecting to ILLiad',
      '#default_value' => 'access ILLiad',
    ),
    'illiad_sso_illiad_url' => array (
      '#type' => 'textfield',
      '#title' => 'ILLiad sign-on URL',
      '#description' => 'the form processing agent for ILLiad\'s API',
      '#default_value' => 'https://ill.libraries.ou.edu/illiad.dll',
    ),
    'illiad_sso_illiad_shared_secret' => array (
      '#type' => 'textfield',
      '#title' => 'ILLiad shared secret',
      '#description' => 'the number to blend with the password',
      '#default_value' => '1492',
    ),
    'illiad_sso_login_instructions' => array (
      '#type' => 'textarea',
      '#title' => 'Login page instructions.',
      '#description' => 'The text that appears under the page title and above the form when the user is not logged in.  It should tell them why they need to log in.  It is raw HTML.',
      '#default_value' => '<strong>Note</strong>: to utilize OU-Norman interlibrary loan and Sooner Xpress services, you must first log into the libraries\' website.
  <br />&nbsp;<br>
  <strong>ILL Links for other OU campuses</strong>
  <ul>
  <li><a href="https://illiad.ouhsc.edu/illiad" target="_blank">OU HSC Interlibrary Loan</a> - Log in here if you are a Health Sciences student</li>
  <li><a href="https://outulsa.illiad.oclc.org/illiad/illiad.dll?Action=10&Form=30&genre=book" target="_blank">OU-Tulsa Interlibrary Loan</a> - Log in here if you are an OU-Tulsa student</li>
  </ul>
  <strong>Off-Site Storage Request link for Non-OU Patrons</strong>
  <ul>
  <li><a href="https://libraries.webdev.lib.ou.edu/content/paging-request-item-site-storage" target="_blank">Off-Site Storage Request form</a></li>
  </ul>',
    ),
    'illiad_sso_sooner_id_description' => array ( 
      '#type' => 'textarea', 
      '#title' => 'Sooner ID instructions',
      '#description' => 'Raw HTML for the description above the Sooner ID form.',
      '#default_value' => '<div class="row">
  <div class="col-md-12">
  <p>The InterLibrary Loan system needs to perform a one-time verification using your Sooner ID <strong>(not your 4x4)</strong>.  </p>
  <hr>
  </div>
  </div>
  <div class="row">
  <div class="col-md-4">
  <img src="https://lib.ou.edu/documents/sooner_id_sm.jpg">
  </div>
  <div class="col-md-8">
  <p>&nbsp;</p>
  <p>This information can be found on the student or faculty/staff ID card assigned to you. For help finding this information, please contact the Sooner Card Office at (405)-325-3113.</p>
  <p>&nbsp;</p>
  <p>OU-Tulsa students can login at  <a href="https://outulsa.illiad.oclc.org/illiad/illiad.dll?Action=10&Form=30&genre=book" target="_blank">OU-Tulsa Interlibrary Loan</a> to use their ILL service.</p></div></div>
  
  </row>',
    ),
    'illiad_sso_contact_message' => array (
      '#type' => 'textarea',
      '#title' => 'ILL contact message',
      '#description' => 'Raw HTML saying how to contact ILL.  This appears underneath the Sooner ID form.',
      '#default_value' => 'You can contact <a href="https://libraries.ou.edu/ill">InterLibrary Loan</a> at <a href="tel:+14053256422">(405)-325-6422</a>.',
    ),
  );
  return system_settings_form( $form);
}


// form builder for missing Sooner ID on redirect page
function illiad_sso_custom_form_sooner_id(){
 $phone_link = "";
 $phone_link = " or <a href=\"tel:+14053253113\">(405)-325-3113</a>";
 $phone_link = " or (405)-325-3113";
 $sooner_card_email = "<a href=\"mailto:soonercard@ou.edu\">soonercard@ou.edu</a>";
 $sooner_card_email = "soonercard@ou.edu";
 $contact_message = t("For help finding this information, please contact the Sooner Card Office at " . $sooner_card_email . $phone_link . ".");
 $contact_message = "";
 $sooner_id_field = array(
  "#type" => "textfield",
  "#title" => t("Sooner ID"),
  "#description" => $contact_message,
  "#required" => TRUE,
  "#element_validate" => array("element_validate_integer_positive"), // TODO: custom validator
  "#maxlength" => 9,
 );
 $confirm_sooner_id_field = array(
  "#title" => t("Confirm Sooner ID"),
  "#description" => t("Type your Sooner ID again."),
 );
 $confirm_sooner_id_field += $sooner_id_field;
 return array(
  "sooner_id" => $sooner_id_field,
  "confirm_sooner_id" => $confirm_sooner_id_field,
  "buttons" => array(
   "submit" => array(
    "#type" => "submit",
    "#value" => t("Set your Sooner ID")
   )
  ),
  "#submit" => array("illiad_sso_custom_submit_form_sooner_id")
 );
}

function illiad_sso_custom_submit_form_sooner_id($form, &$form_state){
 global $user;
 $product_name = "ILLiad";

 $redirect_url = $form_state["values"]["illiad_sso_redirect_url"];
 $sooner_id = $form_state["values"]["sooner_id"];
 $confirm_sooner_id = $form_state["values"]["confirm_sooner_id"];
 $error = "";
 if($sooner_id != $confirm_sooner_id)
  $error = t("Sooner ID must match.");
 if(9 != strlen($sooner_id))
  $error = t("Sooner ID must be exactly 9 digits.");
 if($error){
  drupal_set_message($error, "error");
  header("Location: " . $redirect_url);
  exit();
 }

 $account = user_load($user->uid);
 $sooner_id_field_name = variable_get("illiad_sso_sooner_id_profilefield_name");
 $sooner_id_field = field_get_items("user", $account, $sooner_id_field_name);
 if($sooner_id_field)
  if(array_key_exists(0, $sooner_id_field))
   if(array_key_exists("value", $sooner_id_field[0]))
    if($sooner_id_field[0]["value"]){
     drupal_set_message("Your Sooner ID was already set.", "warning");
     return;
    }

 $edit = array();
 $edit[$sooner_id_field_name] = array("und" => array(array("value" => $sooner_id)));
 user_save($account, $edit);

 header("Location: " . $redirect_url);
 exit();
}
