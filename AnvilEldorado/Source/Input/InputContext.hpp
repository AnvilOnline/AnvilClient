#pragma once

namespace AnvilEldorado::Input
{
	/// <summary>
	/// Base class for objects which override game input.
	/// </summary>
	class InputContext
	{
	public:
		virtual ~InputContext() { }

		/// <summary>
		/// Called when the input context is made active
		/// </summary>
		virtual void OnActivate() = 0;

		/// <summary>
		/// Called when the input context is deactivated
		/// </summary>
		virtual void OnDeactivate() = 0;

		/// <summary>
		/// Called on the active context each time the game or window processes input
		/// </summary>
		/// <param name="p_GameUpdate">True if game input, false if ui</param>
		virtual void OnUpdate(bool p_GameUpdate) = 0;
	};
}