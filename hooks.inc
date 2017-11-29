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

  $items = array (
    'illiad_login' => array (
        'title' => 'My ILL Login',
        'description' => 'log in to ILLiad',
        'page callback' => 'illiad_sso_custom_page_login',
        'access arguments' => 
        array (
          0 => 'access content',
        ),
        'type' => 0,
      ),
    'admin/config/illiad' => array (
        'title' => 'ILLiad SSO',
        'description' => 'Configure ILLiad integration.',
        'page callback' => 'system_admin_menu_block_page',
        'access arguments' => array (
          0 => 'administer illiad_sso',
        ),
        'file' => 'system.admin.inc',
        'file path' => 'modules/system',
      ),
    'admin/config/illiad/settings' => array (
        'title' => 'ILLiad SSO Settings',
        'description' => 'Settings for ILLiad',
        'page callback' => 'drupal_get_form',
        'page arguments' => array (
          0 => 'illiad_sso_custom_form_settings',
        ),
        'access arguments' => array (
          0 => 'administer illiad_sso',
        ),
      ),
  );
  return $items;	
}

// implements hook_permission
function illiad_sso_permission(){
	
  $permissions = array (
    'access ILLiad' => array (
      'title' => 'Access ILLiad',
      'description' => 'Log in to ILLiad',
    ),
    'administer illiad_sso' => array (
      'title' => 'Administer ILLiad SSO',
      'description' => 'Configure the ILLiad SSO module',
    ),
  );
  
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
