(function ($) {

  'use strict';

  var fullcalendarViews = fullcalendarViews || {};

  Backdrop.behaviors.fullcalendarViews = {
    attach: function (context, settings) {
      $(settings.fullcalendarViews).each(function () {

        var myevents = Object.values(this)[0].events;
        var mysettings = Object.values(this)[0].settings;
        var id = Object.keys(this)[0];
        var calendarElem = document.getElementById(id);

        var options = {
          events: myevents,
          height: 'auto',
          locale: mysettings.locale,
          headerToolbar: {
            left: mysettings.types,
            center: 'title',
            right: 'prev,next'
          },
          weekNumbers: mysettings.weekNumbers,
          allDaySlot: mysettings.allDaySlot,
          firstDay: mysettings.firstDay,
          eventDidMount: function(arg) {
            if (mysettings.hasTooltips === true) {
              arg.el.setAttribute('data-tooltip', arg.event.extendedProps.tooltip);
              $(arg.el).tooltip({
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
        // @todo let extend.
        var calendar = new FullCalendar.Calendar(calendarElem, options);
        calendar.render();
      });
    }
  };
})(jQuery);
