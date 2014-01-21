/*
 * This file is a part of the ExLibris project.
 *
 * Copyright (C) 2013 Quinten Lansu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
 * SOFTWARE.
 */

#include "ExLibrisGL.PCH.h"

#include "RtfParserState.h"

#include "RtfParserGroup.h"

namespace ExLibris
{

	RtfParserState::RtfParserState()
		: m_GroupCurrent(nullptr)
		, m_GroupIndex(0)
		, m_TargetCurrent(nullptr)
	{
	}

	RtfParserState::~RtfParserState()
	{
		for (std::vector<RtfParserGroup*>::iterator group_it = m_Groups.begin(); group_it != m_Groups.end(); ++group_it)
		{
			delete *group_it;
		}
	}

	void RtfParserState::PushGroup()
	{
		RtfParserGroup* group_create = new RtfParserGroup;
		group_create->index = m_GroupIndex;
		group_create->parent = m_GroupCurrent;

		Entry entry;
		entry.group_container = group_create;
		entry.group_index = m_GroupIndex;
		entry.target = m_TargetCurrent;
		m_Entries.push(entry);

		m_GroupCurrent = group_create;
		m_GroupIndex++;

		m_Groups.push_back(group_create);
	}

	bool RtfParserState::PopGroup()
	{
		if (m_Entries.size() == 0)
		{
			return false;
		}

		Entry& entry = m_Entries.top();
		m_GroupCurrent = entry.group_container;
		m_GroupIndex = entry.group_index;
		m_TargetCurrent = entry.target;

		m_Entries.pop();

		return true;
	}

	int RtfParserState::GetGroupIndex() const
	{
		return m_GroupIndex;
	}

	RtfParserGroup* RtfParserState::GetGroup() const
	{
		return m_GroupCurrent;
	}

	IRtfParseable* RtfParserState::GetTarget() const
	{
		return m_TargetCurrent;
	}

	void RtfParserState::SetTarget(IRtfParseable* a_Target)
	{
		m_TargetCurrent = a_Target;
	}

}; // namespace ExLibris