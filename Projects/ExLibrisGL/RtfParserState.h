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

#pragma once

namespace ExLibris
{
	struct RtfParserGroup;
	class IRtfParseable;
}

namespace ExLibris
{

	struct RtfParserState
	{
		RtfParserState()
			: group_current(nullptr)
			, group_index(0)
			, target_current(nullptr)
		{
		}

		~RtfParserState()
		{
			for (std::vector<RtfParserGroup*>::iterator group_it = groups.begin(); group_it != groups.end(); ++group_it)
			{
				delete *group_it;
			}
		}

		struct Entry
		{
			RtfParserGroup* group_container;
			int group_index;
			IRtfParseable* target;
		};

		void PushGroup()
		{
			RtfParserGroup* group_create = new RtfParserGroup;
			group_create->index = group_index;
			group_create->parent = group_current;

			Entry entry;
			entry.group_container = group_create;
			entry.group_index = group_index;
			entry.target = target_current;
			entries.push(entry);

			group_current = group_create;
			group_index++;

			groups.push_back(group_create);
		}

		bool PopGroup()
		{
			if (entries.size() == 0)
			{
				return false;
			}

			Entry& entry = entries.top();
			group_current = entry.group_container;
			group_index = entry.group_index;
			target_current = entry.target;

			entries.pop();

			return true;
		}

		std::stack<Entry> entries;

		int group_index;
		std::vector<RtfParserGroup*> groups;
		RtfParserGroup* group_current;
		std::stack<IRtfParseable*> targets;
		IRtfParseable* target_current;
	};

}; // namespace ExLibris