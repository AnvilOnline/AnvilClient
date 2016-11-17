#pragma once

namespace AnvilCommon
{
	class IInitializable
	{
	public:
		virtual bool Init() = 0;
		virtual ~IInitializable() { }
	};
}