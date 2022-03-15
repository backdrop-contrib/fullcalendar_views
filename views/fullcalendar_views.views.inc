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
    'display' => array(
      'fullcalendar_feed' => array(
        'title' => t('Fullcalendar Feed'),
        'help' => t('Display the view as a json feed for FullCalendar.'),
        'handler' => 'FullcalendarViewsDisplayFeed',
        'uses hook menu' => TRUE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => FALSE,
        'admin' => t('Fullcalendar Feed'),
      ),
    ),
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
      'fullcalendar_json' => array(
        'title' => 'Fullcalendar JSON',
        'handler' => 'FullcalendarViewsStyleJson',
        'type' => 'fullcalendar_feed',
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
