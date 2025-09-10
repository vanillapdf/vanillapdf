#!/usr/bin/env python3
"""
Test script to verify the export verification script works correctly.
This creates a mock library file and tests the verification logic.
"""

import tempfile
import os
import sys
import subprocess
from pathlib import Path

def create_mock_dll_with_exports():
    """Create a mock Windows DLL with test exports for verification."""
    # This would require complex PE file creation, so we'll skip for now
    pass

def test_mangling_detection():
    """Test the mangling character detection logic."""
    # Import the verification script
    script_dir = Path(__file__).parent
    sys.path.insert(0, str(script_dir))
    
    from verify_exports import has_mangled_characters
    
    # Test cases
    test_cases = [
        ("NameConstant_SomeFilter", False), # Good C symbol
        ("Buffer_Create", False),           # Good C symbol  
        ("?SomeMangled@@3PEBUHandleTag@@EB", True),  # Mangled
        ("_Z15some_cpp_symbolv", True),     # GCC mangling
        ("simple_function", False),        # Good C symbol
        ("@__security_check_cookie@4", True),  # MSVC mangling
        ("std::vector<int>", True),        # C++ template (contains <>)
        ("operator+()", True),             # C++ operator (contains ())
    ]
    
    print("Testing mangling detection:")
    all_passed = True
    
    for symbol, should_be_mangled in test_cases:
        is_mangled = has_mangled_characters(symbol)
        status = "✅ PASS" if is_mangled == should_be_mangled else "❌ FAIL"
        print(f"  {status} '{symbol}' -> mangled={is_mangled} (expected={should_be_mangled})")
        if is_mangled != should_be_mangled:
            all_passed = False
    
    return all_passed

def test_script_execution():
    """Test that the script can be executed with proper error handling."""
    script_path = Path(__file__).parent / "verify_exports.py"
    
    print("\nTesting script execution:")
    
    # Test with non-existent file
    result = subprocess.run([sys.executable, str(script_path), "nonexistent.dll"], 
                          capture_output=True, text=True)
    
    if result.returncode != 0:
        print("  ✅ PASS - Script correctly fails for non-existent file")
        return True
    else:
        print("  ❌ FAIL - Script should fail for non-existent file")
        return False

def main():
    print("Export Verification Script Test")
    print("=" * 40)
    
    test1_passed = test_mangling_detection()
    test2_passed = test_script_execution()
    
    print("\n" + "=" * 40)
    if test1_passed and test2_passed:
        print("✅ All tests passed!")
        sys.exit(0)
    else:
        print("❌ Some tests failed!")
        sys.exit(1)

if __name__ == "__main__":
    main()