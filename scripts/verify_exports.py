#!/usr/bin/env python3
"""
Script to verify DLL exports and check for name mangling.
This script can be used on Windows systems to verify that exported symbols
don't contain special characters that indicate name mangling.

Usage:
    python verify_exports.py <path_to_dll>

Requirements:
    - Windows system
    - Python 3.x
    - pefile library (pip install pefile)
"""

import sys
import os
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
    # Common C++ name mangling characters
    mangling_chars = {'?', '@', '$', '#', '%', '^', '&', '*', '(', ')', '[', ']', '{', '}', '<', '>'}
    
    return any(char in symbol_name for char in mangling_chars)

def extract_exports_with_dumpbin(dll_path: str) -> List[str]:
    """
    Extract exports from a DLL using dumpbin (Visual Studio tool).
    
    Args:
        dll_path: Path to the DLL file
        
    Returns:
        List of exported symbol names
    """
    try:
        import subprocess
        result = subprocess.run(['dumpbin', '/exports', dll_path], 
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
                if len(parts) >= 4:
                    symbol_name = parts[3]
                    exports.append(symbol_name)
        
        return exports
    except (subprocess.CalledProcessError, FileNotFoundError):
        return []

def extract_exports_with_pefile(dll_path: str) -> List[str]:
    """
    Extract exports from a DLL using pefile library.
    
    Args:
        dll_path: Path to the DLL file
        
    Returns:
        List of exported symbol names
    """
    try:
        import pefile
        
        pe = pefile.PE(dll_path)
        
        if not hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
            return []
        
        exports = []
        for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
            if exp.name:
                exports.append(exp.name.decode('utf-8'))
        
        return exports
    except ImportError:
        print("Warning: pefile library not found. Install with: pip install pefile")
        return []
    except Exception as e:
        print(f"Error reading DLL with pefile: {e}")
        return []

def verify_dll_exports(dll_path: str) -> Tuple[List[str], List[str], List[str]]:
    """
    Verify DLL exports and categorize them.
    
    Args:
        dll_path: Path to the DLL file
        
    Returns:
        Tuple of (all_exports, mangled_exports, target_exports)
    """
    if not os.path.exists(dll_path):
        raise FileNotFoundError(f"DLL file not found: {dll_path}")
    
    # Try multiple methods to extract exports
    exports = extract_exports_with_dumpbin(dll_path)
    if not exports:
        exports = extract_exports_with_pefile(dll_path)
    
    if not exports:
        raise RuntimeError("Could not extract exports from DLL. Make sure you have dumpbin or pefile available.")
    
    mangled_exports = []
    target_exports = []
    
    target_symbols = {'NameConstant_LZWDecode', 'NameConstant_JPXDecode'}
    
    for export_name in exports:
        if has_mangled_characters(export_name):
            mangled_exports.append(export_name)
        
        if export_name in target_symbols:
            target_exports.append(export_name)
    
    return exports, mangled_exports, target_exports

def main():
    if len(sys.argv) != 2:
        print("Usage: python verify_exports.py <path_to_dll>")
        sys.exit(1)
    
    dll_path = sys.argv[1]
    
    try:
        all_exports, mangled_exports, target_exports = verify_dll_exports(dll_path)
        
        print(f"DLL Export Analysis: {dll_path}")
        print(f"Total exports found: {len(all_exports)}")
        print()
        
        if mangled_exports:
            print(f"⚠️  Found {len(mangled_exports)} mangled exports:")
            for export_name in sorted(mangled_exports):
                print(f"  - {export_name}")
            print()
        else:
            print("✅ No mangled exports found!")
            print()
        
        print("Target symbol analysis:")
        expected_targets = {'NameConstant_LZWDecode', 'NameConstant_JPXDecode'}
        
        for target in expected_targets:
            if target in target_exports:
                print(f"  ✅ {target} - Found and properly exported")
            else:
                print(f"  ❌ {target} - Missing or mangled")
        
        if target_exports:
            print(f"\nFound {len(target_exports)} target symbols:")
            for target in sorted(target_exports):
                print(f"  - {target}")
        
        # Check for any exports that might be our targets but mangled
        possible_mangled_targets = []
        for export_name in all_exports:
            if 'LZWDecode' in export_name or 'JPXDecode' in export_name:
                if export_name not in target_exports:
                    possible_mangled_targets.append(export_name)
        
        if possible_mangled_targets:
            print(f"\n⚠️  Possible mangled target symbols:")
            for symbol in possible_mangled_targets:
                print(f"  - {symbol}")
        
        # Exit with error code if we found mangled exports or missing targets
        if mangled_exports or len(target_exports) < len(expected_targets):
            sys.exit(1)
        else:
            print("\n✅ All checks passed!")
            sys.exit(0)
        
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()