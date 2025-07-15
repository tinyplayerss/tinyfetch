# 🐧 tinyfetch

**tinyfetch** is a blazing fast, minimalist system info tool written in C — designed to be lighter, faster, and simpler than tools like `neofetch`. It prints system information with optional ASCII branding, all configurable via a `.conf` file.

---

## ✨ Features

- ⚡ Ultra-lightweight C-based fetch tool
- 🧠 Zero dependencies (except standard libraries and a C compiler)
- 🧰 Configurable with a simple `tinyfetch.conf`
- 💻 Shows detailed system info (CPU, GPU, RAM, etc.)
- 🚀 Loads faster than neofetch or fastfetch
- 🐚 Works in terminal and over SSH (with auto-detection)
- 📏 Compact design, clean output

---

## 🔧 Compilation

If you're on a Debian-based distro (like Peppermint OS or Ubuntu), you can compile it like this:

```bash
sudo apt update
sudo apt install build-essential

gcc tinyfetch.c -o tinyfetch
```

Then move it to a system-wide location:

```bash
sudo mv tinyfetch /usr/bin/
sudo chmod +x /usr/bin/tinyfetch
```

---

## 📦 Quick Install (Precompiled Binary)

You can download the precompiled binary and install it:

```bash
wget https://[Coming Soon...]
sudo mv tinyfetch /usr/bin/
sudo chmod +x /usr/bin/tinyfetch
```

---

## ⚙️ Configuration File

The script looks for a config file called `tinyfetch.conf` in the same directory where it's run. This file controls what parts of the system info to show.

Example config (`tinyfetch.conf`):

```ini
# show_logo is the Branding of tinyfetch it Prints out "tiny" in the top of the script
show_logo = true
# show_user shows your Current user you signed in with
show_user = true
# show_installed Shows your installed date of when you installed the Distro OS
show_installed = true
# show_os Shows you which Distro Your running Right Now
show_os = true
# show_shell Shows you which terminal emulator/terminal server console you're using...
# Example terminals: kitty, alacritty, gnome-terminal, konsole, bash, etc.
show_shell = true
# show_wm Shows your current Window Manager
show_wm = true
# show_resolution Shows your display resolution. Won't work over SSH sessions.
show_resolution = true
# show_cpu Shows your CPU Processor info
show_cpu = true
# show_gpu Shows your GPU/Graphics Card
show_gpu = true
# show_uptime Shows how long the system has been running
show_uptime = true
# show_ram Shows used and total system RAM
show_ram = true
# show_disk Shows total disk usage across all connected disks
show_disk = true
# show_packages Shows installed package count (Debian-based only)
show_packages = true
# show_time Shows how fast tinyfetch executed
show_time = true
```

If `tinyfetch.conf` doesn't exist, it will be automatically generated with all features set to `true`.

---

## 🖥️ Output Example

```
   /$$     /$$                    
  | $$    |__/                    
 /$$$$$$   /$$ /$$$$$$$  /$$   /$$
|_  $$_/  | $$| $$__  $$| $$  | $$
  | $$    | $$| $$  \ $$| $$  | $$
  | $$ /$$| $$| $$  | $$| $$  | $$
  |  $$$$/| $$| $$  | $$|  $$$$$$$
   \___/  |__/|__/  |__/ \____  $$
                         /$$  | $$
                        |  $$$$$$/
                         \______/ 

User:         tinyplayerss@ThinkPad-X100e
Installed:    2025/07/12
OS:           Ubuntu 24.04 LTS
Kernel:       6.1.0-37-amd64
Shell:        bash
WM:           XFCE
Resolution:   1366x768
CPU:          AMD Athlon(tm) Neo MV-40
GPU:          Radeon HD 3200
Uptime:       0 days, 4 hours, 12 minutes
RAM:          1426 MiB / 2215 MiB
Disk:         13.0 GB / 915.8 GB
Packages:     2268
Time:         0.1809 seconds
```

---

## 🧪 Tested On

- ✅ Debian 12
- ✅ Peppermint OS (Debian Edition)
- ✅ Ubuntu 24.04 LTS
- ✅ Works over SSH (some visual features may be limited)

---

## 📜 License

MIT License — do whatever you want with it. Attribution appreciated, not required.

---

## 🙌 Credits

- Created by [tinyplayerss](https://x.com/@tinyplayerss)  
- Inspired by `neofetch`, `fastfetch`, and the Unix philosophy: _"Do one thing and do it well."_

---
