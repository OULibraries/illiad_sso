<?php

// contents
//  contents
//  custom_mvc
//  _get_settings_variable


// data-oriented approach to mvc (not really mvc per se)
function illiad_sso_custom_mvc($key){
 if("keys" == $key)
  return array(
   "keys",
   "login",
   "config",
   "settings"
  );
 $t = get_t();
 $module_name = "illiad_sso";
 $product_name = "ILLiad";
 $module_title = $product_name . " SSO";
 $admin_path = "admin/config";
 $config_path = $admin_path . "/" . "illiad";
 $administer_permission_name = "administer " . $module_name;
 $access_permission_name = "access " . $product_name;
 if("login" == $key){
  $path = "illiad_login";
  $title = $product_name . " login";
  $description = "log in to " . $product_name;
  $page_callback = $module_name . "_custom_page_login";
  $access_arguments = array("access content");
  $type = MENU_CALLBACK;
  $menu_item = array(
   "title" => $title,
   "description" => $description,
   "page callback" => $page_callback,
   "access arguments" => $access_arguments,
   "type" => $type
  );
  $menu_data = array(
   "path" => $path,
   "menu" => $menu_item
  );

  $access_permission = array(
   "name" => $access_permission_name,
   "item" => array(
    "title" => $t("Access " . $product_name),
    "description" => $t("Log in to " . $product_name)
   )
  );
  $permissions = array(
   $access_permission
  );
  $result = array(
   "menu" => $menu_data,
   "permissions" => $permissions
  );
  return $result;
 }
 if("config" == $key){
  $path = $config_path;
  $title = $module_title;
  $description = "Configure " . $product_name . " integration.";
  $page_callback = "system_admin_menu_block_page";
  $file = "system.admin.inc";
  $file_path = drupal_get_path("module", "system");
  $permission_name = $administer_permission_name;
  $menu_item = array(
   "title" => $title,
   "description" => $description,
   "page callback" => $page_callback,
   "access arguments" => array($permission_name),
   "file" => $file,
   "file path" => $file_path
  );
  $menu_data = array(
   "path" => $path,
   "menu" => $menu_item
  );

  $permission_description = $t("Configure the " . $title . " module");
  $permission = array(
   "title" => $t("Administer " . $title),
   "description" => $permission_description
  );
  $permission_data = array(
   "name" => $permission_name,
   "item" => $permission
  );
  $result = array(
   "menu" => $menu_data,
   "permissions" => array($permission_data)
  );
  return $result;
 }
 if("settings" == $key){
  $path = $config_path . "/settings";
  $title = $module_title . " Settings";
  $description = "Settings for ILLiad";
  $page_callback = "drupal_get_form";
  $form_name = $module_name . "_custom_form_" . $key;
  $permission_name = $administer_permission_name;
  $menu_item = array(
   "title" => $title,
   "description" => $description,
   "page callback" => $page_callback,
   "page arguments" => array($form_name),
   "access arguments" => array($permission_name)
  );
  $menu_data = array(
   "path" => $path,
   "menu" => $menu_item
  );

  $sooner_id_profilefield_name_field = array(
   "field_name" => "sooner_id_profilefield_name",
   "default" => "field_sooner_id",
   "field" => array(
    "#type" => "textfield", # TODO: can we make this an options field?
    "#title" => $t("Sooner ID profile field name"),
    "#description" => $t("the name of the profile field that represents the Sooner ID")
   )
  );
  $permission_field = array(
   "field_name" => "illiad_sso_access_permission",
   "default" => $access_permission_name,
   "field" => array(
    "#type" => "textfield", # TODO: make this an options field
    "#title" => $t("ILLiad access permission name"),
    "#description" => $t("the user_access() parameter to check before redirecting to ILLiad")
   )
  );
  $url_field = array(
   "field_name" => "illiad_url",
   "default" => "https://ill.libraries.ou.edu/illiad.dll",
   "field" => array(
    "#type" => "textfield", # TODO: is there a URL field?
    "#title" => $t("ILLiad sign-on URL"),
    "#description" => $t("the form processing agent for ILLiad's API")
   )
  );
  $secret_field = array(
   "field_name" => "illiad_shared_secret",
   "default" => "0",
   "field" => array(
    "#type" => "textfield", # TODO: make this a numeric field
    "#title" => $t("ILLiad shared secret"),
    "#description" => $t("the number to blend with the password")
   )
  );
  $fields = array(
   $sooner_id_profilefield_name_field, # unfortunately, order matters
   $permission_field,
   $url_field,
   $secret_field
  );
  $form_data = array(
   "fields" => $fields
  );
  $result = array(
   "menu" => $menu_data,
   "form" => $form_data,
   "permissions" => array()
  );
  return $result;
 }
}


// a helper for using the variables stored by the settings form
function _illiad_sso_get_settings_variable($varname){
 $module_name = "illiad_sso";
 $settings = illiad_sso_custom_mvc("settings");
 $form = $settings["form"];
 $names_to_index = array_flip(
  array(
   "sooner_id profile_field name",
   "illiad_access permission",
   "illiad url",
   "sso secret"
  )
 );
 $index = $names_to_index[$varname];
 $field_data = $form["fields"][$index];
 $variable = $module_name . "_" . $field_data["field_name"];
 $fallback = $field_data["default"];
 return variable_get($variable, $fallback);
}
