compile:
	g++ -std=c++17 scoundrel.cc main.cc -o mc

test:
	test -f cppunit.h || curl https://raw.githubusercontent.com/cppunit/cppunit/refs/heads/master/cppunit.h -o cppunit.h
	g++ -std=c++17 test.cc scoundrel.cc -o runtest
	./runtest
	test -f runtest && rm runtest
