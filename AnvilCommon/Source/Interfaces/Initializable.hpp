#pragma once

namespace AnvilCommon
{
	class Initializable
	{
	public:
		virtual bool Init() = 0;
		virtual ~Initializable() { }
	};
}