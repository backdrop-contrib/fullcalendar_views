(function ($) {

  'use strict';

  var fullcalendarViews = fullcalendarViews || {};

  Backdrop.behaviors.fullcalendarViews = {
    attach: function (context, settings) {
      if (!settings.fullcalendarViews) {
        return;
      }
      $(settings.fullcalendarViews).each(function () {
        var id = Object.keys(this)[0];
        var calendarElem = document.getElementById(id);
        if (!calendarElem) {
          return;
        }
        var myevents = this[id].events;
        var mysettings = this[id].settings;

        var options = {
          eventSources: [ myevents ],
          height: 'auto',
          weekNumbers: mysettings.weekNumbers,
          allDaySlot: mysettings.allDaySlot,
          firstDay: mysettings.firstDay,
          eventDidMount: function(arg) {
            if (mysettings.hasTooltips === true) {
              // Different feeds may have tooltips or not.
              if (typeof arg.event.extendedProps.tooltip === 'undefined') {
                return;
              }
              arg.el.setAttribute('data-tooltip', arg.event.extendedProps.tooltip);
              $(arg.el).tooltip({
                position: {
                  my: 'left top+7',
                  at: 'left bottom',
                  collision: 'flipfit'
                },
                items: '[data-tooltip]',
                content: function() {
                  return $(this).attr('data-tooltip');
                },
                close: function(event, ui) {
                  ui.tooltip.hover(function() {
                    $(this).stop(true).fadeTo(200, 1);
                  },
                  function() {
                    $(this).fadeOut('200', function() {
                      $(this).remove();
                    });
                  });
                }
              });
            }
          }
        };
        if (typeof mysettings.browserLocale !== 'undefined' && mysettings.browserLocale) {
          options.locale = navigator.language || navigator.userLanguage;
        }
        else {
          options.locale = mysettings.locale;
        }
        if (typeof mysettings.slotMinTime !== 'undefined') {
          options.slotMinTime = mysettings.slotMinTime;
        }
        if (typeof mysettings.slotMaxTime !== 'undefined') {
          options.slotMaxTime = mysettings.slotMaxTime;
        }
        // We only need the buttons to switch, if there's more than one
        // enabled calendar type.
        if (mysettings.types.indexOf(',') > -1) {
          options.headerToolbar = {
            left: mysettings.types,
            center: 'title',
            right: 'prev,next'
          };
        }
        else {
          options.headerToolbar = {
            left: 'title',
            right: 'prev,next'
          };
          options.initialView = mysettings.types;
        }

        // Let modules or themes override options by adding a js setting
        // 'fullcalendar_views_custom' to the page.
        $.extend(options, settings.fullcalendar_views_custom);

        var calendar = new FullCalendar.Calendar(calendarElem, options);
        calendar.render();

        // Additional event sources as json feeds.
        if (mysettings.eventSources.length) {
          for (let i = 0; i < mysettings.eventSources.length; i++) {
            calendar.addEventSource(mysettings.eventSources[i]);
          }
        }
      });
    }
  };
})(jQuery);
