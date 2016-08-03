{
	'includes': [
		'../../Common.gypi',
	],
	'targets': [
		{
			'target_name': 'gtest',
			'type': 'static_library',
			'defines': [
				'GTEST_HAS_PTHREAD=0',
			],
			'include_dirs': [
				'.',
				'include',
			],
			'sources': [
				'src/gtest-all.cc',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'include',
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
	],
}