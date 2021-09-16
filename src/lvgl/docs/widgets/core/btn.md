```eval_rst
.. include:: /header.rst 
:github_url: |github_link_base|/widgets/btn.md
```
# Button (lv_btn)

## Overview

Buttons have no new features compared to the [Base object](/widgets/obj). They are usuful for semantic purposes and have slightly different default settings.

Buttons, by default, differ from Base object in the following ways:
- Not scrollable
- Added to the default group
- Default height and width set to `LV_SIZE_CONTENT`

## Parts and Styles
- `LV_PART_MAIN` The background of the button. Uses the typical background style properties.

## Usage

There are no new features compared to [Base object](/widgets/obj).

## Events
- `LV_EVENT_VALUE_CHANGED` when the `LV_OBJ_FLAG_CHECKABLE` flag is enabled and the object is clicked. The event happens on transition to/from the checked state.


Learn more about [Events](/overview/event).

## Keys
If `LV_OBJ_FLAG_CHECKABLE` is enabled `LV_KEY_RIGHT` and `LV_KEY_UP` make the object checked, and `LV_KEY_LEFT` and `LV_KEY_DOWN` make it unchecked.

Note that the state of `LV_KEY_ENTER` is translated to `LV_EVENT_PRESSED/PRESSING/RELEASED` etc.

Learn more about [Keys](/overview/indev).

## Example
```eval_rst

.. include:: ../../../examples/widgets/btn/index.rst

```

## API

```eval_rst

.. doxygenfile:: lv_btn.h
  :project: lvgl

```
