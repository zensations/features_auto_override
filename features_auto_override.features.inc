<?php
/**
 * @file
 * Features hook implementations for features_auto_override.
 */

/**
 * Implements hook_features_export_options().
 */
function features_auto_override_features_export_options() {
  $options = array();
  foreach (features_get_features() as $machine_name => $feature) {
    $options[$machine_name] = $feature->info['name'];
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function features_auto_override_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $overrides = features_override_get_overrides();
  $components = array();
  foreach ($overrides as $component => $override) {
    if (count($override) > 0) {
      $components[] = $component;
    }
  }
  foreach ($data as $feature) {
    foreach ($components as $comp) {
      $items = array_keys(features_override_module_component_overrides($feature, $comp));
      foreach ($items as $item) {
        $pipe['features_override_items'][] = $comp . '.' . $item;
      }
    }
    $export['features']['features_auto_override'][$feature] = $feature;
  }
  return $pipe;
}
