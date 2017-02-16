#pragma once
#include <cstdint>

namespace AnvilEldorado::Game
{
	/// <summary>
	/// Game localization
	/// </summary>
	enum class GameLocale : uint8_t
	{
		/// <summary>
		/// en-US
		/// </summary>
		English,

		/// <summary>
		/// jp-JP
		/// </summary>
		Japanese,

		/// <summary>
		/// de-DE
		/// </summary>
		German,

		/// <summary>
		/// fr-FR
		/// </summary>
		French,

		/// <summary>
		/// es-ES
		/// </summary>
		Spanish,

		/// <summary>
		/// es-MX
		/// </summary>
		Mexican,

		/// <summary>
		/// it-IT
		/// </summary>
		Italian,

		/// <summary>
		/// ko-KR
		/// </summary>
		Korean,

		/// <summary>
		/// zh-TW
		/// </summary>
		ChineseTraditional,

		/// <summary>
		/// zh-CN
		/// </summary>
		ChineseSimplified,

		/// <summary>
		/// pt-PT
		/// </summary>
		Portuguese,

		/// <summary>
		/// ru-RU
		/// </summary>
		Russian
	};
}