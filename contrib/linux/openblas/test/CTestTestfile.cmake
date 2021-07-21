# CMake generated Testfile for 
# Source directory: /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test
# Build directory: /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(sblas1 "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/sblat1")
set_tests_properties(sblas1 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;52;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(sblas2 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/sblat2" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/sblat2.dat" "SBLAT2.SUMM")
set_tests_properties(sblas2 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;54;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(sblas3 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/sblat3" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/sblat3.dat" "SBLAT3.SUMM")
set_tests_properties(sblas3 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;56;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(dblas1 "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/dblat1")
set_tests_properties(dblas1 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;52;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(dblas2 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/dblat2" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/dblat2.dat" "DBLAT2.SUMM")
set_tests_properties(dblas2 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;54;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(dblas3 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/dblat3" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/dblat3.dat" "DBLAT3.SUMM")
set_tests_properties(dblas3 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;56;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(cblas1 "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/cblat1")
set_tests_properties(cblas1 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;52;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(cblas2 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/cblat2" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/cblat2.dat" "CBLAT2.SUMM")
set_tests_properties(cblas2 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;54;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(cblas3 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/cblat3" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/cblat3.dat" "CBLAT3.SUMM")
set_tests_properties(cblas3 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;56;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(zblas1 "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/zblat1")
set_tests_properties(zblas1 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;52;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(zblas2 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/zblat2" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/zblat2.dat" "ZBLAT2.SUMM")
set_tests_properties(zblas2 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;54;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
add_test(zblas3 "sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/test_helper.sh" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/zblat3" "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/zblat3.dat" "ZBLAT3.SUMM")
set_tests_properties(zblas3 PROPERTIES  _BACKTRACE_TRIPLES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;56;add_test;/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/CMakeLists.txt;0;")
