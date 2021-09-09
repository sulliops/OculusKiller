# Oculus Killer v2.0.0 by Owen Sullivan, https://sulliops.co

[int]$ServicesKilled = 0

# Kill OVRServiceLauncher.exe process
$OVRServiceLauncher = Get-Process OVRServiceLauncher -ErrorAction SilentlyContinue
    if ($OVRServiceLauncher) {
        Stop-Process -Name "OVRServiceLauncher" -Force
        $ServicesKilled++
    }
Remove-Variable OVRServiceLauncher

# Kill OVRRedir.exe process
$OVRRedir = Get-Process OVRRedir -ErrorAction SilentlyContinue
    if ($OVRRedir) {
        Stop-Process -Name "OVRRedir" -Force
        $ServicesKilled++
    }
Remove-Variable OVRRedir

# Kill OVRServer_x64.exe Process
$OVRServer_x64 = Get-Process OVRServer_x64 -ErrorAction SilentlyContinue
    if ($OVRServer_x64) {
        Stop-Process -Name "OVRServer_x64" -Force
        $ServicesKilled++
    }
Remove-Variable OVRServer_x64

# Output baloon notification with result and number of processes killed
if ($ServicesKilled -eq 0) {
    # Oculus Client not running
    Add-Type -AssemblyName System.Windows.Forms
    $global:balloon = New-Object System.Windows.Forms.NotifyIcon
    $path = (Get-Process -id $pid).Path
    $balloon.Icon = [System.Drawing.Icon]::ExtractAssociatedIcon($path)
    $balloon.BalloonTipIcon = [System.Windows.Forms.ToolTipIcon]::Info
    $balloon.BalloonTipText = 'Oculus Client was not running. ' + $ServicesKilled.ToString() + ' processes were killed.'
    $balloon.BalloonTipTitle = "Oculus Killer"
    $balloon.Visible = $true
    $balloon.ShowBalloonTip(5000)
} else {
    # Oculus Client running/killed
    Add-Type -AssemblyName System.Windows.Forms
    $global:balloon = New-Object System.Windows.Forms.NotifyIcon
    $path = (Get-Process -id $pid).Path
    $balloon.Icon = [System.Drawing.Icon]::ExtractAssociatedIcon($path)
    $balloon.BalloonTipIcon = [System.Windows.Forms.ToolTipIcon]::Info
    $balloon.BalloonTipText = 'Oculus Client exited successfully. ' + $ServicesKilled.ToString() + ' processes were killed.'
    $balloon.BalloonTipTitle = "Oculus Killer"
    $balloon.Visible = $true
    $balloon.ShowBalloonTip(5000)
}