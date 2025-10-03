# ⚡ Flash HTTP CLI

> A fast, simple, and powerful HTTP client for your terminal - like Postman, but in CLI

[![Release](https://img.shields.io/github/v/release/aadityansha06/Flash-Http-cli?style=for-the-badge&color=green)](https://github.com/aadityansha06/Flash-Http-cli/releases/tag/1.0)
[![License](https://img.shields.io/badge/License-MIT-blue.svg?style=for-the-badge)](LICENSE)
[![Language](https://img.shields.io/badge/Language-C-orange.svg?style=for-the-badge)](https://en.wikipedia.org/wiki/C_(programming_language))

Flash is a beautiful, terminal-based HTTP client that brings the power of API testing to your command line. Built with C and libcurl, it offers a clean Gruvbox-themed interface that's easy on the eyes and powerful in functionality.

## ✨ Features

- 🌐 **GET Requests** - Fetch data from any REST API
- 📤 **POST Requests** - Send form data and JSON payloads
- 🎨 **Beautiful UI** - Gruvbox dark theme with intuitive navigation
- ⚡ **Fast & Lightweight** - Built in C for maximum performance
- 🛡️ **Memory Safe** - Proper memory management and error handling
- 📱 **Cross-Platform** - Works on Linux, macOS, and Windows (with proper setup)

## 📸 Screenshots

### Main Menu
![Flash Main Menu](screenshots/main-menu.png)
*Clean and intuitive main interface*

### POST Method Selection
![POST Method](screenshots/post.png)
*Easy content-type selection for POST requests*

## 🚀 Quick Start

### Download Pre-built Binary (Recommended)

Download the latest release for Linux:

```bash
# Download the latest release
wget https://github.com/aadityansha06/Flash-Http-cli/releases/download/1.1/flash-linux-x64

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

#### Compilation

```bash
# Clone the repository
git clone https://github.com/aadityansha06/Flash-Http-cli.git
cd Flash-Http-cli

# build
make

# Run
./bin/flash
```

### Termux installation 
```bash
# Install dependencies
pkg update && pkg upgrade
pkg install clang curl

# Clone the repository
git clone https://github.com/aadityansha06/Flash-Http-cli.git
cd Flash-Http-cli

# Compile
clang Flash-cli.c -o Flash -lcurl

# Run
./Flash


Note: You may see warnings about getchar() return values or empty-body while loops during compilation.
These warnings are safe to ignore — the program will still run fine.

```




## 🔧 Usage

### GET Requests

1. Select option `1` from the main menu
2. Enter the URL (e.g., `https://jsonplaceholder.typicode.com/posts/1`)
3. View the formatted response

### POST Requests - Form Data

1. Select option `2` from the main menu
2. Choose `1` for Form Data
3. Enter data in format: `name=John+Doe&email=john%40gmail.com&age=25`
4. Enter the target URL
5. Send the request

### POST Requests - JSON

1. Select option `2` from the main menu
2. Choose `2` for JSON Data
3. Enter JSON data: `{"name":"John","email":"john@gmail.com","age":25}`
4. Enter the target URL
5. Send the request

## 📚 Examples

### Testing a REST API

```bash
# GET request to fetch user data
URL: https://jsonplaceholder.typicode.com/users/1

# POST form data to create a user
Data: name=Flash+User&email=flash%40example.com&phone=1234567890
URL: https://httpbin.org/post

# POST JSON data
Data: {"title":"Flash HTTP CLI","body":"A powerful terminal HTTP client","userId":1}
URL: https://jsonplaceholder.typicode.com/posts
```

### Working with APIs

Flash handles various HTTP scenarios:
- ✅ REST APIs
- ✅ Form submissions  
- ✅ JSON APIs
- ✅ Authentication headers (manual setup)
- ✅ URL encoding
- ✅ Error handling

## 🎨 Theme

Flash uses the **Gruvbox Dark** color scheme for a comfortable terminal experience:

- 🟤 **Background**: Dark, easy on the eyes
- 🟨 **Highlights**: Warm yellows and oranges
- 🟩 **Success**: Bright green indicators  
- 🟥 **Errors**: Clear red error messages
- 🔵 **Info**: Cool blue accents

## 🔧 Advanced Usage

### URL Encoding

Flash automatically handles basic URL encoding, but for complex data:

```bash
# Spaces become +
name=John Doe → name=John+Doe

# Special characters use % encoding  
email=user@domain.com → email=user%40domain.com
```

### JSON Formatting

Ensure your JSON is properly formatted:

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

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. 🍴 Fork the repository
2. 🌿 Create a feature branch (`git checkout -b feature/amazing-feature`)
3. 💾 Commit your changes (`git commit -m 'Add amazing feature'`)
4. 🚀 Push to the branch (`git push origin feature/amazing-feature`)
5. 🎯 Open a Pull Request

### Development Setup

```bash
git clone https://github.com/aadityansha06/Flash-Http-cli.git
cd Flash-Http-cli

# Install development dependencies
sudo apt install build-essential libcurl4-openssl-dev

# Build and test
gcc -g -o flash-dev main.c -lcurl
./flash-dev
```

## 🐛 Known Issues

- Windows support requires additional setup for libcurl
- Very large responses (>1GB) may cause memory issues
- Binary releases are currently Linux-only

## 📋 TODO

- [ ] Custom headers support
- [ ] Authentication methods (Bearer, Basic Auth)
- [ ] Request/Response history
- [ ] Configuration file support
- [ ] Windows binary releases
- [ ] Response syntax highlighting

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Aadityansh** 
- GitHub: [@aadityansha06](https://github.com/aadityansha06)

## 🙏 Acknowledgments

- [libcurl](https://curl.se/libcurl/) - The fantastic HTTP library
- [Gruvbox](https://github.com/morhetz/gruvbox) - The beautiful color scheme
- Inspired by [Postman](https://www.postman.com/) - The API development platform

