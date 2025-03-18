compile:
	g++ -std=c++17 scoundrel.cc main.cc -o mc

test:
	test -f cppunit.h || curl https://raw.githubusercontent.com/cppunit/cppunit/refs/heads/master/cppunit.h -o cppunit.h
	g++ -std=c++17 test.cc scoundrel.cc strategy.cc -o runtest
	./runtest
	test -f runtest && rm runtest

statistics:
	g++ -std=c++17 scoundrel.cc stats.cc strategy.cc -o stats
	./stats
	test -f stats && rm stats

format:
	clang-format -i *.cc *.h
