// Copyright (c) 2017 Marshall A. Greenblatt. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the name Chromium Embedded
// Framework nor the names of its contributors may be used to endorse
// or promote products derived from this software without specific prior
// written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
// $hash=fe38af40efe76e5d6babf0644d7d4af679a4ca6f$
//

#ifndef CEF_INCLUDE_CAPI_CEF_LIFE_SPAN_HANDLER_CAPI_H_
#define CEF_INCLUDE_CAPI_CEF_LIFE_SPAN_HANDLER_CAPI_H_
#pragma once

#include "include/capi/cef_base_capi.h"
#include "include/capi/cef_browser_capi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _cef_client_t;

///
// Implement this structure to handle events related to browser life span. The
// functions of this structure will be called on the UI thread unless otherwise
// indicated.
///
typedef struct _cef_life_span_handler_t {
  ///
  // Base structure.
  ///
  cef_base_ref_counted_t base;

  ///
  // Called on the UI thread before a new popup browser is created. The
  // |browser| and |frame| values represent the source of the popup request. The
  // |target_url| and |target_frame_name| values indicate where the popup
  // browser should navigate and may be NULL if not specified with the request.
  // The |target_disposition| value indicates where the user intended to open
  // the popup (e.g. current tab, new tab, etc). The |user_gesture| value will
  // be true (1) if the popup was opened via explicit user gesture (e.g.
  // clicking a link) or false (0) if the popup opened automatically (e.g. via
  // the DomContentLoaded event). The |popupFeatures| structure contains
  // additional information about the requested popup window. To allow creation
  // of the popup browser optionally modify |windowInfo|, |client|, |settings|
  // and |no_javascript_access| and return false (0). To cancel creation of the
  // popup browser return true (1). The |client| and |settings| values will
  // default to the source browser's values. If the |no_javascript_access| value
  // is set to false (0) the new browser will not be scriptable and may not be
  // hosted in the same renderer process as the source browser. Any
  // modifications to |windowInfo| will be ignored if the parent browser is
  // wrapped in a cef_browser_view_t. Popup browser creation will be canceled if
  // the parent browser is destroyed before the popup browser creation completes
  // (indicated by a call to OnAfterCreated for the popup browser).
  ///
  int(CEF_CALLBACK* on_before_popup)(
      struct _cef_life_span_handler_t* self,
      struct _cef_browser_t* browser,
      struct _cef_frame_t* frame,
      const cef_string_t* target_url,
      const cef_string_t* target_frame_name,
      cef_window_open_disposition_t target_disposition,
      int user_gesture,
      const struct _cef_popup_features_t* popupFeatures,
      struct _cef_window_info_t* windowInfo,
      struct _cef_client_t** client,
      struct _cef_browser_settings_t* settings,
      int* no_javascript_access);

  ///
  // Called after a new browser is created. This callback will be the first
  // notification that references |browser|.
  ///
  void(CEF_CALLBACK* on_after_created)(struct _cef_life_span_handler_t* self,
                                       struct _cef_browser_t* browser);

  ///
  // Called when a browser has recieved a request to close. This may result
  // directly from a call to cef_browser_host_t::*close_browser() or indirectly
  // if the browser is parented to a top-level window created by CEF and the
  // user attempts to close that window (by clicking the 'X', for example). The
  // do_close() function will be called after the JavaScript 'onunload' event
  // has been fired.
  //
  // An application should handle top-level owner window close notifications by
  // calling cef_browser_host_t::try_close_browser() or
  // cef_browser_host_t::CloseBrowser(false (0)) instead of allowing the window
  // to close immediately (see the examples below). This gives CEF an
  // opportunity to process the 'onbeforeunload' event and optionally cancel the
  // close before do_close() is called.
  //
  // When windowed rendering is enabled CEF will internally create a window or
  // view to host the browser. In that case returning false (0) from do_close()
  // will send the standard close notification to the browser's top-level owner
  // window (e.g. WM_CLOSE on Windows, performClose: on OS X, "delete_event" on
  // Linux or cef_window_delegate_t::can_close() callback from Views). If the
  // browser's host window/view has already been destroyed (via view hierarchy
  // tear-down, for example) then do_close() will not be called for that browser
  // since is no longer possible to cancel the close.
  //
  // When windowed rendering is disabled returning false (0) from do_close()
  // will cause the browser object to be destroyed immediately.
  //
  // If the browser's top-level owner window requires a non-standard close
  // notification then send that notification from do_close() and return true
  // (1).
  //
  // The cef_life_span_handler_t::on_before_close() function will be called
  // after do_close() (if do_close() is called) and immediately before the
  // browser object is destroyed. The application should only exit after
  // on_before_close() has been called for all existing browsers.
  //
  // The below examples describe what should happen during window close when the
  // browser is parented to an application-provided top-level window.
  //
  // Example 1: Using cef_browser_host_t::try_close_browser(). This is
  // recommended for clients using standard close handling and windows created
  // on the browser process UI thread. 1.  User clicks the window close button
  // which sends a close notification to
  //     the application's top-level window.
  // 2.  Application's top-level window receives the close notification and
  //     calls TryCloseBrowser() (which internally calls CloseBrowser(false)).
  //     TryCloseBrowser() returns false so the client cancels the window close.
  // 3.  JavaScript 'onbeforeunload' handler executes and shows the close
  //     confirmation dialog (which can be overridden via
  //     CefJSDialogHandler::OnBeforeUnloadDialog()).
  // 4.  User approves the close. 5.  JavaScript 'onunload' handler executes. 6.
  // CEF sends a close notification to the application's top-level window
  //     (because DoClose() returned false by default).
  // 7.  Application's top-level window receives the close notification and
  //     calls TryCloseBrowser(). TryCloseBrowser() returns true so the client
  //     allows the window close.
  // 8.  Application's top-level window is destroyed. 9.  Application's
  // on_before_close() handler is called and the browser object
  //     is destroyed.
  // 10. Application exits by calling cef_quit_message_loop() if no other
  // browsers
  //     exist.
  //
  // Example 2: Using cef_browser_host_t::CloseBrowser(false (0)) and
  // implementing the do_close() callback. This is recommended for clients using
  // non-standard close handling or windows that were not created on the browser
  // process UI thread. 1.  User clicks the window close button which sends a
  // close notification to
  //     the application's top-level window.
  // 2.  Application's top-level window receives the close notification and:
  //     A. Calls CefBrowserHost::CloseBrowser(false).
  //     B. Cancels the window close.
  // 3.  JavaScript 'onbeforeunload' handler executes and shows the close
  //     confirmation dialog (which can be overridden via
  //     CefJSDialogHandler::OnBeforeUnloadDialog()).
  // 4.  User approves the close. 5.  JavaScript 'onunload' handler executes. 6.
  // Application's do_close() handler is called. Application will:
  //     A. Set a flag to indicate that the next close attempt will be allowed.
  //     B. Return false.
  // 7.  CEF sends an close notification to the application's top-level window.
  // 8.  Application's top-level window receives the close notification and
  //     allows the window to close based on the flag from #6B.
  // 9.  Application's top-level window is destroyed. 10. Application's
  // on_before_close() handler is called and the browser object
  //     is destroyed.
  // 11. Application exits by calling cef_quit_message_loop() if no other
  // browsers
  //     exist.
  ///
  int(CEF_CALLBACK* do_close)(struct _cef_life_span_handler_t* self,
                              struct _cef_browser_t* browser);

  ///
  // Called just before a browser is destroyed. Release all references to the
  // browser object and do not attempt to execute any functions on the browser
  // object after this callback returns. This callback will be the last
  // notification that references |browser|. See do_close() documentation for
  // additional usage information.
  ///
  void(CEF_CALLBACK* on_before_close)(struct _cef_life_span_handler_t* self,
                                      struct _cef_browser_t* browser);
} cef_life_span_handler_t;

#ifdef __cplusplus
}
#endif

#endif  // CEF_INCLUDE_CAPI_CEF_LIFE_SPAN_HANDLER_CAPI_H_
