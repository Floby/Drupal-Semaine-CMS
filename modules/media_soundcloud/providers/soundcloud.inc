<?php
// $Id: soundcloud.inc,v 1.2 2010/09/29 14:08:11 joachim Exp $

/**
 * @file
 * This include processes Soundcloud audio files for use by emaudio.module.
 */

define('EMAUDIO_SOUNDCLOUD_MAIN_URL', 'http://soundcloud.com/');
/**
 * Implementation of hook emaudio_PROVIDER_info().
 *
 * This returns information relevant to a specific 3rd party audio provider.
 *
 * @return
 *   An array of strings requested by various admin and other forms.
 *   'name' => The translated name of the provider.
 *   'url' => The url to the main page for the provider.
 *   'settings_description' => A description of the provider that will be posted in the admin settings form.
 *   'supported_features' => An array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emaudio_soundcloud_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
  );
  return array(
    'provider' => 'soundcloud',
    'name' => t('SoundCloud'),
    'url' => EMAUDIO_SOUNDCLOUD_MAIN_URL,
    'settings_description' => t('These settings specifically affect audio displayed from <a href="@soundcloud" target="_blank">SoundCloud</a>.', array('@soundcloud' => EMAUDIO_SOUNDCLOUD_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 * Implementation of hook emaudio_PROVIDER_settings().
 *
 * This should return a subform to be added to the emaudio_settings() admin settings page.
 * Note that a form field will already be provided, at $form['odeo'] (such as $form['podomatic']).
 * So if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emaudio_soundcloud_settings() {
  $form = array();

  return $form;
}

/**
 * Implementation of hook emaudio_PROVIDER_extract().
 *
 * This is called to extract the video code from a pasted URL or embed code.
 *
 * @param $embed
 *   An optional string with the pasted URL or embed code.
 * @return
 *   Either an array of regex expressions to be tested, or a string with the
 *   audio code to be used. If the hook tests the code itself, it should return
 *   either the string of the audio code (if matched), or an empty array.
 *   Otherwise, the calling function will handle testing the embed code against
 *   each regex string in the returned array.
 */
function emaudio_soundcloud_extract($embed = '') {
  return array(
    '@soundcloud\.com/([^"\&]+)@i',
  );
}

/**
 * The embedded flash displaying the SoundCloud audio.
 */
function theme_emaudio_soundcloud_flash($item, $embed, $width, $height, $autoplay) {
  // If the swf element is NULL, then we have an embed code instead of a URL; 
  // return this directly.
  if (is_null($item['data']['swf'])) {
    return $item['embed'];
  }

  $output = '';
  if ($embed) {
    if ($item['data']['emaudio_soundcloud_version'] >= 1) {
      $id = form_clean_id('soundcloud');
      $autoplay = $autoplay ? '&amp;auto_play=true' : '';
      $output .= <<<SOUNDCLOUDEMBED
        <object type="application/x-shockwave-flash" data="{$item['data']['swf']}$autoplay" width="$width" height="81" id="$id">
          <param name="movie" value="{$item['data']['swf']}" />
          <param name="allowscriptaccess" value="always"></param>
          <embed allowscriptaccess="always" height="81" src="{$item['data']['swf']}$autoplay" type="application/x-shockwave-flash" width="$width">
          </embed>
        </object>
SOUNDCLOUDEMBED;
    }
  }

  return $output;
}

/**
 * Implementation of hook_PROVIDER_data().
 */
function emaudio_soundcloud_data($field, $item) {
  // Create the initial data for the enclosure with the thumbnail and asset.
  $data = array(
    'emaudio_soundcloud_version' => 1,
  );

  $links = array();
  $response = drupal_http_request(EMAUDIO_SOUNDCLOUD_MAIN_URL . $item['value']);
  if ($response->code == 200) {
    $patterns = array();
    $h1tags = preg_match('/(<link href="(.*)" rel="video_src" \/>)/i', $response->data, $patterns);
    $data['player'] = isset($patterns[2]) ? explode('&', $patterns[2]) : NULL;
    $data['swf'] = isset($data['player'][0]) ? $data['player'][0] : NULL;
  }
  return $data;
}

/**
 * Implementation of hook emaudio_PROVIDER_thumbnail().
 *
 * Returns the external url for a thumbnail of a specific audio.
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things.
 *
 * @param $field
 *   The field of the requesting node.
 * @param $item
 *   The actual content of the field from the requesting node.
 * @return
 *   A URL pointing to the thumbnail.
 */
function emaudio_soundcloud_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return $tn;
}

/**
 * Implementation of hook emaudio_PROVIDER_audio().
 *
 * This actually displays the full/normal-sized video we want, usually on the default page view.
 *
 * @param $embed
 *   The video code for the audio to embed.
 * @param $width
 *   The width to display the audio.
 * @param $height
 *   The height to display the audio.
 * @param $field
 *   The field info from the requesting node.
 * @param $item
 *   The actual content from the field.
 * @return
 *   The html of the embedded audio.
 */
function emaudio_soundcloud_audio($embed, $width, $height, $field, $item, $node, $autoplay) {
  return theme('emaudio_soundcloud_flash', $item, $embed, $width, $height, $autoplay);
}

/**
 * Implementation of hook emaudio_odeo_preview().
 *
 * This actually displays the preview-sized video we want, commonly for the teaser.
 *
 * @param $embed
 *   The video code for the audio to embed.
 * @param $width
 *   The width to display the audio.
 * @param $height
 *   The height to display the audio.
 * @param $field
 *   The field info from the requesting node.
 * @param $item
 *   The actual content from the field.
 * @return
 *   The html of the embedded audio.
 */
function emaudio_soundcloud_preview($embed, $width, $height, $field, $item, &$node, $autoplay) {
  return theme('emaudio_soundcloud_flash', $item, $embed, $width, $height, $autoplay);
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emaudio_soundcloud_emfield_subtheme() {
  return array (
    'emaudio_soundcloud_flash'  => array(
            'arguments' => array('item' => NULL, 'embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
            'file' => 'providers/soundcloud.inc'
        )
    );
}
