mkdir C:\Qt\4.8.5\include\QtAV\ 
copy /y E:\QtAV-master\tools\install_sdk\..\..\src\QtAV\*.h C:\Qt\4.8.5\include\QtAV\ 
xcopy /s /q /y /i E:\QtAV-master\tools\install_sdk\..\..\src\QtAV\private C:\Qt\4.8.5\include\QtAV\private 
copy /y E:\QtAV-master\lib_win_\*QtAV* C:\Qt\4.8.5\lib 
copy /y E:\QtAV-master\lib_win_\QtAV1.lib C:\Qt\4.8.5\lib\QtAV1.lib 
xcopy /s /q /y /i E:\QtAV-master\tools\install_sdk\..\..\qml\QmlAV C:\Qt\4.8.5\include\QmlAV 
copy /y E:\QtAV-master\tools\install_sdk\qt4av.prf C:\Qt\4.8.5\mkspecs\features\av.prf 
move /y C:\Qt\4.8.5\lib\QtAV*.dll C:\Qt\4.8.5\bin 
