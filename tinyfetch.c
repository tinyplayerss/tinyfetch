#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <mntent.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>

#define YELLOW "\033[1;33m"
#define WHITE  "\033[1;37m"
#define RESET  "\033[0m"
#define LABEL(label) printf(YELLOW "%-14s" RESET, label)

#define CONFIG_FILE "./tinyfetch.conf"

bool show_logo = true;
bool show_user = true;
bool show_installed = true;
bool show_os = true;
bool show_shell = true;
bool show_wm = true;
bool show_resolution = true;
bool show_cpu = true;
bool show_gpu = true;
bool show_uptime = true;
bool show_ram = true;
bool show_disk = true;
bool show_packages = true;
bool show_time = true;

// Fixed trim function
void trim(char *str) {
    char *start = str;
    char *end;

    // Skip leading whitespace by shifting string forward
    while (isspace((unsigned char)*start)) start++;
    if (start != str) {
        memmove(str, start, strlen(start) + 1); // +1 to include terminating null
    }

    if (*str == 0) return;

    // Trim trailing whitespace
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}

void generate_default_config() {
    FILE *fp = fopen(CONFIG_FILE, "w");
    if (!fp) return;
    fprintf(fp,
        "# show_logo is the branding of tinyfetch. It prints out the \"tiny\" ASCII art at the top.\n"
        "show_logo = true\n\n"

        "# show_user shows the current user you are logged in as.\n"
        "show_user = true\n\n"

        "# show_installed displays the date when your OS was installed.\n"
        "show_installed = true\n\n"

        "# show_os shows the name of the distro you are currently running.\n"
        "show_os = true\n\n"

        "# show_shell shows the terminal shell you are using...\n"
        "# Example: bash, zsh, fish. It may also reflect the terminal emulator like gnome-terminal, kitty, etc.\n"
        "show_shell = true\n\n"

        "# show_wm shows your current Window Manager (WM) or Desktop Environment (DE).\n"
        "show_wm = true\n\n"

        "# show_resolution shows your display resolution. Keep in mind it won't work on SSH sessions...\n"
        "# Only the local machine can fetch monitor/display resolution.\n"
        "show_resolution = true\n\n"

        "# show_cpu displays the name/model of your CPU processor.\n"
        "show_cpu = true\n\n"

        "# show_gpu displays the name/model of your graphics card (GPU).\n"
        "show_gpu = true\n\n"

        "# show_uptime shows how long your machine has been running without a reboot.\n"
        "show_uptime = true\n\n"

        "# show_ram shows how much RAM is used and total available RAM.\n"
        "# Useful to verify new RAM installations.\n"
        "show_ram = true\n\n"

        "# show_disk displays total disk usage across all mounted disks.\n"
        "# Note: It does not show each disk individually.\n"
        "show_disk = true\n\n"

        "# show_packages displays the total number of installed packages.\n"
        "# It includes system packages and user-installed ones.\n"
        "show_packages = true\n\n"

        "# show_time shows how long it took for tinyfetch to run.\n"
        "# A fun way to benchmark your system's script execution time!\n"
        "show_time = true\n"
    );
    fclose(fp);
}

void load_config() {
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (!fp) {
        generate_default_config();
        return;
    }
    char line[128];
    while (fgets(line, sizeof(line), fp)) {
        char *eq = strchr(line, '=');
        if (!eq) continue;
        *eq = '\0';
        char *key = line;
        char *value = eq + 1;
        trim(key);
        trim(value);
        bool val = strcmp(value, "true") == 0;
        if (strcmp(key, "show_logo") == 0) show_logo = val;
        else if (strcmp(key, "show_user") == 0) show_user = val;
        else if (strcmp(key, "show_installed") == 0) show_installed = val;
        else if (strcmp(key, "show_os") == 0) show_os = val;
        else if (strcmp(key, "show_shell") == 0) show_shell = val;
        else if (strcmp(key, "show_wm") == 0) show_wm = val;
        else if (strcmp(key, "show_resolution") == 0) show_resolution = val;
        else if (strcmp(key, "show_cpu") == 0) show_cpu = val;
        else if (strcmp(key, "show_gpu") == 0) show_gpu = val;
        else if (strcmp(key, "show_uptime") == 0) show_uptime = val;
        else if (strcmp(key, "show_ram") == 0) show_ram = val;
        else if (strcmp(key, "show_disk") == 0) show_disk = val;
        else if (strcmp(key, "show_packages") == 0) show_packages = val;
        else if (strcmp(key, "show_time") == 0) show_time = val;
    }
    fclose(fp);
}

void print_ascii_logo() {
    printf(
        WHITE
        "   /$$     /$$                    \n"
        "  | $$    |__/                    \n"
        " /$$$$$$   /$$ /$$$$$$$  /$$   /$$\n"
        "|_  $$_/  | $$| $$__  $$| $$  | $$\n"
        "  | $$    | $$| $$  \\ $$| $$  | $$\n"
        "  | $$ /$$| $$| $$  | $$| $$  | $$\n"
        "  |  $$$$/| $$| $$  | $$|  $$$$$$$\n"
        "   \\___/  |__/|__/  |__/ \\____  $$\n"
        "                         /$$  | $$\n"
        "                        |  $$$$$$/\n"
        "                         \\______/ \n\n"
        RESET
    );
}

