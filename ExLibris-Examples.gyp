{
	'includes': [
		'Common.gypi',
	],
	'targets': [
		{
			'target_name': 'Framework',
			'type': 'static_library',
			'variables': {
				'project_dir': 'Projects/FrameworkGL',
			},
			'dependencies': [
				'Dependencies/ExLibris-Dependencies.gyp:glew',
				'Dependencies/ExLibris-Dependencies.gyp:glfw',
				'Dependencies/ExLibris-Dependencies.gyp:glm',
				'Dependencies/ExLibris-Dependencies.gyp:vld',
				'ExLibris-SDK.gyp:ExLibris',
			],
			'sources': [
				'<(project_dir)/Application.cpp',
				'<(project_dir)/Application.h',
				'<(project_dir)/DebugHelper.cpp',
				'<(project_dir)/DebugHelper.h',
				'<(project_dir)/FrameworkGL.PCH.cpp',
				'<(project_dir)/FrameworkGL.PCH.h',
				'<(project_dir)/IRenderCommand.h',
				'<(project_dir)/MeshOpenGL.cpp',
				'<(project_dir)/MeshOpenGL.h',
				'<(project_dir)/RenderCommandLines.cpp',
				'<(project_dir)/RenderCommandLines.h',
				'<(project_dir)/RenderCommandText.cpp',
				'<(project_dir)/RenderCommandText.h',
				'<(project_dir)/ShaderProgram.cpp',
				'<(project_dir)/ShaderProgram.h',
				'<(project_dir)/ShaderSource.cpp',
				'<(project_dir)/ShaderSource.h',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'<(project_dir)',
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
				'Dependencies/ExLibris-Dependencies.gyp:glew',
				'Dependencies/ExLibris-Dependencies.gyp:glfw',
				'Dependencies/ExLibris-Dependencies.gyp:glm',
				'Dependencies/ExLibris-Dependencies.gyp:vld',
				'ExLibris-SDK.gyp:ExLibris',
				'Framework',
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
		{
			'target_name': 'ExampleFaceSelection',
			'type': 'executable',
			'dependencies': [
				'Dependencies/ExLibris-Dependencies.gyp:freetype',
				'Dependencies/ExLibris-Dependencies.gyp:glew',
				'Dependencies/ExLibris-Dependencies.gyp:glfw',
				'Dependencies/ExLibris-Dependencies.gyp:glm',
				'Dependencies/ExLibris-Dependencies.gyp:vld',
				'ExLibris-SDK.gyp:ExLibris',
				'Framework',
			],
			'sources': [
				'Projects/ExampleFaceSelection/ExampleFaceSelection.cpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
		{
			'target_name': 'ExampleFontOutline',
			'type': 'executable',
			'dependencies': [
				'Dependencies/ExLibris-Dependencies.gyp:freetype',
				'Dependencies/ExLibris-Dependencies.gyp:glew',
				'Dependencies/ExLibris-Dependencies.gyp:glfw',
				'Dependencies/ExLibris-Dependencies.gyp:glm',
				'Dependencies/ExLibris-Dependencies.gyp:poly2tri',
				'Dependencies/ExLibris-Dependencies.gyp:vld',
				'ExLibris-SDK.gyp:ExLibris',
				'Framework',
			],
			'sources': [
				'Projects/ExampleFontOutline/ExampleFontOutline.cpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
		{
			'target_name': 'ExampleLineDrawing',
			'type': 'executable',
			'dependencies': [
				'Dependencies/ExLibris-Dependencies.gyp:glew',
				'Dependencies/ExLibris-Dependencies.gyp:glfw',
				'Dependencies/ExLibris-Dependencies.gyp:glm',
				'Dependencies/ExLibris-Dependencies.gyp:poly2tri',
				'Dependencies/ExLibris-Dependencies.gyp:vld',
				'ExLibris-SDK.gyp:ExLibris',
				'Framework',
			],
			'sources': [
				'Projects/ExampleLineDrawing/ExampleLineDrawing.cpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
		{
			'target_name': 'ExampleTextField',
			'type': 'executable',
			'dependencies': [
				'Dependencies/ExLibris-Dependencies.gyp:freetype',
				'Dependencies/ExLibris-Dependencies.gyp:glew',
				'Dependencies/ExLibris-Dependencies.gyp:glfw',
				'Dependencies/ExLibris-Dependencies.gyp:glm',
				'Dependencies/ExLibris-Dependencies.gyp:vld',
				'ExLibris-SDK.gyp:ExLibris',
				'Framework',
			],
			'sources': [
				'Projects/FrameworkGL/ExampleTextField/ExampleTextField.cpp',
			],
			'conditions': [
				['OS!="win"', {
					'product_dir': 'output/<(platform_name)/<(architecture_name)/<(CONFIGURATION_NAME)',
				}]
			],
		},
		{
			'target_name': 'ExLibris.Test',
			'type': 'executable',
			'variables': {
				'project_dir': 'Projects/ExLibris.Test',
			},
			'dependencies': [
				'Dependencies/ExLibris-Dependencies.gyp:freetype',
				'Dependencies/ExLibris-Dependencies.gyp:glew',
				'Dependencies/ExLibris-Dependencies.gyp:glfw',
				'Dependencies/ExLibris-Dependencies.gyp:glm',
				'Dependencies/ExLibris-Dependencies.gyp:gtest',
				'Dependencies/ExLibris-Dependencies.gyp:poly2tri',
				'Dependencies/ExLibris-Dependencies.gyp:vld',
				'ExLibris-SDK.gyp:ExLibris',
				'Framework',
			],
			'sources': [
				'<(project_dir)/ExLibris.Test.Main.cpp',
				'<(project_dir)/ExLibris.Test.PCH.cpp',
				'<(project_dir)/ExLibris.Test.PCH.h',
				'<(project_dir)/Mock.FontLoader.h',
				'<(project_dir)/Mock.GlyphProvider.h',
				'<(project_dir)/Mock.MeshVisitor.h',
				'<(project_dir)/Mock.TextLayoutVisitor.h',
				'<(project_dir)/Suite.BoundingBox.cpp',
				'<(project_dir)/Suite.CurvePath.cpp',
				'<(project_dir)/Suite.Face.cpp',
				'<(project_dir)/Suite.FaceRequest.cpp',
				'<(project_dir)/Suite.Family.cpp',
				'<(project_dir)/Suite.FontLoaderFreetype.cpp',
				'<(project_dir)/Suite.FreetypeErrors.cpp',
				'<(project_dir)/Suite.GlyphProvider.cpp',
				'<(project_dir)/Suite.GlyphProviderFreetype.cpp',
				'<(project_dir)/Suite.GlyphProviderSystem.cpp',
				'<(project_dir)/Suite.Library.cpp',
				'<(project_dir)/Suite.Line.cpp',
				'<(project_dir)/Suite.LineShape.cpp',
				'<(project_dir)/Suite.MeshBuilder.cpp',
				'<(project_dir)/Suite.Polygon.cpp',
				'<(project_dir)/Suite.Streambuf.cpp',
				'<(project_dir)/Suite.TextLayout.cpp',
				'<(project_dir)/Suite.Triangle.cpp',
			],
		},
	],
}