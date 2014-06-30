<?php

/**
 * Implementation of hook_drush_command().
 */
function block_benchmark_drush_command() {
  $items['block_benchmark'] = array(
    'description' => 'Runs a benchmark test against blocks.',
  );
  return $items;
}

/**
 * Drush command callback for Block Benchmark.
 */
function drush_block_benchmark() {
  $blocks = array();

  foreach (module_implements('block_info') as $module) {
    $blocks[$module] = module_invoke($module, 'block_info');
  }

  $benchmark_data = array();
  foreach ($blocks as $module => $infos) {
    foreach ($infos as $delta => $info) {
      $timer_name = "${module}_${delta}";
      timer_start($timer_name);

      // Call the module's hook_block_view();
      $func = "${module}_block_view";
      $data = $func($delta);

      $block = new stdClass;
      $block->module = $module;
      $block->delta = $delta;

      // Pass rendered block to the hook_block_view_alter() hooks.
      drupal_alter("block_view_{$block->module}_{$delta}", $data, $block);
      drupal_alter("block_view", $data, $block);

      render($data);

      timer_stop($timer_name);

      $benchmark_data["${module}_${delta}"] = array(
        'module' => $module,
        'label' => $blocks[$module][$delta]['info'],
        'delta' => $delta,
        'timer' => timer_read($timer_name),
      );
    }
  }

  uasort($benchmark_data, 'drush_block_benchmark_sort_timer');

  array_unshift($benchmark_data, array('Module', 'Delta', 'Time (ms)'));
  drush_print_table($benchmark_data, TRUE);
}

function drush_block_benchmark_sort_timer($a, $b) {
  $a_weight = (is_array($a) && isset($a['timer'])) ? $a['timer'] : 0;
  $b_weight = (is_array($b) && isset($b['timer'])) ? $b['timer'] : 0;
  if ($a_weight == $b_weight) {
    return 0;
  }
  return ($a_weight < $b_weight) ? 1 : -1;
}
