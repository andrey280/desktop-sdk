// Copyright (c) 2023 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool. If making changes by
// hand only do so within the body of existing method and function
// implementations. See the translator.README.txt file in the tools directory
// for more information.
//
// $hash=ace627f34f6c16512fb0d7a9a4ebb96e9c00c78d$
//

#ifndef CEF_LIBCEF_DLL_CTOCPP_RESOURCE_SKIP_CALLBACK_CTOCPP_H_
#define CEF_LIBCEF_DLL_CTOCPP_RESOURCE_SKIP_CALLBACK_CTOCPP_H_
#pragma once

#if !defined(WRAPPING_CEF_SHARED)
#error This file can be included wrapper-side only
#endif

#include "include/capi/cef_resource_handler_capi.h"
#include "include/cef_resource_handler.h"
#include "libcef_dll/ctocpp/ctocpp_ref_counted.h"

// Wrap a C structure with a C++ class.
// This class may be instantiated and accessed wrapper-side only.
class CefResourceSkipCallbackCToCpp
    : public CefCToCppRefCounted<CefResourceSkipCallbackCToCpp,
                                 CefResourceSkipCallback,
                                 cef_resource_skip_callback_t> {
 public:
  CefResourceSkipCallbackCToCpp();
  virtual ~CefResourceSkipCallbackCToCpp();

  // CefResourceSkipCallback methods.
  void Continue(int64 bytes_skipped) override;
};

#endif  // CEF_LIBCEF_DLL_CTOCPP_RESOURCE_SKIP_CALLBACK_CTOCPP_H_
