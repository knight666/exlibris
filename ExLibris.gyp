{
	'includes': [
		'Common.gypi',
	],
	'targets': [
		{
			'target_name': 'ExLibris',
			'type': 'static_library',
			'dependencies': [
				'Dependencies/freetype-2.5.0.1/freetype.gyp:freetype',
				'Dependencies/glm-0.9.4.5/glm.gyp:glm',
				'Dependencies/poly2tri-0.3.3/poly2tri.gyp:poly2tri',
			],
			'sources': [
				'Projects/ExLibrisGL/Alignment.h',
				'Projects/ExLibrisGL/BoundingBox.cpp',
				'Projects/ExLibrisGL/BoundingBox.h',
				'Projects/ExLibrisGL/CurvePath.cpp',
				'Projects/ExLibrisGL/CurvePath.h',
				'Projects/ExLibrisGL/CurveSettings.h',
				'Projects/ExLibrisGL/Exception.cpp',
				'Projects/ExLibrisGL/Exception.h',
				'Projects/ExLibrisGL/ExLibrisGL.PCH.cpp',
				'Projects/ExLibrisGL/ExLibrisGL.PCH.h',
				'Projects/ExLibrisGL/Face.cpp',
				'Projects/ExLibrisGL/Face.h',
				'Projects/ExLibrisGL/FaceOptions.h',
				'Projects/ExLibrisGL/FaceRequest.cpp',
				'Projects/ExLibrisGL/FaceRequest.h',
				'Projects/ExLibrisGL/Family.cpp',
				'Projects/ExLibrisGL/Family.h',
				'Projects/ExLibrisGL/FontLoaderFreetype.cpp',
				'Projects/ExLibrisGL/FontLoaderFreetype.h',
				'Projects/ExLibrisGL/FontMetrics.h',
				'Projects/ExLibrisGL/FreetypeConversion.h',
				'Projects/ExLibrisGL/FreetypeErrors.cpp',
				'Projects/ExLibrisGL/FreetypeErrors.h',
				'Projects/ExLibrisGL/GlyphBitmap.h',
				'Projects/ExLibrisGL/GlyphMetrics.h',
				'Projects/ExLibrisGL/GlyphProviderFreetype.cpp',
				'Projects/ExLibrisGL/GlyphProviderFreetype.h',
				'Projects/ExLibrisGL/GlyphProviderSystem.cpp',
				'Projects/ExLibrisGL/GlyphProviderSystem.h',
				'Projects/ExLibrisGL/IFontLoader.h',
				'Projects/ExLibrisGL/IGlyphProvider.h',
				'Projects/ExLibrisGL/IMeshVisitor.h',
				'Projects/ExLibrisGL/ITextLayoutVisitor.h',
				'Projects/ExLibrisGL/Library.cpp',
				'Projects/ExLibrisGL/Library.h',
				'Projects/ExLibrisGL/Line.cpp',
				'Projects/ExLibrisGL/Line.h',
				'Projects/ExLibrisGL/LineMeshOptions.h',
				'Projects/ExLibrisGL/LineShape.cpp',
				'Projects/ExLibrisGL/LineShape.h',
				'Projects/ExLibrisGL/Macros.h',
				'Projects/ExLibrisGL/MeshBuilder.cpp',
				'Projects/ExLibrisGL/MeshBuilder.h',
				'Projects/ExLibrisGL/Polygon.cpp',
				'Projects/ExLibrisGL/Polygon.h',
				'Projects/ExLibrisGL/Quad.cpp',
				'Projects/ExLibrisGL/Quad.h',
				'Projects/ExLibrisGL/SizePolicy.h',
				'Projects/ExLibrisGL/Style.h',
				'Projects/ExLibrisGL/TextCharacter.h',
				'Projects/ExLibrisGL/TextLayout.cpp',
				'Projects/ExLibrisGL/TextLayout.h',
				'Projects/ExLibrisGL/TextLine.h',
				'Projects/ExLibrisGL/Triangle.cpp',
				'Projects/ExLibrisGL/Triangle.h',
				'Projects/ExLibrisGL/TriangleList.h',
				'Projects/ExLibrisGL/TriangleOrientation.h',
				'Projects/ExLibrisGL/Weight.h',
			],
			'direct_dependent_settings': {
				'include_dirs': [
					'Projects/ExLibrisGL',
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