{
	'includes': [
		'Common.gypi',
	],
	'targets': [
		{
			'target_name': 'Framework',
			'type': 'static_library',
			'dependencies': [
				'Dependencies/glew-1.10.0/glew.gyp:glew',
				'Dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Dependencies/VisualLeakDetector-2.3/vld.gyp:vld',
				'ExLibris.gyp:ExLibris',
			],
			'sources': [
				'Projects/FrameworkGL/Application.cpp',
				'Projects/FrameworkGL/Application.h',
				'Projects/FrameworkGL/DebugHelper.cpp',
				'Projects/FrameworkGL/DebugHelper.h',
				'Projects/FrameworkGL/FrameworkGL.PCH.cpp',
				'Projects/FrameworkGL/FrameworkGL.PCH.h',
				'Projects/FrameworkGL/IRenderCommand.h',
				'Projects/FrameworkGL/MeshOpenGL.cpp',
				'Projects/FrameworkGL/MeshOpenGL.h',
				'Projects/FrameworkGL/RenderCommandLines.cpp',
				'Projects/FrameworkGL/RenderCommandLines.h',
				'Projects/FrameworkGL/RenderCommandText.cpp',
				'Projects/FrameworkGL/RenderCommandText.h',
				'Projects/FrameworkGL/ShaderProgram.cpp',
				'Projects/FrameworkGL/ShaderProgram.h',
				'Projects/FrameworkGL/ShaderSource.cpp',
				'Projects/FrameworkGL/ShaderSource.h',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'Projects/FrameworkGL',
				],
			},
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
		{
			'target_name': 'Example',
			'type': 'executable',
			'dependencies': [
				'Dependencies/glew-1.10.0/glew.gyp:glew',
				'Dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Framework',
				'ExLibris.gyp:ExLibris',
			],
			'sources': [
				'Projects/Example/Main.cpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
	],
}