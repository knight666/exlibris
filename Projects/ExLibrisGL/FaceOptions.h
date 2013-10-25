#pragma once

namespace ExLibris
{

	enum BitmapQuality
	{
		eBitmapQuality_None,
		eBitmapQuality_Mono,
		eBitmapQuality_Color
	};

	struct FaceOptions
	{

		FaceOptions()
			: size(0.0f)
			, bitmap_quality(eBitmapQuality_Color)
			, outline(true)
			, kerning(true)
		{
		}

		float size;
		BitmapQuality bitmap_quality;
		bool outline;
		bool kerning;

	}; // struct FaceOptions

}; // namespace ExLibris