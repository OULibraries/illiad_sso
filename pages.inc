<?php

// contents
//  contents
//  _login_action
//  custom_page_login


// a helper for determining the "action" URL of the ILLiad login form
function _illiad_sso_login_action(){
 $querystring = $_SERVER["QUERY_STRING"];
 $url = _illiad_sso_get_settings_variable("illiad url");
 if(!$querystring) return $url;
 $task = "";
 if(array_key_exists("task", $_GET))
  $task = $_GET["task"];

 if("tulsapass" == $task) return $url;

 $replacements = array();
 foreach(array("title", "jtitle", "stitle", "au", "isbn", "issn") as $k){
  $replacements["&" . $k . "=&"] = "&";
  $replacements["&rft." . $k . "=&"] = "&";
  $replacements["&rft_" . $k . "=&"] = "&"; # account for cas module querystring replacement
 }

 $value = "";
 
 if("sxloan" == $task){
  $replacements["genre=book&"] = "genre=sxloan&";
  $replacements["genre=unknown"] = "genre=sxloan";
  $value = "GenericRequestSoonerLoan";
 }
 
 if("sxcopy" == $task){
   foreach(array("article", "journal", "bookitem", "unknown", "issue") as $genre)
    $replacements["genre=" . $genre] = "genre=sxarticle";
   $value = "GenericRequestSoonerArticle";
 }
 
 if(array_key_exists("rft.atitle", $_GET) || array_key_exists("rft_atitle", $_GET)){
  $atitle = "";
  if(array_key_exists("rft.atitle", $_GET)){
   $atitle = $_GET["rft.atitle"];
  } else {
   $atitle = $_GET["rft_atitle"];
  }
  if($atitle){
   foreach(array("unknown", "issue") as $genre)
    $replacements["genre=" . $genre] = "genre=article";
  }
 }
 
 foreach(array(":", "%3A") as $colon)
  $replacements["primo.exlibrisgroup.com" . $colon] = "";
 
 foreach($replacements as $k => $v)
  $querystring = str_replace($k, $v, $querystring);

 $value_param = $value ? "&Value=" . $value : "";
 return $url . "?Action=10&Form=30" . $value_param . "&" . $querystring;
}

// callback for the login page
function illiad_sso_custom_page_login(){
 global $user;
 global $illiad_sso_volatile_url;
 $module_name = "illiad_sso";
 $product_name = "ILLiad";

 $illiad_sso_volatile_url = "";
 if(!$user->uid){
  $illiad_sso_volatile_url = $_SERVER["REQUEST_URI"];
  $form = drupal_get_form("user_login");
  $illiad_sso_volatile_url = "";
  $message = _illiad_sso_get_settings_variable("login_instructions");
  return array(
   array("#markup" => "<div class=\"my_ill_login\">" . $message . "</div>\n"),
   $form
  );
 }

 $access_check = _illiad_sso_get_settings_variable("illiad_access permission");
 if(!user_access($access_check)){
  $error = t("You do not have permission to access " . $product_name);
  drupal_set_message($error, "error");
  return "";
 }

 $account = user_load($user->uid);
 $sooner_id_field_name = _illiad_sso_get_settings_variable("sooner_id profile_field name");
 $sooner_id_field = field_get_items("user", $account, $sooner_id_field_name);
 $sooner_id = "";
 if($sooner_id_field)
  if(array_key_exists(0, $sooner_id_field))
   if(array_key_exists("value", $sooner_id_field[0]))
    $sooner_id = $sooner_id_field[0]["value"];
 if(!$sooner_id){
  $illiad_sso_volatile_url = $_SERVER["REQUEST_URI"];
  $form = drupal_get_form("illiad_sso_custom_form_sooner_id");
  $illiad_sso_volatile_url = "";
  drupal_set_message(t("Sooner ID Verification"), "warning");
  $message = _illiad_sso_get_settings_variable("sooner_id_description");
  $contact_message = _illiad_sso_get_settings_variable("contact_ill");
  return array(
   array(
    "#markup" => "<div class=\"sooner_id_intro\">" . $message . "</div>"
   ),
   $form,
   array(
    "#markup" => "<div class=\"sooner_id\">" . $contact_message . "</div>"
   )
  );
 }

 $ounetid = $user->name;

 $secret = _illiad_sso_get_settings_variable("sso secret");
 $password = $sooner_id * 4 + $secret; # wow

 $login_form_action = _illiad_sso_login_action();

 $form_name = "frmLogon";
 $submit_button_name = "SubmitButton";
 $form_attributes = array(
  "name" => $form_name,
  "method" => "POST",
  "action" => $login_form_action
 );

 $form_open_tag = array(array("#markup" => "<form"));
 foreach($form_attributes as $k => $v){
  $markup = " " . $k . "=\"" . htmlspecialchars($v) . "\"";
  $form_open_tag[] = array("#markup" => $markup);
 }
 $form_open_tag[] = array("#markup" => ">\n");

 $fields = array(
  "ILLiadForm" => array("hidden", "Logon"),
  "Username" => array("hidden", $ounetid),
  "Password" => array("hidden", $password),
  $submit_button_name => array("submit", "Logon to " . $product_name)
 );
 $form = array(); // TODO: build a form instead
 $form[] = $form_open_tag;
 foreach($fields as $name => $field){
  $type = $field[0];
  $value = $field[1];
  $markup = "<input type=\"" . $type . "\" name=\"" . $name . "\" value=\"" . htmlspecialchars($value) . "\">";
  $form[] = array("#markup" => " " . $markup . "\n");
 }
 $form[] = array("#markup" => "</form>\n");
 $script = "document." . $form_name . "." . $submit_button_name . ".click();";
 $script_tag = array(
  array("#markup"=>"<script>\n"),
  array("#markup"=>" " . $script . "\n"),
  array("#markup"=>"</script>\n")
 );
 return array(
  $form,
  $script_tag
 );
}
