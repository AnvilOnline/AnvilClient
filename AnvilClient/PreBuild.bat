FOR /f "usebackq tokens=1,2,3 delims= " %%a IN ("src\Misc\BuildInfo.hpp") DO (
	set /a buildnum = %%c
	set /a newbuild = %%c + 1
)

@echo. > "src\Misc\BuildInfo.hpp"
@echo #define ANVIL_BUILD %newbuild% > "src\Misc\BuildInfo.hpp"
