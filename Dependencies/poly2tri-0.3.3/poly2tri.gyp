{
	'includes': [
		'../../Common.gypi',
	],
	'targets': [
		{
			'target_name': 'poly2tri',
			'type': 'static_library',
			'sources': [
				'poly2tri/common/shapes.cc',
				'poly2tri/common/shapes.h',
				'poly2tri/common/utils.h',
				'poly2tri/sweep/advancing_front.cc',
				'poly2tri/sweep/advancing_front.h',
				'poly2tri/sweep/cdt.cc',
				'poly2tri/sweep/cdt.h',
				'poly2tri/sweep/sweep.cc',
				'poly2tri/sweep/sweep.h',
				'poly2tri/sweep/sweep_context.cc',
				'poly2tri/sweep/sweep_context.h',
				'poly2tri/poly2tri.h',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'.',
				],
			},
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
	],
}