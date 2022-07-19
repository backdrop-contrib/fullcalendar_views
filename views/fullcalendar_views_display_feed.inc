<?php
/**
 * @file
 * Definition of FullcalendarViewsDisplayFeed.
 */

/**
 * Display plugin to render json feeds for FullCalendar.
 */
class FullcalendarViewsDisplayFeed extends views_plugin_display_page {

  /**
   * Overrides views_plugin_display_page::uses_breadcrumb.
   *
   * @return false
   */
  public function uses_breadcrumb() {
    return FALSE;
  }

  /**
   * Overrides views_plugin_display::get_style_type.
   *
   * @return string
   */
  public function get_style_type() {
    return 'fullcalendar_feed';
  }

  /**
   * Overrides views_plugin_display_page::execute().
   *
   * @return mixed
   */
  public function execute() {
    $output = $this->view->render();
    if (!empty($this->view->build_info['denied'])) {
      return MENU_ACCESS_DENIED;
    }
    backdrop_json_output($output);
    backdrop_exit();
  }

  /**
   * Overrides views_plugin_display_page::preview().
   *
   * @return string|null
   */
  public function preview() {
    if (!empty($this->view->live_preview)) {
      $rendered = $this->view->render();
      $json = (string) json_encode($rendered, JSON_PRETTY_PRINT);
      return '<pre>' . check_plain($json) . '</pre>';
    }
    return NULL;
  }

  /**
   * Overrides views_plugin_display_page::render().
   *
   * @return string
   */
  public function render() {
    return $this->view->style_plugin->render();
  }

}
