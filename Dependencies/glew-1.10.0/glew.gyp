{
	'includes': [
		'../../Common.gypi',
	],
	'targets': [
		{
			'target_name': 'glew',
			'type': 'static_library',
			'defines': [
				'GLEW_STATIC',
			],
			'include_dirs': [
				'include',
			],
			'sources': [
				'include/GL/glew.h',
				'include/GL/wglew.h',
				'src/glew.c',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'include',
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
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
	],
}