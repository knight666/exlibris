#include "ExLibrisGL.PCH.h"

#include "FaceMetrics.h"

#include "GlyphMetrics.h"

namespace ExLibris
{

	FaceMetrics::FaceMetrics()
	{
	}

	FaceMetrics::~FaceMetrics()
	{
		for (std::map<unsigned int, GlyphMetrics*>::iterator metrics_it = m_Metrics.begin(); metrics_it != m_Metrics.end(); ++metrics_it)
		{
			delete metrics_it->second;
		}
		m_Metrics.clear();
	}

	void FaceMetrics::AddGlyphMetrics(GlyphMetrics* a_Metrics)
	{
		//m_Metrics.insert(std::make_pair(a_Metrics->codepoint, a_Metrics));
	}

	GlyphMetrics* FaceMetrics::FindGlyphMetrics(unsigned int a_CodePoint) const
	{
		std::map<unsigned int, GlyphMetrics*>::const_iterator found = m_Metrics.find(a_CodePoint);
		if (found != m_Metrics.end())
		{
			return found->second;
		}

		return nullptr;
	}

}; // namespace ExLibris