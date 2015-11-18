Purpose
-------

Allows Drupal to share user sessions with ILLiad.


Installation
------------

1. Place the illiad_sso module into your modules directory (sites/all/modules)

2. Enable the module in admin >> site configuration >> modules.

3. Set up access permissions in admin >> user >> permissions.


Configuration
-------------

1. Go to the configuration page for ILLiad (admin >> site
   configuration >> illiad_sso) and enter the relevant settings
   for your ILLiad server.  The ILLiad shared secret defaults
   to zero, which is incorrect, but I can't very well go around
   storing secrets in the default values in the source code, so
   you'll have to ask somebody if you don't already know it.


Usage
-----

1. When you visit the ILLiad login URL (starting with
   "/illiad_login?") , it will first ask Drupal if you are
   authenticated.  If the user is currently logged in and has the
   relevant permission, then Drupal will fill out and submit the
   ILLiad login form, and the user will be taken to ILLiad. If
   the user is not logged in, Drupal will prompt them to log in.


Author/Maintainer
-----------------

Montana Rowe montana.s.rowe-1@ou.edu
