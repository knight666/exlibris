{
	'includes': [
		'../../Common.gypi',
	],
	'targets': [
		{
			'target_name': 'vld',
			'type': 'none',
			'direct_dependent_settings': {
				'include_dirs': [
					'include',
				],
				'link_settings': {
					'libraries': [
						'lib/Win32/vld.lib'
					],
				},
			},
		},
	],
}