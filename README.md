# Flash HTTP Client ⚡

> A fast, simple, and powerful HTTP client for your terminal - like Postman, but in CLI

[![Release](https://img.shields.io/github/v/release/aadityansha06/Flash-Http-cli?style=for-the-badge&color=green)](https://github.com/aadityansha06/Flash-Http-cli/releases/tag/1.0)
[![License](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C-orange.svg?style=for-the-badge)](https://en.wikipedia.org/wiki/C_(programming_language))

Flash is a terminal-based HTTP client that brings the power of API testing to your command line. Built with C and libcurl, it offers a clean Gruvbox-themed interface that's easy on the eyes and powerful in functionality.

## Features

- GET Requests — Fetch data from any REST API
- POST Requests — Send form data and JSON payloads
- Beautiful UI — Gruvbox dark theme with intuitive navigation
- Fast and Lightweight — Built in C for maximum performance
- Memory Safe — Proper memory management and error handling
- Cross-Platform — Works on Linux, macOS, and Windows (with proper setup)

## Screenshots

### Main Menu
![Flash Main Menu](screenshots/main-menu.png)

### POST Method Selection
![POST Method](screenshots/post.png)

---

## Quick Start

### Download Pre-built Binary (Recommended)

```bash
# Download the latest release
wget https://github.com/aadityansha06/Flash-Http-cli/releases/download/1.1.0/flash-linux-x64

# Make it executable
chmod +x flash-linux-x64

# Run Flash
./flash-linux-x64
```

### Build from Source

#### Prerequisites

- GCC compiler
- libcurl development libraries
- Make (optional)

#### Ubuntu/Debian

```bash
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev
```

#### CentOS/RHEL/Fedora

```bash
# CentOS/RHEL
sudo yum install gcc libcurl-devel

# Fedora
sudo dnf install gcc libcurl-devel
```

#### macOS

```bash
# Using Homebrew
brew install curl

# Using MacPorts
sudo port install curl
```

#### Compile and Run

```bash
git clone https://github.com/aadityansha06/Flash-Http-cli.git
cd Flash-Http-cli

make

./bin/flash
```

#### Termux

```bash
pkg update && pkg upgrade
pkg install clang curl

git clone https://github.com/aadityansha06/Flash-Http-cli.git
cd Flash-Http-cli

clang Flash-cli.c -o Flash -lcurl

./Flash
```

> Note: You may see warnings about `getchar()` return values or empty-body while loops during compilation. These are safe to ignore — the program will still run fine.

---

## Usage

### GET Requests

1. Select option `1` from the main menu
2. Enter the URL (e.g., `https://jsonplaceholder.typicode.com/posts/1`)
3. View the formatted response

### POST — Form Data

1. Select option `2` from the main menu
2. Choose `1` for Form Data
3. Enter data in format: `name=John+Doe&email=john%40gmail.com&age=25`
4. Enter the target URL
5. Send the request

### POST — JSON

1. Select option `2` from the main menu
2. Choose `2` for JSON Data
3. Enter your JSON payload: `{"name":"John","email":"john@gmail.com","age":25}`
4. Enter the target URL
5. Send the request

---

## Examples

```bash
# GET — fetch user data
URL: https://jsonplaceholder.typicode.com/users/1

# POST — form data
Data: name=Flash+User&email=flash%40example.com&phone=1234567890
URL: https://httpbin.org/post

# POST — JSON
Data: {"title":"Flash HTTP CLI","body":"A powerful terminal HTTP client","userId":1}
URL: https://jsonplaceholder.typicode.com/posts
```

### URL Encoding

```bash
# Spaces become +
name=John Doe → name=John+Doe

# Special characters use % encoding
email=user@domain.com → email=user%40domain.com
```

### JSON Formatting

```json
{
  "name": "Flash User",
  "data": {
    "email": "user@example.com",
    "active": true,
    "id": 123
  }
}
```

---

## Theme

Flash uses the **Gruvbox Dark** color scheme:

- Background: Dark, easy on the eyes
- Highlights: Warm yellows and oranges
- Success: Bright green indicators
- Errors: Clear red error messages
- Info: Cool blue accents

---

## Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](Developer-Docs/CONTRIBUTING.md) before submitting a PR. It covers code standards, version management, commit formatting, and the submission process.

**Quick start:**

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Commit your changes: `git commit -m 'Add amazing feature'`
4. Update the version in `lib/version.h`
5. Push the branch: `git push origin feature/amazing-feature`
6. Open a Pull Request

> **Important:** Always update `lib/version.h` with your changes. See [CONTRIBUTING.md](Developer-Docs/CONTRIBUTING.md#-version-management) for details.

---

## Contributors

Thanks to everyone who has contributed to Flash HTTP CLI!

**Core Team**
- [@aadityansha06](https://github.com/aadityansha06) — Creator & Maintainer

**Contributors**
- [@dexter-xD](https://github.com/dexter-xD) — Enhanced JSON input with multi-line support and improved UI alignment (#6)
- [@sakshamg567](https://github.com/sakshamg567) — Improved form-data input handling (#2)

Want to contribute? Check out the open issues or raise a PR!

---

## Known Issues

- Windows support requires additional setup for libcurl
- Very large responses (>1GB) may cause memory issues
- Binary releases are currently Linux-only

---

## TODO

- [ ] Custom headers support
- [ ] Authentication methods (Bearer, Basic Auth)
- [ ] Request/Response history
- [ ] Configuration file support
- [ ] Windows binary releases
- [ ] Response syntax highlighting

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author

**Aadityansh** — [@aadityansha06](https://github.com/aadityansha06)

## Acknowledgments

- [libcurl](https://curl.se/libcurl/) — The HTTP library powering Flash
- [Gruvbox](https://github.com/morhetz/gruvbox) — The color scheme
- Inspired by [Postman](https://www.postman.com/)

## Community

Join the discussions to help improve the project!
