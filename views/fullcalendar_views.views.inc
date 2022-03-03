<?php
/**
 * @file
 * Define views plugin for Fullcalendar Views.
 */

/**
 * Implements hook_views_plugins().
 */
function fullcalendar_views_views_plugins() {
  $path = backdrop_get_path('module', 'fullcalendar_views') . '/views';
  $data = array(
    'style' => array(
      'fullcalendar_view' => array(
        'title' => 'Fullcalendar NG',
        'handler' => 'FullcalendarViewsStyleCalendar',
        'type' => 'normal',
        'path' => $path,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'even empty' => TRUE,
      ),
    ),
  );
  return $data;
}
