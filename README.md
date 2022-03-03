# Fullcalendar Views NG

Views style plugin to render all sorts of date fields as events with FullCalendar
 5.x.

![Calendar screenshot](https://raw.githubusercontent.com/backdrop-contrib/fullcalendar_views/1.x-1.x/screenshots/calendar.png)

## Supported (date) field types

- Core date
- Core datetime
- Core datestamp
- [Repeating date](https://backdropcms.org/project/repeating_dates)
- [Resource timeslot](https://backdropcms.org/project/resource_timeslots)
- Anything supported by views_handler_field_date, like "node created"

If your favorite field type is missing, let us know.

## Installation

Install this module using the official Backdrop CMS
 [instructions](https://docs.backdropcms.org/documentation/extend-with-modules)

Create a view with a supported date field type, select the "Fullcalendar NG"
 display type and configure.

Important note: if it's a multi-value field, make sure that the "Display all values in the same
row" setting under "Multiple field settings" of that view field is **not checked**.

### Additional event sources

FullCalendar can parse (external) json feeds for additional events. See the official
 documentation about [EventSources](https://fullcalendar.io/docs/event-source-object)
 for more information.

## Dependencies

- The [Fullcalendar Library](https://backdropcms.org/project/fullcalendar_lib) module

## Issues

Bugs and feature requests should be reported in the
 [Issue Queue](https://github.com/backdrop-contrib/fullcalendar_views/issues).

## Current maintainers

* [Indigoxela](https://github.com/indigoxela)

## Similar modules

This module is inspired by, but in no way similar to
 [Fullcalendar](https://backdropcms.org/project/fullcalendar). It has pros and
 cons:

- *Pro:* It utilizes a current FullCalendar library
- *Pro:* It supports a lot more date (field) types
- *Pro:* Simpler and more lightweight (OK, that's maybe only relevant for
 maintainers)
- *Con:* It doesn't enable you to edit these field values via calendar, only display
 (that's by intention)
- *Con:* It has less settings (but also an easier configuration form)

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
