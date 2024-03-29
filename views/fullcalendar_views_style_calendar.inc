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
   *
   * @return array<mixed>
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
   *
   * @param array<mixed> $form
   * @param array<mixed> $form_state
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $date_fields = $this->getDateFieldCandidates();
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
      '#options' => array('' => t('None (empty calendar)')) + $date_fields,
      '#default_value' => $mappings['date'],
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
   *
   * @param array<mixed> $form
   * @param array<mixed> $form_state
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
    else {
      // Reset empty string to empty array.
      $form_state['values']['style_options']['calendar_setup']['add_event_sources'] = array();
    }
  }

  /**
   * Overrides views_plugin_style::options_submit().
   *
   * Color input element workaround to reset colors.
   *
   * @param array<mixed> $form
   * @param array<mixed> $form_state
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
   *
   * @return null|string
   */
  public function render() {
    // Do nothing on layout admin pages containing fullcalendar_views blocks.
    $path = current_path();
    if (strpos($path, 'admin/structure/layouts/manage') === 0) {
      return NULL;
    }

    global $language;
    $view = $this->view;
    $field = $view->field;
    $options = $this->options;

    $events = array();
    if (!empty($options['field_mapping']['date'])) {
      // Render the fields with all token replacements and overrides.
      $this->render_fields($view->result);
      foreach ($view->result as $index => $row) {
        $events[$index] = $this->renderRow($view, $index);
      }
    }

    $settings = $this->buildCalendarSettings();
    // Only needed if tooltips are enabled.
    if ($options['field_mapping']['tooltip']) {
      backdrop_add_library('system', 'ui.tooltip');
      $settings['hasTooltips'] = TRUE;
    }

    // The ajax preview breaks when adding those in live preview.
    // @see views_fullcalendar_preprocess_page
    if (empty($this->view->live_preview)) {
      $module_path = backdrop_get_path('module', 'fullcalendar_views');
      backdrop_add_library('fullcalendar_lib', 'fullcalendar');
      backdrop_add_js($module_path . '/js/fullcalendar-views.js');
      backdrop_add_css($module_path . '/css/fullcalendar-views.css');
    }

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
        if (!empty($field->options['group_rows'])) {
          $field_info = field_info_field($field_name);
          if ($field_info['cardinality'] > 1) {
            $message = t('The "Multiple field setting" for the %field field is currently to display all in one row, which is not recommended.', array(
              '%field' => $field_name,
            ));
            backdrop_set_message($message, 'warning');
          }
        }
      }
    }

    return '<div id="' . $id . '" class="fullcalendar-view"></div>';
  }

  /**
   * Prepare row output for FullCalendar.
   *
   * @param view $view
   * @param int $row_index
   *
   * @return array<mixed>
   */
  public function renderRow($view, $row_index) {
    $mappings = $this->options['field_mapping'];
    $rendered_fields = array();

    foreach ($mappings as $target => $view_field) {
      // If an optional date field is empty, we can't parse it. Skip to prevent
      // notices.
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
          fullcalendar_views_parse_date_fields($raw_values, $type, $date_only, $rendered_fields);
        }
        // Anything directly supported by views_handler_field_date.
        else {
          $rendered_fields['start'] = $this->get_field_value($row_index, $view_field) * 1000;
        }
      }
      else {
        // Title, url and tooltip are optional.
        if (!empty($this->rendered_fields[$row_index][$view_field])) {
          $rendered_field = $this->rendered_fields[$row_index][$view_field];
          if ($target == 'className') {
            $rendered_field = fullcalendar_views_sanitize_css($rendered_field);
          }
          // Prevent double encoding of special characters like "&".
          elseif ($target == 'title') {
            $rendered_field = strip_tags(decode_entities($rendered_field));
          }
          $rendered_fields[$target] = $rendered_field;
        }
      }
    }

    return $rendered_fields;
  }

  /**
   * Filter the list of views fields down to only supported date-type fields.
   *
   * @return array<mixed>
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
        $supported = fullcalendar_views_supported_field_types();
        $field_type = $handlers[$alias]->field_info['type'];
        if (in_array($field_type, $supported)) {
          $field_candidates[$alias] = $label;
        }
      }
    }
    return $field_candidates;
  }

  /**
   * Helper function to create js settings for FC.
   *
   * @return array<mixed>
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
      $settings['slotMaxTime'] = str_pad((string) $hour, 2, '0', STR_PAD_LEFT) . ':00';
    }

    return $settings;
  }

  /**
   * Helper function to set the proper locale for FullCalendar.
   *
   * @return string
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
