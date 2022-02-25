# Fullcalendar Views NG

Views style plugin to render all sorts of date fields as events with FullCalendar
 5.x.

Brand new, work in progress...

## Installation

Install this module using the official Backdrop CMS
 [instructions](https://docs.backdropcms.org/documentation/extend-with-modules)

Create a view with a supported date field type, select the "Fullcalendar NG"
 display type and configure.

## Supported (date) field types

- Core date
- Core datetime
- Core datestamp
- Repeating date
- Resource timeslot
- Anything supported by views_handler_field_date, like "node created"

If your favorite field type is missing, let us know.

## Issues

Bugs and feature requests should be reported in the
 [Issue Queue](https://github.com/backdrop-contrib/fullcalendar_views/issues).

## Current maintainers

* [Indigoxela](https://github.com/indigoxela)

## Similar modules

This module is inspired by, but in no way similar to Fullcalendar. It has
pros and cons:

- pro: It utilizes a current FullCalendar library (currently provided by Resource
 Timeslots)
- pro: It supports a lot more date (field) types
- con: It does not enable you to edit these field values via calendar, only display
 (that's by intention)
- con: It has sparse settings (this will change)

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
