<?php // $Id: views_fluidgrid.views.inc,v 1.1 2010/03/02 09:24:50 doublethink Exp $

/**
 * @file
 * Provide a fluid grid display style for Views. This file is autoloaded by views.
 */

/**
  * Implementation of hook_views_plugin().
  */
function views_fluidgrid_views_plugins() {
  return array(
    'style' => array(
      'views_fluidgrid' => array(
        'title' => t('Views Fluid Grid - jQuery Masonry'),
        'theme' => 'views_view_fluidgrid',
        'help' => t('Provide a fluid grid display style for Views.'),
        'handler' => 'views_fluidgrid_style_plugin',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

