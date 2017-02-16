#pragma once
#include <cstdint>

namespace AnvilEldorado::Game
{
	/// <summary>
	/// Game lobbies
	/// </summary>
	enum class GameLobbyType : uint32_t
	{
		/// <summary>
		/// No lobby
		/// </summary>
		None,

		/// <summary>
		/// Campaign lobby
		/// </summary>
		Campaign,

		/// <summary>
		/// Custom game lobby
		/// </summary>
		CustomGame,

		/// <summary>
		/// Forge lobby
		/// </summary>
		Forge
	};
}