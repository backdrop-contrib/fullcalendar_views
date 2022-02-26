# Fullcalendar Views NG

Views style plugin to render all sorts of date fields as events with FullCalendar
 5.x.

Brand new, work in progress, testing is highly appreciated (not on production
 sites!).

## Installation

Install this module using the official Backdrop CMS
 [instructions](https://docs.backdropcms.org/documentation/extend-with-modules)

Create a view with a supported date field type, select the "Fullcalendar NG"
 display type and configure.

Important note: if it's a multi-value field, make sure that the "Display all values in the same
row" setting under "Multiple field settings" of that view field is **not checked**.

### Install from GitHub

To test the early dev version follow these steps:

- Click on the green "Code" button on the top right corner (expands a little
 dialog)
- Click on "Download ZIP"
- Extract that zip file locally - note that the directory has a version
 extension (1.x-1.x)
- Move that directory from "fullcalendar_views-1.x-1.x" to
 "fullcalendar_views"
- Upload (via FTP) or move locally to your /modules directory under your backdrop root
- Enable it as you normally would via UI on /admin/modules/list

Again: do not yet use on production sites. Too early.

## Supported (date) field types

- Core date
- Core datetime
- Core datestamp
- [Repeating date](https://backdropcms.org/project/repeating_dates)
- [Resource timeslot](https://backdropcms.org/project/resource_timeslots)
- Anything supported by views_handler_field_date, like "node created"

If your favorite field type is missing, let us know.

## Issues

Bugs and feature requests should be reported in the
 [Issue Queue](https://github.com/backdrop-contrib/fullcalendar_views/issues).

## Known Issues

Because of a silly developer decision ;-), this module currently depends on
 the Resource Timeslots module, as that module provides the library.

If there's enough interest in decoupling them, it could be solved by
 splitting the library to its own module (Fullcalendar Library) and let the
 modules depend on the library module.

## Current maintainers

* [Indigoxela](https://github.com/indigoxela)

## Similar modules

This module is inspired by, but in no way similar to
 [Fullcalendar](https://backdropcms.org/project/fullcalendar). It has pros and
 cons:

- *pro:* It utilizes a current FullCalendar library (currently provided by Resource
 Timeslots)
- *pro:* It supports a lot more date (field) types
- *pro:* Simpler and more lightweight (OK, that's maybe only relevant for
 maintainers)
- *con:* It doesn't enable you to edit these field values via calendar, only display
 (that's by intention)
- *con:* It has sparse settings (this will change)

## License

This project is GPL v2 software. See the LICENSE.txt file in this directory for complete text.
