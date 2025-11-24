#!/bin/bash

# Code Quality Check Script for Needles VST Plugin
# Runs static analysis tools and formatting checks

echo "Running code quality checks for Needles VST Plugin..."

PROJECT_ROOT="$(dirname "$0")"
SOURCE_DIR="$PROJECT_ROOT/Source"
TEST_DIR="$PROJECT_ROOT/Tests"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

ERRORS=0

echo "============================================"
echo "1. Running clang-format checks..."
echo "============================================"

# Check if clang-format is available
if ! command -v clang-format &> /dev/null; then
    echo -e "${RED}clang-format not found. Please install clang-format.${NC}"
    ERRORS=$((ERRORS + 1))
else
    # Check formatting for all .cpp and .h files
    find "$SOURCE_DIR" "$TEST_DIR" -name "*.cpp" -o -name "*.h" | while read -r file; do
        if [ -f "$file" ]; then
            # Check if file needs formatting
            if ! clang-format --dry-run --Werror "$file" &>/dev/null; then
                echo -e "${YELLOW}Format issues in: $file${NC}"
                echo "To fix: clang-format -i $file"
                ERRORS=$((ERRORS + 1))
            fi
        fi
    done
    
    if [ $ERRORS -eq 0 ]; then
        echo -e "${GREEN}✓ All files properly formatted${NC}"
    fi
fi

echo ""
echo "============================================"
echo "2. Running cppcheck static analysis..."
echo "============================================"

# Check if cppcheck is available
if ! command -v cppcheck &> /dev/null; then
    echo -e "${RED}cppcheck not found. Please install cppcheck.${NC}"
    echo "Install with: brew install cppcheck (macOS) or apt-get install cppcheck (Linux)"
    ERRORS=$((ERRORS + 1))
else
    # Run cppcheck on source code
    cppcheck --enable=all \
             --std=c++17 \
             --platform=native \
             --suppress=missingIncludeSystem \
             --suppress=unusedFunction \
             --quiet \
             --error-exitcode=1 \
             "$SOURCE_DIR" 2>&1 | tee /tmp/cppcheck_output.txt
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ No static analysis issues found${NC}"
    else
        echo -e "${RED}✗ Static analysis issues found. See output above.${NC}"
        ERRORS=$((ERRORS + 1))
    fi
fi

echo ""
echo "============================================"
echo "3. Constitution compliance checks..."
echo "============================================"

# Check for proper header guards or pragma once
echo "Checking header guard usage..."
find "$SOURCE_DIR" -name "*.h" | while read -r file; do
    if [ -f "$file" ]; then
        if ! grep -q "#pragma once" "$file" && ! grep -q "#ifndef.*_H" "$file"; then
            echo -e "${YELLOW}Missing header guard in: $file${NC}"
            ERRORS=$((ERRORS + 1))
        fi
    fi
done

# Check for proper includes
echo "Checking include patterns..."
find "$SOURCE_DIR" -name "*.cpp" -o -name "*.h" | while read -r file; do
    if [ -f "$file" ]; then
        # Check if JUCE headers come before standard headers
        if grep -n "#include" "$file" | grep -E "^[0-9]+:#include <[^j]" | head -1 | \
           grep -q -B999 "#include.*juce" 2>/dev/null; then
            echo -e "${YELLOW}JUCE headers should come before standard headers in: $file${NC}"
            ERRORS=$((ERRORS + 1))
        fi
    fi
done

echo ""
echo "============================================"
echo "Summary"
echo "============================================"

if [ $ERRORS -eq 0 ]; then
    echo -e "${GREEN}✓ All code quality checks passed!${NC}"
    exit 0
else
    echo -e "${RED}✗ Found $ERRORS code quality issues${NC}"
    echo "Please fix the issues above before committing."
    exit 1
fi