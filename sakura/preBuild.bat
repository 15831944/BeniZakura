@echo off

HeaderMake -in=..\sakura_core\Funccode_x.hsrc -out=..\sakura_core\Funccode_define.h -mode=define
HeaderMake -in=..\sakura_core\Funccode_x.hsrc -out=..\sakura_core\Funccode_enum.h -mode=enum -enum=EFunctionCode
MakefileMake -file=..\sakura_core\Makefile -dir=..\sakura_core

hg parent --template "#define HG_REV \"{latesttag}\"\n" > "..\sakura_core\hgrev.h"
hg parent --template "#define HG_REV_DISTANCE {latesttagdistance}\n" >> "..\sakura_core\hgrev.h"

exit 0
