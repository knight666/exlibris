#!/usr/bin/env python

# Copyright (c) 2015 Google Inc. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""
Verifies builds are the same even with different PYTHONHASHSEEDs.
Tests needed_variables.
"""

import os
import sys
import TestGyp

test = TestGyp.TestGyp()
if test.format == 'ninja':
  os.environ["PYTHONHASHSEED"] = "1"
  test.run_gyp('needed-variables.gyp')
  base = open(test.built_file_path('test.ninja', subdir='obj')).read()

  for i in range(1,5):
    os.environ["PYTHONHASHSEED"] = str(i)
    test.run_gyp('needed-variables.gyp')
    contents = open(test.built_file_path('test.ninja', subdir='obj')).read()
    if base != contents:
      test.fail_test()

  del os.environ["PYTHONHASHSEED"]
  test.pass_test()
