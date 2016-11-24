FOR /f "usebackq tokens=1,2,3 delims= " %%a IN ("..\AnvilCommon\Source\BuildInfo.hpp") DO (
	set /a buildnum = %%c
	set /a newbuild = %%c + 1
)

@echo. > "..\AnvilCommon\Source\BuildInfo.hpp"
@echo #define ANVIL_BUILD %newbuild% > "..\AnvilCommon\Source\BuildInfo.hpp"