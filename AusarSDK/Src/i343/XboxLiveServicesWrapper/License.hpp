#pragma once
#include <i343/XboxLiveServicesWrapper.hpp>

namespace i343
{
	class XboxLiveServicesWrapper::License
	{
	public:
		/* TODO: Structure */

		typedef bool(*IsACELicensed_t)(License* p_This);
		static IsACELicensed_t IsACELicensed;

		typedef bool(*IsDLCLicensed_t)(License* p_This, wchar_t const*);
		static IsDLCLicensed_t IsDLCLicensed;

		typedef bool(*IsRelatedProductLicensed_t)(License* p_This, wchar_t const*);
		static IsRelatedProductLicensed_t IsRelatedProductLicensed;
	};
}