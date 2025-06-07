# CMake generated Testfile for 
# Source directory: /home/kali/workspace/projects/lab05
# Build directory: /home/kali/workspace/projects/lab05/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(my_test "/home/kali/workspace/projects/lab05/build/my_test")
set_tests_properties(my_test PROPERTIES  _BACKTRACE_TRIPLES "/home/kali/workspace/projects/lab05/CMakeLists.txt;33;add_test;/home/kali/workspace/projects/lab05/CMakeLists.txt;0;")
subdirs("third-party/gtest")
