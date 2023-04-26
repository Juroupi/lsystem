CXXFLAGS = -I../bnf/include -O2
LDFLAGS = -lgmp -lgmpxx -lallegro -lallegro_primitives

lsystem: lsystem.cpp ../bnf/bnf.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

trace: trace.cpp ../bnf/bnf.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf lsystem trace

# exemples

tree: lsystem # card = 13 * n - (n - 1)
	./lsystem file=tests/tree.bnf start=T angle=30 card=601 y=350 a=-90 method=nuniform

tree2: lsystem # card = 14 * n - (n - 1)
	./lsystem file=tests/tree2.bnf start=T angle=30 growth=1.15 card=391 y=350 a=-90 d=50 method=nuniform

treeh: lsystem
	./lsystem file=tests/treeh.bnf method=height start=T angle=30 growth=1.15 card=8 y=350 a=-90 d=50

curve: lsystem # card = 5 * n - (n - 1)
	./lsystem file=tests/curve.bnf start=A angle=60 card=53

tcurve: lsystem # card = 17, 17 * 3 + 2 = 53, 53 * 3 + 2 = 161, 161 * 3 + 2 = 485
	./lsystem file=tests/tcurve.bnf start=A1 angle=60 card=485 x=10 y=570 width=650 height=580

dragon: lsystem # card = 3 * n - (n - 1)
	./lsystem file=tests/dragon.bnf start=F angle=90 card=21

spath: lsystem # card = 11 * n - (n - 1)
	./lsystem file=tests/spath.bnf start=P angle=90 card=101 growth=3 x=0 d=400

tpath: lsystem # card = 8 * n - (n - 1)
	./lsystem file=tests/tpath.bnf start=P angle=22.5 card=71 growth=1.84 x=0 d=400 method=nuniform

spiral: lsystem # card = 3 * n
	./lsystem file=tests/spiral.bnf start=S angle=10 growth=1.02 card=600 d=0.5

# traces

trace_tree: trace
	./trace file=tests/tree.bnf start=T angle=30 card=601 y=350 a=-90 gens=100 alpha=20 method=uniform
