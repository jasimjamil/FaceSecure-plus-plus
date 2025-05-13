# FaceSecure++ Windows Installer
# Run this script as administrator

# Display banner
Write-Host "  ______              _____                         _      _" -ForegroundColor Blue
Write-Host " |  ____|            / ____|                       | |    | |" -ForegroundColor Blue
Write-Host " | |__ __ _  ___ ___| (___   ___  ___ _   _ _ __ ___| |_  | |" -ForegroundColor Blue
Write-Host " |  __/ _` |/ __/ _ \___ \ / _ \/ __| | | | '__/ _ \ __| | |" -ForegroundColor Blue
Write-Host " | | | (_| | (_|  __/____) |  __/ (__| |_| | | |  __/ |_  |_|" -ForegroundColor Blue
Write-Host " |_|  \__,_|\___\___|_____/ \___|\___|\__,_|_|  \___|\__| (_)" -ForegroundColor Blue
Write-Host ""
Write-Host "Advanced Face Recognition Attendance System - Windows Installer" -ForegroundColor Green
Write-Host "Version 1.0" -ForegroundColor Yellow
Write-Host ""

# Check administrator permissions
$isAdmin = ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]"Administrator")
if (-not $isAdmin) {
    Write-Host "This script requires administrator privileges. Please run as administrator." -ForegroundColor Red
    exit
}

# Installation directory
$installDir = "C:\Program Files\FaceSecure++"

# Create installation directory
Write-Host "Creating installation directory..." -ForegroundColor Blue
New-Item -Path $installDir -ItemType Directory -Force | Out-Null
New-Item -Path "$installDir\bin" -ItemType Directory -Force | Out-Null
New-Item -Path "$installDir\data" -ItemType Directory -Force | Out-Null
New-Item -Path "$installDir\faces" -ItemType Directory -Force | Out-Null

# Check if build exists
if (-not (Test-Path ".\build\FaceSecure++.exe")) {
    Write-Host "Application binary not found. Please build the application first." -ForegroundColor Red
    exit
}

# Copy files
Write-Host "Copying application files..." -ForegroundColor Blue
Copy-Item ".\build\FaceSecure++.exe" -Destination "$installDir\bin\" -Force
Copy-Item ".\data\*" -Destination "$installDir\data\" -Force -Recurse

# Create start menu shortcut
Write-Host "Creating shortcuts..." -ForegroundColor Blue
$WshShell = New-Object -ComObject WScript.Shell
$Shortcut = $WshShell.CreateShortcut("$env:APPDATA\Microsoft\Windows\Start Menu\Programs\FaceSecure++.lnk")
$Shortcut.TargetPath = "$installDir\bin\FaceSecure++.exe"
$Shortcut.WorkingDirectory = "$installDir\bin"
$Shortcut.Description = "Advanced Face Recognition Attendance System"
$Shortcut.Save()

# Create desktop shortcut
$Shortcut = $WshShell.CreateShortcut("$env:USERPROFILE\Desktop\FaceSecure++.lnk")
$Shortcut.TargetPath = "$installDir\bin\FaceSecure++.exe"
$Shortcut.WorkingDirectory = "$installDir\bin"
$Shortcut.Description = "Advanced Face Recognition Attendance System"
$Shortcut.Save()

# Add to PATH
Write-Host "Adding application to PATH..." -ForegroundColor Blue
$path = [Environment]::GetEnvironmentVariable("Path", "Machine")
if (-not $path.Contains($installDir)) {
    [Environment]::SetEnvironmentVariable("Path", "$path;$installDir\bin", "Machine")
}

Write-Host "Installation complete!" -ForegroundColor Green
Write-Host "FaceSecure++ is now installed." -ForegroundColor Yellow
Write-Host "You can run it from the Start Menu, Desktop, or by typing 'FaceSecure++' in a command prompt." -ForegroundColor Yellow 