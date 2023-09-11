Script to set up Visual Studio Dev Tools into powershell 

``` pushd "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools" cmd /c "VsDevCmd.bat&set" | foreach {     if ($_ -match "=") {         $v = $_.split("=", 2); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"     } } popd Write-Host "`nVisual Studio 2022 Command Prompt variables set." -ForegroundColor Yellow ``` 

Path to Visual Studio Dev Tools Command line:  
```"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"``   

# Resources Links 
|   Website    |                    What                                                             |            Links             | |:------------:|:-----------------------------------------------------------------------------------:|:----------------------------:|
|  Keystroke   | Get Started with Keystroke Win32 and C++                                            |    [Keystroke][Keystroke]    |
| WinLogon.exe | Winlogon handles interface functions that are independent of authentication policy. | [WinLogon.exe][Winlogon.exe] |  |:------------:|:-----------------------------------------------------------------------------------:|:----------------------------:|
[Keystroke]: https://learn.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input 
[Winlogon.exe]: https://learn.microsoft.com/en-us/windows/win32/secauthn/winlogon
