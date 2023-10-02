<?php
/**
 * @file
 * Definition of FullcalendarViewsStyleJson.
 */

/**
 * Views style to render date fields for FullCalendar.
 */
class FullcalendarViewsStyleJson extends views_plugin_style {

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

    $mappings = $this->options['field_mapping'];
    $date_fields = $this->getDateFieldCandidates();
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
  }

  /**
   * Overrides views_plugin_style::render().
   *
   * @return array<mixed>
   */
  public function render() {
    $view = $this->view;
    $options = $this->options;
    $events = array();

    // Render the fields with all token replacements and overrides.
    $this->render_fields($view->result);
    foreach ($view->result as $index => $row) {
      $events[$index] = $this->renderRow($view, $index);
    }
    return $events;
  }

  /**
   * Custom callback to override row style.
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

}
