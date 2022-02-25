<?php
/**
 * @file
 * Definition of FullcalendarViewsStyleCalendar.
 */

/**
 * Provides a views style to display dates in a FullCalendar.
 */
class FullcalendarViewsStyleCalendar extends views_plugin_style {

  /**
   * Overrides views_plugin_style::option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['field_mapping'] = array();
    $options['field_mapping']['date'] = NULL;
    $options['field_mapping']['title'] = NULL;
    $options['field_mapping']['url'] = NULL;
    $options['field_mapping']['tooltip'] = NULL;

    return $options;
  }

  /**
   * Overrides views_plugin_style::options_form().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = $this->options['field_mapping'];
    $views_fields = $this->display->handler->get_field_labels();
    $date_fields = self::getDateFieldCandidates($views_fields);
    $all_fields = array('' => t('<none>')) + $views_fields;
    if (empty($date_fields)) {
      backdrop_set_message(t('Fullcalendar views need a supported date field.'), 'warning', FALSE);
    }

    $form['field_mapping'] = array(
      '#type' => 'container',
      '#prefix' => t('Map the fields you added to the view to FullCalendar event fields.'),
    );
    $form['field_mapping']['date'] = array(
      '#type' => 'select',
      '#title' => t('Date field'),
      '#options' => $date_fields,
      '#default_value' => $options['date'],
      '#required' => TRUE,
      '#description' => t('Supported types are core Date fields, Resource timeslots, Repeating dates, and all timestamps natively handled by views.'),
    );
    $form['field_mapping']['title'] = array(
      '#type' => 'select',
      '#title' => 'Title field',
      '#options' => $all_fields,
      '#default_value' => $options['title'],
      '#description' => t('The title right after the date displayed in the event area. Is not supposed to contain any markup.'),
    );
    $form['field_mapping']['url'] = array(
      '#type' => 'select',
      '#title' => 'URL field',
      '#options' => $all_fields,
      '#default_value' => $options['url'],
      '#description' => t('A path or external URL to go to when the event gets clicked.'),
    );
    $form['field_mapping']['tooltip'] = array(
      '#type' => 'select',
      '#title' => 'Tooltip field',
      '#options' => $all_fields,
      '#default_value' => $options['tooltip'],
      '#description' => t('Extended information displayed in a tooltip when hovering or focusing the event. Can contain markup.'),
    );
  }

  /**
   * Overrides views_plugin_style::query().
   */
  public function query() {
    // Get rid of the pager.
    $pager = array(
      'type' => 'none',
      'options' => $this->display->handler->options['pager']['options'],
    );
    $this->display->handler->set_option('pager', $pager);
  }

  /**
   * Overrides views_plugin_style::render().
   */
  public function render() {
    // A calendar always needs dates.
    $all_fields = $this->display->handler->get_field_labels();
    $date_fields = self::getDateFieldCandidates($all_fields);
    if (empty($date_fields)) {
      // Show the message only in live preview, not on possibly public pages.
      if (!empty($this->view->live_preview)) {
        backdrop_set_message(t('No date field available.'), 'warning');
      }
      return;
    }

    global $language;
    $view = $this->view;
    $field = $view->field;
    $options = $this->options;

    $events = $settings = array();
    // Render the fields with all token replacements and overrides.
    $this->render_fields($view->result);
    foreach ($view->result as $index => $row) {
      $events[$index] = $this->renderRow($view, $index);
    }

    // Only needed if tooltips are enabled.
    if ($options['field_mapping']['tooltip']) {
      backdrop_add_library('system', 'ui.tooltip');
      $settings['hasTooltips'] = TRUE;
    }
    backdrop_add_library('resource_timeslots', 'fullcalendar');
    backdrop_add_js(backdrop_get_path('module', 'fullcalendar_views') . '/js/fullcalendar-views.js');

    $settings['locale'] = $language->langcode;// @todo en_GB!
    $id = $view->name . ucfirst($view->current_display);
    $setup = array(
      'fullcalendarViews' => array(
        $id => array(
          'events' => $events,
          'settings' => $settings,
        ),
      ),
    );
    backdrop_add_js($setup, 'setting');

    return '<div id="' . $id . '" class="fullcalendar-view"></div>';
  }