void get_user_and_host() {
    struct passwd *pw = getpwuid(getuid());
    struct utsname uts;
    uname(&uts);

    const char *ssh = getenv("SSH_CONNECTION");
    LABEL("User:");
    printf("%s@%s%s%s\n", pw->pw_name, uts.nodename, ssh ? " (SSH from: " : "", ssh ? strtok(strdup(ssh), " ") : "", ssh ? ")" : "");
}

void get_install_date() {
    struct stat st;
    if (stat("/", &st) == 0) {
        struct tm *tm = localtime(&st.st_ctime);
        if (tm) {
            char buf[64];
            strftime(buf, sizeof(buf), "%Y/%m/%d", tm);
            LABEL("Installed:");
            printf("%s\n", buf);
        }
    } else {
        LABEL("Installed:");
        printf("Unknown\n");
    }
}

void get_os_and_kernel() {
    struct utsname uts;
    uname(&uts);
    LABEL("OS:");
    printf("Ubuntu 24.04 LTS\n");
    LABEL("Kernel:");
    printf("%s\n", uts.release);
}

void get_shell() {
    const char *shell = getenv("SHELL");
    const char *base = strrchr(shell, '/');
    LABEL("Shell:");
    printf("%s\n", base ? base + 1 : (shell ? shell : "Unknown"));
}

void get_window_manager() {
    const char *wm = getenv("XDG_CURRENT_DESKTOP");
    if (!wm) wm = getenv("DESKTOP_SESSION");

    LABEL("WM:");
    printf("%s\n", wm ? wm : "Unknown");
}

void get_display_resolution() {
    FILE *fp = popen("xdpyinfo | grep dimensions | awk '{print $2}'", "r");
    char res[64] = "";
    if (fp && fgets(res, sizeof(res), fp)) {
        res[strcspn(res, "\n")] = 0;
        LABEL("Resolution:");
        printf("%s\n", res);
        pclose(fp);
        return;
    }
    LABEL("Resolution:");
    printf("Unknown\n");
    if (fp) pclose(fp);
}

void get_installed_packages() {
    FILE *fp = popen("dpkg --get-selections | wc -l", "r");
    char count[32];
    if (fp && fgets(count, sizeof(count), fp)) {
        count[strcspn(count, "\n")] = 0;
        LABEL("Packages:");
        printf("%s\n", count);
    } else {
        LABEL("Packages:");
        printf("Unknown\n");
    }
    if (fp) pclose(fp);
}

void get_total_disk_usage() {
    FILE *mnt = setmntent("/etc/mtab", "r");
    struct mntent *ent;
    unsigned long total = 0, used = 0;

    while ((ent = getmntent(mnt)) != NULL) {
        if (strstr(ent->mnt_fsname, "/dev/")) {
            struct statvfs fs;
            if (statvfs(ent->mnt_dir, &fs) == 0) {
                unsigned long t = fs.f_blocks * fs.f_frsize;
                unsigned long f = fs.f_bfree * fs.f_frsize;
                total += t;
                used += (t - f);
            }
        }
    }
    endmntent(mnt);

    LABEL("Disk:");
    printf("%.1f GB / %.1f GB\n", (float)used / (1<<30), (float)total / (1<<30));
}

void get_uptime() {
    struct sysinfo si;
    sysinfo(&si);
    int days = si.uptime / 86400;
    int hours = (si.uptime % 86400) / 3600;
    int mins = (si.uptime % 3600) / 60;
    LABEL("Uptime:");
    printf("%d days, %d hours, %d minutes\n", days, hours, mins);
}

void get_cpu_info() {
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        LABEL("CPU:");
        printf("Unknown\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *cpu = strchr(line, ':');
            if (cpu) {
                cpu += 2;
                LABEL("CPU:");
                printf("%s", cpu);
                break;
            }
        }
    }
    fclose(fp);
}

void get_gpu_info() {
    FILE *fp = popen("lspci | grep -i 'vga\\|3d'", "r");
    if (!fp) return;

    char line[512];
    char gpu_desc[256] = "";

    while (fgets(line, sizeof(line), fp)) {
        char *desc = strstr(line, "controller:");
        if (desc) {
            desc += strlen("controller:");
            while (*desc == ' ') desc++;
            strncpy(gpu_desc, desc, sizeof(gpu_desc) - 1);
            gpu_desc[strcspn(gpu_desc, "\n")] = 0;
            break;
        }
    }
    pclose(fp);

    LABEL("GPU:");
    printf("%s\n", strlen(gpu_desc) ? gpu_desc : "Unknown");
}

void get_ram_info() {
    struct sysinfo si;
    sysinfo(&si);
    long total = si.totalram / 1024 / 1024;
    long used = (si.totalram - si.freeram) / 1024 / 1024;
    LABEL("RAM:");
    printf("%ld MiB / %ld MiB\n", used, total);
}

int main() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    load_config();

    if (show_logo) print_ascii_logo();
    if (show_user) get_user_and_host();
    if (show_installed) get_install_date();
    if (show_os) get_os_and_kernel();
    if (show_shell) get_shell();
    if (show_wm) get_window_manager();
    if (show_resolution) get_display_resolution();
    if (show_cpu) get_cpu_info();
    if (show_gpu) get_gpu_info();
    if (show_uptime) get_uptime();
    if (show_ram) get_ram_info();
    if (show_disk) get_total_disk_usage();
    if (show_packages) get_installed_packages();

    clock_gettime(CLOCK_MONOTONIC, &end);
    if (show_time) {
        double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("\n");
        LABEL("Time:");
        printf("%.4f seconds\n", elapsed);
    }

    return 0;
}
