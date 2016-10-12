#include "Initializer.hpp"
#include "Functions.hpp"

#include "i343/XboxLiveServicesWrapper.hpp"

using namespace AsuarSDK;

bool Initializer::Initialize()
{
	auto s_FunctionsInitialized = InitializeFunctions();

	auto s_PointersInitialized = InitializePointers();

	return s_FunctionsInitialized && s_PointersInitialized;
}

bool Initializer::InitializeFunctions()
{
	AssignFunctionPointer(i343::XboxLiveServicesWrapper::ForceRecheckLiveConnectivity, 0);
	AssignFunctionPointer(i343::XboxLiveServicesWrapper::ForceRecheckTMS, 0);

	return true;
}

bool Initializer::InitializePointers()
{
	return true;
}