<?php
/**
 * @file
 * Handles adding theme stylesheets into WYSIWYG editors.
 */

/**
 * A simple page callback for a checking if a theme is active.
 *
 * A theme the user does not have permission to use can not be set active.
 *
 * @see _wysiwyg_theme_callback()
 * @see _wysiwyg_delivery_dummy()
 */
function _wysiwyg_theme_check_active($theme) {
  global $theme_key;
  return $theme === $theme_key;
}

/**
 * A simple page delivery dummy implementation.
 *
 * Acts like a normal HTML delivery mechanism but only "renders" a dummy string
 * and prints a short string telling if the page callback result was "true".
 *
 * Useful for returning the results of an access check, performed by the page
 * callback. The actual page callback return value is never printed.
 *
 * Success:
 * - Status header: "200 OK"
 * - Content: "OK"
 *
 * Failure:
 * - Status header: "403 Forbidden"
 * - Content: "Forbidden"
 *
 * @see _wysiwyg_theme_check_active()
 */
function _wysiwyg_delivery_dummy($page_callback_result) {
  global $theme_key;
  drupal_add_http_header('Content-Language', 'en');
  drupal_add_http_header('Content-Type', 'text/html; charset=utf-8');
  if ($page_callback_result) {
    drupal_add_http_header('Status', '200 OK');
  }
  else {
    drupal_add_http_header('Status', '403 Forbidden');
  }
  // Make sure the theme is always initialized.
  drupal_theme_initialize();
  // Render a completely themed empty page to catch as many stylesheets as
  // possible, but don't actually return anything to speed up the response.
  $rendered = drupal_render_page('Dummy');
  // In case headers aren't enough, put the status in the response body.
  print ($page_callback_result ? 'OK' : 'Forbidden');
  // Cleanup and session handling.
  drupal_page_footer();
}

/**
 * Theme callback to simply suggest a theme based on the page argument.
 */
function _wysiwyg_theme_callback($theme) {
  return $theme;
}

/**
 * A filtering pre render callback for style elements.
 *
 * Invokes hook_wysiwyg_editor_stules_alter() to allow other code to filter the
 * list of stylesheets which will be used inside the editors in WYSIWYG mode.
 *
 * Intended to run before Core sorts/groups/aggregates stylesheets.
 */
function _wysiwyg_filter_editor_styles(&$elements) {
  global $theme_key;
  if (strpos(current_path(), 'wysiwyg_theme/') !== 0) {
    return $elements;
  }
  $context = array('theme' => $theme_key);
  drupal_alter('wysiwyg_editor_styles', $elements, $context); $css = array();
  return $elements;
}

/**
 * Creates a cache of the stylesheets used by the currently set theme.
 *
 * Since this is a pre render callback for the styles element, it should run
 * late enough to catch all the stylesheets added just before the actual markup
 * for them is rendered.
 *
 * The first time this runs for a theme it's too late for a module to have any
 * use of the cache, so wysiwyg_get_css() uses drupal_http_request() to fetch a
 * dummy page, filling the cache before the original response is sent.
 *
 * Intended to run after Core has sorted/grouped/aggregated stylesheets.
 */
function _wysiwyg_pre_render_styles($elements) {
  global $theme_key;
  if (strpos(current_path(), 'wysiwyg_theme/') !== 0) {
    return $elements;
  }
  $cached = cache_get('wysiwyg_css');
  foreach (element_children($elements) as $child) {
    if (isset($elements['#groups'][$child]['group']) && $elements['#groups'][$child]['group'] != CSS_THEME) {
      continue;
    }
    switch ($elements[$child]['#tag']) {
      case 'link':
        $css[] = $elements[$child]['#attributes']['href'];
        break;
      case 'style':
        if (!empty($elements[$child]['#attributes']['href'])) {
          $css[] = $elements[$child]['#attributes']['href'];
        }
        elseif (!empty($elements[$child]['#value'])){
          preg_match_all('/\@import url\("([^"]+)"\);/', $elements[$child]['#value'], $matches, PREG_SET_ORDER);
          foreach ($matches as $val) {
            $css[] = $val[1];
          }
        }
        break;
    }
    $all = empty($cached->data) ? array() : $cached->data;
    $all[$theme_key] = array('files' => $css, 'aggregated' => variable_get('preprocess_css', FALSE));
  }
  $all['_css_js_query_string'] = variable_get('css_js_query_string');
  cache_set('wysiwyg_css', $all);
  return $elements;
}
