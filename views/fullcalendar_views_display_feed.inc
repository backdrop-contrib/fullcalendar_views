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
   */
  public function uses_breadcrumb() {
    return FALSE;
  }

  /**
   * Overrides views_plugin_display::get_style_type.
   */
  public function get_style_type() {
    return 'fullcalendar_feed';
  }

  /**
   * Overrides views_plugin_display_page::execute().
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
   */
  public function preview() {
    if (!empty($this->view->live_preview)) {
      $rendered = $this->view->render();
      return '<pre>' . check_plain(json_encode($rendered, JSON_PRETTY_PRINT)) . '</pre>';
    }
    // @todo check when this runs.
    return $this->view->render();
  }

  /**
   * Overrides views_plugin_display_page::render().
   */
  public function render() {
    return $this->view->style_plugin->render($this->view->result);
  }

}
