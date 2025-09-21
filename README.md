# Luckfox Hello World Example

This project demonstrates how to create, build, and deploy a simple C application for the Luckfox Pico platform using the Luckfox Buildroot SDK. It provides a comprehensive example with detailed system information display and serves as a template for developing applications on the Luckfox ecosystem.

## Overview

This hello world application displays a greeting message along with comprehensive system information including kernel details, hardware specifications, memory usage, and CPU information. The project is designed to be used as a submodule in the Luckfox Pico SDK and demonstrates best practices for application development on the platform.

## Project Structure

```text
luckfox_helloworld_example_proj/
├── README.md                    # This file
├── helloworld_tutorial.md       # Detailed tutorial and setup guide
├── helloworld_tutorial.pdf      # PDF version of the tutorial
├── Makefile                     # Build configuration for Luckfox SDK
├── helloworld.c                 # Main C application source code
└── .gitignore                   # Git ignore file
```

## Installation as Submodule

This project is designed to be added as a submodule to your Luckfox Pico SDK project under the apps directory.

### Adding as Submodule

```bash
# Navigate to your Luckfox Pico SDK directory
cd /path/to/luckfox-pico

# Add this project as a submodule under project/app/
git submodule add <repository-url> project/app/helloworld

# Initialize and update the submodule
git submodule update --init --recursive
```

### Directory Structure After Installation

```text
luckfox-pico/
├── project/
│   └── app/
│       ├── helloworld/              # This submodule
│       │   ├── README.md
│       │   ├── Makefile
│       │   ├── helloworld.c
│       │   ├── helloworld_tutorial.md
│       │   ├── helloworld_tutorial.pdf
│       │   └── .gitignore
│       ├── other_app1/
│       └── other_app2/
└── ...
```

## Prerequisites

- Luckfox Buildroot SDK
- Luckfox-provided Docker container for cross-compilation
- Luckfox Pico Ultra device with eMMC storage
- Access to flashing tools (`rkflash.sh` and `upgrade_tool`)

## Building

### Build in Docker Environment

```bash
# Start Docker container
sudo docker start -ai luckfox
cd /home

# Build the application
./project/build.sh app

# Build the complete firmware (including this submodule)
./project/build.sh firmware

# Flash to device
sudo ./rkflash.sh update
```

### Build Output

After building, the following executable will be available on the target system in `/oem/usr/bin/`:

- `helloworld` - Main hello world application with system information display

## Usage

### Running the Application

```bash
# SSH to the Luckfox device
ssh root@<device-ip>
# Default password: luckfox

# Run the hello world application
helloworld

# Or check the path first
which helloworld
```

### Sample Output

When run successfully, the application will display:

```text
Hello, World! This is a simple hello world application for Luckfox Pico.
Program started with 1 arguments:
  argv[0]: /oem/usr/bin/helloworld

=== SYSTEM INFORMATION ===
System Name:     Linux
Node Name:       luckfox
Kernel Release:  5.10.160
Kernel Version:  #1 SMP PREEMPT Wed May 29 10:58:48 CST 2024
Machine:         armv7l
Uptime:          1234 seconds (0 days, 0 hours, 20 minutes)
Load Average:    0.15, 0.10, 0.05
Total RAM:       512 MB
Free RAM:        256 MB
Process Count:   45

=== CPU INFORMATION ===
processor       : 0
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 2400.00
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt lpae evtstrm

=== MEMORY INFORMATION ===
MemTotal:        524288 kB
MemFree:         262144 kB
MemAvailable:    300000 kB
Buffers:         12345 kB
Cached:          54321 kB

=== TIME INFORMATION ===
Current Time:    Sun Sep 22 12:34:56 2024

=============================

Application is running successfully!
```

## Implementation Details

### Application Features

The hello world application demonstrates:

- **Command-line Argument Processing**: Displays all command-line arguments passed to the program
- **System Information Collection**: Uses various system calls to gather comprehensive system data
- **File System Access**: Reads from `/proc/cpuinfo` and `/proc/meminfo` for detailed hardware information
- **Cross-platform Compatibility**: Uses standard POSIX system calls for maximum compatibility

### Key System Calls Used

- `uname()` - System name, kernel version, and machine architecture
- `sysinfo()` - Memory usage, load averages, and system uptime
- `time()` and `ctime()` - Current system time
- File I/O operations for reading `/proc` filesystem

### Build System Integration

The Makefile integrates with the Luckfox SDK build system by:

- Including `../Makefile.param` for SDK configuration
- Using SDK-provided cross-compilation variables (`RK_APP_CROSS`)
- Following SDK naming conventions and output structure
- Automatically copying binaries to the app output directory

## Troubleshooting

### Common Issues

1. **Application Not Found**
   ```bash
   # Check if binary exists
   ls /oem/usr/bin/helloworld
   
   # Verify PATH includes the directory
   echo $PATH
   ```

2. **Build Failures**
   ```bash
   # Ensure Docker container has all dependencies
   sudo docker start -ai luckfox
   
   # Clean and rebuild
   cd /home
   ./project/build.sh clean
   ./project/build.sh app
   ```

3. **Device Connection Issues**
   ```bash
   # Check network connectivity
   ping <device-ip>
   
   # Verify SSH service is running on device
   ssh root@<device-ip>
   ```

### Expected Behavior

- **Execution Time**: Application should start and complete within 1-2 seconds
- **Memory Usage**: Minimal memory footprint (~1-2MB)
- **Output Format**: Structured system information with clear section headers
- **Exit Code**: Should return 0 on successful execution

## Documentation

For detailed setup instructions, hardware configuration, and comprehensive explanations, see:

- [`helloworld_tutorial.md`](helloworld_tutorial.md) - Complete tutorial with step-by-step setup instructions
- [`helloworld_tutorial.pdf`](helloworld_tutorial.pdf) - PDF version of the tutorial for offline reference

## Contributing

This project serves as an example implementation for Luckfox Pico development. When contributing:

1. Follow the existing code style and structure
2. Test applications thoroughly on target hardware
3. Update documentation for any new features
4. Ensure compatibility with Luckfox Pico SDK

## License

This project is provided as an educational example. Please refer to the Luckfox SDK documentation for licensing information.

## References

- [Luckfox Pico SDK Documentation](https://wiki.luckfox.com/)
- [Luckfox Pico Hardware Documentation](https://wiki.luckfox.com/Luckfox-Pico-Ultra/)
- [Linux System Programming Guide](https://man7.org/linux/man-pages/)
- [Buildroot Documentation](https://buildroot.org/documentation.html)
