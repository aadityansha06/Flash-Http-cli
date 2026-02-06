# Contributing to Flash HTTP CLI

Thank you for your interest in contributing to Flash HTTP CLI! This document outlines the guidelines and standards we follow to maintain code quality and consistency.

## Table of Contents

- [Code Standards](#code-standards)
- [Version Management](#version-management)
- [Getting Started](#getting-started)
- [Making Changes](#making-changes)
- [Testing Your Changes](#testing-your-changes)
- [Submitting a Pull Request](#submitting-a-pull-request)

---

## Code Standards

### Naming Conventions

All functions and variables must follow **snake_case** naming convention:

Good:
```c
static void content_application(void);
static void get_api_info(ApiInfo *info);
static char *escape_json_string(const char *input);
```

Bad:
```c
static void ContentApplication(void);
static void getApiInfo(ApiInfo *info);
static char *escapeJsonString(const char *input);
```

### Indentation & Formatting

- Use **4-space indentation** consistently throughout the code
- Keep lines reasonably length (max 100 characters recommended)
- Use proper spacing around operators and function parameters

Good:
```c
static void function_name(void)
{
    int value = 10;
    char *ptr = malloc(sizeof(char) * 100);
    
    if (ptr == NULL) {
        printf("Error: Memory allocation failed\n");
        return;
    }
}
```

Bad:
```c
static void functionName(void){
int value=10;
char *ptr=malloc(sizeof(char)*100);
if(ptr==NULL){printf("Error");return;}}
```

### Comments & Documentation

All functions must have block comments explaining purpose, parameters, and return values:

```c
/*
 * function_name - Brief description of what the function does
 *
 * Longer explanation if needed. Describe the purpose,
 * any important behavior, or edge cases.
 *
 * param_name: Description of parameter
 * Returns: Description of return value (0 on success, 1 on failure)
 */
static int function_name(const char *param_name)
{
    // Implementation
}
```

### Memory Management

- Always check malloc/realloc/calloc return values
- Free allocated memory appropriately
- Avoid memory leaks

Good:
```c
char *buffer = malloc(size);
if (!buffer) {
    fprintf(stderr, "Memory allocation failed\n");
    return NULL;
}
// Use buffer
free(buffer);
```

### Error Handling

- Always check return values of library functions (curl_easy_init, fopen, etc.)
- Provide meaningful error messages
- Clean up resources on error

Good:
```c
CURL *handle = curl_easy_init();
if (!handle) {
    fprintf(stderr, GRV_RED "Failed to initialize CURL\n" RESET);
    return 1;
}
```

---

## Version Management

**This is critical!** Every modification to the codebase requires updating `lib/version.h`.

### Version Format

Version numbers follow **semantic versioning**: `major.minor.patch`

```
Format: major.minor.patch
- major: significant changes that break compatibility
- minor: new features added (backward compatible)
- patch: bug fixes and small improvements (no new features)
```

### When to Update Each Number

| Change Type | Example | Update |
|---|---|---|
| Bug fix, typo, formatting | Fix memory leak, improve comment | patch (1.0.0 → 1.0.1) |
| New feature | Add authentication support | minor (1.0.0 → 1.1.0) |
| Breaking change | Modify API signature, change behavior | major (1.0.0 → 2.0.0) |

### How to Update Version

Edit `lib/version.h`:

```c
#define RELEASE_VERSION "1.0.2"
```

Change the version string to reflect your changes before submitting a PR.

### Examples

**Example 1: Bug Fix**
```
Current version: 1.0.5
Your change: Fixed typo in callback function
New version: 1.0.6
```

**Example 2: New Feature**
```
Current version: 1.0.5
Your change: Added custom headers support
New version: 1.1.0
```

**Example 3: Multiple Improvements**
```
Current version: 1.0.5
Your changes: 
  - Fixed memory leak in get_url()
  - Improved error messages
  - Reformatted code
New version: 1.0.6 (only patch changes, no new features)
```

---

## Getting Started

### Prerequisites

Install required development tools:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev git
```

**macOS:**
```bash
brew install curl
```

**CentOS/Fedora:**
```bash
sudo yum install gcc libcurl-devel
```

### Setup Your Fork

```bash
# 1. Fork the repository on GitHub
# 2. Clone your fork
git clone https://github.com/YOUR-USERNAME/Flash-Http-cli.git
cd Flash-Http-cli

# 3. Add upstream remote
git remote add upstream https://github.com/aadityansha06/Flash-Http-cli.git

# 4. Verify remotes
git remote -v
# origin    your fork
# upstream  original repo
```

---

## Making Changes

### Create a Feature Branch

```bash
# Update main branch
git fetch upstream
git checkout main
git merge upstream/main

# Create feature branch with descriptive name
git checkout -b feature/add-auth-headers
# or
git checkout -b bugfix/fix-memory-leak
```

### Code Changes Checklist

Before committing, ensure:

- [ ] Code follows naming conventions (snake_case)
- [ ] 4-space indentation used consistently
- [ ] All functions have proper comments
- [ ] Memory is properly managed (no leaks)
- [ ] Error handling is implemented
- [ ] Version in `lib/version.h` is updated
- [ ] Code compiles without warnings: `gcc -Wall -Wextra Flash-cli.c -o flash -lcurl`

### Commit Messages

Write clear, descriptive commit messages:

Good:
```
feat: Add support for custom headers in requests

- Allow users to add custom HTTP headers
- Parse headers from user input
- Update version to 1.1.0
```

Bad:
```
fix stuff
updates
v1.1
```

---

## Testing Your Changes

### Build the Project

```bash
# Compile with warnings enabled
gcc -Wall -Wextra Flash-cli.c -o flash-test -lcurl

# Run the test binary
./flash-test
```

### Manual Testing

1. **Test GET requests:**
   - Try a simple GET: `https://jsonplaceholder.typicode.com/posts/1`
   - Verify response displays correctly

2. **Test POST requests:**
   - Form data with key-value pairs
   - JSON with various data types
   - Verify data is sent correctly

3. **Test edge cases:**
   - Empty responses
   - Large responses
   - Invalid URLs
   - Network errors

4. **Memory check (Linux):**
   ```bash
   valgrind --leak-check=full ./flash-test
   ```

---

## Submitting a Pull Request

### Before Submitting

1. **Update version** in `lib/version.h`
2. **Test thoroughly** - compile and run tests
3. **Review your code** - ensure it follows standards
4. **Rebase if needed:**
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

### Create Pull Request

```bash
# Push to your fork
git push origin feature/your-feature-name
```

Then on GitHub:

1. Go to your fork
2. Click "Compare & pull request"
3. Fill in PR description:
   - What changes were made?
   - Why were they needed?
   - Any testing performed?
   - Version number updated?

### PR Description Template

```markdown
## Description
Brief description of changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Code improvement
- [ ] Documentation

## Changes Made
- Item 1
- Item 2
- Item 3

## Testing
Describe how you tested these changes

## Version Update
- Old version: 1.0.5
- New version: 1.0.6
- Reason: Bug fix in callback function
```

---

## Code Review Process

Your PR will be reviewed for:

1. Code quality and standards compliance
2. Proper memory management
3. Clear documentation and comments
4. Version number correctness
5. Testing completeness
6. No breaking changes (unless major version bump)

Maintainers may request changes before merging.

---

## Reporting Issues

Found a bug? Please report it with:

- **Title:** Brief description of the issue
- **Description:** Detailed explanation
- **Steps to reproduce:** How to trigger the bug
- **Expected behavior:** What should happen
- **Actual behavior:** What actually happens
- **Environment:** OS, compiler version, etc.

---

## Questions?

Feel free to:
- Open an issue for discussion
- Comment on existing PRs
- Ask questions in PR descriptions

---

## Thank You!

Thank you for contributing to Flash HTTP CLI! Your efforts help make this tool better for everyone.

Happy coding!
