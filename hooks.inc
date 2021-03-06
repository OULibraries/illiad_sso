<?php

// contents
//  contents
//  menu
//  permission
//  custom_form_alter_redirect_url
//  form_alter
//  user_login


// implements hook_menu
function illiad_sso_menu(){
 $menu_items = array();
 $keys = illiad_sso_custom_mvc("keys");
 foreach($keys as $key){
  $mvc = illiad_sso_custom_mvc($key);
  if(array_key_exists("menu", $mvc)){
   $menu = $mvc["menu"];
   $menu_items[$menu["path"]] = $menu["menu"];
  }
 }
 return $menu_items;
}

// implements hook_permission
function illiad_sso_permission(){
 $permissions = array();
 $keys = illiad_sso_custom_mvc("keys");
 foreach($keys as $key){
  $mvc = illiad_sso_custom_mvc($key);
  if(array_key_exists("permissions", $mvc)){
   foreach($mvc["permissions"] as $permission)
    $permissions[$permission["name"]] = $permission["item"];
  }
 }
 return $permissions;
}


// helper for form_alter
function illiad_sso_custom_form_alter_redirect_url(&$form){
 global $illiad_sso_volatile_url;
 if(!$illiad_sso_volatile_url) return;
 $form["illiad_sso_redirect_url"] = array(
  "#type" => "hidden",
  "#value" => $illiad_sso_volatile_url
 );
 $illiad_sso_volatile_url = "";
}

// implements hook_form_alter
function illiad_sso_form_alter(&$form, &$form_state, $form_id){
 $target_forms = array(
  "user_login",
  "illiad_sso_custom_form_sooner_id"
 );
 if("illiad_login" != current_path())
  return;
 if(!in_array($form_id, $target_forms))
  return;
 illiad_sso_custom_form_alter_redirect_url($form);
}


// implements hook_user_login
function illiad_sso_user_login(&$edit, $account){
 $values = $edit["values"];
 if(empty($values["illiad_sso_redirect_url"])) return;
 if(!$account->uid) return; // how does this state happen?
 $url = $values["illiad_sso_redirect_url"];
 header("Location: " . $url);
 exit();
}
