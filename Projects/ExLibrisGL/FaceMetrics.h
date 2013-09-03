#ifndef _FACEMETRICS_H_
#define _FACEMETRICS_H_

#include <map>

namespace ExLibris
{
	struct GlyphMetrics;
}

namespace ExLibris
{

	class FaceMetrics
	{
	
	public:
	
		FaceMetrics();
		~FaceMetrics();
	
		void AddGlyphMetrics(GlyphMetrics* a_Metrics);
		GlyphMetrics* FindGlyphMetrics(unsigned int a_CodePoint) const;

	private:

		std::map<unsigned int, GlyphMetrics*> m_Metrics;
	
	}; // class FaceMetrics

}; // namespace ExLibris

#endif