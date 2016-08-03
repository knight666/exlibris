{
	'includes': [
		'../../Common.gypi',
	],
	'targets': [
		{
			'target_name': 'glm',
			'type': 'none',
			'sources': [
				'glm/glm.hpp',
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