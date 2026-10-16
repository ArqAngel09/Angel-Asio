#define MyAppName "UltraASIO Lab"
#define MyAppVersion "0.1.0"
#define MyPublisher "Angel Ortiz Audio Lab"
#define MyExeName "UltraASIOLab.exe"

[Setup]
AppId={{A1A47E19-4DCE-4A70-90C3-8A6C1E8B4E10}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyPublisher}
DefaultDirName={autopf}\UltraASIO Lab
DefaultGroupName={#MyAppName}
OutputDir=output
OutputBaseFilename=UltraASIOLab-Setup-x64
ArchitecturesInstallIn64BitMode=x64
ArchitecturesAllowed=x64
Compression=lzma2
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=lowest

[Files]
; Build the app first, then compile this installer from the repository root.
Source: "..\build\Release\UltraASIOLab.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\UltraASIO Lab"; Filename: "{app}\{#MyExeName}"
Name: "{autodesktop}\UltraASIO Lab"; Filename: "{app}\{#MyExeName}"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional icons:"

[Run]
Filename: "{app}\{#MyExeName}"; Description: "Launch UltraASIO Lab"; Flags: postinstall nowait skipifsilent
