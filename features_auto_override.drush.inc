<?php
/**
 * @file
 * Drush commands for features_auto_override.
 */

/**
 * Implements hook_drush_command().
 */
function features_auto_override_drush_command() {
  $items = array();
  $items['features-auto-override-update'] = array(
    'description' => "Update all auto-override features.",
    'drupal dependencies' => array('features_auto_override'),
    'aliases' => array('faou'),
  );
  $items['features-auto-override-init'] = array(
    'description' => "Initiate configuration features.",
    'drupal dependencies' => array('features_auto_override'),
    'aliases' => array('faoi'),
  );
  return $items;
}

function drush_features_auto_override_update() {

}

function drush_features_auto_override_init($path) {
  if (!is_dir($path)) {
    if ($path != '' && is_dir('profiles/' . $path)) {
      $path = 'profiles/' . $path . '/modules';
    }
    elseif ($path != '' && is_dir('sites/' . $path)) {
      $path = 'profiles/' . $path . '/modules';
    }
    else {
      $path = drush_site_path() . '/modules';
    }
  }
  drupal_set_message('Installing configuration features to "' . $path . '".');
  if (!is_dir($path)) {
    mkdir($path);
  }
  $features = features_get_features();
  $module_list = array();
  foreach ($features as $name => $feature) {
    if (@$feature->info['hidden'] && @$feature->info['features']['features_auto_override']) {
      $module_list[] = $name;
      if (is_dir($path . '/' . $name)) {
        continue;
      }
      $src = drupal_get_path('module', $name);
      $dest = $path . '/' . $name;
      drush_copy_dir($src, $dest);
      $file = $dest . '/' . $name . '.info';
      $info = explode("\n", file_get_contents($file));
      foreach ($info as $index => $line) {
        if (preg_match('/hidden\s*\=/', $line)) {
          unset($info[$index]);
        }
      }
      file_put_contents($file, implode("\n", $info));
    }
  }
  drush_print_r($module_list);
  module_enable($module_list, TRUE);
}
