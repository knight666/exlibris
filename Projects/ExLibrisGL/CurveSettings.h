#ifndef _CURVESETTINGS_H_
#define _CURVESETTINGS_H_

namespace ExLibris
{

	struct CurveSettings
	{

		enum Type
		{
			eType_Fixed,
			eType_Dynamic
		};

	public:

		CurveSettings()
			: type(eType_Fixed)
			, precision(0)
			, resolution(0.0f)
		{
		}
	
		Type type;
		int precision;
		float resolution;
	
	}; // struct CurveSettings

}; // namespace ExLibris

#endif