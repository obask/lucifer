all:
	CFLAGS="-stdlib=libc++ -Wno-c++11-extensions" python3 setup.py install

run:
	python3 miner.py

clean:
	rm -rf build/

rebuild: clean all

