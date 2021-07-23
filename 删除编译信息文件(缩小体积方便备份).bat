rd /Q /S MDK-ARM\Objects
rd /Q /S MDK-ARM\Listings
rd /Q /S MDK-ARM\DebugConfig
rd /Q /S MDK-ARM\debug

del /Q MDK-ARM\*.uvguix.*
del /Q MDK-ARM\*.scvd
#del /Q MDK-ARM\JLink*
del /Q MDK-ARM\JLinkLog.txt

rd  /Q /S EmbeddedProject1\.VisualGDB
rd  /Q /S EmbeddedProject1\VisualGDB
rd  /Q /S EmbeddedProject1\.vs
del /Q EmbeddedProject1\*.vcxproj.user

exit