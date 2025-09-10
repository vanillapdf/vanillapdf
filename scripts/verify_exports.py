#!/usr/bin/env python3
"""
Cross-platform script to verify library exports and check for name mangling.
This script works on Windows (DLL), Linux (SO), and macOS (DYLIB) to verify 
that ALL exported symbols don't contain special characters that indicate name mangling.

Usage:
    python verify_exports.py <path_to_library>

Requirements:
    - Python 3.x
    - For Windows: dumpbin (Visual Studio) or pefile library (pip install pefile)
    - For Linux: objdump or readelf
    - For macOS: nm or otool
"""

import sys
import os
import platform
import subprocess
import re
from typing import List, Tuple, Set

def has_mangled_characters(symbol_name: str) -> bool:
    """
    Check if a symbol name contains characters that indicate C++ name mangling.
    
    Args:
        symbol_name: The symbol name to check
        
    Returns:
        True if the symbol appears to be mangled, False otherwise
    """
    # Common C++ name mangling characters across platforms
    mangling_chars = {'?', '@', '$', '#', '%', '^', '&', '*', '(', ')', '[', ']', '{', '}', '<', '>'}
    
    # Check for character-based mangling
    if any(char in symbol_name for char in mangling_chars):
        return True
    
    # Check for GCC-style mangling (starts with _Z)
    if symbol_name.startswith('_Z'):
        return True
    
    # Check for other common mangling patterns
    if '::' in symbol_name:  # C++ namespace separator
        return True
    
    return False

def extract_exports_windows(lib_path: str) -> List[str]:
    """
    Extract exports from a Windows DLL using dumpbin or pefile.
    
    Args:
        lib_path: Path to the DLL file
        
    Returns:
        List of exported symbol names
    """
    # Try dumpbin first (part of Visual Studio)
    try:
        result = subprocess.run(['dumpbin', '/exports', lib_path], 
                              capture_output=True, text=True, check=True)
        
        exports = []
        in_exports_section = False
        
        for line in result.stdout.split('\n'):
            line = line.strip()
            
            if 'ordinal hint RVA      name' in line:
                in_exports_section = True
                continue
            
            if in_exports_section and line:
                # Parse line format: "    1    0 00001000 SymbolName"
                parts = line.split()
                if len(parts) >= 4 and not parts[3].startswith('Summary'):
                    symbol_name = parts[3]
                    exports.append(symbol_name)
        
        return exports
    except (subprocess.CalledProcessError, FileNotFoundError):
        pass
    
    # Try pefile as fallback
    try:
        import pefile
        
        pe = pefile.PE(lib_path)
        
        if not hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
            return []
        
        exports = []
        for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
            if exp.name:
                exports.append(exp.name.decode('utf-8'))
        
        return exports
    except ImportError:
        print("Warning: Neither dumpbin nor pefile library found.")
        print("Install pefile with: pip install pefile")
        return []
    except Exception as e:
        print(f"Error reading DLL: {e}")
        return []

def extract_exports_linux(lib_path: str) -> List[str]:
    """
    Extract exports from a Linux shared library using objdump or readelf.
    
    Args:
        lib_path: Path to the .so file
        
    Returns:
        List of exported symbol names
    """
    # Try objdump first
    try:
        result = subprocess.run(['objdump', '-T', lib_path], 
                              capture_output=True, text=True, check=True)
        
        exports = []
        for line in result.stdout.split('\n'):
            line = line.strip()
            # Look for dynamic symbol table entries
            # Format: address flags section size version symbol_name
            if line and not line.startswith('DYNAMIC SYMBOL TABLE') and not line.startswith('file format'):
                parts = line.split()
                if len(parts) >= 6 and parts[1] != 'l':  # Skip local symbols
                    symbol_name = parts[-1]
                    # Filter out version suffixes like @@GLIBC_2.2.5
                    if '@@' in symbol_name:
                        symbol_name = symbol_name.split('@@')[0]
                    exports.append(symbol_name)
        
        return exports
    except (subprocess.CalledProcessError, FileNotFoundError):
        pass
    
    # Try readelf as fallback
    try:
        result = subprocess.run(['readelf', '-Ws', lib_path], 
                              capture_output=True, text=True, check=True)
        
        exports = []
        for line in result.stdout.split('\n'):
            line = line.strip()
            if line and ':' in line:
                parts = line.split()
                if len(parts) >= 8:
                    symbol_name = parts[-1]
                    # Filter out version suffixes
                    if '@@' in symbol_name:
                        symbol_name = symbol_name.split('@@')[0]
                    exports.append(symbol_name)
        
        return exports
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("Warning: Neither objdump nor readelf found.")
        return []

def extract_exports_macos(lib_path: str) -> List[str]:
    """
    Extract exports from a macOS dylib using nm or otool.
    
    Args:
        lib_path: Path to the .dylib file
        
    Returns:
        List of exported symbol names
    """
    # Try nm first
    try:
        result = subprocess.run(['nm', '-gU', lib_path], 
                              capture_output=True, text=True, check=True)
        
        exports = []
        for line in result.stdout.split('\n'):
            line = line.strip()
            if line:
                parts = line.split()
                if len(parts) >= 3:
                    symbol_name = parts[-1]
                    # Remove leading underscore if present (common in macOS)
                    if symbol_name.startswith('_'):
                        symbol_name = symbol_name[1:]
                    exports.append(symbol_name)
        
        return exports
    except (subprocess.CalledProcessError, FileNotFoundError):
        pass
    
    # Try otool as fallback
    try:
        result = subprocess.run(['otool', '-TV', lib_path], 
                              capture_output=True, text=True, check=True)
        
        exports = []
        for line in result.stdout.split('\n'):
            line = line.strip()
            if line and 'external' in line:
                parts = line.split()
                if len(parts) >= 2:
                    symbol_name = parts[-1]
                    if symbol_name.startswith('_'):
                        symbol_name = symbol_name[1:]
                    exports.append(symbol_name)
        
        return exports
    except (subprocess.CalledProcessError, FileNotFoundError):
        print("Warning: Neither nm nor otool found.")
        return []

