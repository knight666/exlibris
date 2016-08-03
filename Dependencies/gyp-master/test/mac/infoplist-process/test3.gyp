# Copyright (c) 2011 Google Inc. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
{
  'targets': [
    {
      'target_name': 'test_app',
      'product_name': 'Test App',
      'type': 'executable',
      'mac_bundle': 1,
      'sources': [
        'main.c',
      ],
      'configurations': {
        'Three': {
        },
      },
      'xcode_settings': {
        'INFOPLIST_FILE': 'Info.plist',
        'INFOPLIST_PREPROCESS': 'NO',
        'INFOPLIST_PREPROCESSOR_DEFINITIONS': 'PROCESSED_KEY1=Foo',
      },
    },
  ],
}
