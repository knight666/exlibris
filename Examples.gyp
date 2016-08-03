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
				'Dependencies/VisualLeakDetector-2.3/vld.gyp:vld',
				'ExLibris.gyp:ExLibris',
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
				'Dependencies/freetype-2.5.0.1/freetype.gyp:freetype',
				'Dependencies/glew-1.10.0/glew.gyp:glew',
				'Dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Dependencies/VisualLeakDetector-2.3/vld.gyp:vld',
				'ExLibris.gyp:ExLibris',
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
				'Dependencies/freetype-2.5.0.1/freetype.gyp:freetype',
				'Dependencies/glew-1.10.0/glew.gyp:glew',
				'Dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Dependencies/poly2tri-0.3.3/poly2tri.gyp:poly2tri',
				'Dependencies/VisualLeakDetector-2.3/vld.gyp:vld',
				'ExLibris.gyp:ExLibris',
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
				'Dependencies/glew-1.10.0/glew.gyp:glew',
				'Dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Dependencies/poly2tri-0.3.3/poly2tri.gyp:poly2tri',
				'Dependencies/VisualLeakDetector-2.3/vld.gyp:vld',
				'ExLibris.gyp:ExLibris',
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
				'Dependencies/freetype-2.5.0.1/freetype.gyp:freetype',
				'Dependencies/glew-1.10.0/glew.gyp:glew',
				'Dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Dependencies/VisualLeakDetector-2.3/vld.gyp:vld',
				'ExLibris.gyp:ExLibris',
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
			'dependencies': [
				'Dependencies/freetype-2.5.0.1/freetype.gyp:freetype',
				'Dependencies/glew-1.10.0/glew.gyp:glew',
				'Dependencies/glfw-3.0.2/glfw.gyp:glfw',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Dependencies/gtest-1.7.0-rc1/gtest.gyp:gtest',
				'Dependencies/poly2tri-0.3.3/poly2tri.gyp:poly2tri',
				'Dependencies/VisualLeakDetector-2.3/vld.gyp:vld',
				'ExLibris.gyp:ExLibris',
			],
			'sources': [
				'Projects/ExLibris.Test/ExLibris.Test.Main.cpp',
				'Projects/ExLibris.Test/ExLibris.Test.PCH.cpp',
				'Projects/ExLibris.Test/ExLibris.Test.PCH.h',
				'Projects/ExLibris.Test/Mock.FontLoader.h',
				'Projects/ExLibris.Test/Mock.GlyphProvider.h',
				'Projects/ExLibris.Test/Mock.MeshVisitor.h',
				'Projects/ExLibris.Test/Mock.TextLayoutVisitor.h',
				'Projects/ExLibris.Test/Suite.BoundingBox.cpp',
				'Projects/ExLibris.Test/Suite.CurvePath.cpp',
				'Projects/ExLibris.Test/Suite.Face.cpp',
				'Projects/ExLibris.Test/Suite.FaceRequest.cpp',
				'Projects/ExLibris.Test/Suite.Family.cpp',
				'Projects/ExLibris.Test/Suite.FontLoaderFreetype.cpp',
				'Projects/ExLibris.Test/Suite.FreetypeErrors.cpp',
				'Projects/ExLibris.Test/Suite.GlyphProvider.cpp',
				'Projects/ExLibris.Test/Suite.GlyphProviderFreetype.cpp',
				'Projects/ExLibris.Test/Suite.GlyphProviderSystem.cpp',
				'Projects/ExLibris.Test/Suite.Library.cpp',
				'Projects/ExLibris.Test/Suite.Line.cpp',
				'Projects/ExLibris.Test/Suite.LineShape.cpp',
				'Projects/ExLibris.Test/Suite.MeshBuilder.cpp',
				'Projects/ExLibris.Test/Suite.Polygon.cpp',
				'Projects/ExLibris.Test/Suite.Streambuf.cpp',
				'Projects/ExLibris.Test/Suite.TextLayout.cpp',
				'Projects/ExLibris.Test/Suite.Triangle.cpp',
			],
		},
	],
}