def extract_exports(lib_path: str) -> List[str]:
    """
    Extract exports from a library file using platform-appropriate tools.
    
    Args:
        lib_path: Path to the library file
        
    Returns:
        List of exported symbol names
    """
    if not os.path.exists(lib_path):
        raise FileNotFoundError(f"Library file not found: {lib_path}\n"
                              f"Make sure the library was built successfully before running export verification.\n"
                              f"For CMake builds, ensure the 'vanillapdf' target was built first.")
    
    system = platform.system().lower()
    
    if system == 'windows' or lib_path.endswith('.dll'):
        return extract_exports_windows(lib_path)
    elif system == 'linux' or lib_path.endswith('.so'):
        return extract_exports_linux(lib_path)
    elif system == 'darwin' or lib_path.endswith('.dylib'):
        return extract_exports_macos(lib_path)
    else:
        # Try to guess from file extension
        if lib_path.endswith('.dll'):
            return extract_exports_windows(lib_path)
        elif lib_path.endswith('.so'):
            return extract_exports_linux(lib_path)
        elif lib_path.endswith('.dylib'):
            return extract_exports_macos(lib_path)
        else:
            raise RuntimeError(f"Unsupported platform or file type: {system}, {lib_path}")

def verify_library_exports(lib_path: str) -> Tuple[List[str], List[str], List[str]]:
    """
    Verify library exports and categorize them.
    
    Args:
        lib_path: Path to the library file
        
    Returns:
        Tuple of (all_exports, mangled_exports, vanillapdf_exports)
    """
    exports = extract_exports(lib_path)
    
    if not exports:
        raise RuntimeError("Could not extract exports from library. Make sure appropriate tools are installed.")
    
    mangled_exports = []
    vanillapdf_exports = []
    
    for export_name in exports:
        if has_mangled_characters(export_name):
            mangled_exports.append(export_name)
        
        # Collect VanillaPDF-related exports (functions, constants, etc.)
        if any(prefix in export_name for prefix in ['vanillapdf', 'VanillaPDF', 'NameConstant_', 'Buffer_', 'Document_', 'Page_', 'Filter_']):
            vanillapdf_exports.append(export_name)
    
    return exports, mangled_exports, vanillapdf_exports

def main():
    if len(sys.argv) != 2:
        print("Usage: python verify_exports.py <path_to_library>")
        sys.exit(1)
    
    lib_path = sys.argv[1]
    
    try:
        all_exports, mangled_exports, vanillapdf_exports = verify_library_exports(lib_path)
        
        print(f"Library Export Analysis: {lib_path}")
        print(f"Platform: {platform.system()}")
        print(f"Total exports found: {len(all_exports)}")
        print(f"VanillaPDF-related exports: {len(vanillapdf_exports)}")
        print()
        
        if mangled_exports:
            print(f"⚠️  Found {len(mangled_exports)} mangled exports:")
            for export_name in sorted(mangled_exports):
                print(f"  - {export_name}")
            print()
        else:
            print("✅ No mangled exports found!")
            print()
        
        # Report on VanillaPDF export status
        print("VanillaPDF export analysis:")
        if vanillapdf_exports:
            clean_exports = [e for e in vanillapdf_exports if not has_mangled_characters(e)]
            mangled_vanillapdf = [e for e in vanillapdf_exports if has_mangled_characters(e)]
            
            print(f"  ✅ Clean exports: {len(clean_exports)}")
            print(f"  ⚠️  Mangled exports: {len(mangled_vanillapdf)}")
        else:
            print("  ⚠️  No VanillaPDF exports found")
        
        # Show some VanillaPDF exports for verification
        if vanillapdf_exports:
            print(f"\nVanillaPDF exports sample (showing first 10):")
            for export in sorted(vanillapdf_exports)[:10]:
                status = "⚠️ MANGLED" if has_mangled_characters(export) else "✅"
                print(f"  {status} {export}")
            
            if len(vanillapdf_exports) > 10:
                print(f"  ... and {len(vanillapdf_exports) - 10} more")
        
        # Summary
        total_mangled = len([e for e in vanillapdf_exports if has_mangled_characters(e)])
        if total_mangled > 0:
            print(f"\n❌ FAILED: Found {total_mangled} mangled VanillaPDF exports out of {len(vanillapdf_exports)} total")
            sys.exit(1)
        elif len(vanillapdf_exports) == 0:
            print(f"\n⚠️  WARNING: No VanillaPDF exports found - this might indicate an issue with export detection")
            sys.exit(1)
        else:
            print(f"\n✅ SUCCESS: All {len(vanillapdf_exports)} VanillaPDF exports are properly exported without mangling")
            sys.exit(0)
        
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()