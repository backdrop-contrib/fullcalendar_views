<?php
/**
 * @file
 * Definition of FullcalendarViewsStyleCalendar.
 */

define('FC_VIEWS_DEFAULT_BG_COLOR', '#5c88aa');
define('FC_VIEWS_DEFAULT_TEXT_COLOR', '#ffffff');

/**
 * Provides a views style to display dates in a FullCalendar.
 */
class FullcalendarViewsStyleCalendar extends views_plugin_style {

  /**
   * Overrides views_plugin_style::option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();

    $options['field_mapping'] = array(
      'default' => array(
        'date' => '',
        'title' => '',
        'url' => '',
        'tooltip' => '',
        'className' => '',
      ),
    );
    $options['calendar_setup'] = array(
      'default' => array(
        'types' => array(
          'dayGridMonth' => 'dayGridMonth',
          'timeGridWeek' => 'timeGridWeek',
          'timeGridDay' => 0,
        ),
        'weekNumbers' => FALSE,
        'browserLocale' => FALSE,
        'allDaySlot' => TRUE,
        'slotMinTime' => '',
        'slotMaxTime' => '',
        'event_bg_color' => FC_VIEWS_DEFAULT_BG_COLOR,
        'event_text_color' => FC_VIEWS_DEFAULT_TEXT_COLOR,
        'add_event_sources' => array(),
      ),
    );

    return $options;
  }

  /**
   * Overrides views_plugin_style::options_form().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $date_fields = $this->getDateFieldCandidates();
    if (empty($date_fields)) {
      backdrop_set_message(t('Fullcalendar views need a supported date field.'), 'warning', FALSE);
    }
    $mappings = $this->options['field_mapping'];
    $setup = $this->options['calendar_setup'];
    $all_fields = array('' => t('<none>')) + $this->display->handler->get_field_labels();

    $form['field_mapping'] = array(
      '#type' => 'container',
      '#prefix' => t('Map the fields you added to the view to FullCalendar event fields.'),
    );
    $form['field_mapping']['date'] = array(
      '#type' => 'select',
      '#title' => t('Date field'),
      '#options' => $date_fields,
      '#default_value' => $mappings['date'],
      '#required' => TRUE,
      '#description' => t('Supported types are core Date fields, Resource timeslots, Repeating dates, and all timestamps natively handled by views.'),
    );
    $form['field_mapping']['title'] = array(
      '#type' => 'select',
      '#title' => 'Title field',
      '#options' => $all_fields,
      '#default_value' => $mappings['title'],
      '#description' => t('The title right after the date displayed in the event area. Is not supposed to contain any markup.'),
    );
    $form['field_mapping']['url'] = array(
      '#type' => 'select',
      '#title' => 'URL field',
      '#options' => $all_fields,
      '#default_value' => $mappings['url'],
      '#description' => t('A path or external URL to go to when the event gets clicked.'),
    );
    $form['field_mapping']['tooltip'] = array(
      '#type' => 'select',
      '#title' => 'Tooltip field',
      '#options' => $all_fields,
      '#default_value' => $mappings['tooltip'],
      '#description' => t('Extended information displayed in a tooltip when hovering or focusing the event. Can contain markup.'),
    );
    $form['field_mapping']['className'] = array(
      '#type' => 'select',
      '#title' => t('CSS class field'),
      '#options' => $all_fields,
      '#default_value' => $mappings['className'],
      '#description' => t('Additional event CSS class based on fields, for example taxonomy term.'),
    );

    $form['calendar_setup'] = array(
      '#type' => 'fieldset',
      '#title' => t('Calendar options'),
    );
    $form['calendar_setup']['types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Available calendar types'),
      '#options' => array(
        'dayGridMonth' => t('Month'),
        'timeGridWeek' => t('Week'),
        'timeGridDay' => t('Day'),
      ),
      '#default_value' => $setup['types'],
      '#required' => TRUE,
    );
    $form['calendar_setup']['weekNumbers'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display week numbers'),
      '#default_value' => $setup['weekNumbers'],
      '#prefix' => '<p><strong>' . t('Display options') . '</strong></p>',
    );
    $form['calendar_setup']['browserLocale'] = array(
      '#type' => 'checkbox',
      '#title' => t('Follow browser locale'),
      '#default_value' => $setup['browserLocale'],
      '#description' => t('Let the calendar localize date lables based on the browser language instead of the page language.'),
    );
    $form['calendar_setup']['allDaySlot'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display the "all-day" slot'),
      '#default_value' => $setup['allDaySlot'],
      '#description' => t('Adds a slot at the top of the calendar to display all-day events.'),
    );
    $form['calendar_setup']['slotMinTime'] = array(
      '#type' => 'number',
      '#title' => t('First hour'),
      '#min' => 0,
      '#max' => 23,
      '#default_value' => $setup['slotMinTime'],
      '#description' => t('First hour to display in week or day view.'),
    );
    $form['calendar_setup']['slotMaxTime'] = array(
      '#type' => 'number',
      '#title' => t('Last hour'),
      '#min' => 1,
      '#max' => 23,
      '#default_value' => $setup['slotMaxTime'],
      '#description' => t('Last hour to display in week or day view.'),
    );
    $form['calendar_setup']['event_bg_color'] = array(
      '#type' => 'color',
      '#title' => t('Event background color'),
      '#default_value' => $setup['event_bg_color'],
    );
    $form['calendar_setup']['event_text_color'] = array(
      '#type' => 'color',
      '#title' => t('Event text color'),
      '#default_value' => $setup['event_text_color'],
      '#description' => t('Applies only to text on background color.'),
    );
    $form['calendar_setup']['event_colors_reset'] = array(
      '#type' => 'checkbox',
      '#title' => t('Reset colors to default'),
      '#default_value' => FALSE,
      '#description' => t('After form submission colors will get reset to initial values.'),
    );
    $form['calendar_setup']['add_event_sources'] = array(
      '#type' => 'textarea',
      '#title' => t('Additional event source URLs'),
      '#default_value' => implode("\n", $setup['add_event_sources']),
      '#resizable' => FALSE,
      '#rows' => 4,
      '#description' => t('FullCalendar can additionally parse json feeds from other sources. Add one URL per line.'),
    );
    $form['#attached']['css'][] = backdrop_get_path('module', 'fullcalendar_views') . '/css/fullcalendar-views-admin.css';
  }

  /**
   * Overrides views_plugin_style::options_validate().
   */
  public function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    if (!empty($form_state['values']['style_options']['calendar_setup']['add_event_sources'])) {
      $sources = array();
      $input = explode("\n", $form_state['values']['style_options']['calendar_setup']['add_event_sources']);
      foreach ($input as $line) {
        $line = trim($line);
        if (empty($line)) {
          continue;
        }
        // Check for valid external or internal url.
        if (valid_url($line, TRUE)) {
          $sources[] = $line;
          continue;
        }
        if (valid_url($line, FALSE)) {
          $sources[] = $line;
        }
      }
      $form_state['values']['style_options']['calendar_setup']['add_event_sources'] = $sources;
    }
  }

  /**
   * Overrides views_plugin_style::options_submit().
   *
   * Color input element workaround to reset colors.
   */
  public function options_submit(&$form, &$form_state) {
    if ($form_state['values']['style_options']['calendar_setup']['event_colors_reset']) {
      $form_state['values']['style_options']['calendar_setup']['event_bg_color'] = FC_VIEWS_DEFAULT_BG_COLOR;
      $form_state['values']['style_options']['calendar_setup']['event_text_color'] = FC_VIEWS_DEFAULT_TEXT_COLOR;
      unset($form_state['values']['style_options']['calendar_setup']['event_colors_reset']);
    }
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
    $date_fields = $this->getDateFieldCandidates();
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

    $events = array();
    // Render the fields with all token replacements and overrides.
    $this->render_fields($view->result);
    foreach ($view->result as $index => $row) {
      $events[$index] = $this->renderRow($view, $index);
    }

    $settings = $this->buildCalendarSettings();
    // Only needed if tooltips are enabled.
    if ($options['field_mapping']['tooltip']) {
      backdrop_add_library('system', 'ui.tooltip');
      $settings['hasTooltips'] = TRUE;
    }
    $module_path = backdrop_get_path('module', 'fullcalendar_views');
    backdrop_add_library('fullcalendar_lib', 'fullcalendar');
    backdrop_add_js($module_path . '/js/fullcalendar-views.js');
    backdrop_add_css($module_path . '/css/fullcalendar-views.css');

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

    // Add inline css variable.
    $style_var = ':root{';
    $style_var .= '--fc-event-bg-color:' . $options['calendar_setup']['event_bg_color'] . ';';
    $style_var .= '--fc-event-border-color:' . $options['calendar_setup']['event_bg_color'] . ';';
    $style_var .= '--fc-event-text-color:' . $options['calendar_setup']['event_text_color'] . ';';
    $style_var .= '}';
    backdrop_add_css($style_var, array(
      'type' => 'inline',
    ));

    if (!empty($view->live_preview)) {
      $field_name = $this->options['field_mapping']['date'];
      if (!empty($field_name)) {
        $field = $view->field[$field_name];
        if ($field->options['group_rows']) {
          $message = t('The "Multiple field setting" for the %field field is currently to display all in one row, which is not recommended.', array(
            '%field' => $field_name,
          ));
          backdrop_set_message($message, 'warning');
        }
      }
    }

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
          $date_only = FALSE;
          if (!empty($field->field_info['settings']['granularity']) && !$field->field_info['settings']['granularity']['hour']) {
            $date_only = TRUE;
          }
          self::parseDateFields($raw_values, $type, $date_only, $rendered_fields);
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
          $rendered_field = $this->rendered_fields[$row_index][$view_field];
          if ($target == 'className') {
            $output = '';
            $classes = explode(' ', $rendered_field);
            foreach ($classes as $class) {
              $output .= ' ' . backdrop_clean_css_identifier($class);
            }
            $rendered_field = trim($output);
          }
          $rendered_fields[$target] = $rendered_field;
        }
      }
    }

    return $rendered_fields;
  }

  /**
   * Filter the list of views fields down to only supported date-type fields.
   */
  protected function getDateFieldCandidates() {
    $all_fields = $this->display->handler->get_field_labels();
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
   * Parse the various supported date field types into values FC can handle.
   *
   * @param array $raw_values
   *   Raw views result values provided by get_field_value().
   * @param string $type
   *   The field type as defined in hook_field_info().
   * @param bool $date_only
   *   Whether this field should be handled without time.
   * @param array $rendered_fields
   *   Array of rendered field values, usually empty at this point.
   */
  public static function parseDateFields(array $raw_values, $type, $date_only, &$rendered_fields) {
    switch ($type) {
      case 'date':
      case 'datetime':
        $tz = $raw_values[0]['timezone_db'];
        $start = new BackdropDateTime($raw_values[0]['value'], $tz);
        if ($date_only) {
          $rendered_fields['start'] = $start->format('Y-m-d');
        }
        else {
          $rendered_fields['start'] = $start->format('U') * 1000;
        }
        if (!empty($raw_values[0]['value2'])) {
          $end = new BackdropDateTime($raw_values[0]['value2'], $tz);
          if ($date_only) {
            $rendered_fields['end'] = $end->format('Y-m-d');
          }
          else {
            $rendered_fields['end'] = $end->format('U') * 1000;
          }
        }
        break;

      case 'datestamp':
        if ($date_only) {
          $rendered_fields['start'] = format_date($raw_values[0]['value'], 'custom', 'Y-m-d');
        }
        else {
          $rendered_fields['start'] = $raw_values[0]['value'] * 1000;
        }
        if (!empty($raw_values[0]['value2'])) {
          if ($date_only) {
            $rendered_fields['end'] = format_date($raw_values[0]['value2'], 'custom', 'Y-m-d');
          }
          else {
            $rendered_fields['end'] = $raw_values[0]['value2'] * 1000;
          }
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

  /**
   * Helper function to create js settings for FC.
   */
  public function buildCalendarSettings() {
    $options = $this->options['calendar_setup'];

    $settings = array(
      'locale' => self::getLangcode(),
      'types' => implode(',', array_filter($options['types'])),
      'weekNumbers' => (bool) $options['weekNumbers'],
      'browserLocale' => (bool) $options['browserLocale'],
      'allDaySlot' => (bool) $options['allDaySlot'],
      'firstDay' => config_get('system.date', 'first_day'),
      'eventSources' => $options['add_event_sources'],
    );
    if (!empty($options['slotMinTime'])) {
      $settings['slotMinTime'] = str_pad($options['slotMinTime'], 2, '0', STR_PAD_LEFT) . ':00';
    }
    if (!empty($options['slotMaxTime'])) {
      // End display is exclusive.
      $hour = $options['slotMaxTime'] + 1;
      $settings['slotMaxTime'] = str_pad($hour, 2, '0', STR_PAD_LEFT) . ':00';
    }

    return $settings;
  }

  /**
   * Helper function to set the proper locale for FullCalendar.
   */
  public static function getLangcode() {
    global $language;
    $langcode = $language->langcode;
    $default_country = config_get('system.date', 'default_country');

    // Otherwise people in UK get the wrong date/month order in calendars.
    if ($default_country == 'GB' && $langcode == 'en') {
      return 'en-gb';
    }

    return $langcode;
  }

}
