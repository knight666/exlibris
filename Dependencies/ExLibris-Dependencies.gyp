{
	'includes': [
		'../Common.gypi',
	],
	'targets': [
		{
			'target_name': 'freetype',
			'type': 'static_library',
			'variables': {
				'project_dir': 'freetype-2.5.0.1',
			},
			'defines': [
				'FT_DEBUG_LEVEL_ERROR',
				'FT_DEBUG_LEVEL_TRACE',
				'FT2_BUILD_LIBRARY',
			],
			'defines!': [
				'DEBUG', # Causes linker errors due to external trace API
			],
			'include_dirs': [
				'<(project_dir)/include',
			],
			'sources': [
				'<(project_dir)/builds/win32/ftdebug.c',
				'<(project_dir)/include/freetype/config/ftconfig.h',
				'<(project_dir)/include/freetype/config/ftheader.h',
				'<(project_dir)/include/freetype/config/ftmodule.h',
				'<(project_dir)/include/freetype/config/ftoption.h',
				'<(project_dir)/include/freetype/config/ftstdlib.h',
				'<(project_dir)/include/freetype/freetype.h',
				'<(project_dir)/include/ft2build.h',
				'<(project_dir)/src/autofit/autofit.c',
				'<(project_dir)/src/base/ftbase.c',
				'<(project_dir)/src/base/ftbbox.c',
				'<(project_dir)/src/base/ftbitmap.c',
				'<(project_dir)/src/base/ftfstype.c',
				'<(project_dir)/src/base/ftgasp.c',
				'<(project_dir)/src/base/ftglyph.c',
				'<(project_dir)/src/base/ftgxval.c',
				'<(project_dir)/src/base/ftinit.c',
				'<(project_dir)/src/base/ftlcdfil.c',
				'<(project_dir)/src/base/ftmm.c',
				'<(project_dir)/src/base/ftotval.c',
				'<(project_dir)/src/base/ftpatent.c',
				'<(project_dir)/src/base/ftpfr.c',
				'<(project_dir)/src/base/ftstroke.c',
				'<(project_dir)/src/base/ftsynth.c',
				'<(project_dir)/src/base/ftsystem.c',
				'<(project_dir)/src/base/fttype1.c',
				'<(project_dir)/src/base/ftwinfnt.c',
				'<(project_dir)/src/base/ftxf86.c',
				'<(project_dir)/src/bdf/bdf.c',
				'<(project_dir)/src/cache/ftcache.c',
				'<(project_dir)/src/cff/cff.c',
				'<(project_dir)/src/cid/type1cid.c',
				'<(project_dir)/src/gzip/ftgzip.c',
				'<(project_dir)/src/lzw/ftlzw.c',
				'<(project_dir)/src/pcf/pcf.c',
				'<(project_dir)/src/pfr/pfr.c',
				'<(project_dir)/src/psaux/psaux.c',
				'<(project_dir)/src/pshinter/pshinter.c',
				'<(project_dir)/src/psnames/psmodule.c',
				'<(project_dir)/src/raster/raster.c',
				'<(project_dir)/src/sfnt/sfnt.c',
				'<(project_dir)/src/smooth/smooth.c',
				'<(project_dir)/src/truetype/truetype.c',
				'<(project_dir)/src/type1/type1.c',
				'<(project_dir)/src/type42/type42.c',
				'<(project_dir)/src/winfonts/winfnt.c',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)/include',
				],
			},
		},

		{
			'target_name': 'glew',
			'type': 'static_library',
			'variables': {
				'project_dir': 'glew-1.10.0',
			},
			'defines': [
				'GLEW_STATIC',
			],
			'include_dirs': [
				'<(project_dir)/include',
			],
			'sources': [
				'<(project_dir)/include/GL/glew.h',
				'<(project_dir)/include/GL/wglew.h',
				'<(project_dir)/src/glew.c',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)/include',
				],
				'defines': [
					'GLEW_STATIC',
				],
				'link_settings': {
					'libraries': [
						'-lOpenGL32',
					],
				},
			},
		},

		{
			'target_name': 'glfw',
			'type': 'static_library',
			'variables': {
				'project_dir': 'glfw-3.0.2',
			},
			'sources': [
				'<(project_dir)/include/GLFW/glfw3.h',
				'<(project_dir)/include/GLFW/glfw3native.h',
				'<(project_dir)/src/config.h',
				'<(project_dir)/src/clipboard.c',
				'<(project_dir)/src/context.c',
				'<(project_dir)/src/gamma.c',
				'<(project_dir)/src/init.c',
				'<(project_dir)/src/input.c',
				'<(project_dir)/src/internal.h',
				'<(project_dir)/src/joystick.c',
				'<(project_dir)/src/monitor.c',
				'<(project_dir)/src/time.c',
				'<(project_dir)/src/wgl_platform.h',
				'<(project_dir)/src/window.c',
				'<(project_dir)/src/win32_clipboard.c',
				'<(project_dir)/src/win32_gamma.c',
				'<(project_dir)/src/win32_init.c',
				'<(project_dir)/src/win32_joystick.c',
				'<(project_dir)/src/win32_monitor.c',
				'<(project_dir)/src/win32_platform.h',
				'<(project_dir)/src/win32_time.c',
				'<(project_dir)/src/win32_window.c',
				'<(project_dir)/src/wgl_context.c',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)/include',
				],
				'conditions': [
					['OS=="win"', {
						'direct_dependent_settings': {
							'libraries': [
								'-lkernel32',
								'-luser32',
								'-lgdi32',
								'-lshell32',
							],
						},
					}],
				],
			},
		},

		{
			'target_name': 'glm',
			'type': 'none',
			'variables': {
				'project_dir': 'glm-0.9.4.5',
			},
			'sources': [
				'<(project_dir)/glm/glm.hpp',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)',
				],
			},
		},

		{
			'target_name': 'gtest',
			'type': 'static_library',
			'variables': {
				'project_dir': 'gtest-1.7.0-rc1',
			},
			'defines': [
				'GTEST_HAS_PTHREAD=0',
			],
			'include_dirs': [
				'<(project_dir)',
				'<(project_dir)/include',
			],
			'sources': [
				'<(project_dir)/src/gtest-all.cc',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)/include',
				],
				'defines': [
					'GTEST_HAS_PTHREAD=0',
				],
				'conditions': [
					['OS=="win"', {
						'defines': [
							'GTEST_HAS_TR1_TUPLE=0',
						],
					}],
				],
			},
			'conditions': [
				['OS=="win"', {
					'defines': [
						'GTEST_HAS_TR1_TUPLE=0',
					],
				}],
			],
		},

		{
			'target_name': 'poly2tri',
			'type': 'static_library',
			'variables': {
				'project_dir': 'poly2tri-0.3.3',
			},
			'sources': [
				'<(project_dir)/poly2tri/common/shapes.cc',
				'<(project_dir)/poly2tri/common/shapes.h',
				'<(project_dir)/poly2tri/common/utils.h',
				'<(project_dir)/poly2tri/sweep/advancing_front.cc',
				'<(project_dir)/poly2tri/sweep/advancing_front.h',
				'<(project_dir)/poly2tri/sweep/cdt.cc',
				'<(project_dir)/poly2tri/sweep/cdt.h',
				'<(project_dir)/poly2tri/sweep/sweep.cc',
				'<(project_dir)/poly2tri/sweep/sweep.h',
				'<(project_dir)/poly2tri/sweep/sweep_context.cc',
				'<(project_dir)/poly2tri/sweep/sweep_context.h',
				'<(project_dir)/poly2tri/poly2tri.h',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)',
				],
			},
		},

		{
			'target_name': 'vld',
			'type': 'none',
			'variables': {
				'project_dir': 'VisualLeakDetector-2.3',
			},
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)/include',
				],
				'link_settings': {
					'libraries': [
						'<(project_dir)/lib/Win32/vld.lib'
					],
				},
			},
		},
	],
}