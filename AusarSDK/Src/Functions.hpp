#pragma once

#define AssignFunctionPointer(func, address) \
	func = (func ## _t) address