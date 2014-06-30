# Block Benchmark

Quick and dirty drush plugin to benchmark the render time for each of the blocks on your Drupal site.

## Usage

1. [Download](https://github.com/nicksantamaria/drush-block_benchmark/archive/master.zip) this to plugin to `${HOME}/.drush`
1. Run `drush cc drush`
1. From your sites docroot, run `drush block_benchmark`

It will return you a table with all of the blocks and their render times, sorted by longest to shortest render time.
