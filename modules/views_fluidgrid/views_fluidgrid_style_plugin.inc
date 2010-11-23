<?php // $Id: views_fluidgrid_style_plugin.inc,v 1.1.2.2 2010/03/04 12:45:38 doublethink Exp $

/**
  * Implementation of views_plugin_style().
  */
class views_fluidgrid_style_plugin extends views_plugin_style {

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['single_mode'] = array('default' => 'false');
    $options['column_width'] = array('default' => '');
    $options['resizeable'] = array('default' => 'true');
    $options['callback'] = array('default' => '');

    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['single_mode'] = array(
      '#type' => 'radios',
      '#title' => t('Single mode'),
      '#options' => array('true' => t('True'), 'false' => t('False')),
      '#default_value' => $this->options['single_mode'],
      '#description' => t('Disables measuring the width of each floated element. Set to true if floated elements have the same width.'),
    );
    $form['column_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Column width'),
      '#default_value' => $this->options['column_width'],
      '#description' => t('Width in pixels of 1 column of your grid. default: outer width of the first floated element.'),
    );
    $form['resizeable'] = array(
      '#type' => 'radios',
      '#title' => t('Resizable'),
      '#options' => array('true' => t('True'), 'false' => t('False')),
      '#default_value' => $this->options['resizeable'],
      '#description' => t('Binds a Masonry call to window resizes.'),
    );
    $form['callback'] = array(
      '#type' => 'textarea',
      '#title' => t('Custom action on each item'),
      '#default_value' => $this->options['callback'],
      '#description' => t('<i>$(this)</i> will refer to the applicable elements Masonry just rearranged.'),
    );
  }
}