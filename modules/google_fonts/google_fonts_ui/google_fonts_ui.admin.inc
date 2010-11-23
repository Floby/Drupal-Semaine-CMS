<?php
// $Id$

/**
 * @file
 * Administrative page callbacks for the Google Fonts UI module.
 */

/**
 * Implementation of hook_admin_settings() for configuring the module
 */
function google_fonts_ui_admin_settings_form(&$form_state) {
  $form = array();
  $enabled_fonts = variable_get('google_fonts_enabled_fonts', array());
  
  if (empty($enabled_fonts)) {
    $form['introduction'] = array(
      '#value' => t('You have not enabled any fonts. Please do this first, before you can apply any CSS rules.'),
    );
    return $form;
  }
  
  $form['introduction'] = array(
    '#value' => t('For each enabled font you can add one or more rules.<br />Examples given: <code>.node H2</code> or <code>#logo-floater h1</code>'),
  );

  $fonts_to_load = array();
  $style_to_add = '<style type="text/css">.font_example label{ font-size: 30px; } ';
  foreach ($enabled_fonts as $font) {
    if ($font) {
      $font_info = _google_fonts_get_font_info($font);
      $form['google_fonts_ui_' . _google_fonts_system_fontname($font)] = array(
        '#type' => 'textarea',
        '#title' => $font,
        '#default_value' => variable_get('google_fonts_ui_' . _google_fonts_system_fontname($font), NULL),
        '#description' => 'Add one or more CSS selectors, one for each line.',
        '#prefix' => '<div class="font_example ' . _google_fonts_path_to_classname($font_info['path']) . '">',
        '#suffix' => '</div>',
        '#rows' => 3,
      );
      
      $style_to_add .= '.' . _google_fonts_path_to_classname($font_info['path']) . ' label{ font-family: "' . $font_info['family'] . '"; font-style: ' . $font_info['style'] . '; font-weight: ' . $font_info['weight'] . '; } ' . PHP_EOL;
      $fonts_to_load[] = $font;
    }
  }
  $style_to_add .= '</style>';
  
  $form['enabled_fonts'] = array(
    '#type' => 'hidden',
    '#value' => implode('|', $fonts_to_load),
  );
  
  $form['#submit'][] = 'google_fonts_ui_admin_settings_form_submit';
  
  drupal_set_html_head($style_to_add);
  return system_settings_form($form);
}

/**
 * Implementation of hook_form_submit() for creating the CSS file
 */
function google_fonts_ui_admin_settings_form_submit($form, &$form_state) {
  $css_value = '/* CSS code for the Google Fonts module */' . PHP_EOL . PHP_EOL;
  foreach (explode('|', $form['enabled_fonts']["#value"]) as $font) {
    $selectors = $form['google_fonts_ui_' . _google_fonts_system_fontname($font)]["#value"];
    if (!empty($selectors)) {
      $font_info = _google_fonts_get_font_info($font);
      $selectors = check_plain(str_replace("\r\n", ",\r\n", trim($selectors)));
      $selectors = str_replace(',,', ',', $selectors);
      $css_value .= $selectors . '{ font-family: "' . $font_info['family'] . '"; font-style: ' . $font_info['style'] . '; font-weight: ' . $font_info['weight'] . '; }' . PHP_EOL . PHP_EOL;
    }
  }
  
  variable_set('google_fonts_ui_css_contents', $css_value);
  
  // Save the CSS code as local file
  if (!_google_fonts_cache($css_value, TRUE)) {
    drupal_set_message(t('Could not generate the CSS code for the Google Fonts.'), 'error');
  }
}