  /**
   * Prepare row output for FullCalendar.
   */
  public function renderRow($view, $row_index) {
    $mappings = $this->options['field_mapping'];
    $rendered_fields = array();

    foreach ($mappings as $target => $view_field) {
      // If an optional date field is empty, we can't display anything in a
      // calendar. Completely skip it.
      if ($target == 'date' && empty($this->rendered_fields[$row_index][$view_field])) {
        continue;
      }
      // Override dates for FullCalendar events to timestamps as msecs.
      if ($target == 'date') {
        $field = $view->field[$view_field];

        // Field API fields.
        if (isset($field->field_info)) {
          $raw_values = $this->get_field_value($row_index, $view_field);
          $type = $field->field_info['type'];
          self::parseDateFields($raw_values, $type, $rendered_fields);
        }
        // Anything directly supported by views_handler_field_date.
        else {
          $rendered_fields['start'] = $this->get_field_value($row_index, $view_field) * 1000;
        }
      }
      else {
        // Title, url and tooltip stay unchanged. Note that the title seems to
        // get sanitized by FullCalendar. These fields are optional.
        if (!empty($this->rendered_fields[$row_index][$view_field])) {
          $rendered_fields[$target] = $this->rendered_fields[$row_index][$view_field];
        }
      }
    }

    return $rendered_fields;
  }

  /**
   * Filter the list of views fields down to only supported date-type fields.
   */
  protected function getDateFieldCandidates(array $all_fields) {
    $handlers = $this->display->handler->get_handlers('field');
    $field_candidates = array();

    foreach ($all_fields as $alias => $label) {
      $handler_class = get_class($handlers[$alias]);
      // Simple timestamp fields.
      if ($handler_class == 'views_handler_field_date') {
        $field_candidates[$alias] = $label;
      }
      elseif ($handler_class == 'views_handler_field_field') {
        // Core date fields, other date fields.
        $supported = array(
          'date',
          'datetime',
          'datestamp',
          'repeating_date',
          'resource_timeslot',
        );
        $field_type = $handlers[$alias]->field_info['type'];
        if (in_array($field_type, $supported)) {
          $field_candidates[$alias] = $label;
        }
      }
    }
    return $field_candidates;
  }

  /**
   * Parse the various supported date field types into timestamps as msecs.
   *
   * @param array $raw_values
   *   Raw views result values provided by get_field_value().
   * @param string $type
   *   The field type as defined in hook_field_info().
   * @param array $rendered_fields
   *   Array of rendered field values, usually empty at this point.
   */
  protected function parseDateFields(array $raw_values, $type, &$rendered_fields) {
    switch ($type) {
      // @todo Can the creepy "time granularity" of core Date fields be a
      // problem, when converting to timestamps?
      case 'date':
      case 'datetime':
        $tz = $raw_values[0]['timezone_db'];
        $start = new BackdropDateTime($raw_values[0]['value'], $tz);
        $rendered_fields['start'] = $start->format('U') * 1000;
        if (!empty($raw_values[0]['value2'])) {
          $end = new BackdropDateTime($raw_values[0]['value2'], $tz);
          $rendered_fields['end'] = $end->format('U') * 1000;
        }
        break;

      case 'datestamp':
        $rendered_fields['start'] = $raw_values[0]['value'] * 1000;
        if (!empty($raw_values[0]['value2'])) {
          $rendered_fields['end'] = $raw_values[0]['value2'] * 1000;
        }
        break;

      case 'repeating_date':
        $rendered_fields['start'] = $raw_values[0]['dtstart'] * 1000;
        $rendered_fields['end'] = $raw_values[0]['dtend'] * 1000;
        break;

      case 'resource_timeslot':
        $rendered_fields['start'] = $raw_values[0]['start'] * 1000;
        $rendered_fields['end'] = $raw_values[0]['end'] * 1000;
        break;
    }
  }

}
