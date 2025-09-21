# Building and Running a Hello World App on Luckfox Pico Ultra (eMMC)

## What We'll Cover

By following this tutorial, we will:

1. Create a new app in the Luckfox Buildroot SDK
2. Build it with the cross-toolchain
3. Flash the updated image to eMMC
4. Log in and run your binary

This guide walks through creating, building, flashing, and running a minimal C program (`helloworld.c`) using the Luckfox Buildroot SDK.

---
**Environment:**

- Build environment: **Luckfox-provided Docker image** ([official guide](https://wiki.luckfox.com/Luckfox-Pico-Ultra/Docker-Image-Build))

All build and cross-compilation steps are performed inside the Docker container for consistency and to avoid host-side dependency issues.

---

## 1. Project Structure

The Luckfox SDK expects user apps under:

```
project/app/<app_name>/
```

So we'll place our app here:

```
project/app/helloworld/
    ├── helloworld.c
    └── Makefile
```

---

## 2. Create the Source File

Create `project/app/helloworld/helloworld.c` with the following code:

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>

int main(int argc, char **argv) {
    struct utsname u;
    uname(&u);

    printf("Hello, Luckfox!\n");
    printf("Kernel: %s %s\n", u.sysname, u.release);
    printf("Machine: %s\n", u.machine);
    printf("PID: %d\n", getpid());

    return 0;
}
```

This prints a greeting plus system information.

---

## 3. Create the Makefile

Create `project/app/helloworld/Makefile`:

```make
ifeq ($(APP_PARAM), )
    APP_PARAM := ../Makefile.param
    include $(APP_PARAM)
endif

export LC_ALL := C
SHELL := /bin/bash

CURRENT_DIR := $(shell pwd)

PKG_NAME := helloworld
PKG_BIN  ?= out

HELLOWORLD_CFLAGS  := $(RK_APP_OPTS) $(RK_APP_CROSS_CFLAGS)
HELLOWORLD_LDFLAGS := $(RK_APP_OPTS) $(RK_APP_LDFLAGS)

PKG_TARGET := helloworld-build

all: $(PKG_TARGET)
	@echo "build $(PKG_NAME) done"

helloworld-build:
	@rm -rf $(PKG_BIN); \
	mkdir -p $(PKG_BIN)/bin
	$(RK_APP_CROSS)-gcc $(HELLOWORLD_CFLAGS) helloworld.c -o $(PKG_BIN)/bin/helloworld $(HELLOWORLD_LDFLAGS)
	$(call MAROC_COPY_PKG_TO_APP_OUTPUT, $(RK_APP_OUTPUT), $(PKG_BIN))

clean:
	@rm -rf $(PKG_BIN)

distclean: clean

info:
	@echo "This is $(PKG_NAME) for $(RK_APP_CHIP) with $(RK_APP_CROSS)"
```

This Makefile is modeled after existing apps (e.g. `fastboot_client`), and integrates with the SDK build system.

---

## 4. Build the App

Start the Docker container and build apps:

```bash
sudo docker start -ai luckfox
cd /home
./project/build.sh app
```

Check that the binary was created:

```bash
ls -l project/app/helloworld/out/bin/helloworld
ls -l output/out/app_out/bin/helloworld
```

---

## 5. Build the Firmware

Rebuild the full image (to include the new app):

```bash
cd /home
./project/build.sh firmware
```

The new images appear under:

```
output/image/
```

The key file is `update.img`.

---

## 6. Flash to eMMC

On your host (not inside Docker), flash the firmware with the provided script:

```bash
cd luckfox-pico
sudo ./rkflash.sh update
```

* Put the board in **Loader/Maskrom mode** before flashing:
  1. Hold down both **RESET** and **BOOT** buttons
  2. Release the **RESET** button
  3. Wait ~1 second
  4. Release the **BOOT** button
* The script uses Rockchip's `upgrade_tool` to write `update.img` to eMMC.

---

## 7. SSH Into the Board

Once the board reboots, log in:

```bash
ssh root@172.32.0.93
# Default password: luckfox
```

---

## 8. Run the Hello World App

Check the path and run:

```bash
which helloworld
helloworld
```

Expected output:

```bash
[root@luckfox root]# which helloworld
/oem/usr/bin/helloworld
[root@luckfox root]# helloworld
Hello, Luckfox!
Kernel: Linux 5.10.160
Machine: armv7l
PID: 3570
```

### Why `which helloworld` works

The Luckfox SDK automatically places binaries under `/oem/usr/bin`, which is included in the system's default `$PATH`. You can verify this by running:

```bash
echo $PATH
```

Expected output:

```bash
/bin:/sbin:/usr/bin:/usr/sbin:/oem:/oem/bin:/oem/usr/bin:/oem/sbin:/oem/usr/sbin
```

---

## 9. Notes

* The SDK automatically places binaries under `/oem/usr/bin`, which is already in `$PATH`.
* Rebuilding firmware after adding/modifying an app ensures it's included in the image.
* SSH keys can be configured for passwordless login (see separate `luckfox-ssh-setup.md`).

---

## 10. Bonus: Enhanced System Info Version

If you want to see more detailed system information, here's an enhanced version of `helloworld.c`:

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <string.h>

void print_system_info() {
    struct utsname system_info;
    struct sysinfo sys_info;
    FILE *fp;
    char buffer[256];
    
    printf("\n=== SYSTEM INFORMATION ===\n");
    
    // Get system name, kernel version, etc.
    if (uname(&system_info) == 0) {
        printf("System Name:     %s\n", system_info.sysname);
        printf("Node Name:       %s\n", system_info.nodename);
        printf("Kernel Release:  %s\n", system_info.release);
        printf("Kernel Version:  %s\n", system_info.version);
        printf("Machine:         %s\n", system_info.machine);
    }
    
    // Get system uptime and load
    if (sysinfo(&sys_info) == 0) {
        printf("Uptime:          %ld seconds (%ld days, %ld hours, %ld minutes)\n", 
               sys_info.uptime,
               sys_info.uptime / 86400,
               (sys_info.uptime % 86400) / 3600,
               (sys_info.uptime % 3600) / 60);
        
        printf("Load Average:    %.2f, %.2f, %.2f\n",
               sys_info.loads[0] / 65536.0,
               sys_info.loads[1] / 65536.0,
               sys_info.loads[2] / 65536.0);
        
        printf("Total RAM:       %lu MB\n", sys_info.totalram / (1024 * 1024));
        printf("Free RAM:        %lu MB\n", sys_info.freeram / (1024 * 1024));
        printf("Process Count:   %d\n", sys_info.procs);
    }
    
    // Try to get CPU info
    fp = fopen("/proc/cpuinfo", "r");
    if (fp != NULL) {
        printf("\n=== CPU INFORMATION ===\n");
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "processor", 9) == 0 ||
                strncmp(buffer, "model name", 10) == 0 ||
                strncmp(buffer, "cpu MHz", 7) == 0 ||
                strncmp(buffer, "BogoMIPS", 8) == 0 ||
                strncmp(buffer, "Features", 8) == 0) {
                printf("%s", buffer);
            }
        }
        fclose(fp);
    }
    
    // Try to get memory info
    fp = fopen("/proc/meminfo", "r");
    if (fp != NULL) {
        printf("\n=== MEMORY INFORMATION ===\n");
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "MemTotal", 8) == 0 ||
                strncmp(buffer, "MemFree", 7) == 0 ||
                strncmp(buffer, "MemAvailable", 12) == 0 ||
                strncmp(buffer, "Buffers", 7) == 0 ||
                strncmp(buffer, "Cached", 6) == 0) {
                printf("%s", buffer);
            }
        }
        fclose(fp);
    }
    
    // Get current time
    time_t current_time = time(NULL);
    printf("\n=== TIME INFORMATION ===\n");
    printf("Current Time:    %s", ctime(&current_time));
    
    printf("=============================\n\n");
}

int main(int argc, char *argv[])
{
    printf("Hello, World! This is a simple hello world application for Luckfox Pico.\n");
    printf("Program started with %d arguments:\n", argc);
    
    for (int i = 0; i < argc; i++) {
        printf("  argv[%d]: %s\n", i, argv[i]);
    }
    
    // Print system information
    print_system_info();
    
    printf("Application is running successfully!\n");
    
    return 0;
}
```

Replace the simple version with this enhanced one, then repeat the build and flash steps (4-6) to see detailed system information including CPU features, memory usage, load averages, and more!

---

That's it! You now have a working development workflow for the Luckfox Buildroot SDK.