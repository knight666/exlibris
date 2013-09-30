#ifndef _LINEMESHOPTIONS_H_
#define _LINEMESHOPTIONS_H_

namespace ExLibris
{

	struct LineMeshOptions
	{

		enum Quality
		{
			eQuality_Fast,
			eQuality_Gapless
		};

	public:

		LineMeshOptions()
			: thickness(1.0f)
			, quality(eQuality_Gapless)
		{
		}

		float thickness;
		Quality quality;
	
	}; // struct LineMeshOptions

}; // namespace ExLibris

#endif