#!/usr/bin/env bash
# Install PicoJSON library with specified version

set -e  # Exit on error

# Check if version is provided
if [[ -z "$1" ]] || [[ "$1" == "-h" ]] || [[ "$1" == "--help" ]]; then
    echo "Usage: $0 VERSION"
    echo ""
    echo "Install PicoJSON library"
    echo ""
    echo "Arguments:"
    echo "  VERSION  PicoJSON commit SHA or tag to install (required)"
    echo ""
    echo "Examples:"
    echo "  $0 111c9be5188f7350c2eac9ddaedd8cca3d7bf394"
    echo "  $0 v1.3.0"
    exit 1
fi

# Configuration
PICOJSON_VERSION="$1"
INSTALL_DIR="/tmp"
BUILD_DIR="${INSTALL_DIR}/picojson-${PICOJSON_VERSION#v}" # Remove 'v' prefix if present

echo "Installing PicoJSON ${PICOJSON_VERSION}..."

# Download
echo "Downloading PicoJSON ${PICOJSON_VERSION}..."
cd "${INSTALL_DIR}"
wget -q "https://github.com/kazuho/picojson/archive/${PICOJSON_VERSION}.tar.gz"

# Extract
echo "Extracting archive..."
tar -zxf "${PICOJSON_VERSION}.tar.gz"

# Install
echo "Installing..."
cd "${BUILD_DIR}"
sudo make install

# Cleanup
echo "Cleaning up..."
rm -f "${INSTALL_DIR}/${PICOJSON_VERSION}.tar.gz"
rm -rf "${BUILD_DIR}"

echo "✓ PicoJSON ${PICOJSON_VERSION} installed successfully!"
